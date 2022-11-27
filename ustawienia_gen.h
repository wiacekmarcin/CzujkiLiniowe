
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

protected:
    QSettings settings;
    
    bool toBool(const QString & val) const { return QVariant(val).toBool(); }
    double toDouble(const QString & val) const { return val.toDouble(); }
    unsigned int toUInt(const QString & val) const { return val.toUInt(); }
    QString toQString(const QString & val) const { return val; }

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
	double getMotorPrzelozenie1() const;
	void setMotorPrzelozenie1(const double & przelozenie1);
	inline void setMotorPrzelozenie1(const QString & przelozenie1) { setMotorPrzelozenie1(toDouble(przelozenie1)); }
	double getMotorPrzelozenie2() const;
	void setMotorPrzelozenie2(const double & przelozenie2);
	inline void setMotorPrzelozenie2(const QString & przelozenie2) { setMotorPrzelozenie2(toDouble(przelozenie2)); }
	double getMotorPrzelozenie3() const;
	void setMotorPrzelozenie3(const double & przelozenie3);
	inline void setMotorPrzelozenie3(const QString & przelozenie3) { setMotorPrzelozenie3(toDouble(przelozenie3)); }
	double getMotorPrzelozenie4() const;
	void setMotorPrzelozenie4(const double & przelozenie4);
	inline void setMotorPrzelozenie4(const QString & przelozenie4) { setMotorPrzelozenie4(toDouble(przelozenie4)); }
	double getMotorPrzelozenie5() const;
	void setMotorPrzelozenie5(const double & przelozenie5);
	inline void setMotorPrzelozenie5(const QString & przelozenie5) { setMotorPrzelozenie5(toDouble(przelozenie5)); }
	double getMotorPrzelozenie6() const;
	void setMotorPrzelozenie6(const double & przelozenie6);
	inline void setMotorPrzelozenie6(const QString & przelozenie6) { setMotorPrzelozenie6(toDouble(przelozenie6)); }
	double getMotorPrzelozenie7() const;
	void setMotorPrzelozenie7(const double & przelozenie7);
	inline void setMotorPrzelozenie7(const QString & przelozenie7) { setMotorPrzelozenie7(toDouble(przelozenie7)); }
	double getMotorPrzelozenie8() const;
	void setMotorPrzelozenie8(const double & przelozenie8);
	inline void setMotorPrzelozenie8(const QString & przelozenie8) { setMotorPrzelozenie8(toDouble(przelozenie8)); }
	double getMotorPrzelozenie9() const;
	void setMotorPrzelozenie9(const double & przelozenie9);
	inline void setMotorPrzelozenie9(const QString & przelozenie9) { setMotorPrzelozenie9(toDouble(przelozenie9)); }
	double getMotorPrzelozenie(const short & nrMotor) const;
	void setMotorPrzelozenie(const short & nrMotor, const double & przelozenie);
	double getMotorMaksPredkosc1() const;
	void setMotorMaksPredkosc1(const double & maksPredkosc1);
	inline void setMotorMaksPredkosc1(const QString & maksPredkosc1) { setMotorMaksPredkosc1(toDouble(maksPredkosc1)); }
	double getMotorMaksPredkosc2() const;
	void setMotorMaksPredkosc2(const double & maksPredkosc2);
	inline void setMotorMaksPredkosc2(const QString & maksPredkosc2) { setMotorMaksPredkosc2(toDouble(maksPredkosc2)); }
	double getMotorMaksPredkosc3() const;
	void setMotorMaksPredkosc3(const double & maksPredkosc3);
	inline void setMotorMaksPredkosc3(const QString & maksPredkosc3) { setMotorMaksPredkosc3(toDouble(maksPredkosc3)); }
	double getMotorMaksPredkosc4() const;
	void setMotorMaksPredkosc4(const double & maksPredkosc4);
	inline void setMotorMaksPredkosc4(const QString & maksPredkosc4) { setMotorMaksPredkosc4(toDouble(maksPredkosc4)); }
	double getMotorMaksPredkosc5() const;
	void setMotorMaksPredkosc5(const double & maksPredkosc5);
	inline void setMotorMaksPredkosc5(const QString & maksPredkosc5) { setMotorMaksPredkosc5(toDouble(maksPredkosc5)); }
	double getMotorMaksPredkosc6() const;
	void setMotorMaksPredkosc6(const double & maksPredkosc6);
	inline void setMotorMaksPredkosc6(const QString & maksPredkosc6) { setMotorMaksPredkosc6(toDouble(maksPredkosc6)); }
	double getMotorMaksPredkosc7() const;
	void setMotorMaksPredkosc7(const double & maksPredkosc7);
	inline void setMotorMaksPredkosc7(const QString & maksPredkosc7) { setMotorMaksPredkosc7(toDouble(maksPredkosc7)); }
	double getMotorMaksPredkosc8() const;
	void setMotorMaksPredkosc8(const double & maksPredkosc8);
	inline void setMotorMaksPredkosc8(const QString & maksPredkosc8) { setMotorMaksPredkosc8(toDouble(maksPredkosc8)); }
	double getMotorMaksPredkosc9() const;
	void setMotorMaksPredkosc9(const double & maksPredkosc9);
	inline void setMotorMaksPredkosc9(const QString & maksPredkosc9) { setMotorMaksPredkosc9(toDouble(maksPredkosc9)); }
	double getMotorMaksPredkosc(const short & nrMotor) const;
	void setMotorMaksPredkosc(const short & nrMotor, const double & maksPredkosc);
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
	int getMotorOpoznienieImp1() const;
	void setMotorOpoznienieImp1(const int & opoznienieImp1);
	inline void setMotorOpoznienieImp1(const QString & opoznienieImp1) { setMotorOpoznienieImp1(toUInt(opoznienieImp1)); }
	int getMotorOpoznienieImp2() const;
	void setMotorOpoznienieImp2(const int & opoznienieImp2);
	inline void setMotorOpoznienieImp2(const QString & opoznienieImp2) { setMotorOpoznienieImp2(toUInt(opoznienieImp2)); }
	int getMotorOpoznienieImp3() const;
	void setMotorOpoznienieImp3(const int & opoznienieImp3);
	inline void setMotorOpoznienieImp3(const QString & opoznienieImp3) { setMotorOpoznienieImp3(toUInt(opoznienieImp3)); }
	int getMotorOpoznienieImp4() const;
	void setMotorOpoznienieImp4(const int & opoznienieImp4);
	inline void setMotorOpoznienieImp4(const QString & opoznienieImp4) { setMotorOpoznienieImp4(toUInt(opoznienieImp4)); }
	int getMotorOpoznienieImp5() const;
	void setMotorOpoznienieImp5(const int & opoznienieImp5);
	inline void setMotorOpoznienieImp5(const QString & opoznienieImp5) { setMotorOpoznienieImp5(toUInt(opoznienieImp5)); }
	int getMotorOpoznienieImp6() const;
	void setMotorOpoznienieImp6(const int & opoznienieImp6);
	inline void setMotorOpoznienieImp6(const QString & opoznienieImp6) { setMotorOpoznienieImp6(toUInt(opoznienieImp6)); }
	int getMotorOpoznienieImp7() const;
	void setMotorOpoznienieImp7(const int & opoznienieImp7);
	inline void setMotorOpoznienieImp7(const QString & opoznienieImp7) { setMotorOpoznienieImp7(toUInt(opoznienieImp7)); }
	int getMotorOpoznienieImp8() const;
	void setMotorOpoznienieImp8(const int & opoznienieImp8);
	inline void setMotorOpoznienieImp8(const QString & opoznienieImp8) { setMotorOpoznienieImp8(toUInt(opoznienieImp8)); }
	int getMotorOpoznienieImp9() const;
	void setMotorOpoznienieImp9(const int & opoznienieImp9);
	inline void setMotorOpoznienieImp9(const QString & opoznienieImp9) { setMotorOpoznienieImp9(toUInt(opoznienieImp9)); }
	int getMotorOpoznienieImp(const short & nrMotor) const;
	void setMotorOpoznienieImp(const short & nrMotor, const int & opoznienieImp);
	int getMotorMaksIloscKrokow1() const;
	void setMotorMaksIloscKrokow1(const int & maksIloscKrokow1);
	inline void setMotorMaksIloscKrokow1(const QString & maksIloscKrokow1) { setMotorMaksIloscKrokow1(toUInt(maksIloscKrokow1)); }
	int getMotorMaksIloscKrokow2() const;
	void setMotorMaksIloscKrokow2(const int & maksIloscKrokow2);
	inline void setMotorMaksIloscKrokow2(const QString & maksIloscKrokow2) { setMotorMaksIloscKrokow2(toUInt(maksIloscKrokow2)); }
	int getMotorMaksIloscKrokow3() const;
	void setMotorMaksIloscKrokow3(const int & maksIloscKrokow3);
	inline void setMotorMaksIloscKrokow3(const QString & maksIloscKrokow3) { setMotorMaksIloscKrokow3(toUInt(maksIloscKrokow3)); }
	int getMotorMaksIloscKrokow4() const;
	void setMotorMaksIloscKrokow4(const int & maksIloscKrokow4);
	inline void setMotorMaksIloscKrokow4(const QString & maksIloscKrokow4) { setMotorMaksIloscKrokow4(toUInt(maksIloscKrokow4)); }
	int getMotorMaksIloscKrokow5() const;
	void setMotorMaksIloscKrokow5(const int & maksIloscKrokow5);
	inline void setMotorMaksIloscKrokow5(const QString & maksIloscKrokow5) { setMotorMaksIloscKrokow5(toUInt(maksIloscKrokow5)); }
	int getMotorMaksIloscKrokow6() const;
	void setMotorMaksIloscKrokow6(const int & maksIloscKrokow6);
	inline void setMotorMaksIloscKrokow6(const QString & maksIloscKrokow6) { setMotorMaksIloscKrokow6(toUInt(maksIloscKrokow6)); }
	int getMotorMaksIloscKrokow7() const;
	void setMotorMaksIloscKrokow7(const int & maksIloscKrokow7);
	inline void setMotorMaksIloscKrokow7(const QString & maksIloscKrokow7) { setMotorMaksIloscKrokow7(toUInt(maksIloscKrokow7)); }
	int getMotorMaksIloscKrokow8() const;
	void setMotorMaksIloscKrokow8(const int & maksIloscKrokow8);
	inline void setMotorMaksIloscKrokow8(const QString & maksIloscKrokow8) { setMotorMaksIloscKrokow8(toUInt(maksIloscKrokow8)); }
	int getMotorMaksIloscKrokow9() const;
	void setMotorMaksIloscKrokow9(const int & maksIloscKrokow9);
	inline void setMotorMaksIloscKrokow9(const QString & maksIloscKrokow9) { setMotorMaksIloscKrokow9(toUInt(maksIloscKrokow9)); }
	int getMotorMaksIloscKrokow(const short & nrMotor) const;
	void setMotorMaksIloscKrokow(const short & nrMotor, const int & maksIloscKrokow);
	double getFiltr880_A_0() const;
	void setFiltr880_A_0(const double & val);
	inline void setFiltr880_A_0(const QString & vals) { setFiltr880_A_0(toDouble(vals)); }
	double getFiltr880_B_0() const;
	void setFiltr880_B_0(const double & val);
	inline void setFiltr880_B_0(const QString & vals) { setFiltr880_B_0(toDouble(vals)); }
	double getFiltr880_C_0() const;
	void setFiltr880_C_0(const double & val);
	inline void setFiltr880_C_0(const QString & vals) { setFiltr880_C_0(toDouble(vals)); }
	double getFiltr880_A_1() const;
	void setFiltr880_A_1(const double & val);
	inline void setFiltr880_A_1(const QString & vals) { setFiltr880_A_1(toDouble(vals)); }
	double getFiltr880_B_1() const;
	void setFiltr880_B_1(const double & val);
	inline void setFiltr880_B_1(const QString & vals) { setFiltr880_B_1(toDouble(vals)); }
	double getFiltr880_C_1() const;
	void setFiltr880_C_1(const double & val);
	inline void setFiltr880_C_1(const QString & vals) { setFiltr880_C_1(toDouble(vals)); }
	double getFiltr880_A_2() const;
	void setFiltr880_A_2(const double & val);
	inline void setFiltr880_A_2(const QString & vals) { setFiltr880_A_2(toDouble(vals)); }
	double getFiltr880_B_2() const;
	void setFiltr880_B_2(const double & val);
	inline void setFiltr880_B_2(const QString & vals) { setFiltr880_B_2(toDouble(vals)); }
	double getFiltr880_C_2() const;
	void setFiltr880_C_2(const double & val);
	inline void setFiltr880_C_2(const QString & vals) { setFiltr880_C_2(toDouble(vals)); }
	double getFiltr880_A_3() const;
	void setFiltr880_A_3(const double & val);
	inline void setFiltr880_A_3(const QString & vals) { setFiltr880_A_3(toDouble(vals)); }
	double getFiltr880_B_3() const;
	void setFiltr880_B_3(const double & val);
	inline void setFiltr880_B_3(const QString & vals) { setFiltr880_B_3(toDouble(vals)); }
	double getFiltr880_C_3() const;
	void setFiltr880_C_3(const double & val);
	inline void setFiltr880_C_3(const QString & vals) { setFiltr880_C_3(toDouble(vals)); }
	double getFiltr880_A_4() const;
	void setFiltr880_A_4(const double & val);
	inline void setFiltr880_A_4(const QString & vals) { setFiltr880_A_4(toDouble(vals)); }
	double getFiltr880_B_4() const;
	void setFiltr880_B_4(const double & val);
	inline void setFiltr880_B_4(const QString & vals) { setFiltr880_B_4(toDouble(vals)); }
	double getFiltr880_C_4() const;
	void setFiltr880_C_4(const double & val);
	inline void setFiltr880_C_4(const QString & vals) { setFiltr880_C_4(toDouble(vals)); }
	double getFiltr880_A_5() const;
	void setFiltr880_A_5(const double & val);
	inline void setFiltr880_A_5(const QString & vals) { setFiltr880_A_5(toDouble(vals)); }
	double getFiltr880_B_5() const;
	void setFiltr880_B_5(const double & val);
	inline void setFiltr880_B_5(const QString & vals) { setFiltr880_B_5(toDouble(vals)); }
	double getFiltr880_C_5() const;
	void setFiltr880_C_5(const double & val);
	inline void setFiltr880_C_5(const QString & vals) { setFiltr880_C_5(toDouble(vals)); }
	double getFiltr655_A_0() const;
	void setFiltr655_A_0(const double & val);
	inline void setFiltr655_A_0(const QString & vals) { setFiltr655_A_0(toDouble(vals)); }
	double getFiltr655_B_0() const;
	void setFiltr655_B_0(const double & val);
	inline void setFiltr655_B_0(const QString & vals) { setFiltr655_B_0(toDouble(vals)); }
	double getFiltr655_C_0() const;
	void setFiltr655_C_0(const double & val);
	inline void setFiltr655_C_0(const QString & vals) { setFiltr655_C_0(toDouble(vals)); }
	double getFiltr655_A_1() const;
	void setFiltr655_A_1(const double & val);
	inline void setFiltr655_A_1(const QString & vals) { setFiltr655_A_1(toDouble(vals)); }
	double getFiltr655_B_1() const;
	void setFiltr655_B_1(const double & val);
	inline void setFiltr655_B_1(const QString & vals) { setFiltr655_B_1(toDouble(vals)); }
	double getFiltr655_C_1() const;
	void setFiltr655_C_1(const double & val);
	inline void setFiltr655_C_1(const QString & vals) { setFiltr655_C_1(toDouble(vals)); }
	double getFiltr655_A_2() const;
	void setFiltr655_A_2(const double & val);
	inline void setFiltr655_A_2(const QString & vals) { setFiltr655_A_2(toDouble(vals)); }
	double getFiltr655_B_2() const;
	void setFiltr655_B_2(const double & val);
	inline void setFiltr655_B_2(const QString & vals) { setFiltr655_B_2(toDouble(vals)); }
	double getFiltr655_C_2() const;
	void setFiltr655_C_2(const double & val);
	inline void setFiltr655_C_2(const QString & vals) { setFiltr655_C_2(toDouble(vals)); }
	double getFiltr655_A_3() const;
	void setFiltr655_A_3(const double & val);
	inline void setFiltr655_A_3(const QString & vals) { setFiltr655_A_3(toDouble(vals)); }
	double getFiltr655_B_3() const;
	void setFiltr655_B_3(const double & val);
	inline void setFiltr655_B_3(const QString & vals) { setFiltr655_B_3(toDouble(vals)); }
	double getFiltr655_C_3() const;
	void setFiltr655_C_3(const double & val);
	inline void setFiltr655_C_3(const QString & vals) { setFiltr655_C_3(toDouble(vals)); }
	double getFiltr655_A_4() const;
	void setFiltr655_A_4(const double & val);
	inline void setFiltr655_A_4(const QString & vals) { setFiltr655_A_4(toDouble(vals)); }
	double getFiltr655_B_4() const;
	void setFiltr655_B_4(const double & val);
	inline void setFiltr655_B_4(const QString & vals) { setFiltr655_B_4(toDouble(vals)); }
	double getFiltr655_C_4() const;
	void setFiltr655_C_4(const double & val);
	inline void setFiltr655_C_4(const QString & vals) { setFiltr655_C_4(toDouble(vals)); }
	double getFiltr655_A_5() const;
	void setFiltr655_A_5(const double & val);
	inline void setFiltr655_A_5(const QString & vals) { setFiltr655_A_5(toDouble(vals)); }
	double getFiltr655_B_5() const;
	void setFiltr655_B_5(const double & val);
	inline void setFiltr655_B_5(const QString & vals) { setFiltr655_B_5(toDouble(vals)); }
	double getFiltr655_C_5() const;
	void setFiltr655_C_5(const double & val);
	inline void setFiltr655_C_5(const QString & vals) { setFiltr655_C_5(toDouble(vals)); }
	double getFiltr880_A(const short & nrPos) const { return getFiltr(880, 'A', nrPos); }
	double getFiltr880_B(const short & nrPos) const { return getFiltr(880, 'B', nrPos); }
	double getFiltr880_C(const short & nrPos) const { return getFiltr(880, 'C', nrPos); }
	double getFiltr655_A(const short & nrPos) const { return getFiltr(655, 'A', nrPos); }
	double getFiltr655_B(const short & nrPos) const { return getFiltr(655, 'B', nrPos); }
	double getFiltr655_C(const short & nrPos) const { return getFiltr(655, 'C', nrPos); }
	double getFiltr880(const char & nrTarczy, const short & nrPos) const { return getFiltr(880, nrTarczy, nrPos); }
	double getFiltr665(const char & nrTarczy, const short & nrPos) const { return getFiltr(665, nrTarczy, nrPos); }
	void setFiltr880_A(const short & nrPos, const double & val) { setFiltr(880, 'A', nrPos, val); }
	void setFiltr880_B(const short & nrPos, const double & val) { setFiltr(880, 'B', nrPos, val); }
	void setFiltr880_C(const short & nrPos, const double & val) { setFiltr(880, 'C', nrPos, val); }
	void setFiltr655_A(const short & nrPos, const double & val) { setFiltr(655, 'A', nrPos, val); }
	void setFiltr655_B(const short & nrPos, const double & val) { setFiltr(655, 'B', nrPos, val); }
	void setFiltr655_C(const short & nrPos, const double & val) { setFiltr(655, 'C', nrPos, val); }
	void setFiltr880(const char & nrTarczy, const short & nrPos, const double & val) { setFiltr(880, nrTarczy, nrPos, val); }
	void setFiltr665(const char & nrTarczy, const short & nrPos, const double & val) { setFiltr(665, nrTarczy, nrPos, val); }
