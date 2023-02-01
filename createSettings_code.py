import os

class UstawieniaCode:
    def __init__(self):
        self.className = "UstawieniaGen";
        self.sourcedir = "CzujkiLinioweApp"
        self.h_filename = "ustawienia_gen.h"
        self.c_filename = "ustawienia_gen.cpp"
        self.h_HEAD_2 = "\n};\n#endif"
        self.h_public_fun = []
        self.h_protected_fun = []
        self.h_private_fun = []
        self.h_private_values = []
        self.c_load = []
        self.c_save = []
        self.c_sources = []
        self.c_check = []
        self.h_HEAD_1 = """
#ifndef USTAWIENIAGEN_H
#define USTAWIENIAGEN_H

#include <QString>
#include <QSettings>

Q_DECLARE_METATYPE(long double)

class %(className)s
{
public:
    %(className)s();
    ~%(className)s();
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
""" % {'className' : self.className}
    
        
        
        self.c_include = """
#include "%(filename_h)s"

#include <QApplication>
#include <QDir>

%(className)s::%(className)s() :
    settings(QDir(QApplication::applicationDirPath()).absoluteFilePath(QApplication::applicationName() + ".ini"), QSettings::IniFormat)
{
    //m_sSettingsFile = QApplication::applicationDirPath() + "/demosettings.ini";
    //QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    
    load();
}

%(className)s::~%(className)s()
{
    
}
        """ % {'className' : self.className,
               'filename_h' : self.h_filename}
    
    
    def cName(self, val):
        return val[0].upper() + val[1:]
    
    def createHeader(self):
        hfile = open(os.path.join(self.sourcedir, self.h_filename), "w")
        hfile.write(self.h_HEAD_1)
        hfile.write("\n".join(["\t"+f for f in self.h_public_fun]))
        hfile.write("\nprotected:\n")
        hfile.write("\n".join(["\t"+f for f in self.h_protected_fun]))
        hfile.write("\nprivate:\n")
        hfile.write("\n".join(["\t"+f for f in self.h_private_fun]))
        hfile.write("\n")
        hfile.write("\n".join(["\t"+f for f in self.h_private_values]))
        hfile.write(self.h_HEAD_2)
        hfile.close()
        
    def createSource(self):
        cfile = open(os.path.join(self.sourcedir, self.c_filename), "w")
        cfile.write(self.c_include)
        cfile.write("\n\nvoid %s::load()\n{\n" % self.className)
        cfile.write("\n".join(["\t"+f for f in self.c_load]))
        cfile.write("\n}\n\nvoid %s::save()\n{\n" % self.className)
        cfile.write("\n".join(["\t"+f for f in self.c_save]))
        cfile.write("\n}\n\n")
        #cfile.write("bool %s::check()\n{\n" % self.className)
        #cfile.write("\n".join(["\tif (!%s) return false;" % f for f in self.c_check]))
        #cfile.write("\n\treturn true;\n}\n\n")
        
        cfile.write("\n\n".join([f for f in self.c_sources]))
        cfile.close()
        
    def addMotor(self, nrMotor, nameVal, typeVal, path, convF, devVal):
        localVal = "motor%s%d" % (self.cName(nameVal), nrMotor)
        self.h_private_values.append("%s %s;" % (typeVal, localVal))
        fget = "%s getMotor%s%d() const" % (typeVal, self.cName(nameVal), nrMotor)
        fset = "void setMotor%s%d(const %s & %s%d)" % (self.cName(nameVal), nrMotor, typeVal, nameVal, nrMotor)
        fset2 = "inline void setMotor%s%d(const QString & %s%d) { setMotor%s%d(%s(%s%d)); }" % (self.cName(nameVal), nrMotor, nameVal, nrMotor, self.cName(nameVal), nrMotor, convF, nameVal, nrMotor)
        fget_c = "%s %s::getMotor%s%d() const" % (typeVal, self.className, self.cName(nameVal), nrMotor)
        fset_c = "void %s::setMotor%s%d(const %s & value)" % (self.className, self.cName(nameVal), nrMotor, typeVal)
        f_check = "checkMotor%s" % self.cName(nameVal)
        self.h_public_fun.append(fget + ";");
        self.h_public_fun.append(fset + ";");
        if typeVal != "QString":
            self.h_public_fun.append(fset2);
        
        settPath = path + "%d" % nrMotor + "/" + self.cName(nameVal)
        
        #m_ratioSilnik1 = settings.value("Ratio_1", m_ratioSilnik1).toString();
        self.c_load.append('%s = %s(settings.value("%s", QVariant::fromValue(%s)).toString());' % (localVal, convF, settPath, devVal))
        
        #settings.setValue("Ratio_1", QVariant::fromValue(m_ratioSilnik1));
        self.c_save.append('settings.setValue("%s", QVariant::fromValue(%s));' % (settPath, localVal))
        
        #checkMotor(settings.value("%s").toQString())
        self.c_check.append(f_check + '(settings.value("%s").toString())' % settPath)
        
        #QString Ustawienia::getRatioSilnik1() const { return m_ratioSilnik1; }
        self.c_sources.append("%s\n{\n\treturn %s;\n}" % (fget_c, localVal))
        
        #void Ustawienia::setRatioSilnik1(const QString & value) { m_ratioSilnik1 = value; settings.setValue("Ratio_1", QVariant::fromValue(m_ratioSilnik1)); }
        self.c_sources.append('%s\n{\n\t%s = value;\n\tsettings.setValue("%s", QVariant::fromValue(value));\n}' % (fset_c, localVal, settPath))
        
    def addSummMotor(self, nameVal, typeVal, convF, checkFuns):
        #QString Ustawienia::getRatioSilnik(uint key) const
        fget = "%s getMotor%s(const short & nrMotor) const" % (typeVal, self.cName(nameVal))
        fset = "void setMotor%s(const short & nrMotor, const %s & %s)" % (self.cName(nameVal), typeVal, nameVal)
        fget_c = "%s %s::getMotor%s(const short & nrMotor) const" % (typeVal, self.className, self.cName(nameVal))
        fset_c = "void %s::setMotor%s(const short & nrMotor, const %s & %s)" % (self.className, self.cName(nameVal), typeVal, nameVal)
        fcheck = "bool checkMotor%s(const QString & val)" % (self.cName(nameVal))
        fcheck_c = "bool %s::checkMotor%s(const QString & val)" % (self.className, self.cName(nameVal))
        self.h_public_fun.append(fget + ";");
        self.h_public_fun.append(fset + ";");
        self.h_protected_fun.append(fcheck + ";");
        
        cont = "\tswitch(nrMotor) {\n"
        for i in range(1, 10):
            cont += "\tcase %d : return getMotor%s%d();\n" % (i, self.cName(nameVal), i)
        cont += '\tdefault: return %s(QString());\n\t}\n' % convF    
        self.c_sources.append("%s\n{\n%s\n}" % (fget_c, cont))
        
        cont = "\tswitch(nrMotor) {\n"
        for i in range(1, 10):
            cont += "\tcase %d : setMotor%s%d(%s); break;\n" % (i, self.cName(nameVal), i, nameVal)
        cont += "\tdefault: break;\n\t}\n" 
        self.c_sources.append("%s\n{\n%s\n}" % (fset_c, cont))
        
        cont = checkFuns[nameVal]()
        self.c_sources.append("%s\n{\n%s\n}" % (fcheck_c, cont))
        
        
    def addFiltr(self, fala, nrTarczy, nrPozycji, path, devVal, suffix):
        localVal = "filtr_%s_%d_%c_%d" % (suffix, fala, nrTarczy, nrPozycji)
        localFunName = self.cName(localVal)
        self.h_private_values.append("double %s;" % localVal)
        
        fget = "double get%s() const" % localFunName
        fset = "void set%s(const double & val)" % localFunName
        fset2 = "inline void set%s(const QString & vals) { set%s(toDouble(vals)); }" % (localFunName, localFunName)
        fget_c = "double %s::get%s() const" % (self.className, localFunName)
        fset_c = "void %s::set%s(const double & value)" % (self.className, localFunName)
        f_check = "checkFilter%sValue" % suffix
        self.h_public_fun.append(fget + ";");
        self.h_public_fun.append(fset + ";");
        self.h_public_fun.append(fset2);

        settPath = path + "%d_%c" % (fala, nrTarczy) + "/Pos%d" % nrPozycji
        
        #m_ratioSilnik1 = settings.value("Ratio_1", m_ratioSilnik1).toString();
        self.c_load.append('%s = toDouble(settings.value("%s", QVariant::fromValue(%s)).toString());' % (localVal, settPath, devVal))
        
        #settings.setValue("Ratio_1", QVariant::fromValue(m_ratioSilnik1));
        self.c_save.append('settings.setValue("%s", QVariant::fromValue(%s));' % (settPath, localVal))
        
        #checkMotor(settings.value("%s").toQString())
        self.c_check.append(f_check + '(settings.value("%s").toString())' % settPath)
        
        #QString Ustawienia::getRatioSilnik1() const { return m_ratioSilnik1; }
        self.c_sources.append("%s\n{\n\treturn %s;\n}" % (fget_c, localVal))
        
        #void Ustawienia::setRatioSilnik1(const QString & value) { m_ratioSilnik1 = value; settings.setValue("Ratio_1", QVariant::fromValue(m_ratioSilnik1)); }
        self.c_sources.append('%s\n{\n\t%s = value;\n\tsettings.setValue("%s", QVariant::fromValue(value));\n}' % (fset_c, localVal, settPath))
        
    def addSummFiltr(self, checkFun, suffix):
        self.h_public_fun.append("double getFiltr_%s_880_A(const short & nrPos) const { return getFiltr_%s(880, 'A', nrPos); }" % (suffix, suffix)) 
        self.h_public_fun.append("double getFiltr_%s_880_B(const short & nrPos) const { return getFiltr_%s(880, 'B', nrPos); }" % (suffix, suffix)) 
        self.h_public_fun.append("double getFiltr_%s_880_C(const short & nrPos) const { return getFiltr_%s(880, 'C', nrPos); }" % (suffix, suffix)) 
        
        self.h_public_fun.append("double getFiltr_%s_655_A(const short & nrPos) const { return getFiltr_%s(655, 'A', nrPos); }" % (suffix, suffix)) 
        self.h_public_fun.append("double getFiltr_%s_655_B(const short & nrPos) const { return getFiltr_%s(655, 'B', nrPos); }" % (suffix, suffix)) 
        self.h_public_fun.append("double getFiltr_%s_655_C(const short & nrPos) const { return getFiltr_%s(655, 'C', nrPos); }" % (suffix, suffix)) 
        
        self.h_public_fun.append("double getFiltr_%s_880(const char & nrTarczy, const short & nrPos) const { return getFiltr_%s(880, nrTarczy, nrPos); }" % (suffix, suffix)) 
        self.h_public_fun.append("double getFiltr_%s_655(const char & nrTarczy, const short & nrPos) const { return getFiltr_%s(655, nrTarczy, nrPos); }" % (suffix, suffix))
        
        self.h_public_fun.append("void setFiltr_%s_880_A(const short & nrPos, const double & val) { setFiltr_%s(880, 'A', nrPos, val); }" % (suffix, suffix)) 
        self.h_public_fun.append("void setFiltr_%s_880_B(const short & nrPos, const double & val) { setFiltr_%s(880, 'B', nrPos, val); }" % (suffix, suffix))
        self.h_public_fun.append("void setFiltr_%s_880_C(const short & nrPos, const double & val) { setFiltr_%s(880, 'C', nrPos, val); }" % (suffix, suffix))
        
        self.h_public_fun.append("void setFiltr_%s_655_A(const short & nrPos, const double & val) { setFiltr_%s(655, 'A', nrPos, val); }" % (suffix, suffix))
        self.h_public_fun.append("void setFiltr_%s_655_B(const short & nrPos, const double & val) { setFiltr_%s(655, 'B', nrPos, val); }" % (suffix, suffix))
        self.h_public_fun.append("void setFiltr_%s_655_C(const short & nrPos, const double & val) { setFiltr_%s(655, 'C', nrPos, val); }" % (suffix, suffix))
        
        self.h_public_fun.append("void setFiltr_%s_880(const char & nrTarczy, const short & nrPos, const double & val) { setFiltr_%s(880, nrTarczy, nrPos, val); }" % (suffix, suffix)) 
        self.h_public_fun.append("void setFiltr_%s_655(const char & nrTarczy, const short & nrPos, const double & val) { setFiltr_%s(655, nrTarczy, nrPos, val); }" % (suffix, suffix))
        
        self.h_protected_fun.append("bool checkFilter%sValue(const QString & val);" % suffix)
        self.h_protected_fun.append("double getFiltr_%s(const int& fala, const char & nrTarczy, const short & nrPos) const;" % suffix)
        self.h_protected_fun.append("void setFiltr_%s(const int& fala, const char & nrTarczy, const short & nrPos, const double & val);" % suffix)
            
        fcheck_c = "bool %s::checkFilter%sValue(const QString & val)" % (self.className, suffix)
        cont = checkFun()
        self.c_sources.append("%s\n{\n%s\n}" % (fcheck_c, cont))
        
        cont = "\tif (false) ;"
        for d in [880, 655]:
            for c in ['A', 'B', 'C']:
                for p in [0,1,2,3,4,5]:
                    cont += "\n\telse if (fala == %d && nrTarczy == '%c' && nrPos == %d) return getFiltr_%s_%d_%c_%d();" % (d, c, p, suffix, d, c, p)
        
        cont += "\n\telse Q_ASSERT(true);\n\t return 0.0;"
        fget_c = "double %s::getFiltr_%s(const int & fala, const char & nrTarczy, const short & nrPos) const" % (self.className, suffix)
        self.c_sources.append("%s\n{\n%s\n}" % (fget_c, cont))
        
        cont = "\tif (false) ;"
        for d in [880, 655]:
            for c in ['A', 'B', 'C']:
                for p in [0,1,2,3,4,5]:
                    cont += "\n\telse if (fala == %d && nrTarczy == '%c' && nrPos == %d) return setFiltr_%s_%d_%c_%d(val);" % (d, c, p, suffix, d, c, p)
        
        cont += "\n\telse Q_ASSERT(true);"
        fset_c = "void %s::setFiltr_%s(const int & fala, const char & nrTarczy, const short & nrPos, const double & val)" % (self.className, suffix)
        self.c_sources.append("%s\n{\n%s\n}" % (fset_c, cont))

    def addDevice(self, nameVal, path, defVal):
        localVal = "serialDevice%s" % (self.cName(nameVal))
        typeVal = "QString"
        self.h_private_values.append("%s %s;" % (typeVal, localVal))
        fget = "%s getSerialDevice%s() const" % (typeVal, self.cName(nameVal))
        fset = "void setSerialDevice%s(const %s & %s)" % (self.cName(nameVal), typeVal, nameVal)
       
        fget_c = "%s %s::getSerialDevice%s() const" % (typeVal, self.className, self.cName(nameVal))
        fset_c = "void %s::setSerialDevice%s(const %s & value)" % (self.className, self.cName(nameVal), typeVal)
        f_check = "checkSerialDeviceIdentString" 

        self.h_public_fun.append(fget + ";");
        self.h_public_fun.append(fset + ";");
        
        settPath = path
        
        #m_ratioSilnik1 = settings.value("Ratio_1", m_ratioSilnik1).toString();
        self.c_load.append('%s = settings.value("%s", QVariant::fromValue(%s)).toString();' % (localVal, settPath, defVal))
        
        #settings.setValue("Ratio_1", QVariant::fromValue(m_ratioSilnik1));
        self.c_save.append('settings.setValue("%s", QVariant::fromValue(%s));' % (settPath, localVal))
        
        #checkMotor(settings.value("%s").toQString())
        self.c_check.append(f_check + '(settings.value("%s").toString())' % settPath)
        
        #QString Ustawienia::getRatioSilnik1() const { return m_ratioSilnik1; }
        self.c_sources.append("%s\n{\n\treturn %s;\n}" % (fget_c, localVal))
        
        #void Ustawienia::setRatioSilnik1(const QString & value) { m_ratioSilnik1 = value; settings.setValue("Ratio_1", QVariant::fromValue(m_ratioSilnik1)); }
        self.c_sources.append('%s\n{\n\t%s = value;\n\tsettings.setValue("%s", QVariant::fromValue(value));\n}' % (fset_c, localVal, settPath))

        
        
    def addDevideSummary(self, content):
        f_check = "checkSerialDeviceIdentString" 

        self.h_protected_fun.append("bool %s(const QString & val) const;" % f_check)
        self.c_sources.append("bool %s::%s(const QString & val) const\n{\n\t%s\n}" % (self.className, f_check, content()))            

    def addZakresy(self, nameVal, type, convVal, path, defVal, checkFun):
        localVal = nameVal
        typeVal = type
        self.h_private_values.append("%s %s;" % (typeVal, localVal))
        fget = "%s get%s() const" % (typeVal, self.cName(nameVal))
        fset = "void set%s(const %s & %s)" % (self.cName(nameVal), typeVal, nameVal)
       
        fget_c = "%s %s::get%s() const" % (typeVal, self.className, self.cName(nameVal))
        fset_c = "void %s::set%s(const %s & value)" % (self.className, self.cName(nameVal), typeVal)
        #f_check = "checkSerialDeviceIdentString" 

        self.h_public_fun.append(fget + ";");
        self.h_public_fun.append(fset + ";");
        
        settPath = path
        
        #m_ratioSilnik1 = settings.value("Ratio_1", m_ratioSilnik1).toString();
        self.c_load.append('%s = %s(settings.value("%s", QVariant::fromValue(%s)).toString());' % (localVal, convVal, settPath, defVal))
        
        #settings.setValue("Ratio_1", QVariant::fromValue(m_ratioSilnik1));
        self.c_save.append('settings.setValue("%s", QVariant::fromValue(%s));' % (settPath, localVal))
        
        #checkMotor(settings.value("%s").toQString())
        #self.c_check.append(f_check + '(settings.value("%s").toString())' % settPath)
        
        #QString Ustawienia::getRatioSilnik1() const { return m_ratioSilnik1; }
        self.c_sources.append("%s\n{\n\treturn %s;\n}" % (fget_c, localVal))
        
        #void Ustawienia::setRatioSilnik1(const QString & value) { m_ratioSilnik1 = value; settings.setValue("Ratio_1", QVariant::fromValue(m_ratioSilnik1)); }
        self.c_sources.append('%s\n{\n\t%s = value;\n\tsettings.setValue("%s", QVariant::fromValue(value));\n}' % (fset_c, localVal, settPath))



