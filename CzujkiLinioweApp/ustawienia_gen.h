
#ifndef USTAWIENIAGEN_H
#define USTAWIENIAGEN_H

#include <QString>
#include <QSettings>

Q_DECLARE_METATYPE(long double)

class UstawieniaGen
{
public:
    UstawieniaGen();
    ~UstawieniaGen();
    void load();
    void save();
    bool check();
    void sync() { settings.sync(); }

protected:
    QSettings settings;
    
    bool toBool(const QString & val) const { return QVariant(val).toBool(); }
    double toDouble(const QString & val) const { return val.toDouble(); }
    unsigned int toUInt(const QString & val) const { return val.toUInt(); }
    QString toQString(const QString & val) const { return val; }
    unsigned long toULong(const QString & val) const { return val.toULong(); }

public:    
/********** GET SET **********/
	QString getMotorNazwa1() const;
	void setMotorNazwa1(const QString & nazwa1);
	QString getMotorNazwa2() const;
	void setMotorNazwa2(const QString & nazwa2);
	QString getMotorNazwa3() const;
	void setMotorNazwa3(const QString & nazwa3);
	QString getMotorNazwa4() const;
	void setMotorNazwa4(const QString & nazwa4);
	QString getMotorNazwa5() const;
	void setMotorNazwa5(const QString & nazwa5);
	QString getMotorNazwa6() const;
	void setMotorNazwa6(const QString & nazwa6);
	QString getMotorNazwa7() const;
	void setMotorNazwa7(const QString & nazwa7);
	QString getMotorNazwa8() const;
	void setMotorNazwa8(const QString & nazwa8);
	QString getMotorNazwa9() const;
	void setMotorNazwa9(const QString & nazwa9);
	QString getMotorNazwa(const short & nrMotor) const;
	void setMotorNazwa(const short & nrMotor, const QString & nazwa);
	double getMotorPrzelozenieImpJedn1() const;
	void setMotorPrzelozenieImpJedn1(const double & przelozenieImpJedn1);
	inline void setMotorPrzelozenieImpJedn1(const QString & przelozenieImpJedn1) { setMotorPrzelozenieImpJedn1(toDouble(przelozenieImpJedn1)); }
	double getMotorPrzelozenieImpJedn2() const;
	void setMotorPrzelozenieImpJedn2(const double & przelozenieImpJedn2);
	inline void setMotorPrzelozenieImpJedn2(const QString & przelozenieImpJedn2) { setMotorPrzelozenieImpJedn2(toDouble(przelozenieImpJedn2)); }
	double getMotorPrzelozenieImpJedn3() const;
	void setMotorPrzelozenieImpJedn3(const double & przelozenieImpJedn3);
	inline void setMotorPrzelozenieImpJedn3(const QString & przelozenieImpJedn3) { setMotorPrzelozenieImpJedn3(toDouble(przelozenieImpJedn3)); }
	double getMotorPrzelozenieImpJedn4() const;
	void setMotorPrzelozenieImpJedn4(const double & przelozenieImpJedn4);
	inline void setMotorPrzelozenieImpJedn4(const QString & przelozenieImpJedn4) { setMotorPrzelozenieImpJedn4(toDouble(przelozenieImpJedn4)); }
	double getMotorPrzelozenieImpJedn5() const;
	void setMotorPrzelozenieImpJedn5(const double & przelozenieImpJedn5);
	inline void setMotorPrzelozenieImpJedn5(const QString & przelozenieImpJedn5) { setMotorPrzelozenieImpJedn5(toDouble(przelozenieImpJedn5)); }
	double getMotorPrzelozenieImpJedn6() const;
	void setMotorPrzelozenieImpJedn6(const double & przelozenieImpJedn6);
	inline void setMotorPrzelozenieImpJedn6(const QString & przelozenieImpJedn6) { setMotorPrzelozenieImpJedn6(toDouble(przelozenieImpJedn6)); }
	double getMotorPrzelozenieImpJedn7() const;
	void setMotorPrzelozenieImpJedn7(const double & przelozenieImpJedn7);
	inline void setMotorPrzelozenieImpJedn7(const QString & przelozenieImpJedn7) { setMotorPrzelozenieImpJedn7(toDouble(przelozenieImpJedn7)); }
	double getMotorPrzelozenieImpJedn8() const;
	void setMotorPrzelozenieImpJedn8(const double & przelozenieImpJedn8);
	inline void setMotorPrzelozenieImpJedn8(const QString & przelozenieImpJedn8) { setMotorPrzelozenieImpJedn8(toDouble(przelozenieImpJedn8)); }
	double getMotorPrzelozenieImpJedn9() const;
	void setMotorPrzelozenieImpJedn9(const double & przelozenieImpJedn9);
	inline void setMotorPrzelozenieImpJedn9(const QString & przelozenieImpJedn9) { setMotorPrzelozenieImpJedn9(toDouble(przelozenieImpJedn9)); }
	double getMotorPrzelozenieImpJedn(const short & nrMotor) const;
	void setMotorPrzelozenieImpJedn(const short & nrMotor, const double & przelozenieImpJedn);
	int getMotorMinOdstepImp1() const;
	void setMotorMinOdstepImp1(const int & minOdstepImp1);
	inline void setMotorMinOdstepImp1(const QString & minOdstepImp1) { setMotorMinOdstepImp1(toUInt(minOdstepImp1)); }
	int getMotorMinOdstepImp2() const;
	void setMotorMinOdstepImp2(const int & minOdstepImp2);
	inline void setMotorMinOdstepImp2(const QString & minOdstepImp2) { setMotorMinOdstepImp2(toUInt(minOdstepImp2)); }
	int getMotorMinOdstepImp3() const;
	void setMotorMinOdstepImp3(const int & minOdstepImp3);
	inline void setMotorMinOdstepImp3(const QString & minOdstepImp3) { setMotorMinOdstepImp3(toUInt(minOdstepImp3)); }
	int getMotorMinOdstepImp4() const;
	void setMotorMinOdstepImp4(const int & minOdstepImp4);
	inline void setMotorMinOdstepImp4(const QString & minOdstepImp4) { setMotorMinOdstepImp4(toUInt(minOdstepImp4)); }
	int getMotorMinOdstepImp5() const;
	void setMotorMinOdstepImp5(const int & minOdstepImp5);
	inline void setMotorMinOdstepImp5(const QString & minOdstepImp5) { setMotorMinOdstepImp5(toUInt(minOdstepImp5)); }
	int getMotorMinOdstepImp6() const;
	void setMotorMinOdstepImp6(const int & minOdstepImp6);
	inline void setMotorMinOdstepImp6(const QString & minOdstepImp6) { setMotorMinOdstepImp6(toUInt(minOdstepImp6)); }
	int getMotorMinOdstepImp7() const;
	void setMotorMinOdstepImp7(const int & minOdstepImp7);
	inline void setMotorMinOdstepImp7(const QString & minOdstepImp7) { setMotorMinOdstepImp7(toUInt(minOdstepImp7)); }
	int getMotorMinOdstepImp8() const;
	void setMotorMinOdstepImp8(const int & minOdstepImp8);
	inline void setMotorMinOdstepImp8(const QString & minOdstepImp8) { setMotorMinOdstepImp8(toUInt(minOdstepImp8)); }
	int getMotorMinOdstepImp9() const;
	void setMotorMinOdstepImp9(const int & minOdstepImp9);
	inline void setMotorMinOdstepImp9(const QString & minOdstepImp9) { setMotorMinOdstepImp9(toUInt(minOdstepImp9)); }
	int getMotorMinOdstepImp(const short & nrMotor) const;
	void setMotorMinOdstepImp(const short & nrMotor, const int & minOdstepImp);
	bool getMotorOdwrocObroty1() const;
	void setMotorOdwrocObroty1(const bool & odwrocObroty1);
	inline void setMotorOdwrocObroty1(const QString & odwrocObroty1) { setMotorOdwrocObroty1(toBool(odwrocObroty1)); }
	bool getMotorOdwrocObroty2() const;
	void setMotorOdwrocObroty2(const bool & odwrocObroty2);
	inline void setMotorOdwrocObroty2(const QString & odwrocObroty2) { setMotorOdwrocObroty2(toBool(odwrocObroty2)); }
	bool getMotorOdwrocObroty3() const;
	void setMotorOdwrocObroty3(const bool & odwrocObroty3);
	inline void setMotorOdwrocObroty3(const QString & odwrocObroty3) { setMotorOdwrocObroty3(toBool(odwrocObroty3)); }
	bool getMotorOdwrocObroty4() const;
	void setMotorOdwrocObroty4(const bool & odwrocObroty4);
	inline void setMotorOdwrocObroty4(const QString & odwrocObroty4) { setMotorOdwrocObroty4(toBool(odwrocObroty4)); }
	bool getMotorOdwrocObroty5() const;
	void setMotorOdwrocObroty5(const bool & odwrocObroty5);
	inline void setMotorOdwrocObroty5(const QString & odwrocObroty5) { setMotorOdwrocObroty5(toBool(odwrocObroty5)); }
	bool getMotorOdwrocObroty6() const;
	void setMotorOdwrocObroty6(const bool & odwrocObroty6);
	inline void setMotorOdwrocObroty6(const QString & odwrocObroty6) { setMotorOdwrocObroty6(toBool(odwrocObroty6)); }
	bool getMotorOdwrocObroty7() const;
	void setMotorOdwrocObroty7(const bool & odwrocObroty7);
	inline void setMotorOdwrocObroty7(const QString & odwrocObroty7) { setMotorOdwrocObroty7(toBool(odwrocObroty7)); }
	bool getMotorOdwrocObroty8() const;
	void setMotorOdwrocObroty8(const bool & odwrocObroty8);
	inline void setMotorOdwrocObroty8(const QString & odwrocObroty8) { setMotorOdwrocObroty8(toBool(odwrocObroty8)); }
	bool getMotorOdwrocObroty9() const;
	void setMotorOdwrocObroty9(const bool & odwrocObroty9);
	inline void setMotorOdwrocObroty9(const QString & odwrocObroty9) { setMotorOdwrocObroty9(toBool(odwrocObroty9)); }
	bool getMotorOdwrocObroty(const short & nrMotor) const;
	void setMotorOdwrocObroty(const short & nrMotor, const bool & odwrocObroty);
	int getMotorCzasMiedzyImpZerow1() const;
	void setMotorCzasMiedzyImpZerow1(const int & czasMiedzyImpZerow1);
	inline void setMotorCzasMiedzyImpZerow1(const QString & czasMiedzyImpZerow1) { setMotorCzasMiedzyImpZerow1(toUInt(czasMiedzyImpZerow1)); }
	int getMotorCzasMiedzyImpZerow2() const;
	void setMotorCzasMiedzyImpZerow2(const int & czasMiedzyImpZerow2);
	inline void setMotorCzasMiedzyImpZerow2(const QString & czasMiedzyImpZerow2) { setMotorCzasMiedzyImpZerow2(toUInt(czasMiedzyImpZerow2)); }
	int getMotorCzasMiedzyImpZerow3() const;
	void setMotorCzasMiedzyImpZerow3(const int & czasMiedzyImpZerow3);
	inline void setMotorCzasMiedzyImpZerow3(const QString & czasMiedzyImpZerow3) { setMotorCzasMiedzyImpZerow3(toUInt(czasMiedzyImpZerow3)); }
	int getMotorCzasMiedzyImpZerow4() const;
	void setMotorCzasMiedzyImpZerow4(const int & czasMiedzyImpZerow4);
	inline void setMotorCzasMiedzyImpZerow4(const QString & czasMiedzyImpZerow4) { setMotorCzasMiedzyImpZerow4(toUInt(czasMiedzyImpZerow4)); }
	int getMotorCzasMiedzyImpZerow5() const;
	void setMotorCzasMiedzyImpZerow5(const int & czasMiedzyImpZerow5);
	inline void setMotorCzasMiedzyImpZerow5(const QString & czasMiedzyImpZerow5) { setMotorCzasMiedzyImpZerow5(toUInt(czasMiedzyImpZerow5)); }
	int getMotorCzasMiedzyImpZerow6() const;
	void setMotorCzasMiedzyImpZerow6(const int & czasMiedzyImpZerow6);
	inline void setMotorCzasMiedzyImpZerow6(const QString & czasMiedzyImpZerow6) { setMotorCzasMiedzyImpZerow6(toUInt(czasMiedzyImpZerow6)); }
	int getMotorCzasMiedzyImpZerow7() const;
	void setMotorCzasMiedzyImpZerow7(const int & czasMiedzyImpZerow7);
	inline void setMotorCzasMiedzyImpZerow7(const QString & czasMiedzyImpZerow7) { setMotorCzasMiedzyImpZerow7(toUInt(czasMiedzyImpZerow7)); }
	int getMotorCzasMiedzyImpZerow8() const;
	void setMotorCzasMiedzyImpZerow8(const int & czasMiedzyImpZerow8);
	inline void setMotorCzasMiedzyImpZerow8(const QString & czasMiedzyImpZerow8) { setMotorCzasMiedzyImpZerow8(toUInt(czasMiedzyImpZerow8)); }
	int getMotorCzasMiedzyImpZerow9() const;
	void setMotorCzasMiedzyImpZerow9(const int & czasMiedzyImpZerow9);
	inline void setMotorCzasMiedzyImpZerow9(const QString & czasMiedzyImpZerow9) { setMotorCzasMiedzyImpZerow9(toUInt(czasMiedzyImpZerow9)); }
	int getMotorCzasMiedzyImpZerow(const short & nrMotor) const;
	void setMotorCzasMiedzyImpZerow(const short & nrMotor, const int & czasMiedzyImpZerow);
	int getMotorCzasMiedzyImpNormal1() const;
	void setMotorCzasMiedzyImpNormal1(const int & czasMiedzyImpNormal1);
	inline void setMotorCzasMiedzyImpNormal1(const QString & czasMiedzyImpNormal1) { setMotorCzasMiedzyImpNormal1(toUInt(czasMiedzyImpNormal1)); }
	int getMotorCzasMiedzyImpNormal2() const;
	void setMotorCzasMiedzyImpNormal2(const int & czasMiedzyImpNormal2);
	inline void setMotorCzasMiedzyImpNormal2(const QString & czasMiedzyImpNormal2) { setMotorCzasMiedzyImpNormal2(toUInt(czasMiedzyImpNormal2)); }
	int getMotorCzasMiedzyImpNormal3() const;
	void setMotorCzasMiedzyImpNormal3(const int & czasMiedzyImpNormal3);
	inline void setMotorCzasMiedzyImpNormal3(const QString & czasMiedzyImpNormal3) { setMotorCzasMiedzyImpNormal3(toUInt(czasMiedzyImpNormal3)); }
	int getMotorCzasMiedzyImpNormal4() const;
	void setMotorCzasMiedzyImpNormal4(const int & czasMiedzyImpNormal4);
	inline void setMotorCzasMiedzyImpNormal4(const QString & czasMiedzyImpNormal4) { setMotorCzasMiedzyImpNormal4(toUInt(czasMiedzyImpNormal4)); }
	int getMotorCzasMiedzyImpNormal5() const;
	void setMotorCzasMiedzyImpNormal5(const int & czasMiedzyImpNormal5);
	inline void setMotorCzasMiedzyImpNormal5(const QString & czasMiedzyImpNormal5) { setMotorCzasMiedzyImpNormal5(toUInt(czasMiedzyImpNormal5)); }
	int getMotorCzasMiedzyImpNormal6() const;
	void setMotorCzasMiedzyImpNormal6(const int & czasMiedzyImpNormal6);
	inline void setMotorCzasMiedzyImpNormal6(const QString & czasMiedzyImpNormal6) { setMotorCzasMiedzyImpNormal6(toUInt(czasMiedzyImpNormal6)); }
	int getMotorCzasMiedzyImpNormal7() const;
	void setMotorCzasMiedzyImpNormal7(const int & czasMiedzyImpNormal7);
	inline void setMotorCzasMiedzyImpNormal7(const QString & czasMiedzyImpNormal7) { setMotorCzasMiedzyImpNormal7(toUInt(czasMiedzyImpNormal7)); }
	int getMotorCzasMiedzyImpNormal8() const;
	void setMotorCzasMiedzyImpNormal8(const int & czasMiedzyImpNormal8);
	inline void setMotorCzasMiedzyImpNormal8(const QString & czasMiedzyImpNormal8) { setMotorCzasMiedzyImpNormal8(toUInt(czasMiedzyImpNormal8)); }
	int getMotorCzasMiedzyImpNormal9() const;
	void setMotorCzasMiedzyImpNormal9(const int & czasMiedzyImpNormal9);
	inline void setMotorCzasMiedzyImpNormal9(const QString & czasMiedzyImpNormal9) { setMotorCzasMiedzyImpNormal9(toUInt(czasMiedzyImpNormal9)); }
	int getMotorCzasMiedzyImpNormal(const short & nrMotor) const;
	void setMotorCzasMiedzyImpNormal(const short & nrMotor, const int & czasMiedzyImpNormal);
	int getMotorMaksIloscImp1() const;
	void setMotorMaksIloscImp1(const int & maksIloscImp1);
	inline void setMotorMaksIloscImp1(const QString & maksIloscImp1) { setMotorMaksIloscImp1(toUInt(maksIloscImp1)); }
	int getMotorMaksIloscImp2() const;
	void setMotorMaksIloscImp2(const int & maksIloscImp2);
	inline void setMotorMaksIloscImp2(const QString & maksIloscImp2) { setMotorMaksIloscImp2(toUInt(maksIloscImp2)); }
	int getMotorMaksIloscImp3() const;
	void setMotorMaksIloscImp3(const int & maksIloscImp3);
	inline void setMotorMaksIloscImp3(const QString & maksIloscImp3) { setMotorMaksIloscImp3(toUInt(maksIloscImp3)); }
	int getMotorMaksIloscImp4() const;
	void setMotorMaksIloscImp4(const int & maksIloscImp4);
	inline void setMotorMaksIloscImp4(const QString & maksIloscImp4) { setMotorMaksIloscImp4(toUInt(maksIloscImp4)); }
	int getMotorMaksIloscImp5() const;
	void setMotorMaksIloscImp5(const int & maksIloscImp5);
	inline void setMotorMaksIloscImp5(const QString & maksIloscImp5) { setMotorMaksIloscImp5(toUInt(maksIloscImp5)); }
	int getMotorMaksIloscImp6() const;
	void setMotorMaksIloscImp6(const int & maksIloscImp6);
	inline void setMotorMaksIloscImp6(const QString & maksIloscImp6) { setMotorMaksIloscImp6(toUInt(maksIloscImp6)); }
	int getMotorMaksIloscImp7() const;
	void setMotorMaksIloscImp7(const int & maksIloscImp7);
	inline void setMotorMaksIloscImp7(const QString & maksIloscImp7) { setMotorMaksIloscImp7(toUInt(maksIloscImp7)); }
	int getMotorMaksIloscImp8() const;
	void setMotorMaksIloscImp8(const int & maksIloscImp8);
	inline void setMotorMaksIloscImp8(const QString & maksIloscImp8) { setMotorMaksIloscImp8(toUInt(maksIloscImp8)); }
	int getMotorMaksIloscImp9() const;
	void setMotorMaksIloscImp9(const int & maksIloscImp9);
	inline void setMotorMaksIloscImp9(const QString & maksIloscImp9) { setMotorMaksIloscImp9(toUInt(maksIloscImp9)); }
	int getMotorMaksIloscImp(const short & nrMotor) const;
	void setMotorMaksIloscImp(const short & nrMotor, const int & maksIloscImp);
	int getMotorIloscImpBaza1() const;
	void setMotorIloscImpBaza1(const int & iloscImpBaza1);
	inline void setMotorIloscImpBaza1(const QString & iloscImpBaza1) { setMotorIloscImpBaza1(toUInt(iloscImpBaza1)); }
	int getMotorIloscImpBaza2() const;
	void setMotorIloscImpBaza2(const int & iloscImpBaza2);
	inline void setMotorIloscImpBaza2(const QString & iloscImpBaza2) { setMotorIloscImpBaza2(toUInt(iloscImpBaza2)); }
	int getMotorIloscImpBaza3() const;
	void setMotorIloscImpBaza3(const int & iloscImpBaza3);
	inline void setMotorIloscImpBaza3(const QString & iloscImpBaza3) { setMotorIloscImpBaza3(toUInt(iloscImpBaza3)); }
	int getMotorIloscImpBaza4() const;
	void setMotorIloscImpBaza4(const int & iloscImpBaza4);
	inline void setMotorIloscImpBaza4(const QString & iloscImpBaza4) { setMotorIloscImpBaza4(toUInt(iloscImpBaza4)); }
	int getMotorIloscImpBaza5() const;
	void setMotorIloscImpBaza5(const int & iloscImpBaza5);
	inline void setMotorIloscImpBaza5(const QString & iloscImpBaza5) { setMotorIloscImpBaza5(toUInt(iloscImpBaza5)); }
	int getMotorIloscImpBaza6() const;
	void setMotorIloscImpBaza6(const int & iloscImpBaza6);
	inline void setMotorIloscImpBaza6(const QString & iloscImpBaza6) { setMotorIloscImpBaza6(toUInt(iloscImpBaza6)); }
	int getMotorIloscImpBaza7() const;
	void setMotorIloscImpBaza7(const int & iloscImpBaza7);
	inline void setMotorIloscImpBaza7(const QString & iloscImpBaza7) { setMotorIloscImpBaza7(toUInt(iloscImpBaza7)); }
	int getMotorIloscImpBaza8() const;
	void setMotorIloscImpBaza8(const int & iloscImpBaza8);
	inline void setMotorIloscImpBaza8(const QString & iloscImpBaza8) { setMotorIloscImpBaza8(toUInt(iloscImpBaza8)); }
	int getMotorIloscImpBaza9() const;
	void setMotorIloscImpBaza9(const int & iloscImpBaza9);
	inline void setMotorIloscImpBaza9(const QString & iloscImpBaza9) { setMotorIloscImpBaza9(toUInt(iloscImpBaza9)); }
	int getMotorIloscImpBaza(const short & nrMotor) const;
	void setMotorIloscImpBaza(const short & nrMotor, const int & iloscImpBaza);
	int getMotorIloscImpSrodek1() const;
	void setMotorIloscImpSrodek1(const int & iloscImpSrodek1);
	inline void setMotorIloscImpSrodek1(const QString & iloscImpSrodek1) { setMotorIloscImpSrodek1(toUInt(iloscImpSrodek1)); }
	int getMotorIloscImpSrodek2() const;
	void setMotorIloscImpSrodek2(const int & iloscImpSrodek2);
	inline void setMotorIloscImpSrodek2(const QString & iloscImpSrodek2) { setMotorIloscImpSrodek2(toUInt(iloscImpSrodek2)); }
	int getMotorIloscImpSrodek3() const;
	void setMotorIloscImpSrodek3(const int & iloscImpSrodek3);
	inline void setMotorIloscImpSrodek3(const QString & iloscImpSrodek3) { setMotorIloscImpSrodek3(toUInt(iloscImpSrodek3)); }
	int getMotorIloscImpSrodek4() const;
	void setMotorIloscImpSrodek4(const int & iloscImpSrodek4);
	inline void setMotorIloscImpSrodek4(const QString & iloscImpSrodek4) { setMotorIloscImpSrodek4(toUInt(iloscImpSrodek4)); }
	int getMotorIloscImpSrodek5() const;
	void setMotorIloscImpSrodek5(const int & iloscImpSrodek5);
	inline void setMotorIloscImpSrodek5(const QString & iloscImpSrodek5) { setMotorIloscImpSrodek5(toUInt(iloscImpSrodek5)); }
	int getMotorIloscImpSrodek6() const;
	void setMotorIloscImpSrodek6(const int & iloscImpSrodek6);
	inline void setMotorIloscImpSrodek6(const QString & iloscImpSrodek6) { setMotorIloscImpSrodek6(toUInt(iloscImpSrodek6)); }
	int getMotorIloscImpSrodek7() const;
	void setMotorIloscImpSrodek7(const int & iloscImpSrodek7);
	inline void setMotorIloscImpSrodek7(const QString & iloscImpSrodek7) { setMotorIloscImpSrodek7(toUInt(iloscImpSrodek7)); }
	int getMotorIloscImpSrodek8() const;
	void setMotorIloscImpSrodek8(const int & iloscImpSrodek8);
	inline void setMotorIloscImpSrodek8(const QString & iloscImpSrodek8) { setMotorIloscImpSrodek8(toUInt(iloscImpSrodek8)); }
	int getMotorIloscImpSrodek9() const;
	void setMotorIloscImpSrodek9(const int & iloscImpSrodek9);
	inline void setMotorIloscImpSrodek9(const QString & iloscImpSrodek9) { setMotorIloscImpSrodek9(toUInt(iloscImpSrodek9)); }
	int getMotorIloscImpSrodek(const short & nrMotor) const;
	void setMotorIloscImpSrodek(const short & nrMotor, const int & iloscImpSrodek);
	double getFiltr_db_880_A_0() const;
	void setFiltr_db_880_A_0(const double & val);
	inline void setFiltr_db_880_A_0(const QString & vals) { setFiltr_db_880_A_0(toDouble(vals)); }
	double getFiltr_db_880_B_0() const;
	void setFiltr_db_880_B_0(const double & val);
	inline void setFiltr_db_880_B_0(const QString & vals) { setFiltr_db_880_B_0(toDouble(vals)); }
	double getFiltr_db_880_C_0() const;
	void setFiltr_db_880_C_0(const double & val);
	inline void setFiltr_db_880_C_0(const QString & vals) { setFiltr_db_880_C_0(toDouble(vals)); }
	double getFiltr_db_880_A_1() const;
	void setFiltr_db_880_A_1(const double & val);
	inline void setFiltr_db_880_A_1(const QString & vals) { setFiltr_db_880_A_1(toDouble(vals)); }
	double getFiltr_db_880_B_1() const;
	void setFiltr_db_880_B_1(const double & val);
	inline void setFiltr_db_880_B_1(const QString & vals) { setFiltr_db_880_B_1(toDouble(vals)); }
	double getFiltr_db_880_C_1() const;
	void setFiltr_db_880_C_1(const double & val);
	inline void setFiltr_db_880_C_1(const QString & vals) { setFiltr_db_880_C_1(toDouble(vals)); }
	double getFiltr_db_880_A_2() const;
	void setFiltr_db_880_A_2(const double & val);
	inline void setFiltr_db_880_A_2(const QString & vals) { setFiltr_db_880_A_2(toDouble(vals)); }
	double getFiltr_db_880_B_2() const;
	void setFiltr_db_880_B_2(const double & val);
	inline void setFiltr_db_880_B_2(const QString & vals) { setFiltr_db_880_B_2(toDouble(vals)); }
	double getFiltr_db_880_C_2() const;
	void setFiltr_db_880_C_2(const double & val);
	inline void setFiltr_db_880_C_2(const QString & vals) { setFiltr_db_880_C_2(toDouble(vals)); }
	double getFiltr_db_880_A_3() const;
	void setFiltr_db_880_A_3(const double & val);
	inline void setFiltr_db_880_A_3(const QString & vals) { setFiltr_db_880_A_3(toDouble(vals)); }
	double getFiltr_db_880_B_3() const;
	void setFiltr_db_880_B_3(const double & val);
	inline void setFiltr_db_880_B_3(const QString & vals) { setFiltr_db_880_B_3(toDouble(vals)); }
	double getFiltr_db_880_C_3() const;
	void setFiltr_db_880_C_3(const double & val);
	inline void setFiltr_db_880_C_3(const QString & vals) { setFiltr_db_880_C_3(toDouble(vals)); }
	double getFiltr_db_880_A_4() const;
	void setFiltr_db_880_A_4(const double & val);
	inline void setFiltr_db_880_A_4(const QString & vals) { setFiltr_db_880_A_4(toDouble(vals)); }
	double getFiltr_db_880_B_4() const;
	void setFiltr_db_880_B_4(const double & val);
	inline void setFiltr_db_880_B_4(const QString & vals) { setFiltr_db_880_B_4(toDouble(vals)); }
	double getFiltr_db_880_C_4() const;
	void setFiltr_db_880_C_4(const double & val);
	inline void setFiltr_db_880_C_4(const QString & vals) { setFiltr_db_880_C_4(toDouble(vals)); }
	double getFiltr_db_880_A_5() const;
	void setFiltr_db_880_A_5(const double & val);
	inline void setFiltr_db_880_A_5(const QString & vals) { setFiltr_db_880_A_5(toDouble(vals)); }
	double getFiltr_db_880_B_5() const;
	void setFiltr_db_880_B_5(const double & val);
	inline void setFiltr_db_880_B_5(const QString & vals) { setFiltr_db_880_B_5(toDouble(vals)); }
	double getFiltr_db_880_C_5() const;
	void setFiltr_db_880_C_5(const double & val);
	inline void setFiltr_db_880_C_5(const QString & vals) { setFiltr_db_880_C_5(toDouble(vals)); }
	double getFiltr_db_655_A_0() const;
	void setFiltr_db_655_A_0(const double & val);
	inline void setFiltr_db_655_A_0(const QString & vals) { setFiltr_db_655_A_0(toDouble(vals)); }
	double getFiltr_db_655_B_0() const;
	void setFiltr_db_655_B_0(const double & val);
	inline void setFiltr_db_655_B_0(const QString & vals) { setFiltr_db_655_B_0(toDouble(vals)); }
	double getFiltr_db_655_C_0() const;
	void setFiltr_db_655_C_0(const double & val);
	inline void setFiltr_db_655_C_0(const QString & vals) { setFiltr_db_655_C_0(toDouble(vals)); }
	double getFiltr_db_655_A_1() const;
	void setFiltr_db_655_A_1(const double & val);
	inline void setFiltr_db_655_A_1(const QString & vals) { setFiltr_db_655_A_1(toDouble(vals)); }
	double getFiltr_db_655_B_1() const;
	void setFiltr_db_655_B_1(const double & val);
	inline void setFiltr_db_655_B_1(const QString & vals) { setFiltr_db_655_B_1(toDouble(vals)); }
	double getFiltr_db_655_C_1() const;
	void setFiltr_db_655_C_1(const double & val);
	inline void setFiltr_db_655_C_1(const QString & vals) { setFiltr_db_655_C_1(toDouble(vals)); }
	double getFiltr_db_655_A_2() const;
	void setFiltr_db_655_A_2(const double & val);
	inline void setFiltr_db_655_A_2(const QString & vals) { setFiltr_db_655_A_2(toDouble(vals)); }
	double getFiltr_db_655_B_2() const;
	void setFiltr_db_655_B_2(const double & val);
	inline void setFiltr_db_655_B_2(const QString & vals) { setFiltr_db_655_B_2(toDouble(vals)); }
	double getFiltr_db_655_C_2() const;
	void setFiltr_db_655_C_2(const double & val);
	inline void setFiltr_db_655_C_2(const QString & vals) { setFiltr_db_655_C_2(toDouble(vals)); }
	double getFiltr_db_655_A_3() const;
	void setFiltr_db_655_A_3(const double & val);
	inline void setFiltr_db_655_A_3(const QString & vals) { setFiltr_db_655_A_3(toDouble(vals)); }
	double getFiltr_db_655_B_3() const;
	void setFiltr_db_655_B_3(const double & val);
	inline void setFiltr_db_655_B_3(const QString & vals) { setFiltr_db_655_B_3(toDouble(vals)); }
	double getFiltr_db_655_C_3() const;
	void setFiltr_db_655_C_3(const double & val);
	inline void setFiltr_db_655_C_3(const QString & vals) { setFiltr_db_655_C_3(toDouble(vals)); }
	double getFiltr_db_655_A_4() const;
	void setFiltr_db_655_A_4(const double & val);
	inline void setFiltr_db_655_A_4(const QString & vals) { setFiltr_db_655_A_4(toDouble(vals)); }
	double getFiltr_db_655_B_4() const;
	void setFiltr_db_655_B_4(const double & val);
	inline void setFiltr_db_655_B_4(const QString & vals) { setFiltr_db_655_B_4(toDouble(vals)); }
	double getFiltr_db_655_C_4() const;
	void setFiltr_db_655_C_4(const double & val);
	inline void setFiltr_db_655_C_4(const QString & vals) { setFiltr_db_655_C_4(toDouble(vals)); }
	double getFiltr_db_655_A_5() const;
	void setFiltr_db_655_A_5(const double & val);
	inline void setFiltr_db_655_A_5(const QString & vals) { setFiltr_db_655_A_5(toDouble(vals)); }
	double getFiltr_db_655_B_5() const;
	void setFiltr_db_655_B_5(const double & val);
	inline void setFiltr_db_655_B_5(const QString & vals) { setFiltr_db_655_B_5(toDouble(vals)); }
	double getFiltr_db_655_C_5() const;
	void setFiltr_db_655_C_5(const double & val);
	inline void setFiltr_db_655_C_5(const QString & vals) { setFiltr_db_655_C_5(toDouble(vals)); }
	double getFiltr_db_880_A(const short & nrPos) const { return getFiltr_db(880, 'A', nrPos); }
	double getFiltr_db_880_B(const short & nrPos) const { return getFiltr_db(880, 'B', nrPos); }
	double getFiltr_db_880_C(const short & nrPos) const { return getFiltr_db(880, 'C', nrPos); }
	double getFiltr_db_655_A(const short & nrPos) const { return getFiltr_db(655, 'A', nrPos); }
	double getFiltr_db_655_B(const short & nrPos) const { return getFiltr_db(655, 'B', nrPos); }
	double getFiltr_db_655_C(const short & nrPos) const { return getFiltr_db(655, 'C', nrPos); }
	double getFiltr_db_880(const char & nrTarczy, const short & nrPos) const { return getFiltr_db(880, nrTarczy, nrPos); }
	double getFiltr_db_655(const char & nrTarczy, const short & nrPos) const { return getFiltr_db(655, nrTarczy, nrPos); }
	void setFiltr_db_880_A(const short & nrPos, const double & val) { setFiltr_db(880, 'A', nrPos, val); }
	void setFiltr_db_880_B(const short & nrPos, const double & val) { setFiltr_db(880, 'B', nrPos, val); }
	void setFiltr_db_880_C(const short & nrPos, const double & val) { setFiltr_db(880, 'C', nrPos, val); }
	void setFiltr_db_655_A(const short & nrPos, const double & val) { setFiltr_db(655, 'A', nrPos, val); }
	void setFiltr_db_655_B(const short & nrPos, const double & val) { setFiltr_db(655, 'B', nrPos, val); }
	void setFiltr_db_655_C(const short & nrPos, const double & val) { setFiltr_db(655, 'C', nrPos, val); }
	void setFiltr_db_880(const char & nrTarczy, const short & nrPos, const double & val) { setFiltr_db(880, nrTarczy, nrPos, val); }
	void setFiltr_db_655(const char & nrTarczy, const short & nrPos, const double & val) { setFiltr_db(655, nrTarczy, nrPos, val); }
	QString getSerialDeviceZasilaczVendor() const;
	void setSerialDeviceZasilaczVendor(const QString & zasilaczVendor);
	QString getSerialDeviceZasilaczProduct() const;
	void setSerialDeviceZasilaczProduct(const QString & zasilaczProduct);
	QString getSerialDeviceSterownikVendor() const;
	void setSerialDeviceSterownikVendor(const QString & sterownikVendor);
	QString getSerialDeviceSterownikProduct() const;
	void setSerialDeviceSterownikProduct(const QString & sterownikProduct);
	QString getSerialDeviceSterownikSerial() const;
	void setSerialDeviceSterownikSerial(const QString & sterownikSerial);
	int getMinNapiecieCzujki() const;
	void setMinNapiecieCzujki(const int & minNapiecieCzujki);
	int getMaxNapiecieCzujki() const;
	void setMaxNapiecieCzujki(const int & maxNapiecieCzujki);
	int getMinCzasStabCzujki() const;
	void setMinCzasStabCzujki(const int & minCzasStabCzujki);
	int getMaxCzasStabCzujki() const;
	void setMaxCzasStabCzujki(const int & maxCzasStabCzujki);
	double getMinPrzekrPradZasCzujki() const;
	void setMinPrzekrPradZasCzujki(const double & minPrzekrPradZasCzujki);
	double getMaxPrzekrPradZasCzujki() const;
	void setMaxPrzekrPradZasCzujki(const double & maxPrzekrPradZasCzujki);
	int getMinCzasPoZmianieFiltra() const;
	void setMinCzasPoZmianieFiltra(const int & minCzasPoZmianieFiltra);
	int getMaxCzasPoZmianieFiltra() const;
	void setMaxCzasPoZmianieFiltra(const int & maxCzasPoZmianieFiltra);
	unsigned int getCzasWylaczeniaCzujkiDlaResetu() const;
	void setCzasWylaczeniaCzujkiDlaResetu(const unsigned int & czasWylaczeniaCzujkiDlaResetu);
	unsigned int getCzasOczekiwaniaPowtarzalnosc4Test() const;
	void setCzasOczekiwaniaPowtarzalnosc4Test(const unsigned int & czasOczekiwaniaPowtarzalnosc4Test);
	unsigned int getMinimalnyCzasOczekiwaniaPowtarzalnosc1Test() const;
	void setMinimalnyCzasOczekiwaniaPowtarzalnosc1Test(const unsigned int & minimalnyCzasOczekiwaniaPowtarzalnosc1Test);
	unsigned int getMaksymalnyCzasOczekiwaniaPowtarzalnosc1Test() const;
	void setMaksymalnyCzasOczekiwaniaPowtarzalnosc1Test(const unsigned int & maksymalnyCzasOczekiwaniaPowtarzalnosc1Test);
	double getMinimalnaWartoscCzujkiCn() const;
	void setMinimalnaWartoscCzujkiCn(const double & minimalnaWartoscCzujkiCn);
	unsigned int getCzasStabilizacjiDlaKataNieWspolosiowosci() const;
	void setCzasStabilizacjiDlaKataNieWspolosiowosci(const unsigned int & czasStabilizacjiDlaKataNieWspolosiowosci);
	double getWartoscTlumienieDlaKataNieWspolosiowosci() const;
	void setWartoscTlumienieDlaKataNieWspolosiowosci(const double & wartoscTlumienieDlaKataNieWspolosiowosci);
	unsigned int getMaksCzasZadzialaniaCzujkiDlaKataNieWspolosiowosci() const;
	void setMaksCzasZadzialaniaCzujkiDlaKataNieWspolosiowosci(const unsigned int & maksCzasZadzialaniaCzujkiDlaKataNieWspolosiowosci);
	unsigned int getMaksCzasTestuZadzialaniaCzujkiDlaKataNieWspolosiowosci() const;
	void setMaksCzasTestuZadzialaniaCzujkiDlaKataNieWspolosiowosci(const unsigned int & maksCzasTestuZadzialaniaCzujkiDlaKataNieWspolosiowosci);
	double getMaksKatNieWspolOsiowosci() const;
	void setMaksKatNieWspolOsiowosci(const double & maksKatNieWspolOsiowosci);
	double getMaksRoznicaKatNieWspolOsiowosci() const;
	void setMaksRoznicaKatNieWspolOsiowosci(const double & maksRoznicaKatNieWspolOsiowosci);
	double getOdtwarzalnoscCmaxCrep() const;
	void setOdtwarzalnoscCmaxCrep(const double & odtwarzalnoscCmaxCrep);
	double getOdtwarzalnoscCrepCmin() const;
	void setOdtwarzalnoscCrepCmin(const double & odtwarzalnoscCrepCmin);
	double getPowtarzalnoscCmaxCmin() const;
	void setPowtarzalnoscCmaxCmin(const double & powtarzalnoscCmaxCmin);
	double getNiewspolosiowoscMinimalnyKatProducentMierzony() const;
	void setNiewspolosiowoscMinimalnyKatProducentMierzony(const double & niewspolosiowoscMinimalnyKatProducentMierzony);
	unsigned int getMaksCzasZadzialaniaCzujkidlaTlumnikaA() const;
	void setMaksCzasZadzialaniaCzujkidlaTlumnikaA(const unsigned int & maksCzasZadzialaniaCzujkidlaTlumnikaA);
	unsigned int getMaksCzasTestuCzujkidlaTlumnikaA() const;
	void setMaksCzasTestuCzujkidlaTlumnikaA(const unsigned int & maksCzasTestuCzujkidlaTlumnikaA);
	unsigned int getMaksCzasZadzialaniaCzujkidlaTlumnikaB() const;
	void setMaksCzasZadzialaniaCzujkidlaTlumnikaB(const unsigned int & maksCzasZadzialaniaCzujkidlaTlumnikaB);
	unsigned int getMaksCzasTestuCzujkidlaTlumnikaB() const;
	void setMaksCzasTestuCzujkidlaTlumnikaB(const unsigned int & maksCzasTestuCzujkidlaTlumnikaB);
	double getSzybkieZmianyWartoscTlumnikaA() const;
	void setSzybkieZmianyWartoscTlumnikaA(const double & szybkieZmianyWartoscTlumnikaA);
	double getSzybkieZmianyWartoscTlumnikaB() const;
	void setSzybkieZmianyWartoscTlumnikaB(const double & szybkieZmianyWartoscTlumnikaB);
	double getDlugoscDrogiOptycznejCmaxCmin() const;
	void setDlugoscDrogiOptycznejCmaxCmin(const double & dlugoscDrogiOptycznejCmaxCmin);
	double getRozproszoneSwiatloCmaxCmin() const;
	void setRozproszoneSwiatloCmaxCmin(const double & rozproszoneSwiatloCmaxCmin);
	double getTolerancjaNapieciaZasilaniaCmaxCmin() const;
	void setTolerancjaNapieciaZasilaniaCmaxCmin(const double & tolerancjaNapieciaZasilaniaCmaxCmin);
	double getMinimalneNapieciaTolerancjaNapiecia() const;
	void setMinimalneNapieciaTolerancjaNapiecia(const double & minimalneNapieciaTolerancjaNapiecia);
	double getMaksymalneNapieciaTolerancjaNapiecia() const;
	void setMaksymalneNapieciaTolerancjaNapiecia(const double & maksymalneNapieciaTolerancjaNapiecia);
	double getMinimalnaTemperatura() const;
	void setMinimalnaTemperatura(const double & minimalnaTemperatura);
	double getMaksymalnaTemperatura() const;
	void setMaksymalnaTemperatura(const double & maksymalnaTemperatura);
	double getMinimalnaWilgotnosc() const;
	void setMinimalnaWilgotnosc(const double & minimalnaWilgotnosc);
	double getMaksymalnaWilgotnosc() const;
	void setMaksymalnaWilgotnosc(const double & maksymalnaWilgotnosc);
	double getMinimalnaCisnienie() const;
	void setMinimalnaCisnienie(const double & minimalnaCisnienie);
	double getMaksymalnaCisnienie() const;
	void setMaksymalnaCisnienie(const double & maksymalnaCisnienie);
protected:
	bool checkMotorNazwa(const QString & val);
	bool checkMotorPrzelozenieImpJedn(const QString & val);
	bool checkMotorMinOdstepImp(const QString & val);
	bool checkMotorOdwrocObroty(const QString & val);
	bool checkMotorCzasMiedzyImpZerow(const QString & val);
	bool checkMotorCzasMiedzyImpNormal(const QString & val);
	bool checkMotorMaksIloscImp(const QString & val);
	bool checkMotorIloscImpBaza(const QString & val);
	bool checkMotorIloscImpSrodek(const QString & val);
	bool checkFilterdbValue(const QString & val);
	double getFiltr_db(const int& fala, const char & nrTarczy, const short & nrPos) const;
	void setFiltr_db(const int& fala, const char & nrTarczy, const short & nrPos, const double & val);
	bool checkSerialDeviceIdentString(const QString & val) const;
private:

