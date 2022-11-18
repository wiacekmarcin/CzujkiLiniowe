#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>
#include <QMessageBox>

static const char blankString[] = "";
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_status(new QLabel)
    , m_serialZas(new QSerialPort(this))
    , m_serialSter(new QSerialPort(this))
    , m_portNameZas("N/A")
    , readMeas(this)
{
    ui->setupUi(this);

    ui->statusbar->addWidget(m_status);

    connect(m_serialZas, &QSerialPort::errorOccurred, this, &MainWindow::handleErrorZasilacz);
    connect(m_serialZas, &QSerialPort::readyRead, this, &MainWindow::readDataZasilacz);

    connect(ui->pbZasOpen, &QPushButton::clicked, this, &MainWindow::findZasilacz);
    connect(ui->pbZasClose, &QPushButton::clicked, this, &MainWindow::closeSerialPortZasilacz);

    connect(&readMeas, &QTimer::timeout, this, &MainWindow::timeoutZasilacz);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSerialPortZasilacz()
{

    m_serialZas->setPortName(m_portNameZas);
    m_serialZas->setBaudRate(QSerialPort::Baud115200);
    m_serialZas->setDataBits(QSerialPort::Data8);
    m_serialZas->setParity(QSerialPort::NoParity);
    m_serialZas->setStopBits(QSerialPort::OneStop);
    m_serialZas->setFlowControl(QSerialPort::NoFlowControl);
    if (m_serialZas->open(QIODevice::ReadWrite)) {
        showStatusMessage(tr("Connected to %1")
                          .arg(m_portNameZas));
    } else {
        QMessageBox::critical(this, tr("Error"), m_serialZas->errorString());

        showStatusMessage(tr("Open error"));
    }
}

void MainWindow::closeSerialPortZasilacz()
{
    if (m_serialZas->isOpen())
        m_serialZas->close();
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::readDataZasilacz()
{
    QByteArray data;
    while (m_serialZas->bytesAvailable()) {
        data.append(m_serialZas->readAll());
        if (m_serialZas->waitForReadyRead(50))
            ;

    }



    if (zasWork == SEND_IDENT) {
        QString sident(data);
        sident.remove(QChar(13)).remove(QChar(10));
        ui->lZasIdent->setText(sident);
        readMeas.setInterval(100);
        readMeas.start();
    }
    if (zasWork == SEND_VOLT) {
        zasWork = SEND_CURR;
        QString voltage(data);
        voltage.remove(QChar(13)).remove(QChar(10));
        ui->mVoltage->setText(voltage);
        QByteArray identMsg("MEAS:CURR?\n", 11);
        writeDataZasilacz(identMsg);

    }
    if (zasWork == SEND_CURR) {
        QString current(data);
        current.remove(QChar(13)).remove(QChar(10));
        ui->mCurrent->setText(current);
        zasWork = SEND_NONE;
    }
    qDebug() << "[" << data.toStdString().data() << "]";
    //m_console->putData(data);
    //WON,P4305,2210355,FV:V1.8.0
}

void MainWindow::writeDataZasilacz(const QByteArray &data)
{
    m_serialZas->write(data);
}

void MainWindow::handleErrorZasilacz(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_serialZas->errorString());
        closeSerialPortZasilacz();
    }
}

void MainWindow::findZasilacz()
{
    QString vendor, product;
    for (const auto & port : getComPorts())
    {
        if (port.count() != 7)
            continue;

        vendor = port.at(5);
        product = port.at(6);

        if (vendor.isEmpty() || product.isEmpty())
            continue;

        if (vendor != "67b" || product != "2303")
            continue;

        m_portNameZas = port.at(0);
        openSerialPortZasilacz();
        ui->lZasSterPort->setText(m_portNameZas);

        if (!m_serialZas->isOpen()) {
            ui->lZasSterPort->setText("-");
            continue;
        }

        zasWork = SEND_IDENT;
        QByteArray identMsg("*IDN?\n", 6);
        writeDataZasilacz(identMsg);
        /*
        m_serialZas->write(identMsg);
        m_serialZas->waitForBytesWritten();

        m_serialZas->waitForReadyRead();
        QByteArray ident = m_serialZas->readAll();
        ui->lZasIdent->setText(QString(ident));
        */
    }

}

void MainWindow::timeoutZasilacz()
{
    zasWork = SEND_VOLT;
    QByteArray identMsg("MEAS:VOLT?\n", 11);
    writeDataZasilacz(identMsg);
}

QList<QStringList> MainWindow::getComPorts()
{
    QString description;
    QString manufacturer;
    QString serialNumber;
    QList<QStringList> ports;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ports.append(list);
    }
    return ports;
}

void MainWindow::showStatusMessage(const QString &message)
{
    m_status->setText(message);
}
