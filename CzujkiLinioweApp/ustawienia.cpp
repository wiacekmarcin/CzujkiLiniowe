#include "ustawienia.h"

char Ustawienia::appnazwa[]="CzujkiLiniowe";
char Ustawienia::firmnazwa[]="Kolodziejczyk";

Ustawienia::Ustawienia() :
    settings(Ustawienia::firmnazwa, Ustawienia::appnazwa)
{
    read();
}

void Ustawienia::read()
{
    m_ratioSilnik1 = settings.value("Ratio_1", m_ratioSilnik1).toString();
    m_ratioSilnik2 = settings.value("Ratio_2", m_ratioSilnik2).toString();
    m_ratioSilnik3 = settings.value("Ratio_3", m_ratioSilnik3).toString();
    m_ratioSilnik4 = settings.value("Ratio_4", m_ratioSilnik4).toString();
    m_ratioSilnik5 = settings.value("Ratio_5", m_ratioSilnik5).toString();
    m_ratioSilnik6 = settings.value("Ratio_6", m_ratioSilnik6).toString();
    m_ratioSilnik7 = settings.value("Ratio_7", m_ratioSilnik7).toString();
    m_ratioSilnik8 = settings.value("Ratio_8", m_ratioSilnik8).toString();
    m_ratioSilnik9 = settings.value("Ratio_9", m_ratioSilnik9).toString();
    m_maxStepsSilnik1 = settings.value("MaxSteps_1", m_maxStepsSilnik1).toString();
    m_maxStepsSilnik2 = settings.value("MaxSteps_2", m_maxStepsSilnik2).toString();
    m_maxStepsSilnik3 = settings.value("MaxSteps_3", m_maxStepsSilnik3).toString();
    m_maxStepsSilnik4 = settings.value("MaxSteps_4", m_maxStepsSilnik4).toString();
    m_maxStepsSilnik5 = settings.value("MaxSteps_5", m_maxStepsSilnik5).toString();
    m_maxStepsSilnik6 = settings.value("MaxSteps_6", m_maxStepsSilnik6).toString();
    m_maxStepsSilnik7 = settings.value("MaxSteps_7", m_maxStepsSilnik7).toString();
    m_maxStepsSilnik8 = settings.value("MaxSteps_8", m_maxStepsSilnik8).toString();
    m_maxStepsSilnik9 = settings.value("MaxSteps_9", m_maxStepsSilnik9).toString();
    m_baseStepsSilnik1 = settings.value("BaseSteps_1", m_baseStepsSilnik1).toString();
    m_baseStepsSilnik2 = settings.value("BaseSteps_2", m_baseStepsSilnik2).toString();
    m_baseStepsSilnik3 = settings.value("BaseSteps_3", m_baseStepsSilnik3).toString();
    m_baseStepsSilnik4 = settings.value("BaseSteps_4", m_baseStepsSilnik4).toString();
    m_baseStepsSilnik5 = settings.value("BaseSteps_5", m_baseStepsSilnik5).toString();
    m_baseStepsSilnik6 = settings.value("BaseSteps_6", m_baseStepsSilnik6).toString();
    m_baseStepsSilnik7 = settings.value("BaseSteps_7", m_baseStepsSilnik7).toString();
    m_baseStepsSilnik8 = settings.value("BaseSteps_8", m_baseStepsSilnik8).toString();
    m_baseStepsSilnik9 = settings.value("BaseSteps_9", m_baseStepsSilnik9).toString();
    m_delayusSilnik1 = settings.value("Delayus_1", m_delayusSilnik1).toString();
    m_delayusSilnik2 = settings.value("Delayus_2", m_delayusSilnik2).toString();
    m_delayusSilnik3 = settings.value("Delayus_3", m_delayusSilnik3).toString();
    m_delayusSilnik4 = settings.value("Delayus_4", m_delayusSilnik4).toString();
    m_delayusSilnik5 = settings.value("Delayus_5", m_delayusSilnik5).toString();
    m_delayusSilnik6 = settings.value("Delayus_6", m_delayusSilnik6).toString();
    m_delayusSilnik7 = settings.value("Delayus_7", m_delayusSilnik7).toString();
    m_delayusSilnik8 = settings.value("Delayus_8", m_delayusSilnik8).toString();
    m_delayusSilnik9 = settings.value("Delayus_9", m_delayusSilnik9).toString();
    m_leftRotationSilnik1 = settings.value("LeftRotation_1", m_leftRotationSilnik1).toBool();
    m_leftRotationSilnik2 = settings.value("LeftRotation_2", m_leftRotationSilnik2).toBool();
    m_leftRotationSilnik3 = settings.value("LeftRotation_3", m_leftRotationSilnik3).toBool();
    m_leftRotationSilnik4 = settings.value("LeftRotation_4", m_leftRotationSilnik4).toBool();
    m_leftRotationSilnik5 = settings.value("LeftRotation_5", m_leftRotationSilnik5).toBool();
    m_leftRotationSilnik6 = settings.value("LeftRotation_6", m_leftRotationSilnik6).toBool();
    m_leftRotationSilnik7 = settings.value("LeftRotation_7", m_leftRotationSilnik7).toBool();
    m_leftRotationSilnik8 = settings.value("LeftRotation_8", m_leftRotationSilnik8).toBool();
    m_leftRotationSilnik9 = settings.value("LeftRotation_9", m_leftRotationSilnik9).toBool();
    m_enableSilnik1 = settings.value("Enable_1", m_enableSilnik1).toBool();
    m_enableSilnik2 = settings.value("Enable_2", m_enableSilnik2).toBool();
    m_enableSilnik3 = settings.value("Enable_3", m_enableSilnik3).toBool();
    m_enableSilnik4 = settings.value("Enable_4", m_enableSilnik4).toBool();
    m_enableSilnik5 = settings.value("Enable_5", m_enableSilnik5).toBool();
    m_enableSilnik6 = settings.value("Enable_6", m_enableSilnik6).toBool();
    m_enableSilnik7 = settings.value("Enable_7", m_enableSilnik7).toBool();
    m_enableSilnik8 = settings.value("Enable_8", m_enableSilnik8).toBool();
    m_enableSilnik9 = settings.value("Enable_9", m_enableSilnik9).toBool();




}