def checkQStringContent() :
    return """
    if (val.isEmpty()) return false;
    return true;
"""

def checkDoubleContent() :
    return """
    if (val.isEmpty()) return false;
    
    bool ok;
    double v = val.toDouble(&ok);
    if (!ok || v < 0.0)
        return false;
    return true;
"""

def checkBoolContent() :
    return """
    if (val.isEmpty()) return false;
    
    bool v = QVariant::fromValue(val).toBool();
    (void)v;
    return true;
"""

def checkUnsignedIntContent() :
    return """
    if (val.isEmpty()) return false;
    
    bool ok;
    int v = val.toInt(&ok);
    if (!ok || v <= 0)
        return false;
    return ok;
"""

checkFuns = {}
checkFuns["nazwa"] = checkQStringContent
checkFuns["przelozenieImpJedn"] = checkDoubleContent
checkFuns["minOdstepImp"] = checkUnsignedIntContent
checkFuns["odwrocObroty"] = checkBoolContent
checkFuns["czasMiedzyImpZerow"] = checkUnsignedIntContent
checkFuns["czasMiedzyImpNormal"] = checkUnsignedIntContent
checkFuns["maksIloscImp"] = checkUnsignedIntContent
checkFuns["iloscImpBaza"] = checkUnsignedIntContent
checkFuns["iloscImpSrodek"] = checkUnsignedIntContent
checkFuns["jednostka"] = checkUnsignedIntContent


