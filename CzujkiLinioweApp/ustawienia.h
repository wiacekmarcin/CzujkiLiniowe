#ifndef USTAWIENIA_H
#define USTAWIENIA_H

#include <QString>
#include <QSettings>

Q_DECLARE_METATYPE(long double)

class Ustawienia
{
public:
    Ustawienia();

    void read();

    void write();

    QString getRatioSilnik(uint silnik) const;
    void setRatioSilnik(uint silnik, const QString & value);

    QString getRatioSilnik1() const;
    void setRatioSilnik1(const QString & value);

    QString getRatioSilnik2() const;
    void setRatioSilnik2(const QString & value);

    QString getRatioSilnik3() const;
    void setRatioSilnik3(const QString & value);

    QString getRatioSilnik4() const;
    void setRatioSilnik4(const QString & value);

    QString getRatioSilnik5() const;
    void setRatioSilnik5(const QString & value);

    QString getRatioSilnik6() const;
    void setRatioSilnik6(const QString & value);

    QString getRatioSilnik7() const;
    void setRatioSilnik7(const QString & value);

    QString getRatioSilnik8() const;
    void setRatioSilnik8(const QString & value);

    QString getRatioSilnik9() const;
    void setRatioSilnik9(const QString & value);

    QString getMaxStepsSilnik(uint silnik) const;
    void setMaxStepsSilnik(uint silnik, const QString & value);

    QString getMaxStepsSilnik1() const;
    void setMaxStepsSilnik1(const QString & value);

    QString getMaxStepsSilnik2() const;
    void setMaxStepsSilnik2(const QString & value);

    QString getMaxStepsSilnik3() const;
    void setMaxStepsSilnik3(const QString & value);

    QString getMaxStepsSilnik4() const;
    void setMaxStepsSilnik4(const QString & value);

    QString getMaxStepsSilnik5() const;
    void setMaxStepsSilnik5(const QString & value);

    QString getMaxStepsSilnik6() const;
    void setMaxStepsSilnik6(const QString & value);

    QString getMaxStepsSilnik7() const;
    void setMaxStepsSilnik7(const QString & value);

    QString getMaxStepsSilnik8() const;
    void setMaxStepsSilnik8(const QString & value);

    QString getMaxStepsSilnik9() const;
    void setMaxStepsSilnik9(const QString & value);

    QString getBaseStepsSilnik(uint silnik) const;
    void setBaseStepsSilnik(uint silnik, const QString & value);

    QString getBaseStepsSilnik1() const;
    void setBaseStepsSilnik1(const QString & value);

    QString getBaseStepsSilnik2() const;
    void setBaseStepsSilnik2(const QString & value);

    QString getBaseStepsSilnik3() const;
    void setBaseStepsSilnik3(const QString & value);

    QString getBaseStepsSilnik4() const;
    void setBaseStepsSilnik4(const QString & value);

    QString getBaseStepsSilnik5() const;
    void setBaseStepsSilnik5(const QString & value);

    QString getBaseStepsSilnik6() const;
    void setBaseStepsSilnik6(const QString & value);

    QString getBaseStepsSilnik7() const;
    void setBaseStepsSilnik7(const QString & value);

    QString getBaseStepsSilnik8() const;
    void setBaseStepsSilnik8(const QString & value);

    QString getBaseStepsSilnik9() const;
    void setBaseStepsSilnik9(const QString & value);

    QString getDelayusSilnik(uint silnik) const;
    void setDelayusSilnik(uint silnik, const QString & value);

    QString getDelayusSilnik1() const;
    void setDelayusSilnik1(const QString & value);

    QString getDelayusSilnik2() const;
    void setDelayusSilnik2(const QString & value);

    QString getDelayusSilnik3() const;
    void setDelayusSilnik3(const QString & value);

    QString getDelayusSilnik4() const;
    void setDelayusSilnik4(const QString & value);

    QString getDelayusSilnik5() const;
    void setDelayusSilnik5(const QString & value);

    QString getDelayusSilnik6() const;
    void setDelayusSilnik6(const QString & value);

    QString getDelayusSilnik7() const;
    void setDelayusSilnik7(const QString & value);

    QString getDelayusSilnik8() const;
    void setDelayusSilnik8(const QString & value);