void Ustawienia::write()
{
    settings.setValue("Ratio_1", QVariant::fromValue(m_ratioSilnik1));
    settings.setValue("Ratio_2", QVariant::fromValue(m_ratioSilnik2));
    settings.setValue("Ratio_3", QVariant::fromValue(m_ratioSilnik3));
    settings.setValue("Ratio_4", QVariant::fromValue(m_ratioSilnik4));
    settings.setValue("Ratio_5", QVariant::fromValue(m_ratioSilnik5));
    settings.setValue("Ratio_6", QVariant::fromValue(m_ratioSilnik6));
    settings.setValue("Ratio_7", QVariant::fromValue(m_ratioSilnik7));
    settings.setValue("Ratio_8", QVariant::fromValue(m_ratioSilnik8));
    settings.setValue("Ratio_9", QVariant::fromValue(m_ratioSilnik9));
    settings.setValue("MaxSteps_1", QVariant::fromValue(m_maxStepsSilnik1));
    settings.setValue("MaxSteps_2", QVariant::fromValue(m_maxStepsSilnik2));
    settings.setValue("MaxSteps_3", QVariant::fromValue(m_maxStepsSilnik3));
    settings.setValue("MaxSteps_4", QVariant::fromValue(m_maxStepsSilnik4));
    settings.setValue("MaxSteps_5", QVariant::fromValue(m_maxStepsSilnik5));
    settings.setValue("MaxSteps_6", QVariant::fromValue(m_maxStepsSilnik6));
    settings.setValue("MaxSteps_7", QVariant::fromValue(m_maxStepsSilnik7));
    settings.setValue("MaxSteps_8", QVariant::fromValue(m_maxStepsSilnik8));
    settings.setValue("MaxSteps_9", QVariant::fromValue(m_maxStepsSilnik9));
    settings.setValue("BaseSteps_1", QVariant::fromValue(m_baseStepsSilnik1));
    settings.setValue("BaseSteps_2", QVariant::fromValue(m_baseStepsSilnik2));
    settings.setValue("BaseSteps_3", QVariant::fromValue(m_baseStepsSilnik3));
    settings.setValue("BaseSteps_4", QVariant::fromValue(m_baseStepsSilnik4));
    settings.setValue("BaseSteps_5", QVariant::fromValue(m_baseStepsSilnik5));
    settings.setValue("BaseSteps_6", QVariant::fromValue(m_baseStepsSilnik6));
    settings.setValue("BaseSteps_7", QVariant::fromValue(m_baseStepsSilnik7));
    settings.setValue("BaseSteps_8", QVariant::fromValue(m_baseStepsSilnik8));
    settings.setValue("BaseSteps_9", QVariant::fromValue(m_baseStepsSilnik9));
    settings.setValue("Delayus_1", QVariant::fromValue(m_delayusSilnik1));
    settings.setValue("Delayus_2", QVariant::fromValue(m_delayusSilnik2));
    settings.setValue("Delayus_3", QVariant::fromValue(m_delayusSilnik3));
    settings.setValue("Delayus_4", QVariant::fromValue(m_delayusSilnik4));
    settings.setValue("Delayus_5", QVariant::fromValue(m_delayusSilnik5));
    settings.setValue("Delayus_6", QVariant::fromValue(m_delayusSilnik6));
    settings.setValue("Delayus_7", QVariant::fromValue(m_delayusSilnik7));
    settings.setValue("Delayus_8", QVariant::fromValue(m_delayusSilnik8));
    settings.setValue("Delayus_9", QVariant::fromValue(m_delayusSilnik9));
    settings.setValue("LeftRotation_1", QVariant::fromValue(m_leftRotationSilnik1));
    settings.setValue("LeftRotation_2", QVariant::fromValue(m_leftRotationSilnik2));
    settings.setValue("LeftRotation_3", QVariant::fromValue(m_leftRotationSilnik3));
    settings.setValue("LeftRotation_4", QVariant::fromValue(m_leftRotationSilnik4));
    settings.setValue("LeftRotation_5", QVariant::fromValue(m_leftRotationSilnik5));
    settings.setValue("LeftRotation_6", QVariant::fromValue(m_leftRotationSilnik6));
    settings.setValue("LeftRotation_7", QVariant::fromValue(m_leftRotationSilnik7));
    settings.setValue("LeftRotation_8", QVariant::fromValue(m_leftRotationSilnik8));
    settings.setValue("LeftRotation_9", QVariant::fromValue(m_leftRotationSilnik9));
    settings.setValue("Enable_1", QVariant::fromValue(m_enableSilnik1));
    settings.setValue("Enable_2", QVariant::fromValue(m_enableSilnik2));
    settings.setValue("Enable_3", QVariant::fromValue(m_enableSilnik3));
    settings.setValue("Enable_4", QVariant::fromValue(m_enableSilnik4));
    settings.setValue("Enable_5", QVariant::fromValue(m_enableSilnik5));
    settings.setValue("Enable_6", QVariant::fromValue(m_enableSilnik6));
    settings.setValue("Enable_7", QVariant::fromValue(m_enableSilnik7));
    settings.setValue("Enable_8", QVariant::fromValue(m_enableSilnik8));
    settings.setValue("Enable_9", QVariant::fromValue(m_enableSilnik9));

}