motordefVals = {
    1 : {
        "nazwa" : 'QString("Odbiornik Lewo-Prawo")',
        "przelozenieImpJedn" : 0.028,
        "minOdstepImp" : 29,
        "odwrocObroty" : "false",
        "czasMiedzyImpZerow" : 4000,
        "czasMiedzyImpNormal" : 5600000,
        "maksIloscImp" : 1300,
        "iloscImpBaza" : 200,
        "iloscImpSrodek" : 700,
        "jednostka" : "*",
    },
    2 : {
        "nazwa" : 'QString("Odbiornik Gora-Dol")',
        "przelozenieImpJedn" : 0.028,
        "minOdstepImp" : 29,
        "odwrocObroty" : "false",
        "czasMiedzyImpZerow" : 4000,
        "czasMiedzyImpNormal" : 5600000,
        "maksIloscImp" : 1300,
        "iloscImpBaza" : 200,
        "iloscImpSrodek" : 540,
        "jednostka" : "*",
    },
    3 : {
        "nazwa" : 'QString("Filtr A")',
        "przelozenieImpJedn" : 0.023226,
        "minOdstepImp" : 29,
        "odwrocObroty" : "true",
        "czasMiedzyImpZerow" : 400,
        "czasMiedzyImpNormal" : 100,
        "maksIloscImp" : 15501,
        "iloscImpBaza" : 130,
        "iloscImpSrodek" : 0,
        "jednostka" : "*",
    },
    4 : {
        "nazwa" : 'QString("Filtr B")',
        "przelozenieImpJedn" : 0.023226,
        "minOdstepImp" : 29,
        "odwrocObroty" : "true",
        "czasMiedzyImpZerow" : 400,
        "czasMiedzyImpNormal" : 100,
        "maksIloscImp" : 15502,
        "iloscImpBaza" : 130,
        "iloscImpSrodek" : 0,
        "jednostka" : "*",
    },
    5 : {
        "nazwa" : 'QString("Filtr C")',
        "przelozenieImpJedn" : 0.023226,
        "minOdstepImp" : 29,
        "odwrocObroty" : "false",
        "czasMiedzyImpZerow" : 400,
        "czasMiedzyImpNormal" : 100,
        "maksIloscImp" : 15502,
        "iloscImpBaza" : 130,
        "iloscImpSrodek" : 0,
        "jednostka" : "*",
    },
    6 : {
        "nazwa" : 'QString("Wozek Lewo-Prawo")',
        "przelozenieImpJedn" : 0.013,
        "minOdstepImp" : 29,
        "odwrocObroty" : "false",
        "czasMiedzyImpZerow" : 1560,
        "czasMiedzyImpNormal" : 1560,
        "maksIloscImp" : 20000,
        "iloscImpBaza" : 0,
        "iloscImpSrodek" : 0,
        "jednostka" : "mm",
    },
    7 : {
        "nazwa" : 'QString("Wozek Gora-Dol")',
        "przelozenieImpJedn" : 0.000234375,
        "minOdstepImp" : 29,
        "odwrocObroty" : "true",
        "czasMiedzyImpZerow" : 150,
        "czasMiedzyImpNormal" : 150,
        "maksIloscImp" : 250000,
        "iloscImpBaza" : 0,
        "iloscImpSrodek" : 0,
        "jednostka" : "mm",
    },
    8 : {
        "nazwa" : 'QString("Odbiornik Lewo-Prawo")',
        "przelozenieImpJedn" : 0.028,
        "minOdstepImp" : 29,
        "odwrocObroty" : "false",
        "czasMiedzyImpZerow" : 4000,
        "czasMiedzyImpNormal" : 5600000,
        "maksIloscImp" : 1300,
        "iloscImpBaza" : 200,
        "iloscImpSrodek" : 640,
        "jednostka" : "*",
    },
    9 : {
        "nazwa" : 'QString("Odbiornik Gora-Dol")',
        "przelozenieImpJedn" : 0.028,
        "minOdstepImp" : 29,
        "odwrocObroty" : "true",
        "czasMiedzyImpZerow" : 4000,
        "czasMiedzyImpNormal" : 5600000,
        "maksIloscImp" : 1300,
        "iloscImpBaza" : 200,
        "iloscImpSrodek" : 540,
        "jednostka" : "*",
    },
}


