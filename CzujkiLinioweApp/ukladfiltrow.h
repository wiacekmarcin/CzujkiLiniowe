#ifndef UKLADFILTROW_H
#define UKLADFILTROW_H

#include <QObject>
#include <QMutex>
#include "ustawienia.h"

class Sterownik;

class SterownikFiltrow : public QObject
{
    Q_OBJECT
public:
    explicit SterownikFiltrow(QObject *parent = nullptr);

    bool getFARuch() const;
    void setFARuch(bool newFARuch);

    bool isRuch();
    void setUstawienia(const Ustawienia & u);

public slots:
    void setPos(unsigned short pA, unsigned short pB, unsigned short pC);
    void setZero();
    void setPositionDone(short silnik, bool home, bool success, bool move);
signals:
    void zerowanieFiltrowDone();
    void setUkladFiltrowDone();
    void bladFiltrow(short silnik, bool zerowanie);
private :
    bool fARuch;
    bool fBRuch;
    bool fCRuch;

    unsigned short actPosfA;
    unsigned short actPosfB;
    unsigned short actPosfC;

    static constexpr short maxPosF = 6;
    static constexpr float stPerPosImp = 1 / maxPosF;
    static constexpr short nrSilnikFA = 3;
    static constexpr short nrSilnikFB = 4;
    static constexpr short nrSilnikFC = 5;


    unsigned long impPosFA;
    unsigned long impPosFB;
    unsigned long impPosFC;

    unsigned long speedFA;
    unsigned long speedFB;
    unsigned long speedFC;

    Sterownik *sd;
    QMutex mutex;
};

#endif // UKLADFILTROW_H