QString Ustawienia::getRatioSilnik(uint key) const
{
    switch(key) {
    case 1 : return getRatioSilnik1();
    case 2 : return getRatioSilnik2();
    case 3 : return getRatioSilnik3();
    case 4 : return getRatioSilnik4();
    case 5 : return getRatioSilnik5();
    case 6 : return getRatioSilnik6();
    case 7 : return getRatioSilnik7();
    case 8 : return getRatioSilnik8();
    case 9 : return getRatioSilnik9();
    default: return QString();
    }
}

void Ustawienia::setRatioSilnik(uint key, const QString & value)
{
    switch(key) {
    case 1 : setRatioSilnik1(value); break;
    case 2 : setRatioSilnik2(value); break;
    case 3 : setRatioSilnik3(value); break;
    case 4 : setRatioSilnik4(value); break;
    case 5 : setRatioSilnik5(value); break;
    case 6 : setRatioSilnik6(value); break;
    case 7 : setRatioSilnik7(value); break;
    case 8 : setRatioSilnik8(value); break;
    case 9 : setRatioSilnik9(value); break;
    default: break;
    }
}

QString Ustawienia::getRatioSilnik1() const
{
    return m_ratioSilnik1;
}

void Ustawienia::setRatioSilnik1(const QString & value)
{
    m_ratioSilnik1 = value;
    settings.setValue("Ratio_1", QVariant::fromValue(m_ratioSilnik1));
}

QString Ustawienia::getRatioSilnik2() const
{
    return m_ratioSilnik2;
}

void Ustawienia::setRatioSilnik2(const QString & value)
{
    m_ratioSilnik2 = value;
    settings.setValue("Ratio_2", QVariant::fromValue(m_ratioSilnik2));
}

QString Ustawienia::getRatioSilnik3() const
{
    return m_ratioSilnik3;
}

void Ustawienia::setRatioSilnik3(const QString & value)
{
    m_ratioSilnik3 = value;
    settings.setValue("Ratio_3", QVariant::fromValue(m_ratioSilnik3));
}

