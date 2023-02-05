#include "task.h"
#include <QDebug>
#include "pdfcreator.h"

void Task::run()
{
    // Do processing here

    qDebug() << "Starting....";
    PdfCreator p;
    qDebug() << "ret=" << p.getErrCode();
    qDebug() << "ending....";

    emit finished();
}
