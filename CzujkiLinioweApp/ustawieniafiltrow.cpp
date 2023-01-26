#include "ustawieniafiltrow.h"
#include "ui_ustawieniafiltrow.h"



UstawieniaFiltrow::UstawieniaFiltrow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UstawieniaFiltrow)
{
    ui->setupUi(this);
    adjustSize();
}

UstawieniaFiltrow::~UstawieniaFiltrow()
{
    delete ui;
}

#define READ_FILTR(D,T,P) ui->f_##D##_##T##P->setText(QString::number(ust->getFiltr_db_##D##_##T##_##P()));

#define READ_FILTR_T(D, T) READ_FILTR(D, T, 0) \
                          READ_FILTR(D, T, 1) \
                          READ_FILTR(D, T, 2) \
                          READ_FILTR(D, T, 3) \
                          READ_FILTR(D, T, 4) \
                          READ_FILTR(D, T, 5)

#define READ_FILTR_655 READ_FILTR_T(655, A) \
                       READ_FILTR_T(655, B) \
                       READ_FILTR_T(655, C)

#define READ_FILTR_880 READ_FILTR_T(880, A) \
                       READ_FILTR_T(880, B) \
                       READ_FILTR_T(880, C)

void UstawieniaFiltrow::setUstawienia(Ustawienia *u)
{
    ust = u;
    READ_FILTR_655
    READ_FILTR_880

    loadTable();


}


#define WRITE_FILTR(D,T,P) ust->setFiltr_db_##D##_##T##_##P(ui->f_##D##_##T##P->text());

#define WRITE_FILTR_T(D, T) WRITE_FILTR(D, T, 0) \
                            WRITE_FILTR(D, T, 1) \
                            WRITE_FILTR(D, T, 2) \
                            WRITE_FILTR(D, T, 3) \
                            WRITE_FILTR(D, T, 4) \
                            WRITE_FILTR(D, T, 5)

#define WRITE_FILTR_655 WRITE_FILTR_T(655, A) \
                        WRITE_FILTR_T(655, B) \
                        WRITE_FILTR_T(655, C)

#define WRITE_FILTR_880 WRITE_FILTR_T(880, A) \
                        WRITE_FILTR_T(880, B) \
                        WRITE_FILTR_T(880, C)

void UstawieniaFiltrow::saveFiltry()
{
    WRITE_FILTR_655
    WRITE_FILTR_880
            ust->sync();
}