QString Ustawienia::getRatioSilnik4() const
{
    return m_ratioSilnik4;
}

void Ustawienia::setRatioSilnik4(const QString & value)
{
    m_ratioSilnik4 = value;
    settings.setValue("Ratio_4", QVariant::fromValue(m_ratioSilnik4));
}

QString Ustawienia::getRatioSilnik5() const
{
    return m_ratioSilnik5;
}

void Ustawienia::setRatioSilnik5(const QString & value)
{
    m_ratioSilnik5 = value;
    settings.setValue("Ratio_5", QVariant::fromValue(m_ratioSilnik5));
}

QString Ustawienia::getRatioSilnik6() const
{
    return m_ratioSilnik6;
}

void Ustawienia::setRatioSilnik6(const QString & value)
{
    m_ratioSilnik6 = value;
    settings.setValue("Ratio_6", QVariant::fromValue(m_ratioSilnik6));
}

QString Ustawienia::getRatioSilnik7() const
{
    return m_ratioSilnik7;
}

void Ustawienia::setRatioSilnik7(const QString & value)
{
    m_ratioSilnik7 = value;
    settings.setValue("Ratio_7", QVariant::fromValue(m_ratioSilnik7));
}

QString Ustawienia::getRatioSilnik8() const
{
    return m_ratioSilnik8;
}

void Ustawienia::setRatioSilnik8(const QString & value)
{
    m_ratioSilnik8 = value;
    settings.setValue("Ratio_8", QVariant::fromValue(m_ratioSilnik8));
}

QString Ustawienia::getRatioSilnik9() const
{
    return m_ratioSilnik9;
}

void Ustawienia::setRatioSilnik9(const QString & value)
{
    m_ratioSilnik9 = value;
    settings.setValue("Ratio_9", QVariant::fromValue(m_ratioSilnik9));
}

QString Ustawienia::getMaxStepsSilnik(uint key) const
{
    switch(key) {
    case 1 : return getMaxStepsSilnik1();
    case 2 : return getMaxStepsSilnik2();
    case 3 : return getMaxStepsSilnik3();
    case 4 : return getMaxStepsSilnik4();
    case 5 : return getMaxStepsSilnik5();
    case 6 : return getMaxStepsSilnik6();
    case 7 : return getMaxStepsSilnik7();
    case 8 : return getMaxStepsSilnik8();
    case 9 : return getMaxStepsSilnik9();
    default: return QString();
    }
}

void Ustawienia::setMaxStepsSilnik(uint key, const QString & value)
{
    switch(key) {
    case 1 : setMaxStepsSilnik1(value); break;
    case 2 : setMaxStepsSilnik2(value); break;
    case 3 : setMaxStepsSilnik3(value); break;
    case 4 : setMaxStepsSilnik4(value); break;
    case 5 : setMaxStepsSilnik5(value); break;
    case 6 : setMaxStepsSilnik6(value); break;
    case 7 : setMaxStepsSilnik7(value); break;
    case 8 : setMaxStepsSilnik8(value); break;
    case 9 : setMaxStepsSilnik9(value); break;
    default: break;
    }
}

QString Ustawienia::getMaxStepsSilnik1() const
{
    return m_maxStepsSilnik1;
}

void Ustawienia::setMaxStepsSilnik1(const QString & value)
{
    m_maxStepsSilnik1 = value;
    settings.setValue("MaxSteps_1", QVariant::fromValue(m_maxStepsSilnik1));
}

QString Ustawienia::getMaxStepsSilnik2() const
{
    return m_maxStepsSilnik2;
}

void Ustawienia::setMaxStepsSilnik2(const QString & value)
{
    m_maxStepsSilnik2 = value;
    settings.setValue("MaxSteps_2", QVariant::fromValue(m_maxStepsSilnik2));
}

QString Ustawienia::getMaxStepsSilnik3() const
{
    return m_maxStepsSilnik3;
}

void Ustawienia::setMaxStepsSilnik3(const QString & value)
{
    m_maxStepsSilnik3 = value;
    settings.setValue("MaxSteps_3", QVariant::fromValue(m_maxStepsSilnik3));
}

QString Ustawienia::getMaxStepsSilnik4() const
{
    return m_maxStepsSilnik4;
}

void Ustawienia::setMaxStepsSilnik4(const QString & value)
{
    m_maxStepsSilnik4 = value;
    settings.setValue("MaxSteps_4", QVariant::fromValue(m_maxStepsSilnik4));
}

