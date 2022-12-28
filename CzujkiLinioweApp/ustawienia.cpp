#include "ustawienia.h"

#include <QApplication>
#include <algorithm>

char Ustawienia::appnazwa[]="CzujkiLiniowe";
char Ustawienia::firmnazwa[]="Kolodziejczyk";

Ustawienia::Ustawienia() :
    UstawieniaGen()
{
    load();
    loadListUstawienFiltra();
    save();
    saveListUstawienFiltra();
    sync();
}

Ustawienia::~Ustawienia()
{
    //save();
    //saveListUstawienFiltra();
    //sync();
}

double Ustawienia::wyliczPredkosc(const double &ratioImpJedn, const double &impTime)
{
    //impTime w us
    //wartosc zwracana w jedn/min;

    double cntImp1Min = 60 * 1000000 / impTime;
    qDebug() << __FILE__ << ":" << __LINE__ << " " << ratioImpJedn << " " << impTime << " " << cntImp1Min << " => " << cntImp1Min*ratioImpJedn;
    return cntImp1Min*ratioImpJedn;

}

void Ustawienia::loadListUstawienFiltra()
{
    short cntPos = settings.value("Tlumienia_655/IloscPozycji", QVariant::fromValue(0)).toInt();
    if (cntPos == 0) {
        listaUstawienFiltra655["0.0"] = UkladFiltrow{0,0,0};
        listaUstawienFiltra655["0.1"] = UkladFiltrow{1,0,0};
        listaUstawienFiltra655["0.2"] = UkladFiltrow{0,1,0};
        listaUstawienFiltra655["0.3"] = UkladFiltrow{1,1,0};
        listaUstawienFiltra655["0.4"] = UkladFiltrow{0,2,0};
        listaUstawienFiltra655["0.5"] = UkladFiltrow{0,3,0};
        listaUstawienFiltra655["0.6"] = UkladFiltrow{1,3,0};
        listaUstawienFiltra655["0.7"] = UkladFiltrow{2,3,0};
        listaUstawienFiltra655["0.8"] = UkladFiltrow{3,3,0};
        listaUstawienFiltra655["0.9"] = UkladFiltrow{4,3,0};
        listaUstawienFiltra655["1.0"] = UkladFiltrow{0,0,1};
        listaUstawienFiltra655["1.2"] = UkladFiltrow{0,1,1};
        listaUstawienFiltra655["1.4"] = UkladFiltrow{0,2,1};
        listaUstawienFiltra655["1.6"] = UkladFiltrow{2,2,1};
        listaUstawienFiltra655["1.8"] = UkladFiltrow{4,2,1};
        listaUstawienFiltra655["2.0"] = UkladFiltrow{0,0,2};
        listaUstawienFiltra655["2.3"] = UkladFiltrow{1,1,2};
        listaUstawienFiltra655["2.6"] = UkladFiltrow{2,2,2};
        listaUstawienFiltra655["2.9"] = UkladFiltrow{4,3,2};
        listaUstawienFiltra655["3.2"] = UkladFiltrow{0,1,3};
        listaUstawienFiltra655["3.5"] = UkladFiltrow{3,1,3};
        listaUstawienFiltra655["4.0"] = UkladFiltrow{0,0,4};
        listaUstawienFiltra655["4.4"] = UkladFiltrow{0,2,4};
        listaUstawienFiltra655["4.8"] = UkladFiltrow{4,2,4};
        listaUstawienFiltra655["5.2"] = UkladFiltrow{0,1,5};
        listaUstawienFiltra655["5.6"] = UkladFiltrow{4,1,5};
        listaUstawienFiltra655["6.0"] = UkladFiltrow{0,4,0};
        listaUstawienFiltra655["7.0"] = UkladFiltrow{0,4,1};
        listaUstawienFiltra655["8.0"] = UkladFiltrow{0,4,2};
        listaUstawienFiltra655["9.0"] = UkladFiltrow{0,4,3};
        listaUstawienFiltra655["10.0"] = UkladFiltrow{0,4,3};
        listaUstawienFiltra655["11.0"] = UkladFiltrow{0,4,5};
        listaUstawienFiltra655["12.0"] = UkladFiltrow{0,4,5};
        listaUstawienFiltra655["13.0"] = UkladFiltrow{5,0,0};
        listaUstawienFiltra655["14.0"] = UkladFiltrow{5,0,1};
    } else {
        for (short p = 0; p < cntPos; ++p)
        {
            QString k = QString("Tlumienia_655/pos%1").arg((int)(p+1),3,10,QChar('0'));
            QString tl = settings.value(k, QVariant::fromValue(QString())).toString();
            if (tl.isEmpty())
                continue;
            auto r = tl.split(QChar('@'));
            if (r.size() != 2)
                continue;
            auto key = r.at(0);
            auto s = r.at(1).split(QChar('+'));
            if (s.size() != 3)
                continue;
            bool ok;
            short a = s.at(0).toShort(&ok);
            if (!ok || a < 0 || a > 5)
                continue;
            short b = s.at(1).toShort(&ok);
            if (!ok || b < 0 || b > 5)
                continue;
            short c = s.at(2).toShort(&ok);
            if (!ok || c < 0 || c > 5)
                continue;
            listaUstawienFiltra655[key] = UkladFiltrow{a,b,c};
        }
    }

    cntPos = settings.value("Tlumienia_880/IloscPozycji", QVariant::fromValue(0)).toInt();
    if (cntPos == 0) {
        listaUstawienFiltra880["0.0"] = UkladFiltrow{0,0,0};
        listaUstawienFiltra880["0.1"] = UkladFiltrow{1,0,0};
        listaUstawienFiltra880["0.2"] = UkladFiltrow{0,1,0};
        listaUstawienFiltra880["0.3"] = UkladFiltrow{1,1,0};
        listaUstawienFiltra880["0.4"] = UkladFiltrow{0,2,0};
        listaUstawienFiltra880["0.5"] = UkladFiltrow{0,3,0};
        listaUstawienFiltra880["0.6"] = UkladFiltrow{1,3,0};
        listaUstawienFiltra880["0.7"] = UkladFiltrow{2,3,0};
        listaUstawienFiltra880["0.8"] = UkladFiltrow{3,3,0};
        listaUstawienFiltra880["0.9"] = UkladFiltrow{4,3,0};
        listaUstawienFiltra880["1.0"] = UkladFiltrow{0,0,1};
        listaUstawienFiltra880["1.2"] = UkladFiltrow{0,1,1};
        listaUstawienFiltra880["1.4"] = UkladFiltrow{0,2,1};
        listaUstawienFiltra880["1.6"] = UkladFiltrow{2,2,1};
        listaUstawienFiltra880["1.8"] = UkladFiltrow{4,2,1};
        listaUstawienFiltra880["2.0"] = UkladFiltrow{0,0,2};
        listaUstawienFiltra880["2.3"] = UkladFiltrow{1,1,2};
        listaUstawienFiltra880["2.6"] = UkladFiltrow{2,2,2};
        listaUstawienFiltra880["2.9"] = UkladFiltrow{4,3,2};
        listaUstawienFiltra880["3.2"] = UkladFiltrow{0,1,3};
        listaUstawienFiltra880["3.5"] = UkladFiltrow{3,1,3};
        listaUstawienFiltra880["4.0"] = UkladFiltrow{0,0,4};
        listaUstawienFiltra880["4.4"] = UkladFiltrow{0,2,4};
        listaUstawienFiltra880["4.8"] = UkladFiltrow{4,2,4};
        listaUstawienFiltra880["5.2"] = UkladFiltrow{0,1,5};
        listaUstawienFiltra880["5.6"] = UkladFiltrow{4,1,5};
        listaUstawienFiltra880["6.0"] = UkladFiltrow{0,4,0};
        listaUstawienFiltra880["7.0"] = UkladFiltrow{0,4,1};
        listaUstawienFiltra880["8.0"] = UkladFiltrow{0,4,2};
        listaUstawienFiltra880["9.0"] = UkladFiltrow{0,4,3};
        listaUstawienFiltra880["10.0"] = UkladFiltrow{0,4,3};
        listaUstawienFiltra880["11.0"] = UkladFiltrow{0,4,5};
        listaUstawienFiltra880["12.0"] = UkladFiltrow{0,4,5};
        listaUstawienFiltra880["13.0"] = UkladFiltrow{5,0,0};
        listaUstawienFiltra880["14.0"] = UkladFiltrow{5,0,1};
    } else {
        for (short p = 0; p < cntPos; ++p)
        {
            QString k = QString("Tlumienia_655/pos%1").arg((int)(p+1),3,10,QChar('0'));
            QString tl = settings.value(k, QVariant::fromValue(QString())).toString();
            if (tl.isEmpty())
                continue;
            auto r = tl.split(QChar('@'));
            if (r.size() != 2)
                continue;
            auto key = r.at(0);
            auto s = r.at(1).split(QChar('+'));
            if (s.size() != 3)
                continue;
            bool ok;
            short a = s.at(0).toShort(&ok);
            if (!ok || a < 0 || a > 5)
                continue;
            short b = s.at(1).toShort(&ok);
            if (!ok || b < 0 || b > 5)
                continue;
            short c = s.at(2).toShort(&ok);
            if (!ok || c < 0 || c > 5)
                continue;
            listaUstawienFiltra880[key] = UkladFiltrow{a,b,c};
        }
    }
}