defFiltrValsdb = {
    655: {
        'A': [0, 1.0, 0.57, 2.84, 0.47, 7.69],
        'B': [0, 0.56, 0.47, 0.41, 9.31, 18.36],
        'C': [0, 0.44, 2.48, 4.13, 4.41, 6.22],
    },
    880 : {
        'A': [0, 0.16, 0.23, 0.29, 0.39, 6.00],
        'B': [0, 0.22, 0.4, 0.5, 7.12, 13.46],
        'C': [0, 0.99, 1.94, 2.96, 3.95, 4.81],
    }
}



u = UstawieniaCode()
def motorSett(nazwa, typ, fun, checkFuns, devVals): 
    for i in range(1,10):
        u.addMotor(i, nazwa, typ, "Silnik-", fun, devVals[i][nazwa])
    u.addSummMotor(nazwa, typ, fun, checkFuns)

motorSett("nazwa", "QString", "toQString", checkFuns, motordefVals)
motorSett("przelozenieImpJedn", "double", "toDouble", checkFuns, motordefVals)
motorSett("minOdstepImp", "int", "toUInt", checkFuns, motordefVals)
motorSett("odwrocObroty", "bool", "toBool", checkFuns, motordefVals)
motorSett("czasMiedzyImpZerow", "int", "toUInt", checkFuns, motordefVals)
motorSett("czasMiedzyImpNormal", "int", "toUInt", checkFuns, motordefVals)
motorSett("maksIloscImp", "int", "toUInt", checkFuns, motordefVals)
motorSett("iloscImpBaza", "int", "toUInt", checkFuns, motordefVals)
motorSett("iloscImpSrodek", "int", "toUInt", checkFuns, motordefVals)