QString Ustawienia::getMaxStepsSilnik5() const
{
    return m_maxStepsSilnik5;
}

void Ustawienia::setMaxStepsSilnik5(const QString & value)
{
    m_maxStepsSilnik5 = value;
    settings.setValue("MaxSteps_5", QVariant::fromValue(m_maxStepsSilnik5));
}

QString Ustawienia::getMaxStepsSilnik6() const
{
    return m_maxStepsSilnik6;
}

void Ustawienia::setMaxStepsSilnik6(const QString & value)
{
    m_maxStepsSilnik6 = value;
    settings.setValue("MaxSteps_6", QVariant::fromValue(m_maxStepsSilnik6));
}

QString Ustawienia::getMaxStepsSilnik7() const
{
    return m_maxStepsSilnik7;
}

void Ustawienia::setMaxStepsSilnik7(const QString & value)
{
    m_maxStepsSilnik7 = value;
    settings.setValue("MaxSteps_7", QVariant::fromValue(m_maxStepsSilnik7));
}

QString Ustawienia::getMaxStepsSilnik8() const
{
    return m_maxStepsSilnik8;
}

void Ustawienia::setMaxStepsSilnik8(const QString & value)
{
    m_maxStepsSilnik8 = value;
    settings.setValue("MaxSteps_8", QVariant::fromValue(m_maxStepsSilnik8));
}

QString Ustawienia::getMaxStepsSilnik9() const
{
    return m_maxStepsSilnik9;
}

void Ustawienia::setMaxStepsSilnik9(const QString & value)
{
    m_maxStepsSilnik9 = value;
    settings.setValue("MaxSteps_9", QVariant::fromValue(m_maxStepsSilnik9));
}

QString Ustawienia::getBaseStepsSilnik(uint key) const
{
    switch(key) {
    case 1 : return getBaseStepsSilnik1();
    case 2 : return getBaseStepsSilnik2();
    case 3 : return getBaseStepsSilnik3();
    case 4 : return getBaseStepsSilnik4();
    case 5 : return getBaseStepsSilnik5();
    case 6 : return getBaseStepsSilnik6();
    case 7 : return getBaseStepsSilnik7();
    case 8 : return getBaseStepsSilnik8();
    case 9 : return getBaseStepsSilnik9();
    default: return QString();
    }
}

void Ustawienia::setBaseStepsSilnik(uint key, const QString & value)
{
    switch(key) {
    case 1 : setBaseStepsSilnik1(value); break;
    case 2 : setBaseStepsSilnik2(value); break;
    case 3 : setBaseStepsSilnik3(value); break;
    case 4 : setBaseStepsSilnik4(value); break;
    case 5 : setBaseStepsSilnik5(value); break;
    case 6 : setBaseStepsSilnik6(value); break;
    case 7 : setBaseStepsSilnik7(value); break;
    case 8 : setBaseStepsSilnik8(value); break;
    case 9 : setBaseStepsSilnik9(value); break;
    default: break;
    }
}

QString Ustawienia::getBaseStepsSilnik1() const
{
    return m_baseStepsSilnik1;
}

void Ustawienia::setBaseStepsSilnik1(const QString & value)
{
    m_baseStepsSilnik1 = value;
    settings.setValue("BaseSteps_1", QVariant::fromValue(m_baseStepsSilnik1));
}

QString Ustawienia::getBaseStepsSilnik2() const
{
    return m_baseStepsSilnik2;
}

void Ustawienia::setBaseStepsSilnik2(const QString & value)
{
    m_baseStepsSilnik2 = value;
    settings.setValue("BaseSteps_2", QVariant::fromValue(m_baseStepsSilnik2));
}

QString Ustawienia::getBaseStepsSilnik3() const
{
    return m_baseStepsSilnik3;
}

void Ustawienia::setBaseStepsSilnik3(const QString & value)
{
    m_baseStepsSilnik3 = value;
    settings.setValue("BaseSteps_3", QVariant::fromValue(m_baseStepsSilnik3));
}

QString Ustawienia::getBaseStepsSilnik4() const
{
    return m_baseStepsSilnik4;
}

void Ustawienia::setBaseStepsSilnik4(const QString & value)
{
    m_baseStepsSilnik4 = value;
    settings.setValue("BaseSteps_4", QVariant::fromValue(m_baseStepsSilnik4));
}

QString Ustawienia::getBaseStepsSilnik5() const
{
    return m_baseStepsSilnik5;
}