void Ustawienia::saveListUstawienFiltra()
{
    {
        auto keys = listaUstawienFiltra655.keys();
        std::sort(keys.begin(), keys.end(), [](const QString &v1, const QString &v2)
        {
            return v1.toDouble() < v2.toDouble();
        });
        settings.setValue("Tlumienia_655/IloscPozycji", QVariant::fromValue(keys.size()));
        settings.setValue("Tlumienia_655/pos000", QVariant::fromValue(QString("VAL@A+B+C")));
        for (short p = 0; p < keys.size(); ++p)
        {
            QString k = keys[p];
            auto v = listaUstawienFiltra655[k];
            QString val = QString("%1@%2+%3+%4").arg(k).arg(v.A).arg(v.B).arg(v.C);
            QString key = QString("Tlumienia_655/pos%1").arg((int)(p+1),3,10,QChar('0'));
            settings.setValue(key, QVariant::fromValue(val));
        }
    }
    {
        auto keys = listaUstawienFiltra880.keys();
        std::sort(keys.begin(), keys.end(), [](const QString &v1, const QString &v2)
        {
            return v1.toDouble() < v2.toDouble();
        });
        settings.setValue("Tlumienia_880/IloscPozycji", QVariant::fromValue(keys.size()));
        settings.setValue("Tlumienia_880/pos000", QVariant::fromValue(QString("VAL@A+B+C")));
        for (short p = 0; p < keys.size(); ++p)
        {
            QString k = keys[p];
            auto v = listaUstawienFiltra880[k];
            QString val = QString("%1@%2+%3+%4").arg(k).arg(v.A).arg(v.B).arg(v.C);
            QString key = QString("Tlumienia_880/pos%1").arg((int)(p+1),3,10,QChar('0'));
            settings.setValue(key, QVariant::fromValue(val));
        }
    }
}