def addFiltrSett(fala, defFiltrVals, path, suffix):
    for i in range(0, 6):
        for c in ['A', 'B', 'C']:
            u.addFiltr(fala, c, i, path, defFiltrVals[fala][c][i], suffix)
            
addFiltrSett(880, defFiltrValsdb, "Filtr_db_", "db")
addFiltrSett(655, defFiltrValsdb, "Filtr_db_", "db")

u.addSummFiltr(checkDoubleContent, "db")

u.addDevice("zasilaczVendor", "Zasilacz/Vendor", 'QString("67b")')
u.addDevice("zasilaczProduct", "Zasilacz/Product", 'QString("23a3")')
u.addDevice("sterownikVendor", "Sterownik/Vendor", 'QString("403")')
u.addDevice("sterownikProduct", "Sterownik/Product", 'QString("6001")')
u.addDevice("sterownikSerial", "Sterownik/Serial", 'QString("A10KV4VUA")')
u.addDevideSummary(checkQStringContent)

u.addZakresy("minNapiecieCzujki", "int", "toUInt", "ParamentryBadania-Czujka/MinimalneNapiecie", '2.0', checkDoubleContent)
u.addZakresy("maxNapiecieCzujki", "int", "toUInt", "ParamentryBadania-Czujka/MaksymalneNapiecie", '30.0', checkDoubleContent)