void Ustawienia::setBaseStepsSilnik5(const QString & value)
{
    m_baseStepsSilnik5 = value;
    settings.setValue("BaseSteps_5", QVariant::fromValue(m_baseStepsSilnik5));
}

QString Ustawienia::getBaseStepsSilnik6() const
{
    return m_baseStepsSilnik6;
}

void Ustawienia::setBaseStepsSilnik6(const QString & value)
{
    m_baseStepsSilnik6 = value;
    settings.setValue("BaseSteps_6", QVariant::fromValue(m_baseStepsSilnik6));
}

QString Ustawienia::getBaseStepsSilnik7() const
{
    return m_baseStepsSilnik7;
}

void Ustawienia::setBaseStepsSilnik7(const QString & value)
{
    m_baseStepsSilnik7 = value;
    settings.setValue("BaseSteps_7", QVariant::fromValue(m_baseStepsSilnik7));
}

QString Ustawienia::getBaseStepsSilnik8() const
{
    return m_baseStepsSilnik8;
}

void Ustawienia::setBaseStepsSilnik8(const QString & value)
{
    m_baseStepsSilnik8 = value;
    settings.setValue("BaseSteps_8", QVariant::fromValue(m_baseStepsSilnik8));
}

QString Ustawienia::getBaseStepsSilnik9() const
{
    return m_baseStepsSilnik9;
}

void Ustawienia::setBaseStepsSilnik9(const QString & value)
{
    m_baseStepsSilnik9 = value;
    settings.setValue("BaseSteps_9", QVariant::fromValue(m_baseStepsSilnik9));
}

QString Ustawienia::getDelayusSilnik(uint key) const
{
    switch(key) {
    case 1 : return getDelayusSilnik1();
    case 2 : return getDelayusSilnik2();
    case 3 : return getDelayusSilnik3();
    case 4 : return getDelayusSilnik4();
    case 5 : return getDelayusSilnik5();
    case 6 : return getDelayusSilnik6();
    case 7 : return getDelayusSilnik7();
    case 8 : return getDelayusSilnik8();
    case 9 : return getDelayusSilnik9();
    default: return QString();
    }
}

void Ustawienia::setDelayusSilnik(uint key, const QString & value)
{
    switch(key) {
    case 1 : setDelayusSilnik1(value); break;
    case 2 : setDelayusSilnik2(value); break;
    case 3 : setDelayusSilnik3(value); break;
    case 4 : setDelayusSilnik4(value); break;
    case 5 : setDelayusSilnik5(value); break;
    case 6 : setDelayusSilnik6(value); break;
    case 7 : setDelayusSilnik7(value); break;
    case 8 : setDelayusSilnik8(value); break;
    case 9 : setDelayusSilnik9(value); break;
    default: break;
    }
}

QString Ustawienia::getDelayusSilnik1() const
{
    return m_delayusSilnik1;
}

void Ustawienia::setDelayusSilnik1(const QString & value)
{
    m_delayusSilnik1 = value;
    settings.setValue("Delayus_1", QVariant::fromValue(m_delayusSilnik1));
}

QString Ustawienia::getDelayusSilnik2() const
{
    return m_delayusSilnik2;
}

void Ustawienia::setDelayusSilnik2(const QString & value)
{
    m_delayusSilnik2 = value;
    settings.setValue("Delayus_2", QVariant::fromValue(m_delayusSilnik2));
}

QString Ustawienia::getDelayusSilnik3() const
{
    return m_delayusSilnik3;
}

void Ustawienia::setDelayusSilnik3(const QString & value)
{
    m_delayusSilnik3 = value;
    settings.setValue("Delayus_3", QVariant::fromValue(m_delayusSilnik3));
}

QString Ustawienia::getDelayusSilnik4() const
{
    return m_delayusSilnik4;
}

void Ustawienia::setDelayusSilnik4(const QString & value)
{
    m_delayusSilnik4 = value;
    settings.setValue("Delayus_4", QVariant::fromValue(m_delayusSilnik4));
}

QString Ustawienia::getDelayusSilnik5() const
{
    return m_delayusSilnik5;
}

void Ustawienia::setDelayusSilnik5(const QString & value)
{
    m_delayusSilnik5 = value;
    settings.setValue("Delayus_5", QVariant::fromValue(m_delayusSilnik5));
}

QString Ustawienia::getDelayusSilnik6() const
{
    return m_delayusSilnik6;
}

void Ustawienia::setDelayusSilnik6(const QString & value)
{
    m_delayusSilnik6 = value;
    settings.setValue("Delayus_6", QVariant::fromValue(m_delayusSilnik6));
}