void UstawieniaFiltrow::loadTable()
{
    QMap<QString, QPair<QString, QString>> records;
    tlumienia655 = ust->getTlumienia655();

    for (short t1 = 0; t1 < tlumienia655.size(); ++t1) {
        auto r = tlumienia655.at(t1);
        records[r.at(0)] = qMakePair(QString(), QString("A%1+B%2+C%3=%4").arg(r.at(1),r.at(2),r.at(3),r.at(4)));
    }

    tlumienia880 = ust->getTlumienia880();

    for (short t1 = 0; t1 < tlumienia880.size(); ++t1) {
        auto r = tlumienia880.at(t1);
        if (!records.contains(r.at(0)))
            records[r.at(0)] = qMakePair(QString(), QString());
        records[r.at(0)].first = QString("A%1+B%2+C%3=%4").arg(r.at(1), r.at(2), r.at(3), r.at(4));
    }

    auto keys = records.keys();
    std::sort(keys.begin(), keys.end(), [](const QString &v1, const QString &v2)
    {
        return v1.toDouble() < v2.toDouble();
    });
    ui->tableWidget->setRowCount(keys.size());
    for (short p = 0; p < keys.size(); ++p)
    {
        QString kTlum = keys.at(p);
        ui->tableWidget->setItem(p, 0, new QTableWidgetItem(kTlum));
        auto r = records[keys.at(p)];
        ui->tableWidget->setItem(p, 1, new QTableWidgetItem(r.first));
        ui->tableWidget->setItem(p, 2, new QTableWidgetItem(r.second));
        tlumienia.push_back(qMakePair(kTlum.toDouble(), kTlum));
    }
    ui->tableWidget->setColumnWidth(0, 80);
    ui->tableWidget->setColumnWidth(1, 120);
    ui->tableWidget->setColumnWidth(2, 120);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

void UstawieniaFiltrow::countFiltr665(short A, short B, short C, bool wylicz)
{
    //qDebug() << "countFiltr665 " << A << " " << B << " " << C << " " << wylicz;
    double suma = ust->getFiltr_db_655('A', A);
    suma += ust->getFiltr_db_655('B', B);
    suma +=  ust->getFiltr_db_655('C', C);
    ui->C_655->setText(QString::number(suma, 'f', 2));
    if (wylicz) {
        short id=0;
        while (suma < tlumienia.at(id).first)
            ++id;
        ui->wyliczone_tlumienie->setText(tlumienia.at(id).second + QString(" db"));
    }
}

void UstawieniaFiltrow::countFiltr880(short A, short B, short C, bool wylicz)
{
    //qDebug() << "countFiltr880 " << A << " " << B << " " << C << " " << wylicz;
    double suma = ust->getFiltr_db_880('A', A);
    suma += ust->getFiltr_db_880('B', B);
    suma +=  ust->getFiltr_db_880('C', C);
    ui->C_880->setText(QString::number(suma, 'f', 2));
    if (wylicz) {
        short id = 0;
        while (suma < tlumienia.at(id).first)
            ++id;
        ui->wyliczone_tlumienie->setText(tlumienia.at(id).second + QString(" db"));
    }
}

void UstawieniaFiltrow::on_cb_880_A_currentIndexChanged(int index)
{
    countFiltr880(index, ui->cb_880_B->currentIndex(), ui->cb_880_C->currentIndex());
}

void UstawieniaFiltrow::on_cb_880_B_currentIndexChanged(int index)
{
    countFiltr880(ui->cb_880_A->currentIndex(), index, ui->cb_880_C->currentIndex());
}

void UstawieniaFiltrow::on_cb_880_C_currentIndexChanged(int index)
{
    countFiltr880(ui->cb_880_A->currentIndex(), ui->cb_880_B->currentIndex(), index);
}

void UstawieniaFiltrow::on_cb_655_A_currentIndexChanged(int index)
{
    countFiltr880(index, ui->cb_655_B->currentIndex(), ui->cb_655_C->currentIndex());
}

void UstawieniaFiltrow::on_cb_655_B_currentIndexChanged(int index)
{
    countFiltr880(ui->cb_655_A->currentIndex(), index, ui->cb_655_C->currentIndex());
}

void UstawieniaFiltrow::on_cb_655_C_currentIndexChanged(int index)
{
    countFiltr880(ui->cb_655_A->currentIndex(), ui->cb_655_B->currentIndex(), index);
}

void UstawieniaFiltrow::on_tableWidget_cellEntered(int row, int column)
{
    (void)column;
    QString tl = ui->tableWidget->item(row,0)->text();
    if (!ui->tableWidget->item(row, 2)->text().isEmpty()) {
        for (short i = 0; i < tlumienia655.size(); ++i) {
            auto r = tlumienia655.at(i);
            if (r.at(0) == tl) {
                short a = r.at(1).toInt();
                short b = r.at(2).toInt();
                short c = r.at(3).toInt();
                //qDebug() << "655 " << a << " " << b << " " << c;
                ui->cb_655_A->setCurrentIndex(a);
                ui->cb_655_B->setCurrentIndex(b);
                ui->cb_655_C->setCurrentIndex(c);
                countFiltr665(a, b, c, false);
                break;
            }
        }
    }

    if (!ui->tableWidget->item(row, 1)->text().isEmpty()) {
        for (short i = 0; i < tlumienia880.size(); ++i) {
            auto r = tlumienia880.at(i);
            if (r.at(0) == tl) {
                short a = r.at(1).toInt();
                short b = r.at(2).toInt();
                short c = r.at(3).toInt();
                //qDebug() << "880 " << a << " " << b << " " << c;
                ui->cb_880_A->setCurrentIndex(a);
                ui->cb_880_B->setCurrentIndex(b);
                ui->cb_880_C->setCurrentIndex(c);
                countFiltr880(a, b, c, false);
                break;
            }
        }
    }
    ui->wyliczone_tlumienie->setText(tl + QString(" db"));
}


void UstawieniaFiltrow::on_tableWidget_cellClicked(int row, int column)
{
    //qDebug() << row << column;
    on_tableWidget_cellEntered(row, column);
}

void UstawieniaFiltrow::on_tableWidget_cellActivated(int row, int column)
{
    //qDebug() << row << column;
    on_tableWidget_cellEntered(row, column);
}



void UstawieniaFiltrow::on_tableWidget_cellPressed(int row, int column)
{
    //qDebug() << row << column;
    on_tableWidget_cellEntered(row, column);
}