protected:
	bool checkMotorNazwa(const QString & val);
	bool checkMotorPrzelozenie(const QString & val);
	bool checkMotorMaksPredkosc(const QString & val);
	bool checkMotorOdwrocObroty(const QString & val);
	bool checkMotorOpoznienieImp(const QString & val);
	bool checkMotorMaksIloscKrokow(const QString & val);
	bool checkFilterValue(const QString & val);
	double getFiltr(const int& fala, const char & nrTarczy, const short & nrPos) const;
	void setFiltr(const int& fala, const char & nrTarczy, const short & nrPos, const double & val);
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
	double motorPrzelozenie1;
	double motorPrzelozenie2;
	double motorPrzelozenie3;
	double motorPrzelozenie4;
	double motorPrzelozenie5;
	double motorPrzelozenie6;
	double motorPrzelozenie7;
	double motorPrzelozenie8;
	double motorPrzelozenie9;
	double motorMaksPredkosc1;
	double motorMaksPredkosc2;
	double motorMaksPredkosc3;
	double motorMaksPredkosc4;
	double motorMaksPredkosc5;
	double motorMaksPredkosc6;
	double motorMaksPredkosc7;
	double motorMaksPredkosc8;
	double motorMaksPredkosc9;
	bool motorOdwrocObroty1;
	bool motorOdwrocObroty2;
	bool motorOdwrocObroty3;
	bool motorOdwrocObroty4;
	bool motorOdwrocObroty5;
	bool motorOdwrocObroty6;
	bool motorOdwrocObroty7;
	bool motorOdwrocObroty8;
	bool motorOdwrocObroty9;
	int motorOpoznienieImp1;
	int motorOpoznienieImp2;
	int motorOpoznienieImp3;
	int motorOpoznienieImp4;
	int motorOpoznienieImp5;
	int motorOpoznienieImp6;
	int motorOpoznienieImp7;
	int motorOpoznienieImp8;
	int motorOpoznienieImp9;
	int motorMaksIloscKrokow1;
	int motorMaksIloscKrokow2;
	int motorMaksIloscKrokow3;
	int motorMaksIloscKrokow4;
	int motorMaksIloscKrokow5;
	int motorMaksIloscKrokow6;
	int motorMaksIloscKrokow7;
	int motorMaksIloscKrokow8;
	int motorMaksIloscKrokow9;
	double filtr880_A_0;
	double filtr880_B_0;
	double filtr880_C_0;
	double filtr880_A_1;
	double filtr880_B_1;
	double filtr880_C_1;
	double filtr880_A_2;
	double filtr880_B_2;
	double filtr880_C_2;
	double filtr880_A_3;
	double filtr880_B_3;
	double filtr880_C_3;
	double filtr880_A_4;
	double filtr880_B_4;
	double filtr880_C_4;
	double filtr880_A_5;
	double filtr880_B_5;
	double filtr880_C_5;
	double filtr655_A_0;
	double filtr655_B_0;
	double filtr655_C_0;
	double filtr655_A_1;
	double filtr655_B_1;
	double filtr655_C_1;
	double filtr655_A_2;
	double filtr655_B_2;
	double filtr655_C_2;
	double filtr655_A_3;
	double filtr655_B_3;
	double filtr655_C_3;
	double filtr655_A_4;
	double filtr655_B_4;
	double filtr655_C_4;
	double filtr655_A_5;
	double filtr655_B_5;
	double filtr655_C_5;
};
#endif