QString Ustawienia::getDelayusSilnik7() const
{
    return m_delayusSilnik7;
}

void Ustawienia::setDelayusSilnik7(const QString & value)
{
    m_delayusSilnik7 = value;
    settings.setValue("Delayus_7", QVariant::fromValue(m_delayusSilnik7));
}

QString Ustawienia::getDelayusSilnik8() const
{
    return m_delayusSilnik8;
}

void Ustawienia::setDelayusSilnik8(const QString & value)
{
    m_delayusSilnik8 = value;
    settings.setValue("Delayus_8", QVariant::fromValue(m_delayusSilnik8));
}

QString Ustawienia::getDelayusSilnik9() const
{
    return m_delayusSilnik9;
}

void Ustawienia::setDelayusSilnik9(const QString & value)
{
    m_delayusSilnik9 = value;
    settings.setValue("Delayus_9", QVariant::fromValue(m_delayusSilnik9));
}

bool Ustawienia::getLeftRotationSilnik(uint key) const
{
    switch(key) {
    case 1 : return getLeftRotationSilnik1();
    case 2 : return getLeftRotationSilnik2();
    case 3 : return getLeftRotationSilnik3();
    case 4 : return getLeftRotationSilnik4();
    case 5 : return getLeftRotationSilnik5();
    case 6 : return getLeftRotationSilnik6();
    case 7 : return getLeftRotationSilnik7();
    case 8 : return getLeftRotationSilnik8();
    case 9 : return getLeftRotationSilnik9();
    default: return false;
    }
}

void Ustawienia::setLeftRotationSilnik(uint key, bool value)
{
    switch(key) {
    case 1 : setLeftRotationSilnik1(value); break;
    case 2 : setLeftRotationSilnik2(value); break;
    case 3 : setLeftRotationSilnik3(value); break;
    case 4 : setLeftRotationSilnik4(value); break;
    case 5 : setLeftRotationSilnik5(value); break;
    case 6 : setLeftRotationSilnik6(value); break;
    case 7 : setLeftRotationSilnik7(value); break;
    case 8 : setLeftRotationSilnik8(value); break;
    case 9 : setLeftRotationSilnik9(value); break;
    default: break;
    }
}

bool Ustawienia::getLeftRotationSilnik1() const
{
    return m_leftRotationSilnik1;
}

void Ustawienia::setLeftRotationSilnik1(bool value)
{
    m_leftRotationSilnik1 = value;
    settings.setValue("LeftRotation_1", QVariant::fromValue(m_leftRotationSilnik1));
}

bool Ustawienia::getLeftRotationSilnik2() const
{
    return m_leftRotationSilnik2;
}

void Ustawienia::setLeftRotationSilnik2(bool value)
{
    m_leftRotationSilnik2 = value;
    settings.setValue("LeftRotation_2", QVariant::fromValue(m_leftRotationSilnik2));
}

bool Ustawienia::getLeftRotationSilnik3() const
{
    return m_leftRotationSilnik3;
}

void Ustawienia::setLeftRotationSilnik3(bool value)
{
    m_leftRotationSilnik3 = value;
    settings.setValue("LeftRotation_3", QVariant::fromValue(m_leftRotationSilnik3));
}

bool Ustawienia::getLeftRotationSilnik4() const
{
    return m_leftRotationSilnik4;
}

void Ustawienia::setLeftRotationSilnik4(bool value)
{
    m_leftRotationSilnik4 = value;
    settings.setValue("LeftRotation_4", QVariant::fromValue(m_leftRotationSilnik4));
}

bool Ustawienia::getLeftRotationSilnik5() const
{
    return m_leftRotationSilnik5;
}

void Ustawienia::setLeftRotationSilnik5(bool value)
{
    m_leftRotationSilnik5 = value;
    settings.setValue("LeftRotation_5", QVariant::fromValue(m_leftRotationSilnik5));
}

bool Ustawienia::getLeftRotationSilnik6() const
{
    return m_leftRotationSilnik6;
}

void Ustawienia::setLeftRotationSilnik6(bool value)
{
    m_leftRotationSilnik6 = value;
    settings.setValue("LeftRotation_6", QVariant::fromValue(m_leftRotationSilnik6));
}

bool Ustawienia::getLeftRotationSilnik7() const
{
    return m_leftRotationSilnik7;
}