u.addZakresy("minCzasStabCzujki", "int", "toUInt", "ParamentryBadania-Czujka/MinimalnyCzasStabilizacji", '10', checkUnsignedIntContent)
u.addZakresy("maxCzasStabCzujki", "int", "toUInt", "ParamentryBadania-Czujka/MaksymalnyCzasStabilizacji", '3600', checkUnsignedIntContent)

u.addZakresy("minPrzekrPradZasCzujki", "double", "toDouble", "ParamentryBadania-Alarm/MinimalnyPrzekroczonyPradZasilania", '4.0', checkDoubleContent)
u.addZakresy("maxPrzekrPradZasCzujki", "double", "toDouble", "ParamentryBadania-Alarm/MaksymalnyPrzekroczonyPradZasilania", '200.0', checkDoubleContent)

u.addZakresy("minCzasPoZmianieFiltra", "int", "toUInt", "ParamentryBadania-Filtry/MinimalnyCzasPomiedzyZmianami", '10', checkUnsignedIntContent)
u.addZakresy("maxCzasPoZmianieFiltra", "int", "toUInt", "ParamentryBadania-Filtry/MaksymalnyCzasPomiedzyZmianami", '3600', checkUnsignedIntContent)

u.addZakresy("czasWylaczeniaCzujkiDlaResetu", "unsigned int", "toUInt", "ParamentryBadania-Urzadzenie/CzasWylaczeniaCzujkiDlaResetu", '2', checkUnsignedIntContent)

