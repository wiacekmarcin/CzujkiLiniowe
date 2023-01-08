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
    , speedZerFA(100000)
    , speedZerFB(100000)
    , speedZerFC(100000)
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

void SterownikFiltrow::setUstawienia(Sterownik *sd_, const Ustawienia &u)
{
    sd = sd_;
    impPosFA = round(stPerPosImp * u.getMotorMaksIloscImp(nrSilnikFA));
    impPosFB = round(stPerPosImp * u.getMotorMaksIloscImp(nrSilnikFB));
    impPosFC = round(stPerPosImp * u.getMotorMaksIloscImp(nrSilnikFC));

    speedFA = u.getMotorCzasMiedzyImpNormal(nrSilnikFA);
    speedFB = u.getMotorCzasMiedzyImpNormal(nrSilnikFB);
    speedFC = u.getMotorCzasMiedzyImpNormal(nrSilnikFC);

    speedZerFA = u.getMotorCzasMiedzyImpZerow(nrSilnikFA);
    speedZerFB = u.getMotorCzasMiedzyImpZerow(nrSilnikFB);
    speedZerFC = u.getMotorCzasMiedzyImpZerow(nrSilnikFC);
}

void SterownikFiltrow::setPos(unsigned short pA, unsigned short pB, unsigned short pC)
{
    qDebug() << __FILE__ << __LINE__ << "Uklad Filtrow" << pA << pB << pC;
    if (sd == nullptr)
        return;
    do {
        QMutexLocker lock(&mutex);
        fARuch = pA != actPosfA;
        fBRuch = pB != actPosfB;
        fCRuch = pC != actPosfC;
    } while (false);
    if (pA != actPosfA) {
        qDebug() << "Ustawiam silnik A na " << pA;
        sd->setPositionSilnik(nrSilnikFA, false, pA*impPosFA, speedFA);
        actPosfA = pA;
    }
    if (pB != actPosfB) {
        qDebug() << "Ustawiam silnik B na " << pB;
        sd->setPositionSilnik(nrSilnikFB, false, pB*impPosFB, speedFB);
        actPosfB = pB;
    }
    if (pC != actPosfC) {
        qDebug() << "Ustawiam silnik C na " << pC;
        sd->setPositionSilnik(nrSilnikFC, false, pC*impPosFC, speedFC);
        actPosfC = pC;
    }

}

void SterownikFiltrow::setZero()
{
    qDebug() << __FILE__ << __LINE__ << "Zerowanie";
    if (sd == nullptr)
        return;
    do {
        QMutexLocker lock(&mutex);
        fARuch = true;
        fBRuch = true;
        fCRuch = true;
    } while (false);
    qDebug() << "setPosition";
    sd->setPositionSilnik(nrSilnikFA, true, 0, speedZerFA);
    sd->setPositionSilnik(nrSilnikFB, true, 0, speedZerFB);
    sd->setPositionSilnik(nrSilnikFC, true, 0, speedZerFC);
    actPosfA = actPosfB = actPosfC = 0;
}

void SterownikFiltrow::setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt)
{
    if (silnik != nrSilnikFA && silnik != nrSilnikFB && silnik != nrSilnikFC)
        return;

    qDebug() << "Filtr " << (silnik == 3 ? "A" : (silnik == 4 ? "B" : "C")) << (home ? "wyzerowany" : "ustawiony") << "Blad" << error;
    if (error) {
        QMutexLocker lock(&mutex);
        fARuch = false;
        fBRuch = false;
        fCRuch = false;
        emit bladFiltrow(silnik, home);
        return ;
    }

    if (silnik == nrSilnikFA) {
        QMutexLocker lock(&mutex);
        fARuch = move;
    } else if (silnik == nrSilnikFB) {
        QMutexLocker lock(&mutex);
        fBRuch = move;
    } else if (silnik == nrSilnikFC) {
        QMutexLocker lock(&mutex);
        fCRuch = move;
    }

    if (!isRuch()) {
        if (home) {
            qDebug() << "Zerowanie fitrow zakonczone";
            emit zerowanieFiltrowDone();
        } else {
            qDebug() << "Pozycjonowanie filtrow zakonczone";
            emit setUkladFiltrowDone();
        }
    }
}

