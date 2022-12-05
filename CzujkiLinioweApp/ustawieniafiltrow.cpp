#include "ustawieniafiltrow.h"
#include "ui_ustawieniafiltrow.h"



UstawieniaFiltrow::UstawieniaFiltrow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UstawieniaFiltrow)
{
    ui->setupUi(this);
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