#u.addZakresy("minCzasStabCzujki", "int", "toUInt", "ParamentryBadania-Filtry/MinimalnyCzasPomiedzyZmianami", '10', checkUnsignedIntContent)
#u.addZakresy("maxCzasStabCzujki", "int", "toUInt", "ParamentryBadania-Filtry/MaksymalnyCzasPomiedzyZmianami", '3600', checkUnsignedIntContent)

u.addZakresy("czasOczekiwaniaPowtarzalnosc4Test", "unsigned int", "toUInt", "ParamentryBadania-Powtarzalnosc/CzasOczekiwaniaNa4Test", '259200', checkUnsignedIntContent)
u.addZakresy("minimalnyCzasOczekiwaniaPowtarzalnosc1Test", "unsigned int", "toUInt", "ParamentryBadania-Powtarzalnosc/minimalnyCzasOczekiwania1Test", '900', checkUnsignedIntContent)
u.addZakresy("maksymalnyCzasOczekiwaniaPowtarzalnosc1Test", "unsigned int", "toUInt", "ParamentryBadania-Powtarzalnosc/maksymalnyCzasOczekiwania1Test", '3600', checkUnsignedIntContent)
u.addZakresy("minimalnaWartoscCzujkiCn", "double", "toDouble", "ParamentryBadania-Odtwarzalnosc/MinimalnaWartoscCzujki", '0.4', checkDoubleContent)
u.addZakresy("czasStabilizacjiDlaKataNieWspolosiowosci", "unsigned int", "toUInt", "ParamentryBadania-NieWspolOsiowosc/CzasStabilizacjiDlaKataNieWspolosiowosci", '120', checkUnsignedIntContent)
u.addZakresy("wartoscTlumienieDlaKataNieWspolosiowosci", "double", "toDouble", "ParamentryBadania-NieWspolOsiowosc/WartoscTlumienia", '6.0', checkDoubleContent)
u.addZakresy("maksCzasZadzialaniaCzujkiDlaKataNieWspolosiowosci", "unsigned int", "toUInt", "ParamentryBadania-NieWspolOsiowosc/CzasZadzialaniaCzujki", '30', checkUnsignedIntContent)
u.addZakresy("maksCzasTestuZadzialaniaCzujkiDlaKataNieWspolosiowosci", "unsigned int", "toUInt", "ParamentryBadania-NieWspolOsiowosc/CzasCalkowityTestuZadzialaniaCzujki", '40', checkUnsignedIntContent)
u.addZakresy("maksKatNieWspolOsiowosci", "double", "toDouble", "ParamentryBadania-Urzadzenie/MaksymalnyKat", '15', checkUnsignedIntContent)
u.addZakresy("maksRoznicaKatNieWspolOsiowosci", "double", "toDouble", "ParamentryBadania-Urzadzenie/MaksymalnyRzonacaKat", '15', checkUnsignedIntContent)
u.addZakresy("odtwarzalnoscCmaxCrep", "double", "toDouble", "ParamentryBadania-Odtwarzalnosc/CmaxCrep", '1.33', checkDoubleContent)
u.addZakresy("odtwarzalnoscCrepCmin", "double", "toDouble", "ParamentryBadania-Odtwarzalnosc/CrepCmin", '1.5', checkDoubleContent)
u.addZakresy("powtarzalnoscCmaxCmin", "double", "toDouble", "ParamentryBadania-Powtarzalnosc/CmaxCmin", '1.6', checkDoubleContent)


