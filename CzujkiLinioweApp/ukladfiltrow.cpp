#include "ukladfiltrow.h"
#include "sterownik.h"
#include <QMutexLocker>

SterownikFiltrow::SterownikFiltrow(QObject *parent)
    : QObject{parent}
    , fARuch(false)
    , fBRuch(false)
    , fCRuch(false)
    , actPosfA(0)
    , actPosfB(0)
    , actPosfC(0)
    , impPosFA(0)
    , impPosFB(0)
    , impPosFC(0)
    , speedFA(100000)
    , speedFB(100000)
    , speedFC(100000)
    , sd(nullptr)
{

}

bool SterownikFiltrow::isRuch()
{
    QMutexLocker lock(&mutex);
    if (fARuch)
        return true;
    if (fBRuch)
        return true;
    if (fCRuch)
        return true;
    return false;
}

void SterownikFiltrow::setUstawienia(const Ustawienia &u)
{
    impPosFA = round(stPerPosImp * u.getMotorMaksIloscImp(nrSilnikFA));
    impPosFB = round(stPerPosImp * u.getMotorMaksIloscImp(nrSilnikFA));
    impPosFC = round(stPerPosImp * u.getMotorMaksIloscImp(nrSilnikFA));

    speedFA = u.getMotorCzasMiedzyImpNormal(nrSilnikFA);
    speedFB = u.getMotorCzasMiedzyImpNormal(nrSilnikFB);
    speedFC = u.getMotorCzasMiedzyImpNormal(nrSilnikFC);
}

void SterownikFiltrow::setPos(unsigned short pA, unsigned short pB, unsigned short pC)
{
    if (sd == nullptr)
        return;
    do {
        QMutexLocker lock(&mutex);
        fARuch = pA != actPosfA;
        fBRuch = pB != actPosfB;
        fCRuch = pC != actPosfC;
    } while (false);
    if (pA != actPosfA) {
        sd->setPositionSilnik(nrSilnikFA, false, pA*impPosFA, speedFA);
    }
    if (pB != actPosfB) {
        sd->setPositionSilnik(nrSilnikFB, false, pB*impPosFB, speedFB);
    }
    if (pC != actPosfC) {
        sd->setPositionSilnik(nrSilnikFC, false, pC*impPosFC, speedFC);
    }

}

void SterownikFiltrow::setZero()
{
    if (sd == nullptr)
        return;
    do {
        QMutexLocker lock(&mutex);
        fARuch = true;
        fBRuch = true;
        fCRuch = true;
    } while (false);
    sd->setPositionSilnik(nrSilnikFA, true, 0, speedFA);
    sd->setPositionSilnik(nrSilnikFB, true, 0, speedFB);
    sd->setPositionSilnik(nrSilnikFC, true, 0, speedFC);
}

void SterownikFiltrow::setPositionDone(short silnik, bool home, bool success, unsigned int steps)
{
    (void)steps;
    if (silnik != nrSilnikFA && silnik != nrSilnikFB && silnik != nrSilnikFC)
        return;

    if (!success) {
        QMutexLocker lock(&mutex);
        fARuch = false;
        fBRuch = false;
        fCRuch = false;
        emit bladFiltrow(silnik, home);
        return ;
    }


    if (silnik == nrSilnikFA) {
        QMutexLocker lock(&mutex);
        fARuch = false;
    } else if (silnik == nrSilnikFB) {
        QMutexLocker lock(&mutex);
        fBRuch = false;
    } else if (silnik == nrSilnikFC) {
        QMutexLocker lock(&mutex);
        fCRuch = false;
    }

    if (!isRuch()) {
        if (home)
            emit zerowanieFiltrowDone();
        else
            emit setUkladFiltrowDone();
    }
}

