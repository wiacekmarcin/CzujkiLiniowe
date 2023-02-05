/*
 * << Haru Free PDF Library 2.0.0 >> -- text_demo.c
 *
 * Copyright (c) 1999-2006 Takeshi Kanno <takeshi_kanno@est.hi-ho.ne.jp>
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 */

#include <QDebug>

#include "pdfcreator.h"
#include <QCoreApplication>

#include <QtCore>

#include "task.h"

int main (int argc, char **argv)
{
    QCoreApplication a(argc, argv);

    // Task parented to the application so that it
    // will be deleted by the application.
    Task *task = new Task(&a);
    task->run();

    // This will cause the application to exit when
    // the task signals finished.
    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));

    return a.exec();
}