    QString getDelayusSilnik9() const;
    void setDelayusSilnik9(const QString & value);

    bool getLeftRotationSilnik(uint silnik) const;
    void setLeftRotationSilnik(uint silnik, bool value);

    bool getLeftRotationSilnik1() const;
    void setLeftRotationSilnik1(bool value);

    bool getLeftRotationSilnik2() const;
    void setLeftRotationSilnik2(bool value);

    bool getLeftRotationSilnik3() const;
    void setLeftRotationSilnik3(bool value);

    bool getLeftRotationSilnik4() const;
    void setLeftRotationSilnik4(bool value);

    bool getLeftRotationSilnik5() const;
    void setLeftRotationSilnik5(bool value);

    bool getLeftRotationSilnik6() const;
    void setLeftRotationSilnik6(bool value);

    bool getLeftRotationSilnik7() const;
    void setLeftRotationSilnik7(bool value);

    bool getLeftRotationSilnik8() const;
    void setLeftRotationSilnik8(bool value);

    bool getLeftRotationSilnik9() const;
    void setLeftRotationSilnik9(bool value);

    bool getEnableSilnik(uint key) const;
    void setEnableSilnik(uint key, bool value);

    bool getEnableSilnik1() const;
    void setEnableSilnik1(bool value);

    bool getEnableSilnik2() const;
    void setEnableSilnik2(bool value);

    bool getEnableSilnik3() const;
    void setEnableSilnik3(bool value);

    bool getEnableSilnik4() const;
    void setEnableSilnik4(bool value);

    bool getEnableSilnik5() const;
    void setEnableSilnik5(bool value);

    bool getEnableSilnik6() const;
    void setEnableSilnik6(bool value);

    bool getEnableSilnik7() const;
    void setEnableSilnik7(bool value);

    bool getEnableSilnik8() const;
    void setEnableSilnik8(bool value);

    bool getEnableSilnik9() const;
    void setEnableSilnik9(bool value);

private:

    QSettings settings;
    static char appnazwa[];
    static char firmnazwa[];

    QString m_ratioSilnik1;
    QString m_ratioSilnik2;
    QString m_ratioSilnik3;
    QString m_ratioSilnik4;
    QString m_ratioSilnik5;
    QString m_ratioSilnik6;
    QString m_ratioSilnik7;
    QString m_ratioSilnik8;
    QString m_ratioSilnik9;
    QString m_maxStepsSilnik1;
    QString m_maxStepsSilnik2;
    QString m_maxStepsSilnik3;
    QString m_maxStepsSilnik4;
    QString m_maxStepsSilnik5;
    QString m_maxStepsSilnik6;
    QString m_maxStepsSilnik7;
    QString m_maxStepsSilnik8;
    QString m_maxStepsSilnik9;
    QString m_baseStepsSilnik1;
    QString m_baseStepsSilnik2;
    QString m_baseStepsSilnik3;
    QString m_baseStepsSilnik4;
    QString m_baseStepsSilnik5;
    QString m_baseStepsSilnik6;
    QString m_baseStepsSilnik7;
    QString m_baseStepsSilnik8;
    QString m_baseStepsSilnik9;
    QString m_delayusSilnik1;
    QString m_delayusSilnik2;
    QString m_delayusSilnik3;
    QString m_delayusSilnik4;
    QString m_delayusSilnik5;
    QString m_delayusSilnik6;
    QString m_delayusSilnik7;
    QString m_delayusSilnik8;
    QString m_delayusSilnik9;
    bool m_leftRotationSilnik1;
    bool m_leftRotationSilnik2;
    bool m_leftRotationSilnik3;
    bool m_leftRotationSilnik4;
    bool m_leftRotationSilnik5;
    bool m_leftRotationSilnik6;
    bool m_leftRotationSilnik7;
    bool m_leftRotationSilnik8;
    bool m_leftRotationSilnik9;
    bool m_enableSilnik1;
    bool m_enableSilnik2;
    bool m_enableSilnik3;
    bool m_enableSilnik4;
    bool m_enableSilnik5;
    bool m_enableSilnik6;
    bool m_enableSilnik7;
    bool m_enableSilnik8;
    bool m_enableSilnik9;


};

#endif // USTAWIENIA_H
