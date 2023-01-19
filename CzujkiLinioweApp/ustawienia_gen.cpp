
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
	motorPrzelozenieImpJedn1 = toDouble(settings.value("Silnik-1/PrzelozenieImpJedn", QVariant::fromValue(0.0375)).toString());
	motorPrzelozenieImpJedn2 = toDouble(settings.value("Silnik-2/PrzelozenieImpJedn", QVariant::fromValue(0.0375)).toString());
	motorPrzelozenieImpJedn3 = toDouble(settings.value("Silnik-3/PrzelozenieImpJedn", QVariant::fromValue(0.011613)).toString());
	motorPrzelozenieImpJedn4 = toDouble(settings.value("Silnik-4/PrzelozenieImpJedn", QVariant::fromValue(0.011613)).toString());
	motorPrzelozenieImpJedn5 = toDouble(settings.value("Silnik-5/PrzelozenieImpJedn", QVariant::fromValue(0.011613)).toString());
	motorPrzelozenieImpJedn6 = toDouble(settings.value("Silnik-6/PrzelozenieImpJedn", QVariant::fromValue(0.013)).toString());
	motorPrzelozenieImpJedn7 = toDouble(settings.value("Silnik-7/PrzelozenieImpJedn", QVariant::fromValue(0.000266667)).toString());
	motorPrzelozenieImpJedn8 = toDouble(settings.value("Silnik-8/PrzelozenieImpJedn", QVariant::fromValue(0.0375)).toString());
	motorPrzelozenieImpJedn9 = toDouble(settings.value("Silnik-9/PrzelozenieImpJedn", QVariant::fromValue(0.0375)).toString());
	motorMinOdstepImp1 = toUInt(settings.value("Silnik-1/MinOdstepImp", QVariant::fromValue(29)).toString());
	motorMinOdstepImp2 = toUInt(settings.value("Silnik-2/MinOdstepImp", QVariant::fromValue(29)).toString());
	motorMinOdstepImp3 = toUInt(settings.value("Silnik-3/MinOdstepImp", QVariant::fromValue(29)).toString());
	motorMinOdstepImp4 = toUInt(settings.value("Silnik-4/MinOdstepImp", QVariant::fromValue(29)).toString());
	motorMinOdstepImp5 = toUInt(settings.value("Silnik-5/MinOdstepImp", QVariant::fromValue(29)).toString());
	motorMinOdstepImp6 = toUInt(settings.value("Silnik-6/MinOdstepImp", QVariant::fromValue(29)).toString());
	motorMinOdstepImp7 = toUInt(settings.value("Silnik-7/MinOdstepImp", QVariant::fromValue(29)).toString());
	motorMinOdstepImp8 = toUInt(settings.value("Silnik-8/MinOdstepImp", QVariant::fromValue(29)).toString());
	motorMinOdstepImp9 = toUInt(settings.value("Silnik-9/MinOdstepImp", QVariant::fromValue(29)).toString());
	motorOdwrocObroty1 = toBool(settings.value("Silnik-1/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty2 = toBool(settings.value("Silnik-2/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty3 = toBool(settings.value("Silnik-3/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty4 = toBool(settings.value("Silnik-4/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty5 = toBool(settings.value("Silnik-5/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty6 = toBool(settings.value("Silnik-6/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty7 = toBool(settings.value("Silnik-7/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty8 = toBool(settings.value("Silnik-8/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorOdwrocObroty9 = toBool(settings.value("Silnik-9/OdwrocObroty", QVariant::fromValue(false)).toString());
	motorCzasMiedzyImpZerow1 = toUInt(settings.value("Silnik-1/CzasMiedzyImpZerow", QVariant::fromValue(300000)).toString());
	motorCzasMiedzyImpZerow2 = toUInt(settings.value("Silnik-2/CzasMiedzyImpZerow", QVariant::fromValue(1000)).toString());
	motorCzasMiedzyImpZerow3 = toUInt(settings.value("Silnik-3/CzasMiedzyImpZerow", QVariant::fromValue(60)).toString());
	motorCzasMiedzyImpZerow4 = toUInt(settings.value("Silnik-4/CzasMiedzyImpZerow", QVariant::fromValue(60)).toString());
	motorCzasMiedzyImpZerow5 = toUInt(settings.value("Silnik-5/CzasMiedzyImpZerow", QVariant::fromValue(60)).toString());
	motorCzasMiedzyImpZerow6 = toUInt(settings.value("Silnik-6/CzasMiedzyImpZerow", QVariant::fromValue(50)).toString());
	motorCzasMiedzyImpZerow7 = toUInt(settings.value("Silnik-7/CzasMiedzyImpZerow", QVariant::fromValue(250)).toString());
	motorCzasMiedzyImpZerow8 = toUInt(settings.value("Silnik-8/CzasMiedzyImpZerow", QVariant::fromValue(1000)).toString());
	motorCzasMiedzyImpZerow9 = toUInt(settings.value("Silnik-9/CzasMiedzyImpZerow", QVariant::fromValue(1000)).toString());
	motorCzasMiedzyImpNormal1 = toUInt(settings.value("Silnik-1/CzasMiedzyImpNormal", QVariant::fromValue(10000)).toString());
	motorCzasMiedzyImpNormal2 = toUInt(settings.value("Silnik-2/CzasMiedzyImpNormal", QVariant::fromValue(10000)).toString());
	motorCzasMiedzyImpNormal3 = toUInt(settings.value("Silnik-3/CzasMiedzyImpNormal", QVariant::fromValue(50)).toString());
	motorCzasMiedzyImpNormal4 = toUInt(settings.value("Silnik-4/CzasMiedzyImpNormal", QVariant::fromValue(50)).toString());
	motorCzasMiedzyImpNormal5 = toUInt(settings.value("Silnik-5/CzasMiedzyImpNormal", QVariant::fromValue(50)).toString());
	motorCzasMiedzyImpNormal6 = toUInt(settings.value("Silnik-6/CzasMiedzyImpNormal", QVariant::fromValue(500)).toString());
	motorCzasMiedzyImpNormal7 = toUInt(settings.value("Silnik-7/CzasMiedzyImpNormal", QVariant::fromValue(1000)).toString());
	motorCzasMiedzyImpNormal8 = toUInt(settings.value("Silnik-8/CzasMiedzyImpNormal", QVariant::fromValue(10000)).toString());
	motorCzasMiedzyImpNormal9 = toUInt(settings.value("Silnik-9/CzasMiedzyImpNormal", QVariant::fromValue(10000)).toString());
	motorMaksIloscImp1 = toUInt(settings.value("Silnik-1/MaksIloscImp", QVariant::fromValue(1000)).toString());
	motorMaksIloscImp2 = toUInt(settings.value("Silnik-2/MaksIloscImp", QVariant::fromValue(1002)).toString());
	motorMaksIloscImp3 = toUInt(settings.value("Silnik-3/MaksIloscImp", QVariant::fromValue(31003)).toString());
	motorMaksIloscImp4 = toUInt(settings.value("Silnik-4/MaksIloscImp", QVariant::fromValue(31004)).toString());
	motorMaksIloscImp5 = toUInt(settings.value("Silnik-5/MaksIloscImp", QVariant::fromValue(31005)).toString());
	motorMaksIloscImp6 = toUInt(settings.value("Silnik-6/MaksIloscImp", QVariant::fromValue(1006)).toString());
	motorMaksIloscImp7 = toUInt(settings.value("Silnik-7/MaksIloscImp", QVariant::fromValue(1007)).toString());
	motorMaksIloscImp8 = toUInt(settings.value("Silnik-8/MaksIloscImp", QVariant::fromValue(1008)).toString());
	motorMaksIloscImp9 = toUInt(settings.value("Silnik-9/MaksIloscImp", QVariant::fromValue(1009)).toString());
	motorIloscImpBaza1 = toUInt(settings.value("Silnik-1/IloscImpBaza", QVariant::fromValue(0)).toString());
	motorIloscImpBaza2 = toUInt(settings.value("Silnik-2/IloscImpBaza", QVariant::fromValue(0)).toString());
	motorIloscImpBaza3 = toUInt(settings.value("Silnik-3/IloscImpBaza", QVariant::fromValue(260)).toString());
	motorIloscImpBaza4 = toUInt(settings.value("Silnik-4/IloscImpBaza", QVariant::fromValue(260)).toString());
	motorIloscImpBaza5 = toUInt(settings.value("Silnik-5/IloscImpBaza", QVariant::fromValue(260)).toString());
	motorIloscImpBaza6 = toUInt(settings.value("Silnik-6/IloscImpBaza", QVariant::fromValue(0)).toString());
	motorIloscImpBaza7 = toUInt(settings.value("Silnik-7/IloscImpBaza", QVariant::fromValue(0)).toString());
	motorIloscImpBaza8 = toUInt(settings.value("Silnik-8/IloscImpBaza", QVariant::fromValue(0)).toString());
	motorIloscImpBaza9 = toUInt(settings.value("Silnik-9/IloscImpBaza", QVariant::fromValue(0)).toString());
	motorIloscImpSrodek1 = toUInt(settings.value("Silnik-1/IloscImpSrodek", QVariant::fromValue(440)).toString());
	motorIloscImpSrodek2 = toUInt(settings.value("Silnik-2/IloscImpSrodek", QVariant::fromValue(350)).toString());
	motorIloscImpSrodek3 = toUInt(settings.value("Silnik-3/IloscImpSrodek", QVariant::fromValue(0)).toString());
	motorIloscImpSrodek4 = toUInt(settings.value("Silnik-4/IloscImpSrodek", QVariant::fromValue(0)).toString());
	motorIloscImpSrodek5 = toUInt(settings.value("Silnik-5/IloscImpSrodek", QVariant::fromValue(0)).toString());
	motorIloscImpSrodek6 = toUInt(settings.value("Silnik-6/IloscImpSrodek", QVariant::fromValue(0)).toString());
	motorIloscImpSrodek7 = toUInt(settings.value("Silnik-7/IloscImpSrodek", QVariant::fromValue(0)).toString());
	motorIloscImpSrodek8 = toUInt(settings.value("Silnik-8/IloscImpSrodek", QVariant::fromValue(350)).toString());
	motorIloscImpSrodek9 = toUInt(settings.value("Silnik-9/IloscImpSrodek", QVariant::fromValue(350)).toString());
	filtr_db_880_A_0 = toDouble(settings.value("Filtr_db_880_A/Pos0", QVariant::fromValue(0)).toString());
	filtr_db_880_B_0 = toDouble(settings.value("Filtr_db_880_B/Pos0", QVariant::fromValue(0)).toString());
	filtr_db_880_C_0 = toDouble(settings.value("Filtr_db_880_C/Pos0", QVariant::fromValue(0)).toString());
	filtr_db_880_A_1 = toDouble(settings.value("Filtr_db_880_A/Pos1", QVariant::fromValue(0.1)).toString());
	filtr_db_880_B_1 = toDouble(settings.value("Filtr_db_880_B/Pos1", QVariant::fromValue(0.2)).toString());
	filtr_db_880_C_1 = toDouble(settings.value("Filtr_db_880_C/Pos1", QVariant::fromValue(1.0)).toString());
	filtr_db_880_A_2 = toDouble(settings.value("Filtr_db_880_A/Pos2", QVariant::fromValue(0.2)).toString());
	filtr_db_880_B_2 = toDouble(settings.value("Filtr_db_880_B/Pos2", QVariant::fromValue(0.4)).toString());
	filtr_db_880_C_2 = toDouble(settings.value("Filtr_db_880_C/Pos2", QVariant::fromValue(2.0)).toString());
	filtr_db_880_A_3 = toDouble(settings.value("Filtr_db_880_A/Pos3", QVariant::fromValue(0.3)).toString());
	filtr_db_880_B_3 = toDouble(settings.value("Filtr_db_880_B/Pos3", QVariant::fromValue(0.5)).toString());
	filtr_db_880_C_3 = toDouble(settings.value("Filtr_db_880_C/Pos3", QVariant::fromValue(3.0)).toString());
	filtr_db_880_A_4 = toDouble(settings.value("Filtr_db_880_A/Pos4", QVariant::fromValue(0.4)).toString());
	filtr_db_880_B_4 = toDouble(settings.value("Filtr_db_880_B/Pos4", QVariant::fromValue(6.0)).toString());
	filtr_db_880_C_4 = toDouble(settings.value("Filtr_db_880_C/Pos4", QVariant::fromValue(4.0)).toString());
	filtr_db_880_A_5 = toDouble(settings.value("Filtr_db_880_A/Pos5", QVariant::fromValue(13.0)).toString());
	filtr_db_880_B_5 = toDouble(settings.value("Filtr_db_880_B/Pos5", QVariant::fromValue(7.0)).toString());
	filtr_db_880_C_5 = toDouble(settings.value("Filtr_db_880_C/Pos5", QVariant::fromValue(5.0)).toString());
	filtr_db_655_A_0 = toDouble(settings.value("Filtr_db_655_A/Pos0", QVariant::fromValue(0)).toString());
	filtr_db_655_B_0 = toDouble(settings.value("Filtr_db_655_B/Pos0", QVariant::fromValue(0)).toString());
	filtr_db_655_C_0 = toDouble(settings.value("Filtr_db_655_C/Pos0", QVariant::fromValue(0)).toString());
	filtr_db_655_A_1 = toDouble(settings.value("Filtr_db_655_A/Pos1", QVariant::fromValue(0.1)).toString());
	filtr_db_655_B_1 = toDouble(settings.value("Filtr_db_655_B/Pos1", QVariant::fromValue(0.2)).toString());
	filtr_db_655_C_1 = toDouble(settings.value("Filtr_db_655_C/Pos1", QVariant::fromValue(1.0)).toString());
	filtr_db_655_A_2 = toDouble(settings.value("Filtr_db_655_A/Pos2", QVariant::fromValue(0.2)).toString());
	filtr_db_655_B_2 = toDouble(settings.value("Filtr_db_655_B/Pos2", QVariant::fromValue(0.4)).toString());
	filtr_db_655_C_2 = toDouble(settings.value("Filtr_db_655_C/Pos2", QVariant::fromValue(2.0)).toString());
	filtr_db_655_A_3 = toDouble(settings.value("Filtr_db_655_A/Pos3", QVariant::fromValue(0.3)).toString());
	filtr_db_655_B_3 = toDouble(settings.value("Filtr_db_655_B/Pos3", QVariant::fromValue(0.5)).toString());
	filtr_db_655_C_3 = toDouble(settings.value("Filtr_db_655_C/Pos3", QVariant::fromValue(3.0)).toString());
	filtr_db_655_A_4 = toDouble(settings.value("Filtr_db_655_A/Pos4", QVariant::fromValue(0.4)).toString());
	filtr_db_655_B_4 = toDouble(settings.value("Filtr_db_655_B/Pos4", QVariant::fromValue(6.0)).toString());
	filtr_db_655_C_4 = toDouble(settings.value("Filtr_db_655_C/Pos4", QVariant::fromValue(4.0)).toString());
	filtr_db_655_A_5 = toDouble(settings.value("Filtr_db_655_A/Pos5", QVariant::fromValue(13.0)).toString());
	filtr_db_655_B_5 = toDouble(settings.value("Filtr_db_655_B/Pos5", QVariant::fromValue(7.0)).toString());
	filtr_db_655_C_5 = toDouble(settings.value("Filtr_db_655_C/Pos5", QVariant::fromValue(5.0)).toString());
	filtr_prc_880_A_0 = toDouble(settings.value("Filtr_percent_880_A/Pos0", QVariant::fromValue(0)).toString());
	filtr_prc_880_B_0 = toDouble(settings.value("Filtr_percent_880_B/Pos0", QVariant::fromValue(0)).toString());
	filtr_prc_880_C_0 = toDouble(settings.value("Filtr_percent_880_C/Pos0", QVariant::fromValue(0)).toString());
	filtr_prc_880_A_1 = toDouble(settings.value("Filtr_percent_880_A/Pos1", QVariant::fromValue(0)).toString());
	filtr_prc_880_B_1 = toDouble(settings.value("Filtr_percent_880_B/Pos1", QVariant::fromValue(0)).toString());
	filtr_prc_880_C_1 = toDouble(settings.value("Filtr_percent_880_C/Pos1", QVariant::fromValue(0)).toString());
	filtr_prc_880_A_2 = toDouble(settings.value("Filtr_percent_880_A/Pos2", QVariant::fromValue(0)).toString());
	filtr_prc_880_B_2 = toDouble(settings.value("Filtr_percent_880_B/Pos2", QVariant::fromValue(0)).toString());
	filtr_prc_880_C_2 = toDouble(settings.value("Filtr_percent_880_C/Pos2", QVariant::fromValue(0)).toString());
	filtr_prc_880_A_3 = toDouble(settings.value("Filtr_percent_880_A/Pos3", QVariant::fromValue(0)).toString());
	filtr_prc_880_B_3 = toDouble(settings.value("Filtr_percent_880_B/Pos3", QVariant::fromValue(0)).toString());
	filtr_prc_880_C_3 = toDouble(settings.value("Filtr_percent_880_C/Pos3", QVariant::fromValue(0)).toString());
	filtr_prc_880_A_4 = toDouble(settings.value("Filtr_percent_880_A/Pos4", QVariant::fromValue(0)).toString());
	filtr_prc_880_B_4 = toDouble(settings.value("Filtr_percent_880_B/Pos4", QVariant::fromValue(0)).toString());
	filtr_prc_880_C_4 = toDouble(settings.value("Filtr_percent_880_C/Pos4", QVariant::fromValue(0)).toString());
	filtr_prc_880_A_5 = toDouble(settings.value("Filtr_percent_880_A/Pos5", QVariant::fromValue(0)).toString());
	filtr_prc_880_B_5 = toDouble(settings.value("Filtr_percent_880_B/Pos5", QVariant::fromValue(0)).toString());
	filtr_prc_880_C_5 = toDouble(settings.value("Filtr_percent_880_C/Pos5", QVariant::fromValue(0)).toString());
	filtr_prc_655_A_0 = toDouble(settings.value("Filtr_percent_655_A/Pos0", QVariant::fromValue(0)).toString());
	filtr_prc_655_B_0 = toDouble(settings.value("Filtr_percent_655_B/Pos0", QVariant::fromValue(0)).toString());
	filtr_prc_655_C_0 = toDouble(settings.value("Filtr_percent_655_C/Pos0", QVariant::fromValue(0)).toString());
	filtr_prc_655_A_1 = toDouble(settings.value("Filtr_percent_655_A/Pos1", QVariant::fromValue(0)).toString());
	filtr_prc_655_B_1 = toDouble(settings.value("Filtr_percent_655_B/Pos1", QVariant::fromValue(0)).toString());
	filtr_prc_655_C_1 = toDouble(settings.value("Filtr_percent_655_C/Pos1", QVariant::fromValue(0)).toString());
	filtr_prc_655_A_2 = toDouble(settings.value("Filtr_percent_655_A/Pos2", QVariant::fromValue(0)).toString());
	filtr_prc_655_B_2 = toDouble(settings.value("Filtr_percent_655_B/Pos2", QVariant::fromValue(0)).toString());
	filtr_prc_655_C_2 = toDouble(settings.value("Filtr_percent_655_C/Pos2", QVariant::fromValue(0)).toString());
	filtr_prc_655_A_3 = toDouble(settings.value("Filtr_percent_655_A/Pos3", QVariant::fromValue(0)).toString());
	filtr_prc_655_B_3 = toDouble(settings.value("Filtr_percent_655_B/Pos3", QVariant::fromValue(0)).toString());
	filtr_prc_655_C_3 = toDouble(settings.value("Filtr_percent_655_C/Pos3", QVariant::fromValue(0)).toString());
	filtr_prc_655_A_4 = toDouble(settings.value("Filtr_percent_655_A/Pos4", QVariant::fromValue(0)).toString());
	filtr_prc_655_B_4 = toDouble(settings.value("Filtr_percent_655_B/Pos4", QVariant::fromValue(0)).toString());
	filtr_prc_655_C_4 = toDouble(settings.value("Filtr_percent_655_C/Pos4", QVariant::fromValue(0)).toString());
	filtr_prc_655_A_5 = toDouble(settings.value("Filtr_percent_655_A/Pos5", QVariant::fromValue(0)).toString());
	filtr_prc_655_B_5 = toDouble(settings.value("Filtr_percent_655_B/Pos5", QVariant::fromValue(0)).toString());
	filtr_prc_655_C_5 = toDouble(settings.value("Filtr_percent_655_C/Pos5", QVariant::fromValue(0)).toString());
	serialDeviceZasilaczVendor = settings.value("Zasilacz/Vendor", QVariant::fromValue(QString("67b"))).toString();
	serialDeviceZasilaczProduct = settings.value("Zasilacz/Product", QVariant::fromValue(QString("23a3"))).toString();
	serialDeviceSterownikVendor = settings.value("Sterownik/Vendor", QVariant::fromValue(QString("403"))).toString();
	serialDeviceSterownikProduct = settings.value("Sterownik/Product", QVariant::fromValue(QString("6001"))).toString();
	serialDeviceSterownikSerial = settings.value("Sterownik/Serial", QVariant::fromValue(QString("A10KV4VU"))).toString();
	minNapiecieCzujki = toUInt(settings.value("ParamentryBadania-Czujka/MinimalneNapiecie", QVariant::fromValue(2.0)).toString());
	maxNapiecieCzujki = toUInt(settings.value("ParamentryBadania-Czujka/MaksymalneNapiecie", QVariant::fromValue(30.0)).toString());
	minCzasStabCzujki = toUInt(settings.value("ParamentryBadania-Czujka/MinimalnyCzasStabilizacji", QVariant::fromValue(10)).toString());
	maxCzasStabCzujki = toUInt(settings.value("ParamentryBadania-Czujka/MaksymalnyCzasStabilizacji", QVariant::fromValue(3600)).toString());
	minPrzekrPradZasCzujki = toDouble(settings.value("ParamentryBadania-Alarm/MinimalnyPrzekroczonyPradZasilania", QVariant::fromValue(4.0)).toString());
	maxPrzekrPradZasCzujki = toDouble(settings.value("ParamentryBadania-Alarm/MaksymalnyPrzekroczonyPradZasilania", QVariant::fromValue(200.0)).toString());
	minCzasPoZmianieFiltra = toUInt(settings.value("ParamentryBadania-Filtry/MinimalnyCzasPomiedzyZmianami", QVariant::fromValue(10)).toString());
	maxCzasPoZmianieFiltra = toUInt(settings.value("ParamentryBadania-Filtry/MaksymalnyCzasPomiedzyZmianami", QVariant::fromValue(3600)).toString());
	czasOczekiwaniaPowtarzalnosc4Test = toULong(settings.value("ParamentryBadania-Powtarzalnosc/CzasOczekiwaniaNa4Test", QVariant::fromValue(259200)).toString());
	minimalnaWartoscCzujkiCn = toDouble(settings.value("ParamentryBadania-Odtwarzalnosc/MinimalnaWartoscCzujki", QVariant::fromValue(0.4)).toString());
	czasStabilizacjiDlaKataNieWspolosiowosci = toUInt(settings.value("ParamentryBadania-NieWspolOsiowosc/CzasStabilizacjiDlaKataNieWspolosiowosci", QVariant::fromValue(120)).toString());
	wartoscTlumienieDlaKataNieWspolosiowosci = toDouble(settings.value("ParamentryBadania-NieWspolOsiowosc/WartoscTlumienia", QVariant::fromValue(6.0)).toString());
	maksCzasZadzialaniaCzujkiDlaKataNieWspolosiowosci = toUInt(settings.value("ParamentryBadania-NieWspolOsiowosc/CzasZadzialaniaCzujki", QVariant::fromValue(30)).toString());
	maksCzasTestuZadzialaniaCzujkiDlaKataNieWspolosiowosci = toUInt(settings.value("ParamentryBadania-NieWspolOsiowosc/CzasCalkowityTestuZadzialaniaCzujki", QVariant::fromValue(60)).toString());
	maksKatNieWspolOsiowosci = toDouble(settings.value("ParamentryBadania-Urzadzenie/MaksymalnyKat", QVariant::fromValue(15)).toString());
	odtwarzalnoscCmaxCrep = toDouble(settings.value("ParamentryBadania-Odtwarzalnosc/CmaxCrep", QVariant::fromValue(1.33)).toString());
	odtwarzalnoscCrepCmin = toDouble(settings.value("ParamentryBadania-Odtwarzalnosc/CrepCmin", QVariant::fromValue(1.33)).toString());
	powtarzalnoscCmaxCmin = toDouble(settings.value("ParamentryBadania-Powtarzalnosc/CmaxCmin", QVariant::fromValue(1.6)).toString());
	niewspolosiowoscWartoscTlumnika = toDouble(settings.value("ParamentryBadania-NieWspolOsiowosc/WartoscTlumnika", QVariant::fromValue(6.0)).toString());
	czasWylaczeniaCzujkiDlaResetu = toUInt(settings.value("ParamentryBadania-Urzadzenie/CzasWylaczeniaCzujkiDlaResetu", QVariant::fromValue(15)).toString());
	niewspolosiowoscMinimalnyKatProducentMierzony = toDouble(settings.value("ParamentryBadania-NieWspolOsiowosc/MinimalnaRoznicaKataMiedzyZmierzonymAZadeklarowanym", QVariant::fromValue(0.4)).toString());
	maksCzasZadzialaniaCzujkidlaTlumnikaA = toUInt(settings.value("ParamentryBadania-SzybkieZmianyTlumienia/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaA", QVariant::fromValue(30)).toString());
	maksCzasTestuCzujkidlaTlumnikaA = toUInt(settings.value("ParamentryBadania-Urzadzenie/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaA", QVariant::fromValue(40)).toString());
	maksCzasZadzialaniaCzujkidlaTlumnikaB = toUInt(settings.value("ParamentryBadania-SzybkieZmianyTlumienia/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaB", QVariant::fromValue(60)).toString());
	maksCzasTestuCzujkidlaTlumnikaB = toUInt(settings.value("ParamentryBadania-Urzadzenie/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaB", QVariant::fromValue(70)).toString());
	szybkieZmianyWartoscTlumnikaA = toDouble(settings.value("ParamentryBadania-SzybkieZmiany/WartoscTlumnikaA", QVariant::fromValue(6.0)).toString());
	szybkieZmianyWartoscTlumnikaB = toDouble(settings.value("ParamentryBadania-SzybkieZmiany/WartoscTlumnikaB", QVariant::fromValue(10.0)).toString());
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
	settings.setValue("Silnik-1/PrzelozenieImpJedn", QVariant::fromValue(motorPrzelozenieImpJedn1));
	settings.setValue("Silnik-2/PrzelozenieImpJedn", QVariant::fromValue(motorPrzelozenieImpJedn2));
	settings.setValue("Silnik-3/PrzelozenieImpJedn", QVariant::fromValue(motorPrzelozenieImpJedn3));
	settings.setValue("Silnik-4/PrzelozenieImpJedn", QVariant::fromValue(motorPrzelozenieImpJedn4));
	settings.setValue("Silnik-5/PrzelozenieImpJedn", QVariant::fromValue(motorPrzelozenieImpJedn5));
	settings.setValue("Silnik-6/PrzelozenieImpJedn", QVariant::fromValue(motorPrzelozenieImpJedn6));
	settings.setValue("Silnik-7/PrzelozenieImpJedn", QVariant::fromValue(motorPrzelozenieImpJedn7));
	settings.setValue("Silnik-8/PrzelozenieImpJedn", QVariant::fromValue(motorPrzelozenieImpJedn8));
	settings.setValue("Silnik-9/PrzelozenieImpJedn", QVariant::fromValue(motorPrzelozenieImpJedn9));
	settings.setValue("Silnik-1/MinOdstepImp", QVariant::fromValue(motorMinOdstepImp1));
	settings.setValue("Silnik-2/MinOdstepImp", QVariant::fromValue(motorMinOdstepImp2));
	settings.setValue("Silnik-3/MinOdstepImp", QVariant::fromValue(motorMinOdstepImp3));
	settings.setValue("Silnik-4/MinOdstepImp", QVariant::fromValue(motorMinOdstepImp4));
	settings.setValue("Silnik-5/MinOdstepImp", QVariant::fromValue(motorMinOdstepImp5));
	settings.setValue("Silnik-6/MinOdstepImp", QVariant::fromValue(motorMinOdstepImp6));
	settings.setValue("Silnik-7/MinOdstepImp", QVariant::fromValue(motorMinOdstepImp7));
	settings.setValue("Silnik-8/MinOdstepImp", QVariant::fromValue(motorMinOdstepImp8));
	settings.setValue("Silnik-9/MinOdstepImp", QVariant::fromValue(motorMinOdstepImp9));
	settings.setValue("Silnik-1/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty1));
	settings.setValue("Silnik-2/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty2));
	settings.setValue("Silnik-3/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty3));
	settings.setValue("Silnik-4/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty4));
	settings.setValue("Silnik-5/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty5));
	settings.setValue("Silnik-6/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty6));
	settings.setValue("Silnik-7/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty7));
	settings.setValue("Silnik-8/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty8));
	settings.setValue("Silnik-9/OdwrocObroty", QVariant::fromValue(motorOdwrocObroty9));
	settings.setValue("Silnik-1/CzasMiedzyImpZerow", QVariant::fromValue(motorCzasMiedzyImpZerow1));
	settings.setValue("Silnik-2/CzasMiedzyImpZerow", QVariant::fromValue(motorCzasMiedzyImpZerow2));
	settings.setValue("Silnik-3/CzasMiedzyImpZerow", QVariant::fromValue(motorCzasMiedzyImpZerow3));
	settings.setValue("Silnik-4/CzasMiedzyImpZerow", QVariant::fromValue(motorCzasMiedzyImpZerow4));
	settings.setValue("Silnik-5/CzasMiedzyImpZerow", QVariant::fromValue(motorCzasMiedzyImpZerow5));
	settings.setValue("Silnik-6/CzasMiedzyImpZerow", QVariant::fromValue(motorCzasMiedzyImpZerow6));
	settings.setValue("Silnik-7/CzasMiedzyImpZerow", QVariant::fromValue(motorCzasMiedzyImpZerow7));
	settings.setValue("Silnik-8/CzasMiedzyImpZerow", QVariant::fromValue(motorCzasMiedzyImpZerow8));
	settings.setValue("Silnik-9/CzasMiedzyImpZerow", QVariant::fromValue(motorCzasMiedzyImpZerow9));
	settings.setValue("Silnik-1/CzasMiedzyImpNormal", QVariant::fromValue(motorCzasMiedzyImpNormal1));
	settings.setValue("Silnik-2/CzasMiedzyImpNormal", QVariant::fromValue(motorCzasMiedzyImpNormal2));
	settings.setValue("Silnik-3/CzasMiedzyImpNormal", QVariant::fromValue(motorCzasMiedzyImpNormal3));
	settings.setValue("Silnik-4/CzasMiedzyImpNormal", QVariant::fromValue(motorCzasMiedzyImpNormal4));
	settings.setValue("Silnik-5/CzasMiedzyImpNormal", QVariant::fromValue(motorCzasMiedzyImpNormal5));
	settings.setValue("Silnik-6/CzasMiedzyImpNormal", QVariant::fromValue(motorCzasMiedzyImpNormal6));
	settings.setValue("Silnik-7/CzasMiedzyImpNormal", QVariant::fromValue(motorCzasMiedzyImpNormal7));
	settings.setValue("Silnik-8/CzasMiedzyImpNormal", QVariant::fromValue(motorCzasMiedzyImpNormal8));
	settings.setValue("Silnik-9/CzasMiedzyImpNormal", QVariant::fromValue(motorCzasMiedzyImpNormal9));
	settings.setValue("Silnik-1/MaksIloscImp", QVariant::fromValue(motorMaksIloscImp1));
	settings.setValue("Silnik-2/MaksIloscImp", QVariant::fromValue(motorMaksIloscImp2));
	settings.setValue("Silnik-3/MaksIloscImp", QVariant::fromValue(motorMaksIloscImp3));
	settings.setValue("Silnik-4/MaksIloscImp", QVariant::fromValue(motorMaksIloscImp4));
	settings.setValue("Silnik-5/MaksIloscImp", QVariant::fromValue(motorMaksIloscImp5));
	settings.setValue("Silnik-6/MaksIloscImp", QVariant::fromValue(motorMaksIloscImp6));
	settings.setValue("Silnik-7/MaksIloscImp", QVariant::fromValue(motorMaksIloscImp7));
	settings.setValue("Silnik-8/MaksIloscImp", QVariant::fromValue(motorMaksIloscImp8));
	settings.setValue("Silnik-9/MaksIloscImp", QVariant::fromValue(motorMaksIloscImp9));
	settings.setValue("Silnik-1/IloscImpBaza", QVariant::fromValue(motorIloscImpBaza1));
	settings.setValue("Silnik-2/IloscImpBaza", QVariant::fromValue(motorIloscImpBaza2));
	settings.setValue("Silnik-3/IloscImpBaza", QVariant::fromValue(motorIloscImpBaza3));
	settings.setValue("Silnik-4/IloscImpBaza", QVariant::fromValue(motorIloscImpBaza4));
	settings.setValue("Silnik-5/IloscImpBaza", QVariant::fromValue(motorIloscImpBaza5));
	settings.setValue("Silnik-6/IloscImpBaza", QVariant::fromValue(motorIloscImpBaza6));
	settings.setValue("Silnik-7/IloscImpBaza", QVariant::fromValue(motorIloscImpBaza7));
	settings.setValue("Silnik-8/IloscImpBaza", QVariant::fromValue(motorIloscImpBaza8));
	settings.setValue("Silnik-9/IloscImpBaza", QVariant::fromValue(motorIloscImpBaza9));
	settings.setValue("Silnik-1/IloscImpSrodek", QVariant::fromValue(motorIloscImpSrodek1));
	settings.setValue("Silnik-2/IloscImpSrodek", QVariant::fromValue(motorIloscImpSrodek2));
	settings.setValue("Silnik-3/IloscImpSrodek", QVariant::fromValue(motorIloscImpSrodek3));
	settings.setValue("Silnik-4/IloscImpSrodek", QVariant::fromValue(motorIloscImpSrodek4));
	settings.setValue("Silnik-5/IloscImpSrodek", QVariant::fromValue(motorIloscImpSrodek5));
	settings.setValue("Silnik-6/IloscImpSrodek", QVariant::fromValue(motorIloscImpSrodek6));
	settings.setValue("Silnik-7/IloscImpSrodek", QVariant::fromValue(motorIloscImpSrodek7));
	settings.setValue("Silnik-8/IloscImpSrodek", QVariant::fromValue(motorIloscImpSrodek8));
	settings.setValue("Silnik-9/IloscImpSrodek", QVariant::fromValue(motorIloscImpSrodek9));
	settings.setValue("Filtr_db_880_A/Pos0", QVariant::fromValue(filtr_db_880_A_0));
	settings.setValue("Filtr_db_880_B/Pos0", QVariant::fromValue(filtr_db_880_B_0));
	settings.setValue("Filtr_db_880_C/Pos0", QVariant::fromValue(filtr_db_880_C_0));
	settings.setValue("Filtr_db_880_A/Pos1", QVariant::fromValue(filtr_db_880_A_1));
	settings.setValue("Filtr_db_880_B/Pos1", QVariant::fromValue(filtr_db_880_B_1));
	settings.setValue("Filtr_db_880_C/Pos1", QVariant::fromValue(filtr_db_880_C_1));
	settings.setValue("Filtr_db_880_A/Pos2", QVariant::fromValue(filtr_db_880_A_2));
	settings.setValue("Filtr_db_880_B/Pos2", QVariant::fromValue(filtr_db_880_B_2));
	settings.setValue("Filtr_db_880_C/Pos2", QVariant::fromValue(filtr_db_880_C_2));
	settings.setValue("Filtr_db_880_A/Pos3", QVariant::fromValue(filtr_db_880_A_3));
	settings.setValue("Filtr_db_880_B/Pos3", QVariant::fromValue(filtr_db_880_B_3));
	settings.setValue("Filtr_db_880_C/Pos3", QVariant::fromValue(filtr_db_880_C_3));
	settings.setValue("Filtr_db_880_A/Pos4", QVariant::fromValue(filtr_db_880_A_4));
	settings.setValue("Filtr_db_880_B/Pos4", QVariant::fromValue(filtr_db_880_B_4));
	settings.setValue("Filtr_db_880_C/Pos4", QVariant::fromValue(filtr_db_880_C_4));
	settings.setValue("Filtr_db_880_A/Pos5", QVariant::fromValue(filtr_db_880_A_5));
	settings.setValue("Filtr_db_880_B/Pos5", QVariant::fromValue(filtr_db_880_B_5));
	settings.setValue("Filtr_db_880_C/Pos5", QVariant::fromValue(filtr_db_880_C_5));
	settings.setValue("Filtr_db_655_A/Pos0", QVariant::fromValue(filtr_db_655_A_0));
	settings.setValue("Filtr_db_655_B/Pos0", QVariant::fromValue(filtr_db_655_B_0));
	settings.setValue("Filtr_db_655_C/Pos0", QVariant::fromValue(filtr_db_655_C_0));
	settings.setValue("Filtr_db_655_A/Pos1", QVariant::fromValue(filtr_db_655_A_1));
	settings.setValue("Filtr_db_655_B/Pos1", QVariant::fromValue(filtr_db_655_B_1));
	settings.setValue("Filtr_db_655_C/Pos1", QVariant::fromValue(filtr_db_655_C_1));
	settings.setValue("Filtr_db_655_A/Pos2", QVariant::fromValue(filtr_db_655_A_2));
	settings.setValue("Filtr_db_655_B/Pos2", QVariant::fromValue(filtr_db_655_B_2));
	settings.setValue("Filtr_db_655_C/Pos2", QVariant::fromValue(filtr_db_655_C_2));
	settings.setValue("Filtr_db_655_A/Pos3", QVariant::fromValue(filtr_db_655_A_3));
	settings.setValue("Filtr_db_655_B/Pos3", QVariant::fromValue(filtr_db_655_B_3));
	settings.setValue("Filtr_db_655_C/Pos3", QVariant::fromValue(filtr_db_655_C_3));
	settings.setValue("Filtr_db_655_A/Pos4", QVariant::fromValue(filtr_db_655_A_4));
	settings.setValue("Filtr_db_655_B/Pos4", QVariant::fromValue(filtr_db_655_B_4));
	settings.setValue("Filtr_db_655_C/Pos4", QVariant::fromValue(filtr_db_655_C_4));
	settings.setValue("Filtr_db_655_A/Pos5", QVariant::fromValue(filtr_db_655_A_5));
	settings.setValue("Filtr_db_655_B/Pos5", QVariant::fromValue(filtr_db_655_B_5));
	settings.setValue("Filtr_db_655_C/Pos5", QVariant::fromValue(filtr_db_655_C_5));
	settings.setValue("Filtr_percent_880_A/Pos0", QVariant::fromValue(filtr_prc_880_A_0));
	settings.setValue("Filtr_percent_880_B/Pos0", QVariant::fromValue(filtr_prc_880_B_0));
	settings.setValue("Filtr_percent_880_C/Pos0", QVariant::fromValue(filtr_prc_880_C_0));
	settings.setValue("Filtr_percent_880_A/Pos1", QVariant::fromValue(filtr_prc_880_A_1));
	settings.setValue("Filtr_percent_880_B/Pos1", QVariant::fromValue(filtr_prc_880_B_1));
	settings.setValue("Filtr_percent_880_C/Pos1", QVariant::fromValue(filtr_prc_880_C_1));
	settings.setValue("Filtr_percent_880_A/Pos2", QVariant::fromValue(filtr_prc_880_A_2));
	settings.setValue("Filtr_percent_880_B/Pos2", QVariant::fromValue(filtr_prc_880_B_2));
	settings.setValue("Filtr_percent_880_C/Pos2", QVariant::fromValue(filtr_prc_880_C_2));
	settings.setValue("Filtr_percent_880_A/Pos3", QVariant::fromValue(filtr_prc_880_A_3));
	settings.setValue("Filtr_percent_880_B/Pos3", QVariant::fromValue(filtr_prc_880_B_3));
	settings.setValue("Filtr_percent_880_C/Pos3", QVariant::fromValue(filtr_prc_880_C_3));
	settings.setValue("Filtr_percent_880_A/Pos4", QVariant::fromValue(filtr_prc_880_A_4));
	settings.setValue("Filtr_percent_880_B/Pos4", QVariant::fromValue(filtr_prc_880_B_4));
	settings.setValue("Filtr_percent_880_C/Pos4", QVariant::fromValue(filtr_prc_880_C_4));
	settings.setValue("Filtr_percent_880_A/Pos5", QVariant::fromValue(filtr_prc_880_A_5));
	settings.setValue("Filtr_percent_880_B/Pos5", QVariant::fromValue(filtr_prc_880_B_5));
	settings.setValue("Filtr_percent_880_C/Pos5", QVariant::fromValue(filtr_prc_880_C_5));
	settings.setValue("Filtr_percent_655_A/Pos0", QVariant::fromValue(filtr_prc_655_A_0));
	settings.setValue("Filtr_percent_655_B/Pos0", QVariant::fromValue(filtr_prc_655_B_0));
	settings.setValue("Filtr_percent_655_C/Pos0", QVariant::fromValue(filtr_prc_655_C_0));
	settings.setValue("Filtr_percent_655_A/Pos1", QVariant::fromValue(filtr_prc_655_A_1));
	settings.setValue("Filtr_percent_655_B/Pos1", QVariant::fromValue(filtr_prc_655_B_1));
	settings.setValue("Filtr_percent_655_C/Pos1", QVariant::fromValue(filtr_prc_655_C_1));
	settings.setValue("Filtr_percent_655_A/Pos2", QVariant::fromValue(filtr_prc_655_A_2));
	settings.setValue("Filtr_percent_655_B/Pos2", QVariant::fromValue(filtr_prc_655_B_2));
	settings.setValue("Filtr_percent_655_C/Pos2", QVariant::fromValue(filtr_prc_655_C_2));
	settings.setValue("Filtr_percent_655_A/Pos3", QVariant::fromValue(filtr_prc_655_A_3));
	settings.setValue("Filtr_percent_655_B/Pos3", QVariant::fromValue(filtr_prc_655_B_3));
	settings.setValue("Filtr_percent_655_C/Pos3", QVariant::fromValue(filtr_prc_655_C_3));
	settings.setValue("Filtr_percent_655_A/Pos4", QVariant::fromValue(filtr_prc_655_A_4));
	settings.setValue("Filtr_percent_655_B/Pos4", QVariant::fromValue(filtr_prc_655_B_4));
	settings.setValue("Filtr_percent_655_C/Pos4", QVariant::fromValue(filtr_prc_655_C_4));
	settings.setValue("Filtr_percent_655_A/Pos5", QVariant::fromValue(filtr_prc_655_A_5));
	settings.setValue("Filtr_percent_655_B/Pos5", QVariant::fromValue(filtr_prc_655_B_5));
	settings.setValue("Filtr_percent_655_C/Pos5", QVariant::fromValue(filtr_prc_655_C_5));
	settings.setValue("Zasilacz/Vendor", QVariant::fromValue(serialDeviceZasilaczVendor));
	settings.setValue("Zasilacz/Product", QVariant::fromValue(serialDeviceZasilaczProduct));
	settings.setValue("Sterownik/Vendor", QVariant::fromValue(serialDeviceSterownikVendor));
	settings.setValue("Sterownik/Product", QVariant::fromValue(serialDeviceSterownikProduct));
	settings.setValue("Sterownik/Serial", QVariant::fromValue(serialDeviceSterownikSerial));
	settings.setValue("ParamentryBadania-Czujka/MinimalneNapiecie", QVariant::fromValue(minNapiecieCzujki));
	settings.setValue("ParamentryBadania-Czujka/MaksymalneNapiecie", QVariant::fromValue(maxNapiecieCzujki));
	settings.setValue("ParamentryBadania-Czujka/MinimalnyCzasStabilizacji", QVariant::fromValue(minCzasStabCzujki));
	settings.setValue("ParamentryBadania-Czujka/MaksymalnyCzasStabilizacji", QVariant::fromValue(maxCzasStabCzujki));
	settings.setValue("ParamentryBadania-Alarm/MinimalnyPrzekroczonyPradZasilania", QVariant::fromValue(minPrzekrPradZasCzujki));
	settings.setValue("ParamentryBadania-Alarm/MaksymalnyPrzekroczonyPradZasilania", QVariant::fromValue(maxPrzekrPradZasCzujki));
	settings.setValue("ParamentryBadania-Filtry/MinimalnyCzasPomiedzyZmianami", QVariant::fromValue(minCzasPoZmianieFiltra));
	settings.setValue("ParamentryBadania-Filtry/MaksymalnyCzasPomiedzyZmianami", QVariant::fromValue(maxCzasPoZmianieFiltra));
	settings.setValue("ParamentryBadania-Powtarzalnosc/CzasOczekiwaniaNa4Test", QVariant::fromValue(czasOczekiwaniaPowtarzalnosc4Test));
	settings.setValue("ParamentryBadania-Odtwarzalnosc/MinimalnaWartoscCzujki", QVariant::fromValue(minimalnaWartoscCzujkiCn));
	settings.setValue("ParamentryBadania-NieWspolOsiowosc/CzasStabilizacjiDlaKataNieWspolosiowosci", QVariant::fromValue(czasStabilizacjiDlaKataNieWspolosiowosci));
	settings.setValue("ParamentryBadania-NieWspolOsiowosc/WartoscTlumienia", QVariant::fromValue(wartoscTlumienieDlaKataNieWspolosiowosci));
	settings.setValue("ParamentryBadania-NieWspolOsiowosc/CzasZadzialaniaCzujki", QVariant::fromValue(maksCzasZadzialaniaCzujkiDlaKataNieWspolosiowosci));
	settings.setValue("ParamentryBadania-NieWspolOsiowosc/CzasCalkowityTestuZadzialaniaCzujki", QVariant::fromValue(maksCzasTestuZadzialaniaCzujkiDlaKataNieWspolosiowosci));
	settings.setValue("ParamentryBadania-Urzadzenie/MaksymalnyKat", QVariant::fromValue(maksKatNieWspolOsiowosci));
	settings.setValue("ParamentryBadania-Odtwarzalnosc/CmaxCrep", QVariant::fromValue(odtwarzalnoscCmaxCrep));
	settings.setValue("ParamentryBadania-Odtwarzalnosc/CrepCmin", QVariant::fromValue(odtwarzalnoscCrepCmin));
	settings.setValue("ParamentryBadania-Powtarzalnosc/CmaxCmin", QVariant::fromValue(powtarzalnoscCmaxCmin));
	settings.setValue("ParamentryBadania-NieWspolOsiowosc/WartoscTlumnika", QVariant::fromValue(niewspolosiowoscWartoscTlumnika));
	settings.setValue("ParamentryBadania-Urzadzenie/CzasWylaczeniaCzujkiDlaResetu", QVariant::fromValue(czasWylaczeniaCzujkiDlaResetu));
	settings.setValue("ParamentryBadania-NieWspolOsiowosc/MinimalnaRoznicaKataMiedzyZmierzonymAZadeklarowanym", QVariant::fromValue(niewspolosiowoscMinimalnyKatProducentMierzony));
	settings.setValue("ParamentryBadania-SzybkieZmianyTlumienia/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaA", QVariant::fromValue(maksCzasZadzialaniaCzujkidlaTlumnikaA));
	settings.setValue("ParamentryBadania-Urzadzenie/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaA", QVariant::fromValue(maksCzasTestuCzujkidlaTlumnikaA));
	settings.setValue("ParamentryBadania-SzybkieZmianyTlumienia/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaB", QVariant::fromValue(maksCzasZadzialaniaCzujkidlaTlumnikaB));
	settings.setValue("ParamentryBadania-Urzadzenie/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaB", QVariant::fromValue(maksCzasTestuCzujkidlaTlumnikaB));
	settings.setValue("ParamentryBadania-SzybkieZmiany/WartoscTlumnikaA", QVariant::fromValue(szybkieZmianyWartoscTlumnikaA));
	settings.setValue("ParamentryBadania-SzybkieZmiany/WartoscTlumnikaB", QVariant::fromValue(szybkieZmianyWartoscTlumnikaB));
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

double UstawieniaGen::getMotorPrzelozenieImpJedn1() const
{
	return motorPrzelozenieImpJedn1;
}

void UstawieniaGen::setMotorPrzelozenieImpJedn1(const double & value)
{
	motorPrzelozenieImpJedn1 = value;
	settings.setValue("Silnik-1/PrzelozenieImpJedn", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenieImpJedn2() const
{
	return motorPrzelozenieImpJedn2;
}

void UstawieniaGen::setMotorPrzelozenieImpJedn2(const double & value)
{
	motorPrzelozenieImpJedn2 = value;
	settings.setValue("Silnik-2/PrzelozenieImpJedn", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenieImpJedn3() const
{
	return motorPrzelozenieImpJedn3;
}

void UstawieniaGen::setMotorPrzelozenieImpJedn3(const double & value)
{
	motorPrzelozenieImpJedn3 = value;
	settings.setValue("Silnik-3/PrzelozenieImpJedn", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenieImpJedn4() const
{
	return motorPrzelozenieImpJedn4;
}

void UstawieniaGen::setMotorPrzelozenieImpJedn4(const double & value)
{
	motorPrzelozenieImpJedn4 = value;
	settings.setValue("Silnik-4/PrzelozenieImpJedn", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenieImpJedn5() const
{
	return motorPrzelozenieImpJedn5;
}

void UstawieniaGen::setMotorPrzelozenieImpJedn5(const double & value)
{
	motorPrzelozenieImpJedn5 = value;
	settings.setValue("Silnik-5/PrzelozenieImpJedn", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenieImpJedn6() const
{
	return motorPrzelozenieImpJedn6;
}

void UstawieniaGen::setMotorPrzelozenieImpJedn6(const double & value)
{
	motorPrzelozenieImpJedn6 = value;
	settings.setValue("Silnik-6/PrzelozenieImpJedn", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenieImpJedn7() const
{
	return motorPrzelozenieImpJedn7;
}

void UstawieniaGen::setMotorPrzelozenieImpJedn7(const double & value)
{
	motorPrzelozenieImpJedn7 = value;
	settings.setValue("Silnik-7/PrzelozenieImpJedn", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenieImpJedn8() const
{
	return motorPrzelozenieImpJedn8;
}

void UstawieniaGen::setMotorPrzelozenieImpJedn8(const double & value)
{
	motorPrzelozenieImpJedn8 = value;
	settings.setValue("Silnik-8/PrzelozenieImpJedn", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenieImpJedn9() const
{
	return motorPrzelozenieImpJedn9;
}

void UstawieniaGen::setMotorPrzelozenieImpJedn9(const double & value)
{
	motorPrzelozenieImpJedn9 = value;
	settings.setValue("Silnik-9/PrzelozenieImpJedn", QVariant::fromValue(value));
}

double UstawieniaGen::getMotorPrzelozenieImpJedn(const short & nrMotor) const
{
	switch(nrMotor) {
	case 1 : return getMotorPrzelozenieImpJedn1();
	case 2 : return getMotorPrzelozenieImpJedn2();
	case 3 : return getMotorPrzelozenieImpJedn3();
	case 4 : return getMotorPrzelozenieImpJedn4();
	case 5 : return getMotorPrzelozenieImpJedn5();
	case 6 : return getMotorPrzelozenieImpJedn6();
	case 7 : return getMotorPrzelozenieImpJedn7();
	case 8 : return getMotorPrzelozenieImpJedn8();
	case 9 : return getMotorPrzelozenieImpJedn9();
	default: return toDouble(QString());
	}

}

void UstawieniaGen::setMotorPrzelozenieImpJedn(const short & nrMotor, const double & przelozenieImpJedn)
{
	switch(nrMotor) {
	case 1 : setMotorPrzelozenieImpJedn1(przelozenieImpJedn); break;
	case 2 : setMotorPrzelozenieImpJedn2(przelozenieImpJedn); break;
	case 3 : setMotorPrzelozenieImpJedn3(przelozenieImpJedn); break;
	case 4 : setMotorPrzelozenieImpJedn4(przelozenieImpJedn); break;
	case 5 : setMotorPrzelozenieImpJedn5(przelozenieImpJedn); break;
	case 6 : setMotorPrzelozenieImpJedn6(przelozenieImpJedn); break;
	case 7 : setMotorPrzelozenieImpJedn7(przelozenieImpJedn); break;
	case 8 : setMotorPrzelozenieImpJedn8(przelozenieImpJedn); break;
	case 9 : setMotorPrzelozenieImpJedn9(przelozenieImpJedn); break;
	default: break;
	}

}

bool UstawieniaGen::checkMotorPrzelozenieImpJedn(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    double v = val.toDouble(&ok);
    if (!ok || v < 0.0)
        return false;
    return true;

}

int UstawieniaGen::getMotorMinOdstepImp1() const
{
	return motorMinOdstepImp1;
}

void UstawieniaGen::setMotorMinOdstepImp1(const int & value)
{
	motorMinOdstepImp1 = value;
	settings.setValue("Silnik-1/MinOdstepImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMinOdstepImp2() const
{
	return motorMinOdstepImp2;
}

void UstawieniaGen::setMotorMinOdstepImp2(const int & value)
{
	motorMinOdstepImp2 = value;
	settings.setValue("Silnik-2/MinOdstepImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMinOdstepImp3() const
{
	return motorMinOdstepImp3;
}

void UstawieniaGen::setMotorMinOdstepImp3(const int & value)
{
	motorMinOdstepImp3 = value;
	settings.setValue("Silnik-3/MinOdstepImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMinOdstepImp4() const
{
	return motorMinOdstepImp4;
}

void UstawieniaGen::setMotorMinOdstepImp4(const int & value)
{
	motorMinOdstepImp4 = value;
	settings.setValue("Silnik-4/MinOdstepImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMinOdstepImp5() const
{
	return motorMinOdstepImp5;
}

void UstawieniaGen::setMotorMinOdstepImp5(const int & value)
{
	motorMinOdstepImp5 = value;
	settings.setValue("Silnik-5/MinOdstepImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMinOdstepImp6() const
{
	return motorMinOdstepImp6;
}

void UstawieniaGen::setMotorMinOdstepImp6(const int & value)
{
	motorMinOdstepImp6 = value;
	settings.setValue("Silnik-6/MinOdstepImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMinOdstepImp7() const
{
	return motorMinOdstepImp7;
}

void UstawieniaGen::setMotorMinOdstepImp7(const int & value)
{
	motorMinOdstepImp7 = value;
	settings.setValue("Silnik-7/MinOdstepImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMinOdstepImp8() const
{
	return motorMinOdstepImp8;
}

void UstawieniaGen::setMotorMinOdstepImp8(const int & value)
{
	motorMinOdstepImp8 = value;
	settings.setValue("Silnik-8/MinOdstepImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMinOdstepImp9() const
{
	return motorMinOdstepImp9;
}

void UstawieniaGen::setMotorMinOdstepImp9(const int & value)
{
	motorMinOdstepImp9 = value;
	settings.setValue("Silnik-9/MinOdstepImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMinOdstepImp(const short & nrMotor) const
{
	switch(nrMotor) {
	case 1 : return getMotorMinOdstepImp1();
	case 2 : return getMotorMinOdstepImp2();
	case 3 : return getMotorMinOdstepImp3();
	case 4 : return getMotorMinOdstepImp4();
	case 5 : return getMotorMinOdstepImp5();
	case 6 : return getMotorMinOdstepImp6();
	case 7 : return getMotorMinOdstepImp7();
	case 8 : return getMotorMinOdstepImp8();
	case 9 : return getMotorMinOdstepImp9();
	default: return toUInt(QString());
	}

}

void UstawieniaGen::setMotorMinOdstepImp(const short & nrMotor, const int & minOdstepImp)
{
	switch(nrMotor) {
	case 1 : setMotorMinOdstepImp1(minOdstepImp); break;
	case 2 : setMotorMinOdstepImp2(minOdstepImp); break;
	case 3 : setMotorMinOdstepImp3(minOdstepImp); break;
	case 4 : setMotorMinOdstepImp4(minOdstepImp); break;
	case 5 : setMotorMinOdstepImp5(minOdstepImp); break;
	case 6 : setMotorMinOdstepImp6(minOdstepImp); break;
	case 7 : setMotorMinOdstepImp7(minOdstepImp); break;
	case 8 : setMotorMinOdstepImp8(minOdstepImp); break;
	case 9 : setMotorMinOdstepImp9(minOdstepImp); break;
	default: break;
	}

}

bool UstawieniaGen::checkMotorMinOdstepImp(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    int v = val.toInt(&ok);
    if (!ok || v <= 0)
        return false;
    return ok;

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

int UstawieniaGen::getMotorCzasMiedzyImpZerow1() const
{
	return motorCzasMiedzyImpZerow1;
}

void UstawieniaGen::setMotorCzasMiedzyImpZerow1(const int & value)
{
	motorCzasMiedzyImpZerow1 = value;
	settings.setValue("Silnik-1/CzasMiedzyImpZerow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpZerow2() const
{
	return motorCzasMiedzyImpZerow2;
}

void UstawieniaGen::setMotorCzasMiedzyImpZerow2(const int & value)
{
	motorCzasMiedzyImpZerow2 = value;
	settings.setValue("Silnik-2/CzasMiedzyImpZerow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpZerow3() const
{
	return motorCzasMiedzyImpZerow3;
}

void UstawieniaGen::setMotorCzasMiedzyImpZerow3(const int & value)
{
	motorCzasMiedzyImpZerow3 = value;
	settings.setValue("Silnik-3/CzasMiedzyImpZerow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpZerow4() const
{
	return motorCzasMiedzyImpZerow4;
}

void UstawieniaGen::setMotorCzasMiedzyImpZerow4(const int & value)
{
	motorCzasMiedzyImpZerow4 = value;
	settings.setValue("Silnik-4/CzasMiedzyImpZerow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpZerow5() const
{
	return motorCzasMiedzyImpZerow5;
}

void UstawieniaGen::setMotorCzasMiedzyImpZerow5(const int & value)
{
	motorCzasMiedzyImpZerow5 = value;
	settings.setValue("Silnik-5/CzasMiedzyImpZerow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpZerow6() const
{
	return motorCzasMiedzyImpZerow6;
}

void UstawieniaGen::setMotorCzasMiedzyImpZerow6(const int & value)
{
	motorCzasMiedzyImpZerow6 = value;
	settings.setValue("Silnik-6/CzasMiedzyImpZerow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpZerow7() const
{
	return motorCzasMiedzyImpZerow7;
}

void UstawieniaGen::setMotorCzasMiedzyImpZerow7(const int & value)
{
	motorCzasMiedzyImpZerow7 = value;
	settings.setValue("Silnik-7/CzasMiedzyImpZerow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpZerow8() const
{
	return motorCzasMiedzyImpZerow8;
}

void UstawieniaGen::setMotorCzasMiedzyImpZerow8(const int & value)
{
	motorCzasMiedzyImpZerow8 = value;
	settings.setValue("Silnik-8/CzasMiedzyImpZerow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpZerow9() const
{
	return motorCzasMiedzyImpZerow9;
}

void UstawieniaGen::setMotorCzasMiedzyImpZerow9(const int & value)
{
	motorCzasMiedzyImpZerow9 = value;
	settings.setValue("Silnik-9/CzasMiedzyImpZerow", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpZerow(const short & nrMotor) const
{
	switch(nrMotor) {
	case 1 : return getMotorCzasMiedzyImpZerow1();
	case 2 : return getMotorCzasMiedzyImpZerow2();
	case 3 : return getMotorCzasMiedzyImpZerow3();
	case 4 : return getMotorCzasMiedzyImpZerow4();
	case 5 : return getMotorCzasMiedzyImpZerow5();
	case 6 : return getMotorCzasMiedzyImpZerow6();
	case 7 : return getMotorCzasMiedzyImpZerow7();
	case 8 : return getMotorCzasMiedzyImpZerow8();
	case 9 : return getMotorCzasMiedzyImpZerow9();
	default: return toUInt(QString());
	}

}

void UstawieniaGen::setMotorCzasMiedzyImpZerow(const short & nrMotor, const int & czasMiedzyImpZerow)
{
	switch(nrMotor) {
	case 1 : setMotorCzasMiedzyImpZerow1(czasMiedzyImpZerow); break;
	case 2 : setMotorCzasMiedzyImpZerow2(czasMiedzyImpZerow); break;
	case 3 : setMotorCzasMiedzyImpZerow3(czasMiedzyImpZerow); break;
	case 4 : setMotorCzasMiedzyImpZerow4(czasMiedzyImpZerow); break;
	case 5 : setMotorCzasMiedzyImpZerow5(czasMiedzyImpZerow); break;
	case 6 : setMotorCzasMiedzyImpZerow6(czasMiedzyImpZerow); break;
	case 7 : setMotorCzasMiedzyImpZerow7(czasMiedzyImpZerow); break;
	case 8 : setMotorCzasMiedzyImpZerow8(czasMiedzyImpZerow); break;
	case 9 : setMotorCzasMiedzyImpZerow9(czasMiedzyImpZerow); break;
	default: break;
	}

}

bool UstawieniaGen::checkMotorCzasMiedzyImpZerow(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    int v = val.toInt(&ok);
    if (!ok || v <= 0)
        return false;
    return ok;

}

int UstawieniaGen::getMotorCzasMiedzyImpNormal1() const
{
	return motorCzasMiedzyImpNormal1;
}

void UstawieniaGen::setMotorCzasMiedzyImpNormal1(const int & value)
{
	motorCzasMiedzyImpNormal1 = value;
	settings.setValue("Silnik-1/CzasMiedzyImpNormal", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpNormal2() const
{
	return motorCzasMiedzyImpNormal2;
}

void UstawieniaGen::setMotorCzasMiedzyImpNormal2(const int & value)
{
	motorCzasMiedzyImpNormal2 = value;
	settings.setValue("Silnik-2/CzasMiedzyImpNormal", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpNormal3() const
{
	return motorCzasMiedzyImpNormal3;
}

void UstawieniaGen::setMotorCzasMiedzyImpNormal3(const int & value)
{
	motorCzasMiedzyImpNormal3 = value;
	settings.setValue("Silnik-3/CzasMiedzyImpNormal", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpNormal4() const
{
	return motorCzasMiedzyImpNormal4;
}

void UstawieniaGen::setMotorCzasMiedzyImpNormal4(const int & value)
{
	motorCzasMiedzyImpNormal4 = value;
	settings.setValue("Silnik-4/CzasMiedzyImpNormal", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpNormal5() const
{
	return motorCzasMiedzyImpNormal5;
}

void UstawieniaGen::setMotorCzasMiedzyImpNormal5(const int & value)
{
	motorCzasMiedzyImpNormal5 = value;
	settings.setValue("Silnik-5/CzasMiedzyImpNormal", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpNormal6() const
{
	return motorCzasMiedzyImpNormal6;
}

void UstawieniaGen::setMotorCzasMiedzyImpNormal6(const int & value)
{
	motorCzasMiedzyImpNormal6 = value;
	settings.setValue("Silnik-6/CzasMiedzyImpNormal", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpNormal7() const
{
	return motorCzasMiedzyImpNormal7;
}

void UstawieniaGen::setMotorCzasMiedzyImpNormal7(const int & value)
{
	motorCzasMiedzyImpNormal7 = value;
	settings.setValue("Silnik-7/CzasMiedzyImpNormal", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpNormal8() const
{
	return motorCzasMiedzyImpNormal8;
}

void UstawieniaGen::setMotorCzasMiedzyImpNormal8(const int & value)
{
	motorCzasMiedzyImpNormal8 = value;
	settings.setValue("Silnik-8/CzasMiedzyImpNormal", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpNormal9() const
{
	return motorCzasMiedzyImpNormal9;
}

void UstawieniaGen::setMotorCzasMiedzyImpNormal9(const int & value)
{
	motorCzasMiedzyImpNormal9 = value;
	settings.setValue("Silnik-9/CzasMiedzyImpNormal", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorCzasMiedzyImpNormal(const short & nrMotor) const
{
	switch(nrMotor) {
	case 1 : return getMotorCzasMiedzyImpNormal1();
	case 2 : return getMotorCzasMiedzyImpNormal2();
	case 3 : return getMotorCzasMiedzyImpNormal3();
	case 4 : return getMotorCzasMiedzyImpNormal4();
	case 5 : return getMotorCzasMiedzyImpNormal5();
	case 6 : return getMotorCzasMiedzyImpNormal6();
	case 7 : return getMotorCzasMiedzyImpNormal7();
	case 8 : return getMotorCzasMiedzyImpNormal8();
	case 9 : return getMotorCzasMiedzyImpNormal9();
	default: return toUInt(QString());
	}

}

void UstawieniaGen::setMotorCzasMiedzyImpNormal(const short & nrMotor, const int & czasMiedzyImpNormal)
{
	switch(nrMotor) {
	case 1 : setMotorCzasMiedzyImpNormal1(czasMiedzyImpNormal); break;
	case 2 : setMotorCzasMiedzyImpNormal2(czasMiedzyImpNormal); break;
	case 3 : setMotorCzasMiedzyImpNormal3(czasMiedzyImpNormal); break;
	case 4 : setMotorCzasMiedzyImpNormal4(czasMiedzyImpNormal); break;
	case 5 : setMotorCzasMiedzyImpNormal5(czasMiedzyImpNormal); break;
	case 6 : setMotorCzasMiedzyImpNormal6(czasMiedzyImpNormal); break;
	case 7 : setMotorCzasMiedzyImpNormal7(czasMiedzyImpNormal); break;
	case 8 : setMotorCzasMiedzyImpNormal8(czasMiedzyImpNormal); break;
	case 9 : setMotorCzasMiedzyImpNormal9(czasMiedzyImpNormal); break;
	default: break;
	}

}

bool UstawieniaGen::checkMotorCzasMiedzyImpNormal(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    int v = val.toInt(&ok);
    if (!ok || v <= 0)
        return false;
    return ok;

}

int UstawieniaGen::getMotorMaksIloscImp1() const
{
	return motorMaksIloscImp1;
}

void UstawieniaGen::setMotorMaksIloscImp1(const int & value)
{
	motorMaksIloscImp1 = value;
	settings.setValue("Silnik-1/MaksIloscImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscImp2() const
{
	return motorMaksIloscImp2;
}

void UstawieniaGen::setMotorMaksIloscImp2(const int & value)
{
	motorMaksIloscImp2 = value;
	settings.setValue("Silnik-2/MaksIloscImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscImp3() const
{
	return motorMaksIloscImp3;
}

void UstawieniaGen::setMotorMaksIloscImp3(const int & value)
{
	motorMaksIloscImp3 = value;
	settings.setValue("Silnik-3/MaksIloscImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscImp4() const
{
	return motorMaksIloscImp4;
}

void UstawieniaGen::setMotorMaksIloscImp4(const int & value)
{
	motorMaksIloscImp4 = value;
	settings.setValue("Silnik-4/MaksIloscImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscImp5() const
{
	return motorMaksIloscImp5;
}

void UstawieniaGen::setMotorMaksIloscImp5(const int & value)
{
	motorMaksIloscImp5 = value;
	settings.setValue("Silnik-5/MaksIloscImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscImp6() const
{
	return motorMaksIloscImp6;
}

void UstawieniaGen::setMotorMaksIloscImp6(const int & value)
{
	motorMaksIloscImp6 = value;
	settings.setValue("Silnik-6/MaksIloscImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscImp7() const
{
	return motorMaksIloscImp7;
}

void UstawieniaGen::setMotorMaksIloscImp7(const int & value)
{
	motorMaksIloscImp7 = value;
	settings.setValue("Silnik-7/MaksIloscImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscImp8() const
{
	return motorMaksIloscImp8;
}

void UstawieniaGen::setMotorMaksIloscImp8(const int & value)
{
	motorMaksIloscImp8 = value;
	settings.setValue("Silnik-8/MaksIloscImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscImp9() const
{
	return motorMaksIloscImp9;
}

void UstawieniaGen::setMotorMaksIloscImp9(const int & value)
{
	motorMaksIloscImp9 = value;
	settings.setValue("Silnik-9/MaksIloscImp", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorMaksIloscImp(const short & nrMotor) const
{
	switch(nrMotor) {
	case 1 : return getMotorMaksIloscImp1();
	case 2 : return getMotorMaksIloscImp2();
	case 3 : return getMotorMaksIloscImp3();
	case 4 : return getMotorMaksIloscImp4();
	case 5 : return getMotorMaksIloscImp5();
	case 6 : return getMotorMaksIloscImp6();
	case 7 : return getMotorMaksIloscImp7();
	case 8 : return getMotorMaksIloscImp8();
	case 9 : return getMotorMaksIloscImp9();
	default: return toUInt(QString());
	}

}

void UstawieniaGen::setMotorMaksIloscImp(const short & nrMotor, const int & maksIloscImp)
{
	switch(nrMotor) {
	case 1 : setMotorMaksIloscImp1(maksIloscImp); break;
	case 2 : setMotorMaksIloscImp2(maksIloscImp); break;
	case 3 : setMotorMaksIloscImp3(maksIloscImp); break;
	case 4 : setMotorMaksIloscImp4(maksIloscImp); break;
	case 5 : setMotorMaksIloscImp5(maksIloscImp); break;
	case 6 : setMotorMaksIloscImp6(maksIloscImp); break;
	case 7 : setMotorMaksIloscImp7(maksIloscImp); break;
	case 8 : setMotorMaksIloscImp8(maksIloscImp); break;
	case 9 : setMotorMaksIloscImp9(maksIloscImp); break;
	default: break;
	}

}

bool UstawieniaGen::checkMotorMaksIloscImp(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    int v = val.toInt(&ok);
    if (!ok || v <= 0)
        return false;
    return ok;

}

int UstawieniaGen::getMotorIloscImpBaza1() const
{
	return motorIloscImpBaza1;
}

void UstawieniaGen::setMotorIloscImpBaza1(const int & value)
{
	motorIloscImpBaza1 = value;
	settings.setValue("Silnik-1/IloscImpBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpBaza2() const
{
	return motorIloscImpBaza2;
}

void UstawieniaGen::setMotorIloscImpBaza2(const int & value)
{
	motorIloscImpBaza2 = value;
	settings.setValue("Silnik-2/IloscImpBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpBaza3() const
{
	return motorIloscImpBaza3;
}

void UstawieniaGen::setMotorIloscImpBaza3(const int & value)
{
	motorIloscImpBaza3 = value;
	settings.setValue("Silnik-3/IloscImpBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpBaza4() const
{
	return motorIloscImpBaza4;
}

void UstawieniaGen::setMotorIloscImpBaza4(const int & value)
{
	motorIloscImpBaza4 = value;
	settings.setValue("Silnik-4/IloscImpBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpBaza5() const
{
	return motorIloscImpBaza5;
}

void UstawieniaGen::setMotorIloscImpBaza5(const int & value)
{
	motorIloscImpBaza5 = value;
	settings.setValue("Silnik-5/IloscImpBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpBaza6() const
{
	return motorIloscImpBaza6;
}

void UstawieniaGen::setMotorIloscImpBaza6(const int & value)
{
	motorIloscImpBaza6 = value;
	settings.setValue("Silnik-6/IloscImpBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpBaza7() const
{
	return motorIloscImpBaza7;
}

void UstawieniaGen::setMotorIloscImpBaza7(const int & value)
{
	motorIloscImpBaza7 = value;
	settings.setValue("Silnik-7/IloscImpBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpBaza8() const
{
	return motorIloscImpBaza8;
}

void UstawieniaGen::setMotorIloscImpBaza8(const int & value)
{
	motorIloscImpBaza8 = value;
	settings.setValue("Silnik-8/IloscImpBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpBaza9() const
{
	return motorIloscImpBaza9;
}

void UstawieniaGen::setMotorIloscImpBaza9(const int & value)
{
	motorIloscImpBaza9 = value;
	settings.setValue("Silnik-9/IloscImpBaza", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpBaza(const short & nrMotor) const
{
	switch(nrMotor) {
	case 1 : return getMotorIloscImpBaza1();
	case 2 : return getMotorIloscImpBaza2();
	case 3 : return getMotorIloscImpBaza3();
	case 4 : return getMotorIloscImpBaza4();
	case 5 : return getMotorIloscImpBaza5();
	case 6 : return getMotorIloscImpBaza6();
	case 7 : return getMotorIloscImpBaza7();
	case 8 : return getMotorIloscImpBaza8();
	case 9 : return getMotorIloscImpBaza9();
	default: return toUInt(QString());
	}

}

void UstawieniaGen::setMotorIloscImpBaza(const short & nrMotor, const int & iloscImpBaza)
{
	switch(nrMotor) {
	case 1 : setMotorIloscImpBaza1(iloscImpBaza); break;
	case 2 : setMotorIloscImpBaza2(iloscImpBaza); break;
	case 3 : setMotorIloscImpBaza3(iloscImpBaza); break;
	case 4 : setMotorIloscImpBaza4(iloscImpBaza); break;
	case 5 : setMotorIloscImpBaza5(iloscImpBaza); break;
	case 6 : setMotorIloscImpBaza6(iloscImpBaza); break;
	case 7 : setMotorIloscImpBaza7(iloscImpBaza); break;
	case 8 : setMotorIloscImpBaza8(iloscImpBaza); break;
	case 9 : setMotorIloscImpBaza9(iloscImpBaza); break;
	default: break;
	}

}

bool UstawieniaGen::checkMotorIloscImpBaza(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    int v = val.toInt(&ok);
    if (!ok || v <= 0)
        return false;
    return ok;

}

int UstawieniaGen::getMotorIloscImpSrodek1() const
{
	return motorIloscImpSrodek1;
}

void UstawieniaGen::setMotorIloscImpSrodek1(const int & value)
{
	motorIloscImpSrodek1 = value;
	settings.setValue("Silnik-1/IloscImpSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpSrodek2() const
{
	return motorIloscImpSrodek2;
}

void UstawieniaGen::setMotorIloscImpSrodek2(const int & value)
{
	motorIloscImpSrodek2 = value;
	settings.setValue("Silnik-2/IloscImpSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpSrodek3() const
{
	return motorIloscImpSrodek3;
}

void UstawieniaGen::setMotorIloscImpSrodek3(const int & value)
{
	motorIloscImpSrodek3 = value;
	settings.setValue("Silnik-3/IloscImpSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpSrodek4() const
{
	return motorIloscImpSrodek4;
}

void UstawieniaGen::setMotorIloscImpSrodek4(const int & value)
{
	motorIloscImpSrodek4 = value;
	settings.setValue("Silnik-4/IloscImpSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpSrodek5() const
{
	return motorIloscImpSrodek5;
}

void UstawieniaGen::setMotorIloscImpSrodek5(const int & value)
{
	motorIloscImpSrodek5 = value;
	settings.setValue("Silnik-5/IloscImpSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpSrodek6() const
{
	return motorIloscImpSrodek6;
}

void UstawieniaGen::setMotorIloscImpSrodek6(const int & value)
{
	motorIloscImpSrodek6 = value;
	settings.setValue("Silnik-6/IloscImpSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpSrodek7() const
{
	return motorIloscImpSrodek7;
}

void UstawieniaGen::setMotorIloscImpSrodek7(const int & value)
{
	motorIloscImpSrodek7 = value;
	settings.setValue("Silnik-7/IloscImpSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpSrodek8() const
{
	return motorIloscImpSrodek8;
}

void UstawieniaGen::setMotorIloscImpSrodek8(const int & value)
{
	motorIloscImpSrodek8 = value;
	settings.setValue("Silnik-8/IloscImpSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpSrodek9() const
{
	return motorIloscImpSrodek9;
}

void UstawieniaGen::setMotorIloscImpSrodek9(const int & value)
{
	motorIloscImpSrodek9 = value;
	settings.setValue("Silnik-9/IloscImpSrodek", QVariant::fromValue(value));
}

int UstawieniaGen::getMotorIloscImpSrodek(const short & nrMotor) const
{
	switch(nrMotor) {
	case 1 : return getMotorIloscImpSrodek1();
	case 2 : return getMotorIloscImpSrodek2();
	case 3 : return getMotorIloscImpSrodek3();
	case 4 : return getMotorIloscImpSrodek4();
	case 5 : return getMotorIloscImpSrodek5();
	case 6 : return getMotorIloscImpSrodek6();
	case 7 : return getMotorIloscImpSrodek7();
	case 8 : return getMotorIloscImpSrodek8();
	case 9 : return getMotorIloscImpSrodek9();
	default: return toUInt(QString());
	}

}

void UstawieniaGen::setMotorIloscImpSrodek(const short & nrMotor, const int & iloscImpSrodek)
{
	switch(nrMotor) {
	case 1 : setMotorIloscImpSrodek1(iloscImpSrodek); break;
	case 2 : setMotorIloscImpSrodek2(iloscImpSrodek); break;
	case 3 : setMotorIloscImpSrodek3(iloscImpSrodek); break;
	case 4 : setMotorIloscImpSrodek4(iloscImpSrodek); break;
	case 5 : setMotorIloscImpSrodek5(iloscImpSrodek); break;
	case 6 : setMotorIloscImpSrodek6(iloscImpSrodek); break;
	case 7 : setMotorIloscImpSrodek7(iloscImpSrodek); break;
	case 8 : setMotorIloscImpSrodek8(iloscImpSrodek); break;
	case 9 : setMotorIloscImpSrodek9(iloscImpSrodek); break;
	default: break;
	}

}

bool UstawieniaGen::checkMotorIloscImpSrodek(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    int v = val.toInt(&ok);
    if (!ok || v <= 0)
        return false;
    return ok;

}

double UstawieniaGen::getFiltr_db_880_A_0() const
{
	return filtr_db_880_A_0;
}

void UstawieniaGen::setFiltr_db_880_A_0(const double & value)
{
	filtr_db_880_A_0 = value;
	settings.setValue("Filtr_db_880_A/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_B_0() const
{
	return filtr_db_880_B_0;
}

void UstawieniaGen::setFiltr_db_880_B_0(const double & value)
{
	filtr_db_880_B_0 = value;
	settings.setValue("Filtr_db_880_B/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_C_0() const
{
	return filtr_db_880_C_0;
}

void UstawieniaGen::setFiltr_db_880_C_0(const double & value)
{
	filtr_db_880_C_0 = value;
	settings.setValue("Filtr_db_880_C/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_A_1() const
{
	return filtr_db_880_A_1;
}

void UstawieniaGen::setFiltr_db_880_A_1(const double & value)
{
	filtr_db_880_A_1 = value;
	settings.setValue("Filtr_db_880_A/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_B_1() const
{
	return filtr_db_880_B_1;
}

void UstawieniaGen::setFiltr_db_880_B_1(const double & value)
{
	filtr_db_880_B_1 = value;
	settings.setValue("Filtr_db_880_B/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_C_1() const
{
	return filtr_db_880_C_1;
}

void UstawieniaGen::setFiltr_db_880_C_1(const double & value)
{
	filtr_db_880_C_1 = value;
	settings.setValue("Filtr_db_880_C/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_A_2() const
{
	return filtr_db_880_A_2;
}

void UstawieniaGen::setFiltr_db_880_A_2(const double & value)
{
	filtr_db_880_A_2 = value;
	settings.setValue("Filtr_db_880_A/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_B_2() const
{
	return filtr_db_880_B_2;
}

void UstawieniaGen::setFiltr_db_880_B_2(const double & value)
{
	filtr_db_880_B_2 = value;
	settings.setValue("Filtr_db_880_B/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_C_2() const
{
	return filtr_db_880_C_2;
}

void UstawieniaGen::setFiltr_db_880_C_2(const double & value)
{
	filtr_db_880_C_2 = value;
	settings.setValue("Filtr_db_880_C/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_A_3() const
{
	return filtr_db_880_A_3;
}

void UstawieniaGen::setFiltr_db_880_A_3(const double & value)
{
	filtr_db_880_A_3 = value;
	settings.setValue("Filtr_db_880_A/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_B_3() const
{
	return filtr_db_880_B_3;
}

void UstawieniaGen::setFiltr_db_880_B_3(const double & value)
{
	filtr_db_880_B_3 = value;
	settings.setValue("Filtr_db_880_B/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_C_3() const
{
	return filtr_db_880_C_3;
}

void UstawieniaGen::setFiltr_db_880_C_3(const double & value)
{
	filtr_db_880_C_3 = value;
	settings.setValue("Filtr_db_880_C/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_A_4() const
{
	return filtr_db_880_A_4;
}

void UstawieniaGen::setFiltr_db_880_A_4(const double & value)
{
	filtr_db_880_A_4 = value;
	settings.setValue("Filtr_db_880_A/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_B_4() const
{
	return filtr_db_880_B_4;
}

void UstawieniaGen::setFiltr_db_880_B_4(const double & value)
{
	filtr_db_880_B_4 = value;
	settings.setValue("Filtr_db_880_B/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_C_4() const
{
	return filtr_db_880_C_4;
}

void UstawieniaGen::setFiltr_db_880_C_4(const double & value)
{
	filtr_db_880_C_4 = value;
	settings.setValue("Filtr_db_880_C/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_A_5() const
{
	return filtr_db_880_A_5;
}

void UstawieniaGen::setFiltr_db_880_A_5(const double & value)
{
	filtr_db_880_A_5 = value;
	settings.setValue("Filtr_db_880_A/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_B_5() const
{
	return filtr_db_880_B_5;
}

void UstawieniaGen::setFiltr_db_880_B_5(const double & value)
{
	filtr_db_880_B_5 = value;
	settings.setValue("Filtr_db_880_B/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_880_C_5() const
{
	return filtr_db_880_C_5;
}

void UstawieniaGen::setFiltr_db_880_C_5(const double & value)
{
	filtr_db_880_C_5 = value;
	settings.setValue("Filtr_db_880_C/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_A_0() const
{
	return filtr_db_655_A_0;
}

void UstawieniaGen::setFiltr_db_655_A_0(const double & value)
{
	filtr_db_655_A_0 = value;
	settings.setValue("Filtr_db_655_A/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_B_0() const
{
	return filtr_db_655_B_0;
}

void UstawieniaGen::setFiltr_db_655_B_0(const double & value)
{
	filtr_db_655_B_0 = value;
	settings.setValue("Filtr_db_655_B/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_C_0() const
{
	return filtr_db_655_C_0;
}

void UstawieniaGen::setFiltr_db_655_C_0(const double & value)
{
	filtr_db_655_C_0 = value;
	settings.setValue("Filtr_db_655_C/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_A_1() const
{
	return filtr_db_655_A_1;
}

void UstawieniaGen::setFiltr_db_655_A_1(const double & value)
{
	filtr_db_655_A_1 = value;
	settings.setValue("Filtr_db_655_A/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_B_1() const
{
	return filtr_db_655_B_1;
}

void UstawieniaGen::setFiltr_db_655_B_1(const double & value)
{
	filtr_db_655_B_1 = value;
	settings.setValue("Filtr_db_655_B/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_C_1() const
{
	return filtr_db_655_C_1;
}

void UstawieniaGen::setFiltr_db_655_C_1(const double & value)
{
	filtr_db_655_C_1 = value;
	settings.setValue("Filtr_db_655_C/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_A_2() const
{
	return filtr_db_655_A_2;
}

void UstawieniaGen::setFiltr_db_655_A_2(const double & value)
{
	filtr_db_655_A_2 = value;
	settings.setValue("Filtr_db_655_A/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_B_2() const
{
	return filtr_db_655_B_2;
}

void UstawieniaGen::setFiltr_db_655_B_2(const double & value)
{
	filtr_db_655_B_2 = value;
	settings.setValue("Filtr_db_655_B/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_C_2() const
{
	return filtr_db_655_C_2;
}

void UstawieniaGen::setFiltr_db_655_C_2(const double & value)
{
	filtr_db_655_C_2 = value;
	settings.setValue("Filtr_db_655_C/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_A_3() const
{
	return filtr_db_655_A_3;
}

void UstawieniaGen::setFiltr_db_655_A_3(const double & value)
{
	filtr_db_655_A_3 = value;
	settings.setValue("Filtr_db_655_A/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_B_3() const
{
	return filtr_db_655_B_3;
}

void UstawieniaGen::setFiltr_db_655_B_3(const double & value)
{
	filtr_db_655_B_3 = value;
	settings.setValue("Filtr_db_655_B/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_C_3() const
{
	return filtr_db_655_C_3;
}

void UstawieniaGen::setFiltr_db_655_C_3(const double & value)
{
	filtr_db_655_C_3 = value;
	settings.setValue("Filtr_db_655_C/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_A_4() const
{
	return filtr_db_655_A_4;
}

void UstawieniaGen::setFiltr_db_655_A_4(const double & value)
{
	filtr_db_655_A_4 = value;
	settings.setValue("Filtr_db_655_A/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_B_4() const
{
	return filtr_db_655_B_4;
}

void UstawieniaGen::setFiltr_db_655_B_4(const double & value)
{
	filtr_db_655_B_4 = value;
	settings.setValue("Filtr_db_655_B/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_C_4() const
{
	return filtr_db_655_C_4;
}

void UstawieniaGen::setFiltr_db_655_C_4(const double & value)
{
	filtr_db_655_C_4 = value;
	settings.setValue("Filtr_db_655_C/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_A_5() const
{
	return filtr_db_655_A_5;
}

void UstawieniaGen::setFiltr_db_655_A_5(const double & value)
{
	filtr_db_655_A_5 = value;
	settings.setValue("Filtr_db_655_A/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_B_5() const
{
	return filtr_db_655_B_5;
}

void UstawieniaGen::setFiltr_db_655_B_5(const double & value)
{
	filtr_db_655_B_5 = value;
	settings.setValue("Filtr_db_655_B/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_db_655_C_5() const
{
	return filtr_db_655_C_5;
}

void UstawieniaGen::setFiltr_db_655_C_5(const double & value)
{
	filtr_db_655_C_5 = value;
	settings.setValue("Filtr_db_655_C/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_A_0() const
{
	return filtr_prc_880_A_0;
}

void UstawieniaGen::setFiltr_prc_880_A_0(const double & value)
{
	filtr_prc_880_A_0 = value;
	settings.setValue("Filtr_percent_880_A/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_B_0() const
{
	return filtr_prc_880_B_0;
}

void UstawieniaGen::setFiltr_prc_880_B_0(const double & value)
{
	filtr_prc_880_B_0 = value;
	settings.setValue("Filtr_percent_880_B/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_C_0() const
{
	return filtr_prc_880_C_0;
}

void UstawieniaGen::setFiltr_prc_880_C_0(const double & value)
{
	filtr_prc_880_C_0 = value;
	settings.setValue("Filtr_percent_880_C/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_A_1() const
{
	return filtr_prc_880_A_1;
}

void UstawieniaGen::setFiltr_prc_880_A_1(const double & value)
{
	filtr_prc_880_A_1 = value;
	settings.setValue("Filtr_percent_880_A/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_B_1() const
{
	return filtr_prc_880_B_1;
}

void UstawieniaGen::setFiltr_prc_880_B_1(const double & value)
{
	filtr_prc_880_B_1 = value;
	settings.setValue("Filtr_percent_880_B/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_C_1() const
{
	return filtr_prc_880_C_1;
}

void UstawieniaGen::setFiltr_prc_880_C_1(const double & value)
{
	filtr_prc_880_C_1 = value;
	settings.setValue("Filtr_percent_880_C/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_A_2() const
{
	return filtr_prc_880_A_2;
}

void UstawieniaGen::setFiltr_prc_880_A_2(const double & value)
{
	filtr_prc_880_A_2 = value;
	settings.setValue("Filtr_percent_880_A/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_B_2() const
{
	return filtr_prc_880_B_2;
}

void UstawieniaGen::setFiltr_prc_880_B_2(const double & value)
{
	filtr_prc_880_B_2 = value;
	settings.setValue("Filtr_percent_880_B/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_C_2() const
{
	return filtr_prc_880_C_2;
}

void UstawieniaGen::setFiltr_prc_880_C_2(const double & value)
{
	filtr_prc_880_C_2 = value;
	settings.setValue("Filtr_percent_880_C/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_A_3() const
{
	return filtr_prc_880_A_3;
}

void UstawieniaGen::setFiltr_prc_880_A_3(const double & value)
{
	filtr_prc_880_A_3 = value;
	settings.setValue("Filtr_percent_880_A/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_B_3() const
{
	return filtr_prc_880_B_3;
}

void UstawieniaGen::setFiltr_prc_880_B_3(const double & value)
{
	filtr_prc_880_B_3 = value;
	settings.setValue("Filtr_percent_880_B/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_C_3() const
{
	return filtr_prc_880_C_3;
}

void UstawieniaGen::setFiltr_prc_880_C_3(const double & value)
{
	filtr_prc_880_C_3 = value;
	settings.setValue("Filtr_percent_880_C/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_A_4() const
{
	return filtr_prc_880_A_4;
}

void UstawieniaGen::setFiltr_prc_880_A_4(const double & value)
{
	filtr_prc_880_A_4 = value;
	settings.setValue("Filtr_percent_880_A/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_B_4() const
{
	return filtr_prc_880_B_4;
}

void UstawieniaGen::setFiltr_prc_880_B_4(const double & value)
{
	filtr_prc_880_B_4 = value;
	settings.setValue("Filtr_percent_880_B/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_C_4() const
{
	return filtr_prc_880_C_4;
}

void UstawieniaGen::setFiltr_prc_880_C_4(const double & value)
{
	filtr_prc_880_C_4 = value;
	settings.setValue("Filtr_percent_880_C/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_A_5() const
{
	return filtr_prc_880_A_5;
}

void UstawieniaGen::setFiltr_prc_880_A_5(const double & value)
{
	filtr_prc_880_A_5 = value;
	settings.setValue("Filtr_percent_880_A/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_B_5() const
{
	return filtr_prc_880_B_5;
}

void UstawieniaGen::setFiltr_prc_880_B_5(const double & value)
{
	filtr_prc_880_B_5 = value;
	settings.setValue("Filtr_percent_880_B/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_880_C_5() const
{
	return filtr_prc_880_C_5;
}

void UstawieniaGen::setFiltr_prc_880_C_5(const double & value)
{
	filtr_prc_880_C_5 = value;
	settings.setValue("Filtr_percent_880_C/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_A_0() const
{
	return filtr_prc_655_A_0;
}

void UstawieniaGen::setFiltr_prc_655_A_0(const double & value)
{
	filtr_prc_655_A_0 = value;
	settings.setValue("Filtr_percent_655_A/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_B_0() const
{
	return filtr_prc_655_B_0;
}

void UstawieniaGen::setFiltr_prc_655_B_0(const double & value)
{
	filtr_prc_655_B_0 = value;
	settings.setValue("Filtr_percent_655_B/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_C_0() const
{
	return filtr_prc_655_C_0;
}

void UstawieniaGen::setFiltr_prc_655_C_0(const double & value)
{
	filtr_prc_655_C_0 = value;
	settings.setValue("Filtr_percent_655_C/Pos0", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_A_1() const
{
	return filtr_prc_655_A_1;
}

void UstawieniaGen::setFiltr_prc_655_A_1(const double & value)
{
	filtr_prc_655_A_1 = value;
	settings.setValue("Filtr_percent_655_A/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_B_1() const
{
	return filtr_prc_655_B_1;
}

void UstawieniaGen::setFiltr_prc_655_B_1(const double & value)
{
	filtr_prc_655_B_1 = value;
	settings.setValue("Filtr_percent_655_B/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_C_1() const
{
	return filtr_prc_655_C_1;
}

void UstawieniaGen::setFiltr_prc_655_C_1(const double & value)
{
	filtr_prc_655_C_1 = value;
	settings.setValue("Filtr_percent_655_C/Pos1", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_A_2() const
{
	return filtr_prc_655_A_2;
}

void UstawieniaGen::setFiltr_prc_655_A_2(const double & value)
{
	filtr_prc_655_A_2 = value;
	settings.setValue("Filtr_percent_655_A/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_B_2() const
{
	return filtr_prc_655_B_2;
}

void UstawieniaGen::setFiltr_prc_655_B_2(const double & value)
{
	filtr_prc_655_B_2 = value;
	settings.setValue("Filtr_percent_655_B/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_C_2() const
{
	return filtr_prc_655_C_2;
}

void UstawieniaGen::setFiltr_prc_655_C_2(const double & value)
{
	filtr_prc_655_C_2 = value;
	settings.setValue("Filtr_percent_655_C/Pos2", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_A_3() const
{
	return filtr_prc_655_A_3;
}

void UstawieniaGen::setFiltr_prc_655_A_3(const double & value)
{
	filtr_prc_655_A_3 = value;
	settings.setValue("Filtr_percent_655_A/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_B_3() const
{
	return filtr_prc_655_B_3;
}

void UstawieniaGen::setFiltr_prc_655_B_3(const double & value)
{
	filtr_prc_655_B_3 = value;
	settings.setValue("Filtr_percent_655_B/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_C_3() const
{
	return filtr_prc_655_C_3;
}

void UstawieniaGen::setFiltr_prc_655_C_3(const double & value)
{
	filtr_prc_655_C_3 = value;
	settings.setValue("Filtr_percent_655_C/Pos3", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_A_4() const
{
	return filtr_prc_655_A_4;
}

void UstawieniaGen::setFiltr_prc_655_A_4(const double & value)
{
	filtr_prc_655_A_4 = value;
	settings.setValue("Filtr_percent_655_A/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_B_4() const
{
	return filtr_prc_655_B_4;
}

void UstawieniaGen::setFiltr_prc_655_B_4(const double & value)
{
	filtr_prc_655_B_4 = value;
	settings.setValue("Filtr_percent_655_B/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_C_4() const
{
	return filtr_prc_655_C_4;
}

void UstawieniaGen::setFiltr_prc_655_C_4(const double & value)
{
	filtr_prc_655_C_4 = value;
	settings.setValue("Filtr_percent_655_C/Pos4", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_A_5() const
{
	return filtr_prc_655_A_5;
}

void UstawieniaGen::setFiltr_prc_655_A_5(const double & value)
{
	filtr_prc_655_A_5 = value;
	settings.setValue("Filtr_percent_655_A/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_B_5() const
{
	return filtr_prc_655_B_5;
}

void UstawieniaGen::setFiltr_prc_655_B_5(const double & value)
{
	filtr_prc_655_B_5 = value;
	settings.setValue("Filtr_percent_655_B/Pos5", QVariant::fromValue(value));
}

double UstawieniaGen::getFiltr_prc_655_C_5() const
{
	return filtr_prc_655_C_5;
}

void UstawieniaGen::setFiltr_prc_655_C_5(const double & value)
{
	filtr_prc_655_C_5 = value;
	settings.setValue("Filtr_percent_655_C/Pos5", QVariant::fromValue(value));
}

bool UstawieniaGen::checkFilterdbValue(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    double v = val.toDouble(&ok);
    if (!ok || v < 0.0)
        return false;
    return true;

}

double UstawieniaGen::getFiltr_db(const int & fala, const char & nrTarczy, const short & nrPos) const
{
	if (false) ;
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 0) return getFiltr_db_880_A_0();
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 1) return getFiltr_db_880_A_1();
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 2) return getFiltr_db_880_A_2();
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 3) return getFiltr_db_880_A_3();
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 4) return getFiltr_db_880_A_4();
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 5) return getFiltr_db_880_A_5();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 0) return getFiltr_db_880_B_0();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 1) return getFiltr_db_880_B_1();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 2) return getFiltr_db_880_B_2();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 3) return getFiltr_db_880_B_3();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 4) return getFiltr_db_880_B_4();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 5) return getFiltr_db_880_B_5();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 0) return getFiltr_db_880_C_0();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 1) return getFiltr_db_880_C_1();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 2) return getFiltr_db_880_C_2();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 3) return getFiltr_db_880_C_3();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 4) return getFiltr_db_880_C_4();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 5) return getFiltr_db_880_C_5();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 0) return getFiltr_db_655_A_0();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 1) return getFiltr_db_655_A_1();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 2) return getFiltr_db_655_A_2();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 3) return getFiltr_db_655_A_3();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 4) return getFiltr_db_655_A_4();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 5) return getFiltr_db_655_A_5();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 0) return getFiltr_db_655_B_0();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 1) return getFiltr_db_655_B_1();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 2) return getFiltr_db_655_B_2();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 3) return getFiltr_db_655_B_3();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 4) return getFiltr_db_655_B_4();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 5) return getFiltr_db_655_B_5();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 0) return getFiltr_db_655_C_0();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 1) return getFiltr_db_655_C_1();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 2) return getFiltr_db_655_C_2();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 3) return getFiltr_db_655_C_3();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 4) return getFiltr_db_655_C_4();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 5) return getFiltr_db_655_C_5();
	else Q_ASSERT(true);
	 return 0.0;
}

void UstawieniaGen::setFiltr_db(const int & fala, const char & nrTarczy, const short & nrPos, const double & val)
{
	if (false) ;
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 0) return setFiltr_db_880_A_0(val);
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 1) return setFiltr_db_880_A_1(val);
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 2) return setFiltr_db_880_A_2(val);
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 3) return setFiltr_db_880_A_3(val);
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 4) return setFiltr_db_880_A_4(val);
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 5) return setFiltr_db_880_A_5(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 0) return setFiltr_db_880_B_0(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 1) return setFiltr_db_880_B_1(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 2) return setFiltr_db_880_B_2(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 3) return setFiltr_db_880_B_3(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 4) return setFiltr_db_880_B_4(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 5) return setFiltr_db_880_B_5(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 0) return setFiltr_db_880_C_0(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 1) return setFiltr_db_880_C_1(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 2) return setFiltr_db_880_C_2(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 3) return setFiltr_db_880_C_3(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 4) return setFiltr_db_880_C_4(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 5) return setFiltr_db_880_C_5(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 0) return setFiltr_db_655_A_0(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 1) return setFiltr_db_655_A_1(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 2) return setFiltr_db_655_A_2(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 3) return setFiltr_db_655_A_3(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 4) return setFiltr_db_655_A_4(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 5) return setFiltr_db_655_A_5(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 0) return setFiltr_db_655_B_0(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 1) return setFiltr_db_655_B_1(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 2) return setFiltr_db_655_B_2(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 3) return setFiltr_db_655_B_3(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 4) return setFiltr_db_655_B_4(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 5) return setFiltr_db_655_B_5(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 0) return setFiltr_db_655_C_0(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 1) return setFiltr_db_655_C_1(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 2) return setFiltr_db_655_C_2(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 3) return setFiltr_db_655_C_3(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 4) return setFiltr_db_655_C_4(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 5) return setFiltr_db_655_C_5(val);
	else Q_ASSERT(true);
}

bool UstawieniaGen::checkFilterprcValue(const QString & val)
{

    if (val.isEmpty()) return false;
    
    bool ok;
    double v = val.toDouble(&ok);
    if (!ok || v < 0.0)
        return false;
    return true;

}

double UstawieniaGen::getFiltr_prc(const int & fala, const char & nrTarczy, const short & nrPos) const
{
	if (false) ;
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 0) return getFiltr_prc_880_A_0();
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 1) return getFiltr_prc_880_A_1();
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 2) return getFiltr_prc_880_A_2();
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 3) return getFiltr_prc_880_A_3();
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 4) return getFiltr_prc_880_A_4();
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 5) return getFiltr_prc_880_A_5();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 0) return getFiltr_prc_880_B_0();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 1) return getFiltr_prc_880_B_1();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 2) return getFiltr_prc_880_B_2();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 3) return getFiltr_prc_880_B_3();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 4) return getFiltr_prc_880_B_4();
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 5) return getFiltr_prc_880_B_5();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 0) return getFiltr_prc_880_C_0();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 1) return getFiltr_prc_880_C_1();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 2) return getFiltr_prc_880_C_2();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 3) return getFiltr_prc_880_C_3();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 4) return getFiltr_prc_880_C_4();
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 5) return getFiltr_prc_880_C_5();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 0) return getFiltr_prc_655_A_0();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 1) return getFiltr_prc_655_A_1();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 2) return getFiltr_prc_655_A_2();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 3) return getFiltr_prc_655_A_3();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 4) return getFiltr_prc_655_A_4();
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 5) return getFiltr_prc_655_A_5();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 0) return getFiltr_prc_655_B_0();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 1) return getFiltr_prc_655_B_1();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 2) return getFiltr_prc_655_B_2();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 3) return getFiltr_prc_655_B_3();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 4) return getFiltr_prc_655_B_4();
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 5) return getFiltr_prc_655_B_5();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 0) return getFiltr_prc_655_C_0();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 1) return getFiltr_prc_655_C_1();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 2) return getFiltr_prc_655_C_2();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 3) return getFiltr_prc_655_C_3();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 4) return getFiltr_prc_655_C_4();
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 5) return getFiltr_prc_655_C_5();
	else Q_ASSERT(true);
	 return 0.0;
}

void UstawieniaGen::setFiltr_prc(const int & fala, const char & nrTarczy, const short & nrPos, const double & val)
{
	if (false) ;
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 0) return setFiltr_prc_880_A_0(val);
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 1) return setFiltr_prc_880_A_1(val);
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 2) return setFiltr_prc_880_A_2(val);
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 3) return setFiltr_prc_880_A_3(val);
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 4) return setFiltr_prc_880_A_4(val);
	else if (fala == 880 && nrTarczy == 'A' && nrPos == 5) return setFiltr_prc_880_A_5(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 0) return setFiltr_prc_880_B_0(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 1) return setFiltr_prc_880_B_1(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 2) return setFiltr_prc_880_B_2(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 3) return setFiltr_prc_880_B_3(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 4) return setFiltr_prc_880_B_4(val);
	else if (fala == 880 && nrTarczy == 'B' && nrPos == 5) return setFiltr_prc_880_B_5(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 0) return setFiltr_prc_880_C_0(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 1) return setFiltr_prc_880_C_1(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 2) return setFiltr_prc_880_C_2(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 3) return setFiltr_prc_880_C_3(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 4) return setFiltr_prc_880_C_4(val);
	else if (fala == 880 && nrTarczy == 'C' && nrPos == 5) return setFiltr_prc_880_C_5(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 0) return setFiltr_prc_655_A_0(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 1) return setFiltr_prc_655_A_1(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 2) return setFiltr_prc_655_A_2(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 3) return setFiltr_prc_655_A_3(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 4) return setFiltr_prc_655_A_4(val);
	else if (fala == 655 && nrTarczy == 'A' && nrPos == 5) return setFiltr_prc_655_A_5(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 0) return setFiltr_prc_655_B_0(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 1) return setFiltr_prc_655_B_1(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 2) return setFiltr_prc_655_B_2(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 3) return setFiltr_prc_655_B_3(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 4) return setFiltr_prc_655_B_4(val);
	else if (fala == 655 && nrTarczy == 'B' && nrPos == 5) return setFiltr_prc_655_B_5(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 0) return setFiltr_prc_655_C_0(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 1) return setFiltr_prc_655_C_1(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 2) return setFiltr_prc_655_C_2(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 3) return setFiltr_prc_655_C_3(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 4) return setFiltr_prc_655_C_4(val);
	else if (fala == 655 && nrTarczy == 'C' && nrPos == 5) return setFiltr_prc_655_C_5(val);
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

QString UstawieniaGen::getSerialDeviceSterownikSerial() const
{
	return serialDeviceSterownikSerial;
}

void UstawieniaGen::setSerialDeviceSterownikSerial(const QString & value)
{
	serialDeviceSterownikSerial = value;
	settings.setValue("Sterownik/Serial", QVariant::fromValue(value));
}

bool UstawieniaGen::checkSerialDeviceIdentString(const QString & val) const
{
	
    if (val.isEmpty()) return false;
    return true;

}

int UstawieniaGen::getMinNapiecieCzujki() const
{
	return minNapiecieCzujki;
}

void UstawieniaGen::setMinNapiecieCzujki(const int & value)
{
	minNapiecieCzujki = value;
	settings.setValue("ParamentryBadania-Czujka/MinimalneNapiecie", QVariant::fromValue(value));
}

int UstawieniaGen::getMaxNapiecieCzujki() const
{
	return maxNapiecieCzujki;
}

void UstawieniaGen::setMaxNapiecieCzujki(const int & value)
{
	maxNapiecieCzujki = value;
	settings.setValue("ParamentryBadania-Czujka/MaksymalneNapiecie", QVariant::fromValue(value));
}

int UstawieniaGen::getMinCzasStabCzujki() const
{
	return minCzasStabCzujki;
}

void UstawieniaGen::setMinCzasStabCzujki(const int & value)
{
	minCzasStabCzujki = value;
	settings.setValue("ParamentryBadania-Czujka/MinimalnyCzasStabilizacji", QVariant::fromValue(value));
}

int UstawieniaGen::getMaxCzasStabCzujki() const
{
	return maxCzasStabCzujki;
}

void UstawieniaGen::setMaxCzasStabCzujki(const int & value)
{
	maxCzasStabCzujki = value;
	settings.setValue("ParamentryBadania-Czujka/MaksymalnyCzasStabilizacji", QVariant::fromValue(value));
}

double UstawieniaGen::getMinPrzekrPradZasCzujki() const
{
	return minPrzekrPradZasCzujki;
}

void UstawieniaGen::setMinPrzekrPradZasCzujki(const double & value)
{
	minPrzekrPradZasCzujki = value;
	settings.setValue("ParamentryBadania-Alarm/MinimalnyPrzekroczonyPradZasilania", QVariant::fromValue(value));
}

double UstawieniaGen::getMaxPrzekrPradZasCzujki() const
{
	return maxPrzekrPradZasCzujki;
}

void UstawieniaGen::setMaxPrzekrPradZasCzujki(const double & value)
{
	maxPrzekrPradZasCzujki = value;
	settings.setValue("ParamentryBadania-Alarm/MaksymalnyPrzekroczonyPradZasilania", QVariant::fromValue(value));
}

int UstawieniaGen::getMinCzasPoZmianieFiltra() const
{
	return minCzasPoZmianieFiltra;
}

void UstawieniaGen::setMinCzasPoZmianieFiltra(const int & value)
{
	minCzasPoZmianieFiltra = value;
	settings.setValue("ParamentryBadania-Filtry/MinimalnyCzasPomiedzyZmianami", QVariant::fromValue(value));
}

int UstawieniaGen::getMaxCzasPoZmianieFiltra() const
{
	return maxCzasPoZmianieFiltra;
}

void UstawieniaGen::setMaxCzasPoZmianieFiltra(const int & value)
{
	maxCzasPoZmianieFiltra = value;
	settings.setValue("ParamentryBadania-Filtry/MaksymalnyCzasPomiedzyZmianami", QVariant::fromValue(value));
}

unsigned long UstawieniaGen::getCzasOczekiwaniaPowtarzalnosc4Test() const
{
	return czasOczekiwaniaPowtarzalnosc4Test;
}

void UstawieniaGen::setCzasOczekiwaniaPowtarzalnosc4Test(const unsigned long & value)
{
	czasOczekiwaniaPowtarzalnosc4Test = value;
	settings.setValue("ParamentryBadania-Powtarzalnosc/CzasOczekiwaniaNa4Test", QVariant::fromValue(value));
}

double UstawieniaGen::getMinimalnaWartoscCzujkiCn() const
{
	return minimalnaWartoscCzujkiCn;
}

void UstawieniaGen::setMinimalnaWartoscCzujkiCn(const double & value)
{
	minimalnaWartoscCzujkiCn = value;
	settings.setValue("ParamentryBadania-Odtwarzalnosc/MinimalnaWartoscCzujki", QVariant::fromValue(value));
}

unsigned int UstawieniaGen::getCzasStabilizacjiDlaKataNieWspolosiowosci() const
{
	return czasStabilizacjiDlaKataNieWspolosiowosci;
}

void UstawieniaGen::setCzasStabilizacjiDlaKataNieWspolosiowosci(const unsigned int & value)
{
	czasStabilizacjiDlaKataNieWspolosiowosci = value;
	settings.setValue("ParamentryBadania-NieWspolOsiowosc/CzasStabilizacjiDlaKataNieWspolosiowosci", QVariant::fromValue(value));
}

double UstawieniaGen::getWartoscTlumienieDlaKataNieWspolosiowosci() const
{
	return wartoscTlumienieDlaKataNieWspolosiowosci;
}

void UstawieniaGen::setWartoscTlumienieDlaKataNieWspolosiowosci(const double & value)
{
	wartoscTlumienieDlaKataNieWspolosiowosci = value;
	settings.setValue("ParamentryBadania-NieWspolOsiowosc/WartoscTlumienia", QVariant::fromValue(value));
}

unsigned int UstawieniaGen::getMaksCzasZadzialaniaCzujkiDlaKataNieWspolosiowosci() const
{
	return maksCzasZadzialaniaCzujkiDlaKataNieWspolosiowosci;
}

void UstawieniaGen::setMaksCzasZadzialaniaCzujkiDlaKataNieWspolosiowosci(const unsigned int & value)
{
	maksCzasZadzialaniaCzujkiDlaKataNieWspolosiowosci = value;
	settings.setValue("ParamentryBadania-NieWspolOsiowosc/CzasZadzialaniaCzujki", QVariant::fromValue(value));
}

unsigned int UstawieniaGen::getMaksCzasTestuZadzialaniaCzujkiDlaKataNieWspolosiowosci() const
{
	return maksCzasTestuZadzialaniaCzujkiDlaKataNieWspolosiowosci;
}

void UstawieniaGen::setMaksCzasTestuZadzialaniaCzujkiDlaKataNieWspolosiowosci(const unsigned int & value)
{
	maksCzasTestuZadzialaniaCzujkiDlaKataNieWspolosiowosci = value;
	settings.setValue("ParamentryBadania-NieWspolOsiowosc/CzasCalkowityTestuZadzialaniaCzujki", QVariant::fromValue(value));
}

double UstawieniaGen::getMaksKatNieWspolOsiowosci() const
{
	return maksKatNieWspolOsiowosci;
}

void UstawieniaGen::setMaksKatNieWspolOsiowosci(const double & value)
{
	maksKatNieWspolOsiowosci = value;
	settings.setValue("ParamentryBadania-Urzadzenie/MaksymalnyKat", QVariant::fromValue(value));
}

double UstawieniaGen::getOdtwarzalnoscCmaxCrep() const
{
	return odtwarzalnoscCmaxCrep;
}

void UstawieniaGen::setOdtwarzalnoscCmaxCrep(const double & value)
{
	odtwarzalnoscCmaxCrep = value;
	settings.setValue("ParamentryBadania-Odtwarzalnosc/CmaxCrep", QVariant::fromValue(value));
}

double UstawieniaGen::getOdtwarzalnoscCrepCmin() const
{
	return odtwarzalnoscCrepCmin;
}

void UstawieniaGen::setOdtwarzalnoscCrepCmin(const double & value)
{
	odtwarzalnoscCrepCmin = value;
	settings.setValue("ParamentryBadania-Odtwarzalnosc/CrepCmin", QVariant::fromValue(value));
}

double UstawieniaGen::getPowtarzalnoscCmaxCmin() const
{
	return powtarzalnoscCmaxCmin;
}

void UstawieniaGen::setPowtarzalnoscCmaxCmin(const double & value)
{
	powtarzalnoscCmaxCmin = value;
	settings.setValue("ParamentryBadania-Powtarzalnosc/CmaxCmin", QVariant::fromValue(value));
}

double UstawieniaGen::getNiewspolosiowoscWartoscTlumnika() const
{
	return niewspolosiowoscWartoscTlumnika;
}

void UstawieniaGen::setNiewspolosiowoscWartoscTlumnika(const double & value)
{
	niewspolosiowoscWartoscTlumnika = value;
	settings.setValue("ParamentryBadania-NieWspolOsiowosc/WartoscTlumnika", QVariant::fromValue(value));
}

unsigned int UstawieniaGen::getCzasWylaczeniaCzujkiDlaResetu() const
{
	return czasWylaczeniaCzujkiDlaResetu;
}

void UstawieniaGen::setCzasWylaczeniaCzujkiDlaResetu(const unsigned int & value)
{
	czasWylaczeniaCzujkiDlaResetu = value;
	settings.setValue("ParamentryBadania-Urzadzenie/CzasWylaczeniaCzujkiDlaResetu", QVariant::fromValue(value));
}

double UstawieniaGen::getNiewspolosiowoscMinimalnyKatProducentMierzony() const
{
	return niewspolosiowoscMinimalnyKatProducentMierzony;
}

void UstawieniaGen::setNiewspolosiowoscMinimalnyKatProducentMierzony(const double & value)
{
	niewspolosiowoscMinimalnyKatProducentMierzony = value;
	settings.setValue("ParamentryBadania-NieWspolOsiowosc/MinimalnaRoznicaKataMiedzyZmierzonymAZadeklarowanym", QVariant::fromValue(value));
}

unsigned int UstawieniaGen::getMaksCzasZadzialaniaCzujkidlaTlumnikaA() const
{
	return maksCzasZadzialaniaCzujkidlaTlumnikaA;
}

void UstawieniaGen::setMaksCzasZadzialaniaCzujkidlaTlumnikaA(const unsigned int & value)
{
	maksCzasZadzialaniaCzujkidlaTlumnikaA = value;
	settings.setValue("ParamentryBadania-SzybkieZmianyTlumienia/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaA", QVariant::fromValue(value));
}

unsigned int UstawieniaGen::getMaksCzasTestuCzujkidlaTlumnikaA() const
{
	return maksCzasTestuCzujkidlaTlumnikaA;
}

void UstawieniaGen::setMaksCzasTestuCzujkidlaTlumnikaA(const unsigned int & value)
{
	maksCzasTestuCzujkidlaTlumnikaA = value;
	settings.setValue("ParamentryBadania-Urzadzenie/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaA", QVariant::fromValue(value));
}

unsigned int UstawieniaGen::getMaksCzasZadzialaniaCzujkidlaTlumnikaB() const
{
	return maksCzasZadzialaniaCzujkidlaTlumnikaB;
}

void UstawieniaGen::setMaksCzasZadzialaniaCzujkidlaTlumnikaB(const unsigned int & value)
{
	maksCzasZadzialaniaCzujkidlaTlumnikaB = value;
	settings.setValue("ParamentryBadania-SzybkieZmianyTlumienia/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaB", QVariant::fromValue(value));
}

unsigned int UstawieniaGen::getMaksCzasTestuCzujkidlaTlumnikaB() const
{
	return maksCzasTestuCzujkidlaTlumnikaB;
}

void UstawieniaGen::setMaksCzasTestuCzujkidlaTlumnikaB(const unsigned int & value)
{
	maksCzasTestuCzujkidlaTlumnikaB = value;
	settings.setValue("ParamentryBadania-Urzadzenie/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaB", QVariant::fromValue(value));
}

double UstawieniaGen::getSzybkieZmianyWartoscTlumnikaA() const
{
	return szybkieZmianyWartoscTlumnikaA;
}

void UstawieniaGen::setSzybkieZmianyWartoscTlumnikaA(const double & value)
{
	szybkieZmianyWartoscTlumnikaA = value;
	settings.setValue("ParamentryBadania-SzybkieZmiany/WartoscTlumnikaA", QVariant::fromValue(value));
}

double UstawieniaGen::getSzybkieZmianyWartoscTlumnikaB() const
{
	return szybkieZmianyWartoscTlumnikaB;
}

void UstawieniaGen::setSzybkieZmianyWartoscTlumnikaB(const double & value)
{
	szybkieZmianyWartoscTlumnikaB = value;
	settings.setValue("ParamentryBadania-SzybkieZmiany/WartoscTlumnikaB", QVariant::fromValue(value));
}