void Ustawienia::setLeftRotationSilnik7(bool value)
{
    m_leftRotationSilnik7 = value;
    settings.setValue("LeftRotation_7", QVariant::fromValue(m_leftRotationSilnik7));
}

bool Ustawienia::getLeftRotationSilnik8() const
{
    return m_leftRotationSilnik8;
}

void Ustawienia::setLeftRotationSilnik8(bool value)
{
    m_leftRotationSilnik8 = value;
    settings.setValue("LeftRotation_8", QVariant::fromValue(m_leftRotationSilnik8));
}

bool Ustawienia::getLeftRotationSilnik9() const
{
    return m_leftRotationSilnik9;
}

void Ustawienia::setLeftRotationSilnik9(bool value)
{
    m_leftRotationSilnik9 = value;
    settings.setValue("LeftRotation_9", QVariant::fromValue(m_leftRotationSilnik9));
}

bool Ustawienia::getEnableSilnik(uint key) const
{
    switch(key) {
    case 1 : return getEnableSilnik1();
    case 2 : return getEnableSilnik2();
    case 3 : return getEnableSilnik3();
    case 4 : return getEnableSilnik4();
    case 5 : return getEnableSilnik5();
    case 6 : return getEnableSilnik6();
    case 7 : return getEnableSilnik7();
    case 8 : return getEnableSilnik8();
    case 9 : return getEnableSilnik9();
    default: return false;
    }
}

void Ustawienia::setEnableSilnik(uint key, bool value)
{
    switch(key) {
    case 1 : setEnableSilnik1(value); break;
    case 2 : setEnableSilnik2(value); break;
    case 3 : setEnableSilnik3(value); break;
    case 4 : setEnableSilnik4(value); break;
    case 5 : setEnableSilnik5(value); break;
    case 6 : setEnableSilnik6(value); break;
    case 7 : setEnableSilnik7(value); break;
    case 8 : setEnableSilnik8(value); break;
    case 9 : setEnableSilnik9(value); break;
    default: break;
    }
}

bool Ustawienia::getEnableSilnik1() const
{
    return m_enableSilnik1;
}

void Ustawienia::setEnableSilnik1(bool value)
{
    m_enableSilnik1 = value;
    settings.setValue("Enable_1", QVariant::fromValue(m_enableSilnik1));
}

bool Ustawienia::getEnableSilnik2() const
{
    return m_enableSilnik2;
}

void Ustawienia::setEnableSilnik2(bool value)
{
    m_enableSilnik2 = value;
    settings.setValue("Enable_2", QVariant::fromValue(m_enableSilnik2));
}

bool Ustawienia::getEnableSilnik3() const
{
    return m_enableSilnik3;
}

void Ustawienia::setEnableSilnik3(bool value)
{
    m_enableSilnik3 = value;
    settings.setValue("Enable_3", QVariant::fromValue(m_enableSilnik3));
}

bool Ustawienia::getEnableSilnik4() const
{
    return m_enableSilnik4;
}

void Ustawienia::setEnableSilnik4(bool value)
{
    m_enableSilnik4 = value;
    settings.setValue("Enable_4", QVariant::fromValue(m_enableSilnik4));
}

bool Ustawienia::getEnableSilnik5() const
{
    return m_enableSilnik5;
}

void Ustawienia::setEnableSilnik5(bool value)
{
    m_enableSilnik5 = value;
    settings.setValue("Enable_5", QVariant::fromValue(m_enableSilnik5));
}

bool Ustawienia::getEnableSilnik6() const
{
    return m_enableSilnik6;
}

void Ustawienia::setEnableSilnik6(bool value)
{
    m_enableSilnik6 = value;
    settings.setValue("Enable_6", QVariant::fromValue(m_enableSilnik6));
}

bool Ustawienia::getEnableSilnik7() const
{
    return m_enableSilnik7;
}

void Ustawienia::setEnableSilnik7(bool value)
{
    m_enableSilnik7 = value;
    settings.setValue("Enable_7", QVariant::fromValue(m_enableSilnik7));
}

bool Ustawienia::getEnableSilnik8() const
{
    return m_enableSilnik8;
}

void Ustawienia::setEnableSilnik8(bool value)
{
    m_enableSilnik8 = value;
    settings.setValue("Enable_8", QVariant::fromValue(m_enableSilnik8));
}

bool Ustawienia::getEnableSilnik9() const
{
    return m_enableSilnik9;
}

void Ustawienia::setEnableSilnik9(bool value)
{
    m_enableSilnik9 = value;
    settings.setValue("Enable_9", QVariant::fromValue(m_enableSilnik9));
}