	QString motorNazwa1;
	QString motorNazwa2;
	QString motorNazwa3;
	QString motorNazwa4;
	QString motorNazwa5;
	QString motorNazwa6;
	QString motorNazwa7;
	QString motorNazwa8;
	QString motorNazwa9;
	double motorPrzelozenieImpJedn1;
	double motorPrzelozenieImpJedn2;
	double motorPrzelozenieImpJedn3;
	double motorPrzelozenieImpJedn4;
	double motorPrzelozenieImpJedn5;
	double motorPrzelozenieImpJedn6;
	double motorPrzelozenieImpJedn7;
	double motorPrzelozenieImpJedn8;
	double motorPrzelozenieImpJedn9;
	int motorMinOdstepImp1;
	int motorMinOdstepImp2;
	int motorMinOdstepImp3;
	int motorMinOdstepImp4;
	int motorMinOdstepImp5;
	int motorMinOdstepImp6;
	int motorMinOdstepImp7;
	int motorMinOdstepImp8;
	int motorMinOdstepImp9;
	bool motorOdwrocObroty1;
	bool motorOdwrocObroty2;
	bool motorOdwrocObroty3;
	bool motorOdwrocObroty4;
	bool motorOdwrocObroty5;
	bool motorOdwrocObroty6;
	bool motorOdwrocObroty7;
	bool motorOdwrocObroty8;
	bool motorOdwrocObroty9;
	int motorCzasMiedzyImpZerow1;
	int motorCzasMiedzyImpZerow2;
	int motorCzasMiedzyImpZerow3;
	int motorCzasMiedzyImpZerow4;
	int motorCzasMiedzyImpZerow5;
	int motorCzasMiedzyImpZerow6;
	int motorCzasMiedzyImpZerow7;
	int motorCzasMiedzyImpZerow8;
	int motorCzasMiedzyImpZerow9;
	int motorCzasMiedzyImpNormal1;
	int motorCzasMiedzyImpNormal2;
	int motorCzasMiedzyImpNormal3;
	int motorCzasMiedzyImpNormal4;
	int motorCzasMiedzyImpNormal5;
	int motorCzasMiedzyImpNormal6;
	int motorCzasMiedzyImpNormal7;
	int motorCzasMiedzyImpNormal8;
	int motorCzasMiedzyImpNormal9;
	int motorMaksIloscImp1;
	int motorMaksIloscImp2;
	int motorMaksIloscImp3;
	int motorMaksIloscImp4;
	int motorMaksIloscImp5;
	int motorMaksIloscImp6;
	int motorMaksIloscImp7;
	int motorMaksIloscImp8;
	int motorMaksIloscImp9;
	int motorIloscImpBaza1;
	int motorIloscImpBaza2;
	int motorIloscImpBaza3;
	int motorIloscImpBaza4;
	int motorIloscImpBaza5;
	int motorIloscImpBaza6;
	int motorIloscImpBaza7;
	int motorIloscImpBaza8;
	int motorIloscImpBaza9;
	int motorIloscImpSrodek1;
	int motorIloscImpSrodek2;
	int motorIloscImpSrodek3;
	int motorIloscImpSrodek4;
	int motorIloscImpSrodek5;
	int motorIloscImpSrodek6;
	int motorIloscImpSrodek7;
	int motorIloscImpSrodek8;
	int motorIloscImpSrodek9;
	double filtr_db_880_A_0;
	double filtr_db_880_B_0;
	double filtr_db_880_C_0;
	double filtr_db_880_A_1;
	double filtr_db_880_B_1;
	double filtr_db_880_C_1;
	double filtr_db_880_A_2;
	double filtr_db_880_B_2;
	double filtr_db_880_C_2;
	double filtr_db_880_A_3;
	double filtr_db_880_B_3;
	double filtr_db_880_C_3;
	double filtr_db_880_A_4;
	double filtr_db_880_B_4;
	double filtr_db_880_C_4;
	double filtr_db_880_A_5;
	double filtr_db_880_B_5;
	double filtr_db_880_C_5;
	double filtr_db_655_A_0;
	double filtr_db_655_B_0;
	double filtr_db_655_C_0;
	double filtr_db_655_A_1;
	double filtr_db_655_B_1;
	double filtr_db_655_C_1;
	double filtr_db_655_A_2;
	double filtr_db_655_B_2;
	double filtr_db_655_C_2;
	double filtr_db_655_A_3;
	double filtr_db_655_B_3;
	double filtr_db_655_C_3;
	double filtr_db_655_A_4;
	double filtr_db_655_B_4;
	double filtr_db_655_C_4;
	double filtr_db_655_A_5;
	double filtr_db_655_B_5;
	double filtr_db_655_C_5;
	QString serialDeviceZasilaczVendor;
	QString serialDeviceZasilaczProduct;
	QString serialDeviceSterownikVendor;
	QString serialDeviceSterownikProduct;
	QString serialDeviceSterownikSerial;
	int minNapiecieCzujki;
	int maxNapiecieCzujki;
	int minCzasStabCzujki;
	int maxCzasStabCzujki;
	double minPrzekrPradZasCzujki;
	double maxPrzekrPradZasCzujki;
	int minCzasPoZmianieFiltra;
	int maxCzasPoZmianieFiltra;
	unsigned int czasWylaczeniaCzujkiDlaResetu;
	unsigned int czasOczekiwaniaPowtarzalnosc4Test;
	unsigned int minimalnyCzasOczekiwaniaPowtarzalnosc1Test;
	unsigned int maksymalnyCzasOczekiwaniaPowtarzalnosc1Test;
	double minimalnaWartoscCzujkiCn;
	unsigned int czasStabilizacjiDlaKataNieWspolosiowosci;
	double wartoscTlumienieDlaKataNieWspolosiowosci;
	unsigned int maksCzasZadzialaniaCzujkiDlaKataNieWspolosiowosci;
	unsigned int maksCzasTestuZadzialaniaCzujkiDlaKataNieWspolosiowosci;
	double maksKatNieWspolOsiowosci;
	double maksRoznicaKatNieWspolOsiowosci;
	double odtwarzalnoscCmaxCrep;
	double odtwarzalnoscCrepCmin;
	double powtarzalnoscCmaxCmin;
	double niewspolosiowoscMinimalnyKatProducentMierzony;
	unsigned int maksCzasZadzialaniaCzujkidlaTlumnikaA;
	unsigned int maksCzasTestuCzujkidlaTlumnikaA;
	unsigned int maksCzasZadzialaniaCzujkidlaTlumnikaB;
	unsigned int maksCzasTestuCzujkidlaTlumnikaB;
	double szybkieZmianyWartoscTlumnikaA;
	double szybkieZmianyWartoscTlumnikaB;
	double dlugoscDrogiOptycznejCmaxCmin;
	double rozproszoneSwiatloCmaxCmin;
	double tolerancjaNapieciaZasilaniaCmaxCmin;
	double minimalneNapieciaTolerancjaNapiecia;
	double maksymalneNapieciaTolerancjaNapiecia;
	double minimalnaTemperatura;
	double maksymalnaTemperatura;
	double minimalnaWilgotnosc;
	double maksymalnaWilgotnosc;
	double minimalnaCisnienie;
	double maksymalnaCisnienie;
};
#endif