QList<QStringList> Ustawienia::getTlumienia655() const
{
    QList<QStringList> ret;
    auto keys = listaUstawienFiltra655.keys();
    std::sort(keys.begin(), keys.end(), [](const QString &v1, const QString &v2)
    {
        return v1.toDouble() < v2.toDouble();
    });
    for (short p = 0; p < keys.size(); ++p)
    {
        QStringList record;
        QString k = keys[p];
        auto v = listaUstawienFiltra655[k];
        double suma = getFiltr_db_655('A', v.A) + getFiltr_db_655('B', v.B) + getFiltr_db_655('C', v.C);
        record << k << QString::number(v.A) << QString::number(v.B) << QString::number(v.C) << QString::number(suma, 'f', 2);
        ret << record;
    }
    return ret;
}


QList<QStringList> Ustawienia::getTlumienia880() const
{
    QList<QStringList> ret;
    auto keys = listaUstawienFiltra880.keys();
    std::sort(keys.begin(), keys.end(), [](const QString &v1, const QString &v2)
    {
        return v1.toDouble() < v2.toDouble();
    });
    for (short p = 0; p < keys.size(); ++p)
    {
        QStringList record;
        QString k = keys[p];
        auto v = listaUstawienFiltra880[k];
        double suma = getFiltr_db_880('A', v.A) + getFiltr_db_880('B', v.B) + getFiltr_db_880('C', v.C);
        record << k << QString::number(v.A) << QString::number(v.B) << QString::number(v.C) << QString::number(suma, 'f', 2);
        ret << record;
    }
    return ret;
}