u.addZakresy("niewspolosiowoscMinimalnyKatProducentMierzony", "double", "toDouble", "ParamentryBadania-NieWspolOsiowosc/MinimalnaRoznicaKataMiedzyZmierzonymAZadeklarowanym", '0.4', checkDoubleContent)
u.addZakresy("maksCzasZadzialaniaCzujkidlaTlumnikaA", "unsigned int", "toUInt", "ParamentryBadania-SzybkieZmianyTlumienia/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaA", '30', checkUnsignedIntContent)
u.addZakresy("maksCzasTestuCzujkidlaTlumnikaA", "unsigned int", "toUInt", "ParamentryBadania-Urzadzenie/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaA", '40', checkUnsignedIntContent)
u.addZakresy("maksCzasZadzialaniaCzujkidlaTlumnikaB", "unsigned int", "toUInt", "ParamentryBadania-SzybkieZmianyTlumienia/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaB", '60', checkUnsignedIntContent)
u.addZakresy("maksCzasTestuCzujkidlaTlumnikaB", "unsigned int", "toUInt", "ParamentryBadania-Urzadzenie/MaksymalnyCzasZadzialaniaCzujkidlaTlumnikaB", '70', checkUnsignedIntContent)
u.addZakresy("szybkieZmianyWartoscTlumnikaA", "double", "toDouble", "ParamentryBadania-SzybkieZmiany/WartoscTlumnikaA", '6.0', checkDoubleContent)
u.addZakresy("szybkieZmianyWartoscTlumnikaB", "double", "toDouble", "ParamentryBadania-SzybkieZmiany/WartoscTlumnikaB", '10.0', checkDoubleContent)
u.addZakresy("dlugoscDrogiOptycznejCmaxCmin", "double", "toDouble", "ParamentryBadania-DlugoscDrogiOptycznej/CmaxCmin", '1.6', checkDoubleContent)
u.addZakresy("rozproszoneSwiatloCmaxCmin", "double", "toDouble", "ParamentryBadania-RozproszoneSwiatlo/CmaxCmin", '1.6', checkDoubleContent)
u.addZakresy("tolerancjaNapieciaZasilaniaCmaxCmin", "double", "toDouble", "ParamentryBadania-TolerancjaNapieciaZasilania/CmaxCmin", '1.6', checkDoubleContent)
u.addZakresy("minimalneNapieciaTolerancjaNapiecia", "double", "toDouble", "ParamentryBadania-TolerancjaNapieciaZasilania/minimalneNapiecie", '2', checkDoubleContent)
u.addZakresy("maksymalneNapieciaTolerancjaNapiecia", "double", "toDouble", "ParamentryBadania-TolerancjaNapieciaZasilania/maksymalneNapiecie", '30', checkDoubleContent)

u.addZakresy("minimalnaTemperatura", "double", "toDouble", "ParamentryBadania-ParametryTestu/minimalneTemperatura", '15', checkDoubleContent)
u.addZakresy("maksymalnaTemperatura", "double", "toDouble", "ParamentryBadania-ParametryTestu/maksymalnaTemperatura", '35', checkDoubleContent)
u.addZakresy("minimalnaWilgotnosc", "double", "toDouble", "ParamentryBadania-ParametryTestu/minimalneWilgotnosc", '25', checkDoubleContent)
u.addZakresy("maksymalnaWilgotnosc", "double", "toDouble", "ParamentryBadania-ParametryTestu/maksymalnaWilgotnosc", '75', checkDoubleContent)
u.addZakresy("minimalnaCisnienie", "double", "toDouble", "ParamentryBadania-ParametryTestu/minimalneCisnienie", '860', checkDoubleContent)
u.addZakresy("maksymalnaCisnienie", "double", "toDouble", "ParamentryBadania-ParametryTestu/maksymalnaCisnienie", '1060', checkDoubleContent)

u.addZakresy("odpornoscSucheGoraceCmaxCmin", "double", "toDouble", "ParamentryBadania-OdpornoscNaSucheGorace/CmaxCmin", '1.6', checkDoubleContent)
u.addZakresy("odpornoscZimnoCmaxCmin", "double", "toDouble", "ParamentryBadania-OdpornoscNaZimno/CmaxCmin", '1.6', checkDoubleContent)
u.addZakresy("odpornoscWilgotneGoraceCmaxCmin", "double", "toDouble", "ParamentryBadania-OdpornoscNaWilgotneGorace/CmaxCmin", '1.6', checkDoubleContent)
u.addZakresy("wytrzymaloscWilgotneGoraceCmaxCmin", "double", "toDouble", "ParamentryBadania-WytrzymaloscNaWilgotneGorace/CmaxCmin", '1.6', checkDoubleContent)
u.addZakresy("wytrzymaloscWibracjeCmaxCmin", "double", "toDouble", "ParamentryBadania-WytrzymaloscNaWibracje/CmaxCmin", '1.6', checkDoubleContent)
u.addZakresy("odpornoscUderzoniowaCmaxCmin", "double", "toDouble", "ParamentryBadania-OdpornoscUderzeniowa/CmaxCmin", '1.6', checkDoubleContent)
u.addZakresy("wytrzymaloscKorozyjnaSO2CmaxCmin", "double", "toDouble", "ParamentryBadania-WytrzymaloscKorozyjnaSO2/CmaxCmin", '1.6', checkDoubleContent)
u.addZakresy("odpornoscElektroMagnetycznaCmaxCmin", "double", "toDouble", "ParamentryBadania-OdpornoscElektroMagnetyczna/CmaxCmin", '1.6', checkDoubleContent)
u.createHeader()
u.createSource()

#/*"67b"*/ || product != sd->getProduct() /*"23a3"*/)