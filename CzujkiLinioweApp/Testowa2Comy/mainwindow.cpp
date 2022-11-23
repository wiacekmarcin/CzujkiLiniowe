#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_status(new QLabel)
    
{
    ui->setupUi(this);

    ui->statusbar->addWidget(m_status);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showStatusMessage(const QString &message)
{
    m_status->setText(message);
}


