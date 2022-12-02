
#include "ustawienia_gen.h"

#include <QApplication>

UstawieniaGen::UstawieniaGen() :
    settings(QApplication::applicationFilePath() + ".ini", QSettings::IniFormat)
{
    //m_sSettingsFile = QApplication::applicationDirPath() + "/demosettings.ini";
    //QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    
    load();
}

UstawieniaGen::~UstawieniaGen()
{
    
}
        

void UstawieniaGen::load()
{
	motorNazwa1 = toQString(settings.value("Silnik-1/Nazwa", QVariant::fromValue(QString("M1"))).toString());
	motorNazwa2 = toQString(settings.value("Silnik-2/Nazwa", QVariant::fromValue(QString("M2"))).toString());
	motorNazwa3 = toQString(settings.value("Silnik-3/Nazwa", QVariant::fromValue(QString("M3"))).toString());
	motorNazwa4 = toQString(settings.value("Silnik-4/Nazwa", QVariant::fromValue(QString("M4"))).toString());
	motorNazwa5 = toQString(settings.value("Silnik-5/Nazwa", QVariant::fromValue(QString("M5"))).toString());
	motorNazwa6 = toQString(settings.value("Silnik-6/Nazwa", QVariant::fromValue(QString("M6"))).toString());
	motorNazwa7 = toQString(settings.value("Silnik-7/Nazwa", QVariant::fromValue(QString("M7"))).toString());
	motorNazwa8 = toQString(settings.value("Silnik-8/Nazwa", QVariant::fromValue(QString("M8"))).toString());
	motorNazwa9 = toQString(settings.value("Silnik-9/Nazwa", QVariant::fromValue(QString("M9"))).toString());
	motorPrzelozenie1 = toDouble(settings.value("Silnik-1/Przelozenie", QVariant::fromValue(1.1)).toString());
	motorPrzelozenie2 = toDouble(settings.value("Silnik-2/Przelozenie", QVariant::fromValue(1.2)).toString());
	motorPrzelozenie3 = toDouble(settings.value("Silnik-3/Przelozenie", QVariant::fromValue(1.3)).toString());
	motorPrzelozenie4 = toDouble(settings.value("Silnik-4/Przelozenie", QVariant::fromValue(1.4)).toString());
	motorPrzelozenie5 = toDouble(settings.value("Silnik-5/Przelozenie", QVariant::fromValue(1.5)).toString());
	motorPrzelozenie6 = toDouble(settings.value("Silnik-6/Przelozenie", QVariant::fromValue(1.6)).toString());
	motorPrzelozenie7 = toDouble(settings.value("Silnik-7/Przelozenie", QVariant::fromValue(1.7)).toString());
	motorPrzelozenie8 = toDouble(settings.value("Silnik-8/Przelozenie", QVariant::fromValue(1.8)).toString());
	motorPrzelozenie9 = toDouble(settings.value("Silnik-9/Przelozenie", QVariant::fromValue(1.9)).toString());
	motorMaksPredkosc1 = toDouble(settings.value("Silnik-1/MaksPredkosc", QVariant::fromValue(2.1)).toString());
	motorMaksPredkosc2 = toDouble(settings.value("Silnik-2/MaksPredkosc", QVariant::fromValue(2.2)).toString());
	motorMaksPredkosc3 = toDouble(settings.value("Silnik-3/MaksPredkosc", QVariant::fromValue(2.3)).toString());
	motorMaksPredkosc4 = toDouble(settings.value("Silnik-4/MaksPredkosc", QVariant::fromValue(2.4)).toString());
	motorMaksPredkosc5 = toDouble(settings.value("Silnik-5/MaksPredkosc", QVariant::fromValue(2.5)).toString());
	motorMaksPredkosc6 = toDouble(settings.value("Silnik-6/MaksPredkosc", QVariant::fromValue(2.6)).toString());
	motorMaksPredkosc7 = toDouble(settings.value("Silnik-7/MaksPredkosc", QVariant::fromValue(2.7)).toString());
	motorMaksPredkosc8 = toDouble(settings.value("Silnik-8/MaksPredkosc", QVariant::fromValue(2.8)).toString());
	motorMaksPredkosc9 = toDouble(settings.value("Silnik-9/MaksPredkosc", QVariant::fromValue(2.9)).toString());
	motorOdwrocObroty1 = toBool(settings.value("Silnik-1/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty2 = toBool(settings.value("Silnik-2/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty3 = toBool(settings.value("Silnik-3/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty4 = toBool(settings.value("Silnik-4/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty5 = toBool(settings.value("Silnik-5/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty6 = toBool(settings.value("Silnik-6/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty7 = toBool(settings.value("Silnik-7/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty8 = toBool(settings.value("Silnik-8/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty9 = toBool(settings.value("Silnik-9/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOpoznienieImp1 = toUInt(settings.value("Silnik-1/OpoznienieImp", QVariant::fromValue(1251)).toString());
	motorOpoznienieImp2 = toUInt(settings.value("Silnik-2/OpoznienieImp", QVariant::fromValue(1252)).toString());
	motorOpoznienieImp3 = toUInt(settings.value("Silnik-3/OpoznienieImp", QVariant::fromValue(1253)).toString());
	motorOpoznienieImp4 = toUInt(settings.value("Silnik-4/OpoznienieImp", QVariant::fromValue(1254)).toString());
	motorOpoznienieImp5 = toUInt(settings.value("Silnik-5/OpoznienieImp", QVariant::fromValue(1255)).toString());
	motorOpoznienieImp6 = toUInt(settings.value("Silnik-6/OpoznienieImp", QVariant::fromValue(1256)).toString());
	motorOpoznienieImp7 = toUInt(settings.value("Silnik-7/OpoznienieImp", QVariant::fromValue(1257)).toString());
	motorOpoznienieImp8 = toUInt(settings.value("Silnik-8/OpoznienieImp", QVariant::fromValue(1258)).toString());
	motorOpoznienieImp9 = toUInt(settings.value("Silnik-9/OpoznienieImp", QVariant::fromValue(1259)).toString());
	motorMaksIloscKrokow1 = toUInt(settings.value("Silnik-1/MaksIloscKrokow", QVariant::fromValue(1001)).toString());
	motorMaksIloscKrokow2 = toUInt(settings.value("Silnik-2/MaksIloscKrokow", QVariant::fromValue(1002)).toString());
	motorMaksIloscKrokow3 = toUInt(settings.value("Silnik-3/MaksIloscKrokow", QVariant::fromValue(1003)).toString());
	motorMaksIloscKrokow4 = toUInt(settings.value("Silnik-4/MaksIloscKrokow", QVariant::fromValue(1004)).toString());
	motorMaksIloscKrokow5 = toUInt(settings.value("Silnik-5/MaksIloscKrokow", QVariant::fromValue(1005)).toString());
	motorMaksIloscKrokow6 = toUInt(settings.value("Silnik-6/MaksIloscKrokow", QVariant::fromValue(1006)).toString());
	motorMaksIloscKrokow7 = toUInt(settings.value("Silnik-7/MaksIloscKrokow", QVariant::fromValue(1007)).toString());
	motorMaksIloscKrokow8 = toUInt(settings.value("Silnik-8/MaksIloscKrokow", QVariant::fromValue(1008)).toString());
	motorMaksIloscKrokow9 = toUInt(settings.value("Silnik-9/MaksIloscKrokow", QVariant::fromValue(1009)).toString());
	motorIloscKrokowBaza1 = toUInt(settings.value("Silnik-1/IloscKrokowBaza", QVariant::fromValue(0)).toString());
	motorIloscKrokowBaza2 = toUInt(settings.value("Silnik-2/IloscKrokowBaza", QVariant::fromValue(0)).toString());
	motorIloscKrokowBaza3 = toUInt(settings.value("Silnik-3/IloscKrokowBaza", QVariant::fromValue(0)).toString());
	motorIloscKrokowBaza4 = toUInt(settings.value("Silnik-4/IloscKrokowBaza", QVariant::fromValue(0)).toString());
	motorIloscKrokowBaza5 = toUInt(settings.value("Silnik-5/IloscKrokowBaza", QVariant::fromValue(0)).toString());
	motorIloscKrokowBaza6 = toUInt(settings.value("Silnik-6/IloscKrokowBaza", QVariant::fromValue(0)).toString());
	motorIloscKrokowBaza7 = toUInt(settings.value("Silnik-7/IloscKrokowBaza", QVariant::fromValue(0)).toString());
	motorIloscKrokowBaza8 = toUInt(settings.value("Silnik-8/IloscKrokowBaza", QVariant::fromValue(0)).toString());
	motorIloscKrokowBaza9 = toUInt(settings.value("Silnik-9/IloscKrokowBaza", QVariant::fromValue(0)).toString());
	motorIloscKrokowSrodek1 = toUInt(settings.value("Silnik-1/IloscKrokowSrodek", QVariant::fromValue(0)).toString());
	motorIloscKrokowSrodek2 = toUInt(settings.value("Silnik-2/IloscKrokowSrodek", QVariant::fromValue(0)).toString());
	motorIloscKrokowSrodek3 = toUInt(settings.value("Silnik-3/IloscKrokowSrodek", QVariant::fromValue(0)).toString());
	motorIloscKrokowSrodek4 = toUInt(settings.value("Silnik-4/IloscKrokowSrodek", QVariant::fromValue(0)).toString());
	motorIloscKrokowSrodek5 = toUInt(settings.value("Silnik-5/IloscKrokowSrodek", QVariant::fromValue(0)).toString());
	motorIloscKrokowSrodek6 = toUInt(settings.value("Silnik-6/IloscKrokowSrodek", QVariant::fromValue(0)).toString());
	motorIloscKrokowSrodek7 = toUInt(settings.value("Silnik-7/IloscKrokowSrodek", QVariant::fromValue(0)).toString());
	motorIloscKrokowSrodek8 = toUInt(settings.value("Silnik-8/IloscKrokowSrodek", QVariant::fromValue(0)).toString());
	motorIloscKrokowSrodek9 = toUInt(settings.value("Silnik-9/IloscKrokowSrodek", QVariant::fromValue(0)).toString());
	filtr880_A_0 = toDouble(settings.value("Filtr_880_A/Pos0", QVariant::fromValue(0)).toString());
	filtr880_B_0 = toDouble(settings.value("Filtr_880_B/Pos0", QVariant::fromValue(0)).toString());
	filtr880_C_0 = toDouble(settings.value("Filtr_880_C/Pos0", QVariant::fromValue(0)).toString());
	filtr880_A_1 = toDouble(settings.value("Filtr_880_A/Pos1", QVariant::fromValue(0.22)).toString());
	filtr880_B_1 = toDouble(settings.value("Filtr_880_B/Pos1", QVariant::fromValue(0.1)).toString());
	filtr880_C_1 = toDouble(settings.value("Filtr_880_C/Pos1", QVariant::fromValue(0.09)).toString());
	filtr880_A_2 = toDouble(settings.value("Filtr_880_A/Pos2", QVariant::fromValue(1.02)).toString());
	filtr880_B_2 = toDouble(settings.value("Filtr_880_B/Pos2", QVariant::fromValue(1.02)).toString());
	filtr880_C_2 = toDouble(settings.value("Filtr_880_C/Pos2", QVariant::fromValue(3.94)).toString());
	filtr880_A_3 = toDouble(settings.value("Filtr_880_A/Pos3", QVariant::fromValue(0.59)).toString());
	filtr880_B_3 = toDouble(settings.value("Filtr_880_B/Pos3", QVariant::fromValue(2)).toString());
	filtr880_C_3 = toDouble(settings.value("Filtr_880_C/Pos3", QVariant::fromValue(8.04)).toString());
	filtr880_A_4 = toDouble(settings.value("Filtr_880_A/Pos4", QVariant::fromValue(0.82)).toString());
	filtr880_B_4 = toDouble(settings.value("Filtr_880_B/Pos4", QVariant::fromValue(3.02)).toString());
	filtr880_C_4 = toDouble(settings.value("Filtr_880_C/Pos4", QVariant::fromValue(11.94)).toString());
	filtr880_A_5 = toDouble(settings.value("Filtr_880_A/Pos5", QVariant::fromValue(6.39)).toString());
	filtr880_B_5 = toDouble(settings.value("Filtr_880_B/Pos5", QVariant::fromValue(6.43)).toString());
	filtr880_C_5 = toDouble(settings.value("Filtr_880_C/Pos5", QVariant::fromValue(3.06)).toString());
	filtr655_A_0 = toDouble(settings.value("Filtr_655_A/Pos0", QVariant::fromValue(0)).toString());
	filtr655_B_0 = toDouble(settings.value("Filtr_655_B/Pos0", QVariant::fromValue(0)).toString());
	filtr655_C_0 = toDouble(settings.value("Filtr_655_C/Pos0", QVariant::fromValue(0)).toString());
	filtr655_A_1 = toDouble(settings.value("Filtr_655_A/Pos1", QVariant::fromValue(0.21)).toString());
	filtr655_B_1 = toDouble(settings.value("Filtr_655_B/Pos1", QVariant::fromValue(0.1)).toString());
	filtr655_C_1 = toDouble(settings.value("Filtr_655_C/Pos1", QVariant::fromValue(0.1)).toString());
	filtr655_A_2 = toDouble(settings.value("Filtr_655_A/Pos2", QVariant::fromValue(0.4)).toString());
	filtr655_B_2 = toDouble(settings.value("Filtr_655_B/Pos2", QVariant::fromValue(0.98)).toString());
	filtr655_C_2 = toDouble(settings.value("Filtr_655_C/Pos2", QVariant::fromValue(4.28)).toString());
	filtr655_A_3 = toDouble(settings.value("Filtr_655_A/Pos3", QVariant::fromValue(0.62)).toString());
	filtr655_B_3 = toDouble(settings.value("Filtr_655_B/Pos3", QVariant::fromValue(1.35)).toString());
	filtr655_C_3 = toDouble(settings.value("Filtr_655_C/Pos3", QVariant::fromValue(8.79)).toString());
	filtr655_A_4 = toDouble(settings.value("Filtr_655_A/Pos4", QVariant::fromValue(0.83)).toString());
	filtr655_B_4 = toDouble(settings.value("Filtr_655_B/Pos4", QVariant::fromValue(3.3)).toString());
	filtr655_C_4 = toDouble(settings.value("Filtr_655_C/Pos4", QVariant::fromValue(7.6)).toString());
	filtr655_A_5 = toDouble(settings.value("Filtr_655_A/Pos5", QVariant::fromValue(6.98)).toString());
	filtr655_B_5 = toDouble(settings.value("Filtr_655_B/Pos5", QVariant::fromValue(1.97)).toString());
	filtr655_C_5 = toDouble(settings.value("Filtr_655_C/Pos5", QVariant::fromValue(1.97)).toString());
	serialDeviceZasilaczVendor = settings.value("Zasilacz/Vendor", QVariant::fromValue(QString("67b"))).toString();
	serialDeviceZasilaczProduct = settings.value("Zasilacz/Product", QVariant::fromValue(QString("23a3"))).toString();
	serialDeviceSterownikVendor = settings.value("Sterownik/Vendor", QVariant::fromValue(QString("2341"))).toString();
	serialDeviceSterownikProduct = settings.value("Sterownik/Product", QVariant::fromValue(QString("42"))).toString();
}

void UstawieniaGen::save()
{
	settings.setValue("Silnik-1/Nazwa", QVariant::fromValue(motorNazwa1));
	settings.setValue("Silnik-2/Nazwa", QVariant::fromValue(motorNazwa2));
	settings.setValue("Silnik-3/Nazwa", QVariant::fromValue(motorNazwa3));
	settings.setValue("Silnik-4/Nazwa", QVariant::fromValue(motorNazwa4));
	settings.setValue("Silnik-5/Nazwa", QVariant::fromValue(motorNazwa5));
	settings.setValue("Silnik-6/Nazwa", QVariant::fromValue(motorNazwa6));
	settings.setValue("Silnik-7/Nazwa", QVariant::fromValue(motorNazwa7));
	settings.setValue("Silnik-8/Nazwa", QVariant::fromValue(motorNazwa8));
	settings.setValue("Silnik-9/Nazwa", QVariant::fromValue(motorNazwa9));
	settings.setValue("Silnik-1/Przelozenie", QVariant::fromValue(motorPrzelozenie1));
	settings.setValue("Silnik-2/Przelozenie", QVariant::fromValue(motorPrzelozenie2));
	settings.setValue("Silnik-3/Przelozenie", QVariant::fromValue(motorPrzelozenie3));
	settings.setValue("Silnik-4/Przelozenie", QVariant::fromValue(motorPrzelozenie4));
	settings.setValue("Silnik-5/Przelozenie", QVariant::fromValue(motorPrzelozenie5));
	settings.setValue("Silnik-6/Przelozenie", QVariant::fromValue(motorPrzelozenie6));
	settings.setValue("Silnik-7/Przelozenie", QVariant::fromValue(motorPrzelozenie7));
	settings.setValue("Silnik-8/Przelozenie", QVariant::fromValue(motorPrzelozenie8));
	settings.setValue("Silnik-9/Przelozenie", QVariant::fromValue(motorPrzelozenie9));
	settings.setValue("Silnik-1/MaksPredkosc", QVariant::fromValue(motorMaksPredkosc1));
	settings.setValue("Silnik-2/MaksPredkosc", QVariant::fromValue(motorMaksPredkosc2));
	settings.setValue("Silnik-3/MaksPredkosc", QVariant::fromValue(motorMaksPredkosc3));
	settings.setValue("Silnik-4/MaksPredkosc", QVariant::fromValue(motorMaksPredkosc4));
	settings.setValue("Silnik-5/MaksPredkosc", QVariant::fromValue(motorMaksPredkosc5));
	settings.setValue("Silnik-6/MaksPredkosc", QVariant::fromValue(motorMaksPredkosc6));
	settings.setValue("Silnik-7/MaksPredkosc", QVariant::fromValue(motorMaksPredkosc7));
	settings.setValue("Silnik-8/MaksPredkosc", QVariant::fromValue(motorMaksPredkosc8));
	settings.setValue("Silnik-9/MaksPredkosc", QVariant::fromValue(motorMaksPredkosc9));
	settings.setValue("Silnik-1/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty1));
	settings.setValue("Silnik-2/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty2));
	settings.setValue("Silnik-3/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty3));
	settings.setValue("Silnik-4/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty4));
	settings.setValue("Silnik-5/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty5));
	settings.setValue("Silnik-6/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty6));
	settings.setValue("Silnik-7/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty7));
	settings.setValue("Silnik-8/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty8));
	settings.setValue("Silnik-9/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty9));
	settings.setValue("Silnik-1/OpoznienieImp", QVariant::fromValue(motorOpoznienieImp1));
	settings.setValue("Silnik-2/OpoznienieImp", QVariant::fromValue(motorOpoznienieImp2));
	settings.setValue("Silnik-3/OpoznienieImp", QVariant::fromValue(motorOpoznienieImp3));
	settings.setValue("Silnik-4/OpoznienieImp", QVariant::fromValue(motorOpoznienieImp4));
	settings.setValue("Silnik-5/OpoznienieImp", QVariant::fromValue(motorOpoznienieImp5));
	settings.setValue("Silnik-6/OpoznienieImp", QVariant::fromValue(motorOpoznienieImp6));
	settings.setValue("Silnik-7/OpoznienieImp", QVariant::fromValue(motorOpoznienieImp7));
	settings.setValue("Silnik-8/OpoznienieImp", QVariant::fromValue(motorOpoznienieImp8));
	settings.setValue("Silnik-9/OpoznienieImp", QVariant::fromValue(motorOpoznienieImp9));
	settings.setValue("Silnik-1/MaksIloscKrokow", QVariant::fromValue(motorMaksIloscKrokow1));
	settings.setValue("Silnik-2/MaksIloscKrokow", QVariant::fromValue(motorMaksIloscKrokow2));
	settings.setValue("Silnik-3/MaksIloscKrokow", QVariant::fromValue(motorMaksIloscKrokow3));
	settings.setValue("Silnik-4/MaksIloscKrokow", QVariant::fromValue(motorMaksIloscKrokow4));
	settings.setValue("Silnik-5/MaksIloscKrokow", QVariant::fromValue(motorMaksIloscKrokow5));
	settings.setValue("Silnik-6/MaksIloscKrokow", QVariant::fromValue(motorMaksIloscKrokow6));
	settings.setValue("Silnik-7/MaksIloscKrokow", QVariant::fromValue(motorMaksIloscKrokow7));
	settings.setValue("Silnik-8/MaksIloscKrokow", QVariant::fromValue(motorMaksIloscKrokow8));
	settings.setValue("Silnik-9/MaksIloscKrokow", QVariant::fromValue(motorMaksIloscKrokow9));
	settings.setValue("Silnik-1/IloscKrokowBaza", QVariant::fromValue(motorIloscKrokowBaza1));
	settings.setValue("Silnik-2/IloscKrokowBaza", QVariant::fromValue(motorIloscKrokowBaza2));
	settings.setValue("Silnik-3/IloscKrokowBaza", QVariant::fromValue(motorIloscKrokowBaza3));
	settings.setValue("Silnik-4/IloscKrokowBaza", QVariant::fromValue(motorIloscKrokowBaza4));
	settings.setValue("Silnik-5/IloscKrokowBaza", QVariant::fromValue(motorIloscKrokowBaza5));
	settings.setValue("Silnik-6/IloscKrokowBaza", QVariant::fromValue(motorIloscKrokowBaza6));
	settings.setValue("Silnik-7/IloscKrokowBaza", QVariant::fromValue(motorIloscKrokowBaza7));
	settings.setValue("Silnik-8/IloscKrokowBaza", QVariant::fromValue(motorIloscKrokowBaza8));
	settings.setValue("Silnik-9/IloscKrokowBaza", QVariant::fromValue(motorIloscKrokowBaza9));
	settings.setValue("Silnik-1/IloscKrokowSrodek", QVariant::fromValue(motorIloscKrokowSrodek1));
	settings.setValue("Silnik-2/IloscKrokowSrodek", QVariant::fromValue(motorIloscKrokowSrodek2));
	settings.setValue("Silnik-3/IloscKrokowSrodek", QVariant::fromValue(motorIloscKrokowSrodek3));
	settings.setValue("Silnik-4/IloscKrokowSrodek", QVariant::fromValue(motorIloscKrokowSrodek4));
	settings.setValue("Silnik-5/IloscKrokowSrodek", QVariant::fromValue(motorIloscKrokowSrodek5));
	settings.setValue("Silnik-6/IloscKrokowSrodek", QVariant::fromValue(motorIloscKrokowSrodek6));
	settings.setValue("Silnik-7/IloscKrokowSrodek", QVariant::fromValue(motorIloscKrokowSrodek7));
	settings.setValue("Silnik-8/IloscKrokowSrodek", QVariant::fromValue(motorIloscKrokowSrodek8));
	settings.setValue("Silnik-9/IloscKrokowSrodek", QVariant::fromValue(motorIloscKrokowSrodek9));
	settings.setValue("Filtr_880_A/Pos0", QVariant::fromValue(filtr880_A_0));
	settings.setValue("Filtr_880_B/Pos0", QVariant::fromValue(filtr880_B_0));
	settings.setValue("Filtr_880_C/Pos0", QVariant::fromValue(filtr880_C_0));
	settings.setValue("Filtr_880_A/Pos1", QVariant::fromValue(filtr880_A_1));
	settings.setValue("Filtr_880_B/Pos1", QVariant::fromValue(filtr880_B_1));
	settings.setValue("Filtr_880_C/Pos1", QVariant::fromValue(filtr880_C_1));
	settings.setValue("Filtr_880_A/Pos2", QVariant::fromValue(filtr880_A_2));
	settings.setValue("Filtr_880_B/Pos2", QVariant::fromValue(filtr880_B_2));
	settings.setValue("Filtr_880_C/Pos2", QVariant::fromValue(filtr880_C_2));
	settings.setValue("Filtr_880_A/Pos3", QVariant::fromValue(filtr880_A_3));
	settings.setValue("Filtr_880_B/Pos3", QVariant::fromValue(filtr880_B_3));
	settings.setValue("Filtr_880_C/Pos3", QVariant::fromValue(filtr880_C_3));
	settings.setValue("Filtr_880_A/Pos4", QVariant::fromValue(filtr880_A_4));
	settings.setValue("Filtr_880_B/Pos4", QVariant::fromValue(filtr880_B_4));
	settings.setValue("Filtr_880_C/Pos4", QVariant::fromValue(filtr880_C_4));
	settings.setValue("Filtr_880_A/Pos5", QVariant::fromValue(filtr880_A_5));
	settings.setValue("Filtr_880_B/Pos5", QVariant::fromValue(filtr880_B_5));
	settings.setValue("Filtr_880_C/Pos5", QVariant::fromValue(filtr880_C_5));
	settings.setValue("Filtr_655_A/Pos0", QVariant::fromValue(filtr655_A_0));
	settings.setValue("Filtr_655_B/Pos0", QVariant::fromValue(filtr655_B_0));
	settings.setValue("Filtr_655_C/Pos0", QVariant::fromValue(filtr655_C_0));
	settings.setValue("Filtr_655_A/Pos1", QVariant::fromValue(filtr655_A_1));
	settings.setValue("Filtr_655_B/Pos1", QVariant::fromValue(filtr655_B_1));
	settings.setValue("Filtr_655_C/Pos1", QVariant::fromValue(filtr655_C_1));
	settings.setValue("Filtr_655_A/Pos2", QVariant::fromValue(filtr655_A_2));
	settings.setValue("Filtr_655_B/Pos2", QVariant::fromValue(filtr655_B_2));
	settings.setValue("Filtr_655_C/Pos2", QVariant::fromValue(filtr655_C_2));
	settings.setValue("Filtr_655_A/Pos3", QVariant::fromValue(filtr655_A_3));
	settings.setValue("Filtr_655_B/Pos3", QVariant::fromValue(filtr655_B_3));
	settings.setValue("Filtr_655_C/Pos3", QVariant::fromValue(filtr655_C_3));
	settings.setValue("Filtr_655_A/Pos4", QVariant::fromValue(filtr655_A_4));
	settings.setValue("Filtr_655_B/Pos4", QVariant::fromValue(filtr655_B_4));
	settings.setValue("Filtr_655_C/Pos4", QVariant::fromValue(filtr655_C_4));
	settings.setValue("Filtr_655_A/Pos5", QVariant::fromValue(filtr655_A_5));
	settings.setValue("Filtr_655_B/Pos5", QVariant::fromValue(filtr655_B_5));
	settings.setValue("Filtr_655_C/Pos5", QVariant::fromValue(filtr655_C_5));
	settings.setValue("Zasilacz/Vendor", QVariant::fromValue(serialDeviceZasilaczVendor));
	settings.setValue("Zasilacz/Product", QVariant::fromValue(serialDeviceZasilaczProduct));
	settings.setValue("Sterownik/Vendor", QVariant::fromValue(serialDeviceSterownikVendor));
	settings.setValue("Sterownik/Product", QVariant::fromValue(serialDeviceSterownikProduct));
}

bool UstawieniaGen::check()
{
	if (!checkMotorNazwa(settings.value("Silnik-1/Nazwa").toString())) return false;
	if (!checkMotorNazwa(settings.value("Silnik-2/Nazwa").toString())) return false;
	if (!checkMotorNazwa(settings.value("Silnik-3/Nazwa").toString())) return false;
	if (!checkMotorNazwa(settings.value("Silnik-4/Nazwa").toString())) return false;
	if (!checkMotorNazwa(settings.value("Silnik-5/Nazwa").toString())) return false;
	if (!checkMotorNazwa(settings.value("Silnik-6/Nazwa").toString())) return false;
	if (!checkMotorNazwa(settings.value("Silnik-7/Nazwa").toString())) return false;
	if (!checkMotorNazwa(settings.value("Silnik-8/Nazwa").toString())) return false;
	if (!checkMotorNazwa(settings.value("Silnik-9/Nazwa").toString())) return false;
	if (!checkMotorPrzelozenie(settings.value("Silnik-1/Przelozenie").toString())) return false;
	if (!checkMotorPrzelozenie(settings.value("Silnik-2/Przelozenie").toString())) return false;
	if (!checkMotorPrzelozenie(settings.value("Silnik-3/Przelozenie").toString())) return false;
	if (!checkMotorPrzelozenie(settings.value("Silnik-4/Przelozenie").toString())) return false;
	if (!checkMotorPrzelozenie(settings.value("Silnik-5/Przelozenie").toString())) return false;
	if (!checkMotorPrzelozenie(settings.value("Silnik-6/Przelozenie").toString())) return false;
	if (!checkMotorPrzelozenie(settings.value("Silnik-7/Przelozenie").toString())) return false;
	if (!checkMotorPrzelozenie(settings.value("Silnik-8/Przelozenie").toString())) return false;
	if (!checkMotorPrzelozenie(settings.value("Silnik-9/Przelozenie").toString())) return false;
	if (!checkMotorMaksPredkosc(settings.value("Silnik-1/MaksPredkosc").toString())) return false;
	if (!checkMotorMaksPredkosc(settings.value("Silnik-2/MaksPredkosc").toString())) return false;
	if (!checkMotorMaksPredkosc(settings.value("Silnik-3/MaksPredkosc").toString())) return false;
	if (!checkMotorMaksPredkosc(settings.value("Silnik-4/MaksPredkosc").toString())) return false;
	if (!checkMotorMaksPredkosc(settings.value("Silnik-5/MaksPredkosc").toString())) return false;
	if (!checkMotorMaksPredkosc(settings.value("Silnik-6/MaksPredkosc").toString())) return false;
	if (!checkMotorMaksPredkosc(settings.value("Silnik-7/MaksPredkosc").toString())) return false;
	if (!checkMotorMaksPredkosc(settings.value("Silnik-8/MaksPredkosc").toString())) return false;
	if (!checkMotorMaksPredkosc(settings.value("Silnik-9/MaksPredkosc").toString())) return false;
	if (!checkMotorOdwrocObroty(settings.value("Silnik-1/OdwrocObroty").toString())) return false;
	if (!checkMotorOdwrocObroty(settings.value("Silnik-2/OdwrocObroty").toString())) return false;
	if (!checkMotorOdwrocObroty(settings.value("Silnik-3/OdwrocObroty").toString())) return false;
	if (!checkMotorOdwrocObroty(settings.value("Silnik-4/OdwrocObroty").toString())) return false;
	if (!checkMotorOdwrocObroty(settings.value("Silnik-5/OdwrocObroty").toString())) return false;
	if (!checkMotorOdwrocObroty(settings.value("Silnik-6/OdwrocObroty").toString())) return false;
	if (!checkMotorOdwrocObroty(settings.value("Silnik-7/OdwrocObroty").toString())) return false;
	if (!checkMotorOdwrocObroty(settings.value("Silnik-8/OdwrocObroty").toString())) return false;
	if (!checkMotorOdwrocObroty(settings.value("Silnik-9/OdwrocObroty").toString())) return false;
	if (!checkMotorOpoznienieImp(settings.value("Silnik-1/OpoznienieImp").toString())) return false;
	if (!checkMotorOpoznienieImp(settings.value("Silnik-2/OpoznienieImp").toString())) return false;
	if (!checkMotorOpoznienieImp(settings.value("Silnik-3/OpoznienieImp").toString())) return false;
	if (!checkMotorOpoznienieImp(settings.value("Silnik-4/OpoznienieImp").toString())) return false;
	if (!checkMotorOpoznienieImp(settings.value("Silnik-5/OpoznienieImp").toString())) return false;
	if (!checkMotorOpoznienieImp(settings.value("Silnik-6/OpoznienieImp").toString())) return false;
	if (!checkMotorOpoznienieImp(settings.value("Silnik-7/OpoznienieImp").toString())) return false;
	if (!checkMotorOpoznienieImp(settings.value("Silnik-8/OpoznienieImp").toString())) return false;
	if (!checkMotorOpoznienieImp(settings.value("Silnik-9/OpoznienieImp").toString())) return false;
	if (!checkMotorMaksIloscKrokow(settings.value("Silnik-1/MaksIloscKrokow").toString())) return false;
	if (!checkMotorMaksIloscKrokow(settings.value("Silnik-2/MaksIloscKrokow").toString())) return false;
	if (!checkMotorMaksIloscKrokow(settings.value("Silnik-3/MaksIloscKrokow").toString())) return false;
	if (!checkMotorMaksIloscKrokow(settings.value("Silnik-4/MaksIloscKrokow").toString())) return false;
	if (!checkMotorMaksIloscKrokow(settings.value("Silnik-5/MaksIloscKrokow").toString())) return false;
	if (!checkMotorMaksIloscKrokow(settings.value("Silnik-6/MaksIloscKrokow").toString())) return false;
	if (!checkMotorMaksIloscKrokow(settings.value("Silnik-7/MaksIloscKrokow").toString())) return false;
	if (!checkMotorMaksIloscKrokow(settings.value("Silnik-8/MaksIloscKrokow").toString())) return false;
	if (!checkMotorMaksIloscKrokow(settings.value("Silnik-9/MaksIloscKrokow").toString())) return false;
	if (!checkMotorIloscKrokowBaza(settings.value("Silnik-1/IloscKrokowBaza").toString())) return false;
	if (!checkMotorIloscKrokowBaza(settings.value("Silnik-2/IloscKrokowBaza").toString())) return false;
	if (!checkMotorIloscKrokowBaza(settings.value("Silnik-3/IloscKrokowBaza").toString())) return false;
	if (!checkMotorIloscKrokowBaza(settings.value("Silnik-4/IloscKrokowBaza").toString())) return false;
	if (!checkMotorIloscKrokowBaza(settings.value("Silnik-5/IloscKrokowBaza").toString())) return false;
	if (!checkMotorIloscKrokowBaza(settings.value("Silnik-6/IloscKrokowBaza").toString())) return false;
	if (!checkMotorIloscKrokowBaza(settings.value("Silnik-7/IloscKrokowBaza").toString())) return false;
	if (!checkMotorIloscKrokowBaza(settings.value("Silnik-8/IloscKrokowBaza").toString())) return false;
	if (!checkMotorIloscKrokowBaza(settings.value("Silnik-9/IloscKrokowBaza").toString())) return false;
	if (!checkMotorIloscKrokowSrodek(settings.value("Silnik-1/IloscKrokowSrodek").toString())) return false;
	if (!checkMotorIloscKrokowSrodek(settings.value("Silnik-2/IloscKrokowSrodek").toString())) return false;
	if (!checkMotorIloscKrokowSrodek(settings.value("Silnik-3/IloscKrokowSrodek").toString())) return false;
	if (!checkMotorIloscKrokowSrodek(settings.value("Silnik-4/IloscKrokowSrodek").toString())) return false;
	if (!checkMotorIloscKrokowSrodek(settings.value("Silnik-5/IloscKrokowSrodek").toString())) return false;
	if (!checkMotorIloscKrokowSrodek(settings.value("Silnik-6/IloscKrokowSrodek").toString())) return false;
	if (!checkMotorIloscKrokowSrodek(settings.value("Silnik-7/IloscKrokowSrodek").toString())) return false;
	if (!checkMotorIloscKrokowSrodek(settings.value("Silnik-8/IloscKrokowSrodek").toString())) return false;
	if (!checkMotorIloscKrokowSrodek(settings.value("Silnik-9/IloscKrokowSrodek").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_A/Pos0").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_B/Pos0").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_C/Pos0").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_A/Pos1").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_B/Pos1").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_C/Pos1").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_A/Pos2").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_B/Pos2").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_C/Pos2").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_A/Pos3").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_B/Pos3").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_C/Pos3").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_A/Pos4").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_B/Pos4").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_C/Pos4").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_A/Pos5").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_B/Pos5").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_880_C/Pos5").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_A/Pos0").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_B/Pos0").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_C/Pos0").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_A/Pos1").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_B/Pos1").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_C/Pos1").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_A/Pos2").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_B/Pos2").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_C/Pos2").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_A/Pos3").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_B/Pos3").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_C/Pos3").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_A/Pos4").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_B/Pos4").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_C/Pos4").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_A/Pos5").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_B/Pos5").toString())) return false;
	if (!checkFilterValue(settings.value("Filtr_655_C/Pos5").toString())) return false;
	if (!checkSerialDeviceIdentString(settings.value("Zasilacz/Vendor").toString())) return false;
	if (!checkSerialDeviceIdentString(settings.value("Zasilacz/Product").toString())) return false;
	if (!checkSerialDeviceIdentString(settings.value("Sterownik/Vendor").toString())) return false;
	if (!checkSerialDeviceIdentString(settings.value("Sterownik/Product").toString())) return false;
	return true;
}

QString UstawieniaGen::getMotorNazwa1() const
{
	return motorNazwa1;
}

void UstawieniaGen::setMotorNazwa1(const QString & value)
{
	motorNazwa1 = value;
	settings.setValue("Silnik-1/Nazwa", QVariant::fromValue(value));
}

QString UstawieniaGen::getMotorNazwa2() const
{
	return motorNazwa2;
}

void UstawieniaGen::setMotorNazwa2(const QString & value)
{
	motorNazwa2 = value;
	settings.setValue("Silnik-2/Nazwa", QVariant::fromValue(value));
}

QString UstawieniaGen::getMotorNazwa3() const
{
	return motorNazwa3;
}

void UstawieniaGen::setMotorNazwa3(const QString & value)
{
	motorNazwa3 = value;
	settings.setValue("Silnik-3/Nazwa", QVariant::fromValue(value));
}

QString UstawieniaGen::getMotorNazwa4() const
{
	return motorNazwa4;
}

void UstawieniaGen::setMotorNazwa4(const QString & value)
{
	motorNazwa4 = value;
	settings.setValue("Silnik-4/Nazwa", QVariant::fromValue(value));
}

QString UstawieniaGen::getMotorNazwa5() const
{
	return motorNazwa5;
}

void UstawieniaGen::setMotorNazwa5(const QString & value)
{
	motorNazwa5 = value;
	settings.setValue("Silnik-5/Nazwa", QVariant::fromValue(value));
}

QString UstawieniaGen::getMotorNazwa6() const
{
	return motorNazwa6;
}

void UstawieniaGen::setMotorNazwa6(const QString & value)
{
	motorNazwa6 = value;
	settings.setValue("Silnik-6/Nazwa", QVariant::fromValue(value));
}

QString UstawieniaGen::getMotorNazwa7() const
{
	return motorNazwa7;
}

void UstawieniaGen::setMotorNazwa7(const QString & value)
{
	motorNazwa7 = value;
	settings.setValue("Silnik-7/Nazwa", QVariant::fromValue(value));
}

QString UstawieniaGen::getMotorNazwa8() const
{
	return motorNazwa8;
}

void UstawieniaGen::setMotorNazwa8(const QString & value)
{
	motorNazwa8 = value;
	settings.setValue("Silnik-8/Nazwa", QVariant::fromValue(value));
}

QString UstawieniaGen::getMotorNazwa9() const
{
	return motorNazwa9;
}

void UstawieniaGen::setMotorNazwa9(const QString & value)
{
	motorNazwa9 = value;
	settings.setValue("Silnik-9/Nazwa", QVariant::fromValue(value));
}

QString UstawieniaGen::getMotorNazwa(const short & nrMotor) const
{
	switch(nrMotor) {
	case 1 : return getMotorNazwa1();
	case 2 : return getMotorNazwa2();
	case 3 : return getMotorNazwa3();
	case 4 : return getMotorNazwa4();
	case 5 : return getMotorNazwa5();
	case 6 : return getMotorNazwa6();
	case 7 : return getMotorNazwa7();
	case 8 : return getMotorNazwa8();
	case 9 : return getMotorNazwa9();
	default: return toQString(QString());
	}

}

void UstawieniaGen::setMotorNazwa(const short & nrMotor, const QString & nazwa)
{
	switch(nrMotor) {
	case 1 : setMotorNazwa1(nazwa); break;
	case 2 : setMotorNazwa2(nazwa); break;
	case 3 : setMotorNazwa3(nazwa); break;
	case 4 : setMotorNazwa4(nazwa); break;
	case 5 : setMotorNazwa5(nazwa); break;
	case 6 : setMotorNazwa6(nazwa); break;
	case 7 : setMotorNazwa7(nazwa); break;
	case 8 : setMotorNazwa8(nazwa); break;
	case 9 : setMotorNazwa9(nazwa); break;
	default: break;
	}

}

bool UstawieniaGen::checkMotorNazwa(const QString & val)
{

    if (val.isEmpty()) return false;
    return true;

}

double UstawieniaGen::getMotorPrzelozenie1() const
{
	return motorPrzelozenie1;
}

void UstawieniaGen::setMotorPrzelozenie1(const double & value)
{
	motorPrzelozenie1 = value;
	settings.setValue("Silnik-1/Przelozenie", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenie2() const
{
	return motorPrzelozenie2;
}

void UstawieniaGen::setMotorPrzelozenie2(const double & value)
{
	motorPrzelozenie2 = value;
	settings.setValue("Silnik-2/Przelozenie", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenie3() const
{
	return motorPrzelozenie3;
}

void UstawieniaGen::setMotorPrzelozenie3(const double & value)
{
	motorPrzelozenie3 = value;
	settings.setValue("Silnik-3/Przelozenie", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenie4() const
{
	return motorPrzelozenie4;
}

void UstawieniaGen::setMotorPrzelozenie4(const double & value)
{
	motorPrzelozenie4 = value;
	settings.setValue("Silnik-4/Przelozenie", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenie5() const
{
	return motorPrzelozenie5;
}

void UstawieniaGen::setMotorPrzelozenie5(const double & value)
{
	motorPrzelozenie5 = value;
	settings.setValue("Silnik-5/Przelozenie", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenie6() const
{
	return motorPrzelozenie6;
}

void UstawieniaGen::setMotorPrzelozenie6(const double & value)
{
	motorPrzelozenie6 = value;
	settings.setValue("Silnik-6/Przelozenie", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenie7() const
{
	return motorPrzelozenie7;
}

void UstawieniaGen::setMotorPrzelozenie7(const double & value)
{
	motorPrzelozenie7 = value;
	settings.setValue("Silnik-7/Przelozenie", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenie8() const
{
	return motorPrzelozenie8;
}

void UstawieniaGen::setMotorPrzelozenie8(const double & value)
{
	motorPrzelozenie8 = value;
	settings.setValue("Silnik-8/Przelozenie", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenie9() const
{
	return motorPrzelozenie9;
}

void UstawieniaGen::setMotorPrzelozenie9(const double & value)
{
	motorPrzelozenie9 = value;
	settings.setValue("Silnik-9/Przelozenie", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenie(const short & nrMotor) const
{
	switch(nrMotor) {
	case 1 : return getMotorPrzelozenie1();
	case 2 : return getMotorPrzelozenie2();
	case 3 : return getMotorPrzelozenie3();
	case 4 : return getMotorPrzelozenie4();
	case 5 : return getMotorPrzelozenie5();
	case 6 : return getMotorPrzelozenie6();
	case 7 : return getMotorPrzelozenie7();
	case 8 : return getMotorPrzelozenie8();
	case 9 : return getMotorPrzelozenie9();
	default: return toDouble(QString());
	}

}

void UstawieniaGen::setMotorPrzelozenie(const short & nrMotor, const double & przelozenie)
{
	switch(nrMotor) {
	case 1 : setMotorPrzelozenie1(przelozenie); break;
	case 2 : setMotorPrzelozenie2(przelozenie); break;
	case 3 : setMotorPrzelozenie3(przelozenie); break;
	case 4 : setMotorPrzelozenie4(przelozenie); break;
	case 5 : setMotorPrzelozenie5(przelozenie); break;
	case 6 : setMotorPrzelozenie6(przelozenie); break;
	case 7 : setMotorPrzelozenie7(przelozenie); break;
	case 8 : setMotorPrzelozenie8(przelozenie); break;
	case 9 : setMotorPrzelozenie9(przelozenie); break;
	default: break;
	}

}

bool UstawieniaGen::checkMotorPrzelozenie(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    double v = val.toDouble(&ok);
    if (!ok || v < 0.0)
        return false;
    return true;

}

double UstawieniaGen::getMotorMaksPredkosc1() const
{
	return motorMaksPredkosc1;
}

void UstawieniaGen::setMotorMaksPredkosc1(const double & value)
{
	motorMaksPredkosc1 = value;
	settings.setValue("Silnik-1/MaksPredkosc", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorMaksPredkosc2() const
{
	return motorMaksPredkosc2;
}

void UstawieniaGen::setMotorMaksPredkosc2(const double & value)
{
	motorMaksPredkosc2 = value;
	settings.setValue("Silnik-2/MaksPredkosc", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorMaksPredkosc3() const
{
	return motorMaksPredkosc3;
}

void UstawieniaGen::setMotorMaksPredkosc3(const double & value)
{
	motorMaksPredkosc3 = value;
	settings.setValue("Silnik-3/MaksPredkosc", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorMaksPredkosc4() const
{
	return motorMaksPredkosc4;
}

void UstawieniaGen::setMotorMaksPredkosc4(const double & value)
{
	motorMaksPredkosc4 = value;
	settings.setValue("Silnik-4/MaksPredkosc", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorMaksPredkosc5() const
{
	return motorMaksPredkosc5;
}

void UstawieniaGen::setMotorMaksPredkosc5(const double & value)
{
	motorMaksPredkosc5 = value;
	settings.setValue("Silnik-5/MaksPredkosc", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorMaksPredkosc6() const
{
	return motorMaksPredkosc6;
}

void UstawieniaGen::setMotorMaksPredkosc6(const double & value)
{
	motorMaksPredkosc6 = value;
	settings.setValue("Silnik-6/MaksPredkosc", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorMaksPredkosc7() const
{
	return motorMaksPredkosc7;
}

void UstawieniaGen::setMotorMaksPredkosc7(const double & value)
{
	motorMaksPredkosc7 = value;
	settings.setValue("Silnik-7/MaksPredkosc", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorMaksPredkosc8() const
{
	return motorMaksPredkosc8;
}

void UstawieniaGen::setMotorMaksPredkosc8(const double & value)
{
	motorMaksPredkosc8 = value;
	settings.setValue("Silnik-8/MaksPredkosc", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorMaksPredkosc9() const
{
	return motorMaksPredkosc9;
}

void UstawieniaGen::setMotorMaksPredkosc9(const double & value)
{
	motorMaksPredkosc9 = value;
	settings.setValue("Silnik-9/MaksPredkosc", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorMaksPredkosc(const short & nrMotor) const
{
	switch(nrMotor) {
	case 1 : return getMotorMaksPredkosc1();
	case 2 : return getMotorMaksPredkosc2();
	case 3 : return getMotorMaksPredkosc3();
	case 4 : return getMotorMaksPredkosc4();
	case 5 : return getMotorMaksPredkosc5();
	case 6 : return getMotorMaksPredkosc6();
	case 7 : return getMotorMaksPredkosc7();
	case 8 : return getMotorMaksPredkosc8();
	case 9 : return getMotorMaksPredkosc9();
	default: return toDouble(QString());
	}

}

void UstawieniaGen::setMotorMaksPredkosc(const short & nrMotor, const double & maksPredkosc)
{
	switch(nrMotor) {
	case 1 : setMotorMaksPredkosc1(maksPredkosc); break;
	case 2 : setMotorMaksPredkosc2(maksPredkosc); break;
	case 3 : setMotorMaksPredkosc3(maksPredkosc); break;
	case 4 : setMotorMaksPredkosc4(maksPredkosc); break;
	case 5 : setMotorMaksPredkosc5(maksPredkosc); break;
	case 6 : setMotorMaksPredkosc6(maksPredkosc); break;
	case 7 : setMotorMaksPredkosc7(maksPredkosc); break;
	case 8 : setMotorMaksPredkosc8(maksPredkosc); break;
	case 9 : setMotorMaksPredkosc9(maksPredkosc); break;
	default: break;
	}

}

bool UstawieniaGen::checkMotorMaksPredkosc(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    double v = val.toDouble(&ok);
    if (!ok || v < 0.0)
        return false;
    return true;

}

bool UstawieniaGen::getMotorOdwrocObroty1() const
{
	return motorOdwrocObroty1;
}

void UstawieniaGen::setMotorOdwrocObroty1(const bool & value)
{
	motorOdwrocObroty1 = value;
	settings.setValue("Silnik-1/OdwrocObroty", QVariant::fromValue(value));
}

bool UstawieniaGen::getMotorOdwrocObroty2() const
{
	return motorOdwrocObroty2;
}

void UstawieniaGen::setMotorOdwrocObroty2(const bool & value)
{
	motorOdwrocObroty2 = value;
	settings.setValue("Silnik-2/OdwrocObroty", QVariant::fromValue(value));
}

bool UstawieniaGen::getMotorOdwrocObroty3() const
{
	return motorOdwrocObroty3;
}

void UstawieniaGen::setMotorOdwrocObroty3(const bool & value)
{
	motorOdwrocObroty3 = value;
	settings.setValue("Silnik-3/OdwrocObroty", QVariant::fromValue(value));
}

bool UstawieniaGen::getMotorOdwrocObroty4() const
{
	return motorOdwrocObroty4;
}

void UstawieniaGen::setMotorOdwrocObroty4(const bool & value)
{
	motorOdwrocObroty4 = value;
	settings.setValue("Silnik-4/OdwrocObroty", QVariant::fromValue(value));
}

bool UstawieniaGen::getMotorOdwrocObroty5() const
{
	return motorOdwrocObroty5;
}

void UstawieniaGen::setMotorOdwrocObroty5(const bool & value)
{
	motorOdwrocObroty5 = value;
	settings.setValue("Silnik-5/OdwrocObroty", QVariant::fromValue(value));
}

bool UstawieniaGen::getMotorOdwrocObroty6() const
{
	return motorOdwrocObroty6;
}

void UstawieniaGen::setMotorOdwrocObroty6(const bool & value)
{
	motorOdwrocObroty6 = value;
	settings.setValue("Silnik-6/OdwrocObroty", QVariant::fromValue(value));
}

bool UstawieniaGen::getMotorOdwrocObroty7() const
{
	return motorOdwrocObroty7;
}

void UstawieniaGen::setMotorOdwrocObroty7(const bool & value)
{
	motorOdwrocObroty7 = value;
	settings.setValue("Silnik-7/OdwrocObroty", QVariant::fromValue(value));
}

bool UstawieniaGen::getMotorOdwrocObroty8() const
{
	return motorOdwrocObroty8;
}

void UstawieniaGen::setMotorOdwrocObroty8(const bool & value)
{
	motorOdwrocObroty8 = value;
	settings.setValue("Silnik-8/OdwrocObroty", QVariant::fromValue(value));
}

bool UstawieniaGen::getMotorOdwrocObroty9() const
{
	return motorOdwrocObroty9;
}

void UstawieniaGen::setMotorOdwrocObroty9(const bool & value)
{
	motorOdwrocObroty9 = value;
	settings.setValue("Silnik-9/OdwrocObroty", QVariant::fromValue(value));
}

bool UstawieniaGen::getMotorOdwrocObroty(const short & nrMotor) const
{
	switch(nrMotor) {
	case 1 : return getMotorOdwrocObroty1();
	case 2 : return getMotorOdwrocObroty2();
	case 3 : return getMotorOdwrocObroty3();
	case 4 : return getMotorOdwrocObroty4();
	case 5 : return getMotorOdwrocObroty5();
	case 6 : return getMotorOdwrocObroty6();
	case 7 : return getMotorOdwrocObroty7();
	case 8 : return getMotorOdwrocObroty8();
	case 9 : return getMotorOdwrocObroty9();
	default: return toBool(QString());
	}

}

void UstawieniaGen::setMotorOdwrocObroty(const short & nrMotor, const bool & odwrocObroty)
{
	switch(nrMotor) {
	case 1 : setMotorOdwrocObroty1(odwrocObroty); break;
	case 2 : setMotorOdwrocObroty2(odwrocObroty); break;
	case 3 : setMotorOdwrocObroty3(odwrocObroty); break;
	case 4 : setMotorOdwrocObroty4(odwrocObroty); break;
	case 5 : setMotorOdwrocObroty5(odwrocObroty); break;
	case 6 : setMotorOdwrocObroty6(odwrocObroty); break;
	case 7 : setMotorOdwrocObroty7(odwrocObroty); break;
	case 8 : setMotorOdwrocObroty8(odwrocObroty); break;
	case 9 : setMotorOdwrocObroty9(odwrocObroty); break;
	default: break;
	}

}

bool UstawieniaGen::checkMotorOdwrocObroty(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool v = QVariant::fromValue(val).toBool();
    (void)v;
    return true;

}

int UstawieniaGen::getMotorOpoznienieImp1() const
{
	return motorOpoznienieImp1;
}

void UstawieniaGen::setMotorOpoznienieImp1(const int & value)
{
	motorOpoznienieImp1 = value;
	settings.setValue("Silnik-1/OpoznienieImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorOpoznienieImp2() const
{
	return motorOpoznienieImp2;
}

void UstawieniaGen::setMotorOpoznienieImp2(const int & value)
{
	motorOpoznienieImp2 = value;
	settings.setValue("Silnik-2/OpoznienieImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorOpoznienieImp3() const
{
	return motorOpoznienieImp3;
}

void UstawieniaGen::setMotorOpoznienieImp3(const int & value)
{
	motorOpoznienieImp3 = value;
	settings.setValue("Silnik-3/OpoznienieImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorOpoznienieImp4() const
{
	return motorOpoznienieImp4;
}

void UstawieniaGen::setMotorOpoznienieImp4(const int & value)
{
	motorOpoznienieImp4 = value;
	settings.setValue("Silnik-4/OpoznienieImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorOpoznienieImp5() const
{
	return motorOpoznienieImp5;
}

void UstawieniaGen::setMotorOpoznienieImp5(const int & value)
{
	motorOpoznienieImp5 = value;
	settings.setValue("Silnik-5/OpoznienieImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorOpoznienieImp6() const
{
	return motorOpoznienieImp6;
}

void UstawieniaGen::setMotorOpoznienieImp6(const int & value)
{
	motorOpoznienieImp6 = value;
	settings.setValue("Silnik-6/OpoznienieImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorOpoznienieImp7() const
{
	return motorOpoznienieImp7;
}

void UstawieniaGen::setMotorOpoznienieImp7(const int & value)
{
	motorOpoznienieImp7 = value;
	settings.setValue("Silnik-7/OpoznienieImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorOpoznienieImp8() const
{
	return motorOpoznienieImp8;
}

void UstawieniaGen::setMotorOpoznienieImp8(const int & value)
{
	motorOpoznienieImp8 = value;
	settings.setValue("Silnik-8/OpoznienieImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorOpoznienieImp9() const
{
	return motorOpoznienieImp9;
}

void UstawieniaGen::setMotorOpoznienieImp9(const int & value)
{
	motorOpoznienieImp9 = value;
	settings.setValue("Silnik-9/OpoznienieImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorOpoznienieImp(const short & nrMotor) const
{
	switch(nrMotor) {
	case 1 : return getMotorOpoznienieImp1();
	case 2 : return getMotorOpoznienieImp2();
	case 3 : return getMotorOpoznienieImp3();
	case 4 : return getMotorOpoznienieImp4();
	case 5 : return getMotorOpoznienieImp5();
	case 6 : return getMotorOpoznienieImp6();
	case 7 : return getMotorOpoznienieImp7();
	case 8 : return getMotorOpoznienieImp8();
	case 9 : return getMotorOpoznienieImp9();
	default: return toUInt(QString());
	}

}

void UstawieniaGen::setMotorOpoznienieImp(const short & nrMotor, const int & opoznienieImp)
{
	switch(nrMotor) {
	case 1 : setMotorOpoznienieImp1(opoznienieImp); break;
	case 2 : setMotorOpoznienieImp2(opoznienieImp); break;
	case 3 : setMotorOpoznienieImp3(opoznienieImp); break;
	case 4 : setMotorOpoznienieImp4(opoznienieImp); break;
	case 5 : setMotorOpoznienieImp5(opoznienieImp); break;
	case 6 : setMotorOpoznienieImp6(opoznienieImp); break;
	case 7 : setMotorOpoznienieImp7(opoznienieImp); break;
	case 8 : setMotorOpoznienieImp8(opoznienieImp); break;
	case 9 : setMotorOpoznienieImp9(opoznienieImp); break;
	default: break;
	}

}

bool UstawieniaGen::checkMotorOpoznienieImp(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    int v = val.toInt(&ok);
    if (!ok || v <= 0)
        return false;
    return ok;

}

int UstawieniaGen::getMotorMaksIloscKrokow1() const
{
	return motorMaksIloscKrokow1;
}

void UstawieniaGen::setMotorMaksIloscKrokow1(const int & value)
{
	motorMaksIloscKrokow1 = value;
	settings.setValue("Silnik-1/MaksIloscKrokow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscKrokow2() const
{
	return motorMaksIloscKrokow2;
}

void UstawieniaGen::setMotorMaksIloscKrokow2(const int & value)
{
	motorMaksIloscKrokow2 = value;
	settings.setValue("Silnik-2/MaksIloscKrokow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscKrokow3() const
{
	return motorMaksIloscKrokow3;
}

void UstawieniaGen::setMotorMaksIloscKrokow3(const int & value)
{
	motorMaksIloscKrokow3 = value;
	settings.setValue("Silnik-3/MaksIloscKrokow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscKrokow4() const
{
	return motorMaksIloscKrokow4;
}

void UstawieniaGen::setMotorMaksIloscKrokow4(const int & value)
{
	motorMaksIloscKrokow4 = value;
	settings.setValue("Silnik-4/MaksIloscKrokow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscKrokow5() const
{
	return motorMaksIloscKrokow5;
}

void UstawieniaGen::setMotorMaksIloscKrokow5(const int & value)
{
	motorMaksIloscKrokow5 = value;
	settings.setValue("Silnik-5/MaksIloscKrokow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscKrokow6() const
{
	return motorMaksIloscKrokow6;
}

void UstawieniaGen::setMotorMaksIloscKrokow6(const int & value)
{
	motorMaksIloscKrokow6 = value;
	settings.setValue("Silnik-6/MaksIloscKrokow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscKrokow7() const
{
	return motorMaksIloscKrokow7;
}

void UstawieniaGen::setMotorMaksIloscKrokow7(const int & value)
{
	motorMaksIloscKrokow7 = value;
	settings.setValue("Silnik-7/MaksIloscKrokow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscKrokow8() const
{
	return motorMaksIloscKrokow8;
}

void UstawieniaGen::setMotorMaksIloscKrokow8(const int & value)
{
	motorMaksIloscKrokow8 = value;
	settings.setValue("Silnik-8/MaksIloscKrokow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscKrokow9() const
{
	return motorMaksIloscKrokow9;
}

void UstawieniaGen::setMotorMaksIloscKrokow9(const int & value)
{
	motorMaksIloscKrokow9 = value;
	settings.setValue("Silnik-9/MaksIloscKrokow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscKrokow(const short & nrMotor) const
{
	switch(nrMotor) {
	case 1 : return getMotorMaksIloscKrokow1();
	case 2 : return getMotorMaksIloscKrokow2();
	case 3 : return getMotorMaksIloscKrokow3();
	case 4 : return getMotorMaksIloscKrokow4();
	case 5 : return getMotorMaksIloscKrokow5();
	case 6 : return getMotorMaksIloscKrokow6();
	case 7 : return getMotorMaksIloscKrokow7();
	case 8 : return getMotorMaksIloscKrokow8();
	case 9 : return getMotorMaksIloscKrokow9();
	default: return toUInt(QString());
	}

}

void UstawieniaGen::setMotorMaksIloscKrokow(const short & nrMotor, const int & maksIloscKrokow)
{
	switch(nrMotor) {
	case 1 : setMotorMaksIloscKrokow1(maksIloscKrokow); break;
	case 2 : setMotorMaksIloscKrokow2(maksIloscKrokow); break;
	case 3 : setMotorMaksIloscKrokow3(maksIloscKrokow); break;
	case 4 : setMotorMaksIloscKrokow4(maksIloscKrokow); break;
	case 5 : setMotorMaksIloscKrokow5(maksIloscKrokow); break;
	case 6 : setMotorMaksIloscKrokow6(maksIloscKrokow); break;
	case 7 : setMotorMaksIloscKrokow7(maksIloscKrokow); break;
	case 8 : setMotorMaksIloscKrokow8(maksIloscKrokow); break;
	case 9 : setMotorMaksIloscKrokow9(maksIloscKrokow); break;
	default: break;
	}

}

bool UstawieniaGen::checkMotorMaksIloscKrokow(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    int v = val.toInt(&ok);
    if (!ok || v <= 0)
        return false;
    return ok;

}

int UstawieniaGen::getMotorIloscKrokowBaza1() const
{
	return motorIloscKrokowBaza1;
}

void UstawieniaGen::setMotorIloscKrokowBaza1(const int & value)
{
	motorIloscKrokowBaza1 = value;
	settings.setValue("Silnik-1/IloscKrokowBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowBaza2() const
{
	return motorIloscKrokowBaza2;
}

void UstawieniaGen::setMotorIloscKrokowBaza2(const int & value)
{
	motorIloscKrokowBaza2 = value;
	settings.setValue("Silnik-2/IloscKrokowBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowBaza3() const
{
	return motorIloscKrokowBaza3;
}

void UstawieniaGen::setMotorIloscKrokowBaza3(const int & value)
{
	motorIloscKrokowBaza3 = value;
	settings.setValue("Silnik-3/IloscKrokowBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowBaza4() const
{
	return motorIloscKrokowBaza4;
}

void UstawieniaGen::setMotorIloscKrokowBaza4(const int & value)
{
	motorIloscKrokowBaza4 = value;
	settings.setValue("Silnik-4/IloscKrokowBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowBaza5() const
{
	return motorIloscKrokowBaza5;
}

void UstawieniaGen::setMotorIloscKrokowBaza5(const int & value)
{
	motorIloscKrokowBaza5 = value;
	settings.setValue("Silnik-5/IloscKrokowBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowBaza6() const
{
	return motorIloscKrokowBaza6;
}

void UstawieniaGen::setMotorIloscKrokowBaza6(const int & value)
{
	motorIloscKrokowBaza6 = value;
	settings.setValue("Silnik-6/IloscKrokowBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowBaza7() const
{
	return motorIloscKrokowBaza7;
}

void UstawieniaGen::setMotorIloscKrokowBaza7(const int & value)
{
	motorIloscKrokowBaza7 = value;
	settings.setValue("Silnik-7/IloscKrokowBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowBaza8() const
{
	return motorIloscKrokowBaza8;
}

void UstawieniaGen::setMotorIloscKrokowBaza8(const int & value)
{
	motorIloscKrokowBaza8 = value;
	settings.setValue("Silnik-8/IloscKrokowBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowBaza9() const
{
	return motorIloscKrokowBaza9;
}

void UstawieniaGen::setMotorIloscKrokowBaza9(const int & value)
{
	motorIloscKrokowBaza9 = value;
	settings.setValue("Silnik-9/IloscKrokowBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowBaza(const short & nrMotor) const
{
	switch(nrMotor) {
	case 1 : return getMotorIloscKrokowBaza1();
	case 2 : return getMotorIloscKrokowBaza2();
	case 3 : return getMotorIloscKrokowBaza3();
	case 4 : return getMotorIloscKrokowBaza4();
	case 5 : return getMotorIloscKrokowBaza5();
	case 6 : return getMotorIloscKrokowBaza6();
	case 7 : return getMotorIloscKrokowBaza7();
	case 8 : return getMotorIloscKrokowBaza8();
	case 9 : return getMotorIloscKrokowBaza9();
	default: return toUInt(QString());
	}

}

void UstawieniaGen::setMotorIloscKrokowBaza(const short & nrMotor, const int & iloscKrokowBaza)
{
	switch(nrMotor) {
	case 1 : setMotorIloscKrokowBaza1(iloscKrokowBaza); break;
	case 2 : setMotorIloscKrokowBaza2(iloscKrokowBaza); break;
	case 3 : setMotorIloscKrokowBaza3(iloscKrokowBaza); break;
	case 4 : setMotorIloscKrokowBaza4(iloscKrokowBaza); break;
	case 5 : setMotorIloscKrokowBaza5(iloscKrokowBaza); break;
	case 6 : setMotorIloscKrokowBaza6(iloscKrokowBaza); break;
	case 7 : setMotorIloscKrokowBaza7(iloscKrokowBaza); break;
	case 8 : setMotorIloscKrokowBaza8(iloscKrokowBaza); break;
	case 9 : setMotorIloscKrokowBaza9(iloscKrokowBaza); break;
	default: break;
	}

}

bool UstawieniaGen::checkMotorIloscKrokowBaza(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    int v = val.toInt(&ok);
    if (!ok || v <= 0)
        return false;
    return ok;

}

int UstawieniaGen::getMotorIloscKrokowSrodek1() const
{
	return motorIloscKrokowSrodek1;
}

void UstawieniaGen::setMotorIloscKrokowSrodek1(const int & value)
{
	motorIloscKrokowSrodek1 = value;
	settings.setValue("Silnik-1/IloscKrokowSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowSrodek2() const
{
	return motorIloscKrokowSrodek2;
}

void UstawieniaGen::setMotorIloscKrokowSrodek2(const int & value)
{
	motorIloscKrokowSrodek2 = value;
	settings.setValue("Silnik-2/IloscKrokowSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowSrodek3() const
{
	return motorIloscKrokowSrodek3;
}

void UstawieniaGen::setMotorIloscKrokowSrodek3(const int & value)
{
	motorIloscKrokowSrodek3 = value;
	settings.setValue("Silnik-3/IloscKrokowSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowSrodek4() const
{
	return motorIloscKrokowSrodek4;
}

void UstawieniaGen::setMotorIloscKrokowSrodek4(const int & value)
{
	motorIloscKrokowSrodek4 = value;
	settings.setValue("Silnik-4/IloscKrokowSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowSrodek5() const
{
	return motorIloscKrokowSrodek5;
}

void UstawieniaGen::setMotorIloscKrokowSrodek5(const int & value)
{
	motorIloscKrokowSrodek5 = value;
	settings.setValue("Silnik-5/IloscKrokowSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowSrodek6() const
{
	return motorIloscKrokowSrodek6;
}

void UstawieniaGen::setMotorIloscKrokowSrodek6(const int & value)
{
	motorIloscKrokowSrodek6 = value;
	settings.setValue("Silnik-6/IloscKrokowSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowSrodek7() const
{
	return motorIloscKrokowSrodek7;
}

void UstawieniaGen::setMotorIloscKrokowSrodek7(const int & value)
{
	motorIloscKrokowSrodek7 = value;
	settings.setValue("Silnik-7/IloscKrokowSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowSrodek8() const
{
	return motorIloscKrokowSrodek8;
}

void UstawieniaGen::setMotorIloscKrokowSrodek8(const int & value)
{
	motorIloscKrokowSrodek8 = value;
	settings.setValue("Silnik-8/IloscKrokowSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowSrodek9() const
{
	return motorIloscKrokowSrodek9;
}

void UstawieniaGen::setMotorIloscKrokowSrodek9(const int & value)
{
	motorIloscKrokowSrodek9 = value;
	settings.setValue("Silnik-9/IloscKrokowSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscKrokowSrodek(const short & nrMotor) const
{
	switch(nrMotor) {
	case 1 : return getMotorIloscKrokowSrodek1();
	case 2 : return getMotorIloscKrokowSrodek2();
	case 3 : return getMotorIloscKrokowSrodek3();
	case 4 : return getMotorIloscKrokowSrodek4();
	case 5 : return getMotorIloscKrokowSrodek5();
	case 6 : return getMotorIloscKrokowSrodek6();
	case 7 : return getMotorIloscKrokowSrodek7();
	case 8 : return getMotorIloscKrokowSrodek8();
	case 9 : return getMotorIloscKrokowSrodek9();
	default: return toUInt(QString());
	}

}

void UstawieniaGen::setMotorIloscKrokowSrodek(const short & nrMotor, const int & iloscKrokowSrodek)
{
	switch(nrMotor) {
	case 1 : setMotorIloscKrokowSrodek1(iloscKrokowSrodek); break;
	case 2 : setMotorIloscKrokowSrodek2(iloscKrokowSrodek); break;
	case 3 : setMotorIloscKrokowSrodek3(iloscKrokowSrodek); break;
	case 4 : setMotorIloscKrokowSrodek4(iloscKrokowSrodek); break;
	case 5 : setMotorIloscKrokowSrodek5(iloscKrokowSrodek); break;
	case 6 : setMotorIloscKrokowSrodek6(iloscKrokowSrodek); break;
	case 7 : setMotorIloscKrokowSrodek7(iloscKrokowSrodek); break;
	case 8 : setMotorIloscKrokowSrodek8(iloscKrokowSrodek); break;
	case 9 : setMotorIloscKrokowSrodek9(iloscKrokowSrodek); break;
	default: break;
	}

}

bool UstawieniaGen::checkMotorIloscKrokowSrodek(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    int v = val.toInt(&ok);
    if (!ok || v <= 0)
        return false;
    return ok;

}

double UstawieniaGen::getFiltr880_A_0() const
{
	return filtr880_A_0;
}

void UstawieniaGen::setFiltr880_A_0(const double & value)
{
	filtr880_A_0 = value;
	settings.setValue("Filtr_880_A/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_B_0() const
{
	return filtr880_B_0;
}

void UstawieniaGen::setFiltr880_B_0(const double & value)
{
	filtr880_B_0 = value;
	settings.setValue("Filtr_880_B/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_C_0() const
{
	return filtr880_C_0;
}

void UstawieniaGen::setFiltr880_C_0(const double & value)
{
	filtr880_C_0 = value;
	settings.setValue("Filtr_880_C/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_A_1() const
{
	return filtr880_A_1;
}

void UstawieniaGen::setFiltr880_A_1(const double & value)
{
	filtr880_A_1 = value;
	settings.setValue("Filtr_880_A/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_B_1() const
{
	return filtr880_B_1;
}

void UstawieniaGen::setFiltr880_B_1(const double & value)
{
	filtr880_B_1 = value;
	settings.setValue("Filtr_880_B/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_C_1() const
{
	return filtr880_C_1;
}

void UstawieniaGen::setFiltr880_C_1(const double & value)
{
	filtr880_C_1 = value;
	settings.setValue("Filtr_880_C/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_A_2() const
{
	return filtr880_A_2;
}

void UstawieniaGen::setFiltr880_A_2(const double & value)
{
	filtr880_A_2 = value;
	settings.setValue("Filtr_880_A/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_B_2() const
{
	return filtr880_B_2;
}

void UstawieniaGen::setFiltr880_B_2(const double & value)
{
	filtr880_B_2 = value;
	settings.setValue("Filtr_880_B/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_C_2() const
{
	return filtr880_C_2;
}

void UstawieniaGen::setFiltr880_C_2(const double & value)
{
	filtr880_C_2 = value;
	settings.setValue("Filtr_880_C/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_A_3() const
{
	return filtr880_A_3;
}

void UstawieniaGen::setFiltr880_A_3(const double & value)
{
	filtr880_A_3 = value;
	settings.setValue("Filtr_880_A/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_B_3() const
{
	return filtr880_B_3;
}

void UstawieniaGen::setFiltr880_B_3(const double & value)
{
	filtr880_B_3 = value;
	settings.setValue("Filtr_880_B/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_C_3() const
{
	return filtr880_C_3;
}

void UstawieniaGen::setFiltr880_C_3(const double & value)
{
	filtr880_C_3 = value;
	settings.setValue("Filtr_880_C/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_A_4() const
{
	return filtr880_A_4;
}

void UstawieniaGen::setFiltr880_A_4(const double & value)
{
	filtr880_A_4 = value;
	settings.setValue("Filtr_880_A/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_B_4() const
{
	return filtr880_B_4;
}

void UstawieniaGen::setFiltr880_B_4(const double & value)
{
	filtr880_B_4 = value;
	settings.setValue("Filtr_880_B/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_C_4() const
{
	return filtr880_C_4;
}

void UstawieniaGen::setFiltr880_C_4(const double & value)
{
	filtr880_C_4 = value;
	settings.setValue("Filtr_880_C/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_A_5() const
{
	return filtr880_A_5;
}

void UstawieniaGen::setFiltr880_A_5(const double & value)
{
	filtr880_A_5 = value;
	settings.setValue("Filtr_880_A/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_B_5() const
{
	return filtr880_B_5;
}

void UstawieniaGen::setFiltr880_B_5(const double & value)
{
	filtr880_B_5 = value;
	settings.setValue("Filtr_880_B/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr880_C_5() const
{
	return filtr880_C_5;
}

void UstawieniaGen::setFiltr880_C_5(const double & value)
{
	filtr880_C_5 = value;
	settings.setValue("Filtr_880_C/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_A_0() const
{
	return filtr655_A_0;
}

void UstawieniaGen::setFiltr655_A_0(const double & value)
{
	filtr655_A_0 = value;
	settings.setValue("Filtr_655_A/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_B_0() const
{
	return filtr655_B_0;
}

void UstawieniaGen::setFiltr655_B_0(const double & value)
{
	filtr655_B_0 = value;
	settings.setValue("Filtr_655_B/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_C_0() const
{
	return filtr655_C_0;
}

void UstawieniaGen::setFiltr655_C_0(const double & value)
{
	filtr655_C_0 = value;
	settings.setValue("Filtr_655_C/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_A_1() const
{
	return filtr655_A_1;
}

void UstawieniaGen::setFiltr655_A_1(const double & value)
{
	filtr655_A_1 = value;
	settings.setValue("Filtr_655_A/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_B_1() const
{
	return filtr655_B_1;
}

void UstawieniaGen::setFiltr655_B_1(const double & value)
{
	filtr655_B_1 = value;
	settings.setValue("Filtr_655_B/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_C_1() const
{
	return filtr655_C_1;
}

void UstawieniaGen::setFiltr655_C_1(const double & value)
{
	filtr655_C_1 = value;
	settings.setValue("Filtr_655_C/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_A_2() const
{
	return filtr655_A_2;
}

void UstawieniaGen::setFiltr655_A_2(const double & value)
{
	filtr655_A_2 = value;
	settings.setValue("Filtr_655_A/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_B_2() const
{
	return filtr655_B_2;
}

void UstawieniaGen::setFiltr655_B_2(const double & value)
{
	filtr655_B_2 = value;
	settings.setValue("Filtr_655_B/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_C_2() const
{
	return filtr655_C_2;
}

void UstawieniaGen::setFiltr655_C_2(const double & value)
{
	filtr655_C_2 = value;
	settings.setValue("Filtr_655_C/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_A_3() const
{
	return filtr655_A_3;
}

void UstawieniaGen::setFiltr655_A_3(const double & value)
{
	filtr655_A_3 = value;
	settings.setValue("Filtr_655_A/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_B_3() const
{
	return filtr655_B_3;
}

void UstawieniaGen::setFiltr655_B_3(const double & value)
{
	filtr655_B_3 = value;
	settings.setValue("Filtr_655_B/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_C_3() const
{
	return filtr655_C_3;
}

void UstawieniaGen::setFiltr655_C_3(const double & value)
{
	filtr655_C_3 = value;
	settings.setValue("Filtr_655_C/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_A_4() const
{
	return filtr655_A_4;
}

void UstawieniaGen::setFiltr655_A_4(const double & value)
{
	filtr655_A_4 = value;
	settings.setValue("Filtr_655_A/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_B_4() const
{
	return filtr655_B_4;
}

void UstawieniaGen::setFiltr655_B_4(const double & value)
{
	filtr655_B_4 = value;
	settings.setValue("Filtr_655_B/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_C_4() const
{
	return filtr655_C_4;
}

void UstawieniaGen::setFiltr655_C_4(const double & value)
{
	filtr655_C_4 = value;
	settings.setValue("Filtr_655_C/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_A_5() const
{
	return filtr655_A_5;
}

void UstawieniaGen::setFiltr655_A_5(const double & value)
{
	filtr655_A_5 = value;
	settings.setValue("Filtr_655_A/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_B_5() const
{
	return filtr655_B_5;
}

void UstawieniaGen::setFiltr655_B_5(const double & value)
{
	filtr655_B_5 = value;
	settings.setValue("Filtr_655_B/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr655_C_5() const
{
	return filtr655_C_5;
}

void UstawieniaGen::setFiltr655_C_5(const double & value)
{
	filtr655_C_5 = value;
	settings.setValue("Filtr_655_C/Pos5", QVariant::fromValue(value));
}

bool UstawieniaGen::checkFilterValue(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    double v = val.toDouble(&ok);
    if (!ok || v < 0.0)
        return false;
    return true;

}

double UstawieniaGen::getFiltr(const int & fala, const char & nrTarczy, const short & nrPos) const
{
	if (false) ;
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 0) return getFiltr880_A_0();
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 1) return getFiltr880_A_1();
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 2) return getFiltr880_A_2();
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 3) return getFiltr880_A_3();
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 4) return getFiltr880_A_4();
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 5) return getFiltr880_A_5();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 0) return getFiltr880_B_0();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 1) return getFiltr880_B_1();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 2) return getFiltr880_B_2();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 3) return getFiltr880_B_3();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 4) return getFiltr880_B_4();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 5) return getFiltr880_B_5();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 0) return getFiltr880_C_0();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 1) return getFiltr880_C_1();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 2) return getFiltr880_C_2();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 3) return getFiltr880_C_3();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 4) return getFiltr880_C_4();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 5) return getFiltr880_C_5();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 0) return getFiltr655_A_0();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 1) return getFiltr655_A_1();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 2) return getFiltr655_A_2();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 3) return getFiltr655_A_3();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 4) return getFiltr655_A_4();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 5) return getFiltr655_A_5();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 0) return getFiltr655_B_0();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 1) return getFiltr655_B_1();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 2) return getFiltr655_B_2();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 3) return getFiltr655_B_3();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 4) return getFiltr655_B_4();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 5) return getFiltr655_B_5();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 0) return getFiltr655_C_0();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 1) return getFiltr655_C_1();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 2) return getFiltr655_C_2();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 3) return getFiltr655_C_3();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 4) return getFiltr655_C_4();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 5) return getFiltr655_C_5();
	else Q_ASSERT(true);
	 return 0.0;
}

void UstawieniaGen::setFiltr(const int & fala, const char & nrTarczy, const short & nrPos, const double & val)
{
	if (false) ;
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 0) return setFiltr880_A_0(val);
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 1) return setFiltr880_A_1(val);
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 2) return setFiltr880_A_2(val);
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 3) return setFiltr880_A_3(val);
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 4) return setFiltr880_A_4(val);
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 5) return setFiltr880_A_5(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 0) return setFiltr880_B_0(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 1) return setFiltr880_B_1(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 2) return setFiltr880_B_2(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 3) return setFiltr880_B_3(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 4) return setFiltr880_B_4(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 5) return setFiltr880_B_5(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 0) return setFiltr880_C_0(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 1) return setFiltr880_C_1(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 2) return setFiltr880_C_2(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 3) return setFiltr880_C_3(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 4) return setFiltr880_C_4(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 5) return setFiltr880_C_5(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 0) return setFiltr655_A_0(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 1) return setFiltr655_A_1(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 2) return setFiltr655_A_2(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 3) return setFiltr655_A_3(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 4) return setFiltr655_A_4(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 5) return setFiltr655_A_5(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 0) return setFiltr655_B_0(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 1) return setFiltr655_B_1(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 2) return setFiltr655_B_2(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 3) return setFiltr655_B_3(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 4) return setFiltr655_B_4(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 5) return setFiltr655_B_5(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 0) return setFiltr655_C_0(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 1) return setFiltr655_C_1(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 2) return setFiltr655_C_2(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 3) return setFiltr655_C_3(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 4) return setFiltr655_C_4(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 5) return setFiltr655_C_5(val);
	else Q_ASSERT(true);
}

QString UstawieniaGen::getSerialDeviceZasilaczVendor() const
{
	return serialDeviceZasilaczVendor;
}

void UstawieniaGen::setSerialDeviceZasilaczVendor(const QString & value)
{
	serialDeviceZasilaczVendor = value;
	settings.setValue("Zasilacz/Vendor", QVariant::fromValue(value));
}

QString UstawieniaGen::getSerialDeviceZasilaczProduct() const
{
	return serialDeviceZasilaczProduct;
}

void UstawieniaGen::setSerialDeviceZasilaczProduct(const QString & value)
{
	serialDeviceZasilaczProduct = value;
	settings.setValue("Zasilacz/Product", QVariant::fromValue(value));
}

QString UstawieniaGen::getSerialDeviceSterownikVendor() const
{
	return serialDeviceSterownikVendor;
}

void UstawieniaGen::setSerialDeviceSterownikVendor(const QString & value)
{
	serialDeviceSterownikVendor = value;
	settings.setValue("Sterownik/Vendor", QVariant::fromValue(value));
}

QString UstawieniaGen::getSerialDeviceSterownikProduct() const
{
	return serialDeviceSterownikProduct;
}

void UstawieniaGen::setSerialDeviceSterownikProduct(const QString & value)
{
	serialDeviceSterownikProduct = value;
	settings.setValue("Sterownik/Product", QVariant::fromValue(value));
}

bool UstawieniaGen::checkSerialDeviceIdentString(const QString & val) const
{
	
    if (val.isEmpty()) return false;
    
    bool v = QVariant::fromValue(val).toBool();
    (void)v;
    return true;

}