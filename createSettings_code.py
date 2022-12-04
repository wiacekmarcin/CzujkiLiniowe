

class UstawieniaCode:
    def __init__(self):
        self.className = "UstawieniaGen";
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

public:    
/********** GET SET **********/
""" % {'className' : self.className}
    
        
        
        self.c_include = """
#include "%(filename_h)s"

#include <QApplication>

%(className)s::%(className)s() :
    settings(QApplication::applicationFilePath() + ".ini", QSettings::IniFormat)
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
        hfile = open(self.h_filename, "w")
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
        cfile = open(self.c_filename, "w")
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
        
        
    def addFiltr(self, fala, nrTarczy, nrPozycji, path, devVal):
        localVal = "filtr%d_%c_%d" % (fala, nrTarczy, nrPozycji)
        localFunName = self.cName(localVal)
        self.h_private_values.append("double %s;" % localVal)
        
        fget = "double get%s() const" % localFunName
        fset = "void set%s(const double & val)" % localFunName
        fset2 = "inline void set%s(const QString & vals) { set%s(toDouble(vals)); }" % (localFunName, localFunName)
        fget_c = "double %s::get%s() const" % (self.className, localFunName)
        fset_c = "void %s::set%s(const double & value)" % (self.className, localFunName)
        f_check = "checkFilterValue"
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
        
    def addSummFiltr(self, checkFun):
        #QString Ustawienia::getRatioSilnik(uint key) const
        self.h_public_fun.append("double getFiltr880_A(const short & nrPos) const { return getFiltr(880, 'A', nrPos); }") 
        self.h_public_fun.append("double getFiltr880_B(const short & nrPos) const { return getFiltr(880, 'B', nrPos); }") 
        self.h_public_fun.append("double getFiltr880_C(const short & nrPos) const { return getFiltr(880, 'C', nrPos); }") 
        
        self.h_public_fun.append("double getFiltr655_A(const short & nrPos) const { return getFiltr(655, 'A', nrPos); }") 
        self.h_public_fun.append("double getFiltr655_B(const short & nrPos) const { return getFiltr(655, 'B', nrPos); }") 
        self.h_public_fun.append("double getFiltr655_C(const short & nrPos) const { return getFiltr(655, 'C', nrPos); }") 
        
        self.h_public_fun.append("double getFiltr880(const char & nrTarczy, const short & nrPos) const { return getFiltr(880, nrTarczy, nrPos); }") 
        self.h_public_fun.append("double getFiltr655(const char & nrTarczy, const short & nrPos) const { return getFiltr(655, nrTarczy, nrPos); }")
        
        self.h_public_fun.append("void setFiltr880_A(const short & nrPos, const double & val) { setFiltr(880, 'A', nrPos, val); }") 
        self.h_public_fun.append("void setFiltr880_B(const short & nrPos, const double & val) { setFiltr(880, 'B', nrPos, val); }")
        self.h_public_fun.append("void setFiltr880_C(const short & nrPos, const double & val) { setFiltr(880, 'C', nrPos, val); }")
        
        self.h_public_fun.append("void setFiltr655_A(const short & nrPos, const double & val) { setFiltr(655, 'A', nrPos, val); }")
        self.h_public_fun.append("void setFiltr655_B(const short & nrPos, const double & val) { setFiltr(655, 'B', nrPos, val); }")
        self.h_public_fun.append("void setFiltr655_C(const short & nrPos, const double & val) { setFiltr(655, 'C', nrPos, val); }")
        
        self.h_public_fun.append("void setFiltr880(const char & nrTarczy, const short & nrPos, const double & val) { setFiltr(880, nrTarczy, nrPos, val); }") 
        self.h_public_fun.append("void setFiltr655(const char & nrTarczy, const short & nrPos, const double & val) { setFiltr(655, nrTarczy, nrPos, val); }")

        self.h_protected_fun.append("bool checkFilterValue(const QString & val);")
        self.h_protected_fun.append("double getFiltr(const int& fala, const char & nrTarczy, const short & nrPos) const;")
        self.h_protected_fun.append("void setFiltr(const int& fala, const char & nrTarczy, const short & nrPos, const double & val);")
            
        fcheck_c = "bool %s::checkFilterValue(const QString & val)" % self.className
        cont = checkFun()
        self.c_sources.append("%s\n{\n%s\n}" % (fcheck_c, cont))
        
        cont = "\tif (false) ;"
        for d in [880, 655]:
            for c in ['A', 'B', 'C']:
                for p in [0,1,2,3,4,5]:
                    cont += "\n\telse if (fala == %d && nrTarczy == '%c' && nrPos == %d) return getFiltr%d_%c_%d();" % (d, c, p, d, c, p)
        
        cont += "\n\telse Q_ASSERT(true);\n\t return 0.0;"
        fget_c = "double %s::getFiltr(const int & fala, const char & nrTarczy, const short & nrPos) const" % self.className
        self.c_sources.append("%s\n{\n%s\n}" % (fget_c, cont))
        
        cont = "\tif (false) ;"
        for d in [880, 655]:
            for c in ['A', 'B', 'C']:
                for p in [0,1,2,3,4,5]:
                    cont += "\n\telse if (fala == %d && nrTarczy == '%c' && nrPos == %d) return setFiltr%d_%c_%d(val);" % (d, c, p, d, c, p)
        
        cont += "\n\telse Q_ASSERT(true);"
        fset_c = "void %s::setFiltr(const int & fala, const char & nrTarczy, const short & nrPos, const double & val)" % self.className
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
checkFuns["czasMiedzyImp"] = checkUnsignedIntContent
checkFuns["maksIloscImp"] = checkUnsignedIntContent
checkFuns["iloscImpBaza"] = checkUnsignedIntContent
checkFuns["iloscImpSrodek"] = checkUnsignedIntContent
checkFuns["jednostka"] = checkUnsignedIntContent

motordefVals = {
    1 : {
        "nazwa" : 'QString("M1")',
        "przelozenieImpJedn" : 0.028,
        "minOdstepImp" : 20,
        "odwrocObroty" : "false",
        "czasMiedzyImp" : 10000000,
        "maksIloscImp" : 1001,
        "iloscImpBaza" : 0,
        "iloscImpSrodek" : 350,
        "jednostka" : "*",
    },
    2 : {
        "nazwa" : 'QString("M2")',
        "przelozenieImpJedn" : 0.028,
        "minOdstepImp" : 20,
        "odwrocObroty" : "false",
        "czasMiedzyImp" : 10000000,
        "maksIloscImp" : 1002,
        "iloscImpBaza" : 0,
        "iloscImpSrodek" : 350,
        "jednostka" : "*",
    },
    3 : {
        "nazwa" : 'QString("M3")',
        "przelozenieImpJedn" : 84.4527,
        "minOdstepImp" : 20,
        "odwrocObroty" : "false",
        "czasMiedzyImp" : 60,
        "maksIloscImp" : 31003,
        "iloscImpBaza" : 260,
        "iloscImpSrodek" : 0,
        "jednostka" : "*",
    },
    4 : {
        "nazwa" : 'QString("M4")',
        "przelozenieImpJedn" : 84.4527,
        "minOdstepImp" : 20,
        "odwrocObroty" : "false",
        "czasMiedzyImp" : 60,
        "maksIloscImp" : 31004,
        "iloscImpBaza" : 260,
        "iloscImpSrodek" : 0,
        "jednostka" : "*",
    },
    5 : {
        "nazwa" : 'QString("M5")',
        "przelozenieImpJedn" : 84.4527,
        "minOdstepImp" : 20,
        "odwrocObroty" : "false",
        "czasMiedzyImp" : 60,
        "maksIloscImp" : 31005,
        "iloscImpBaza" : 260,
        "iloscImpSrodek" : 0,
        "jednostka" : "*",
    },
    6 : {
        "nazwa" : 'QString("M6")',
        "przelozenieImpJedn" : 1.6,
        "minOdstepImp" : 2.6,
        "odwrocObroty" : "false",
        "czasMiedzyImp" : 1256,
        "maksIloscImp" : 1006,
        "iloscImpBaza" : 0,
        "iloscImpSrodek" : 0,
        "jednostka" : "mm",
    },
    7 : {
        "nazwa" : 'QString("M7")',
        "przelozenieImpJedn" : 1.7,
        "minOdstepImp" : 2.7,
        "odwrocObroty" : "false",
        "czasMiedzyImp" : 1257,
        "maksIloscImp" : 1007,
        "iloscImpBaza" : 0,
        "iloscImpSrodek" : 0,
        "jednostka" : "mm",
    },
    8 : {
        "nazwa" : 'QString("M8")',
        "przelozenieImpJedn" : 0.028,
        "minOdstepImp" : 20,
        "odwrocObroty" : "false",
        "czasMiedzyImp" : 10000000,
        "maksIloscImp" : 1008,
        "iloscImpBaza" : 0,
        "iloscImpSrodek" : 350,
        "jednostka" : "*",
    },
    9 : {
        "nazwa" : 'QString("M9")',
        "przelozenieImpJedn" : 0.028,
        "minOdstepImp" : 20,
        "odwrocObroty" : "false",
        "czasMiedzyImp" : 10000000,
        "maksIloscImp" : 1009,
        "iloscImpBaza" : 0,
        "iloscImpSrodek" : 350,
        "jednostka" : "*",
    },
}

defFiltrVals = {
    655: {
        'A': [0, 0.21, 0.4, 0.62, 0.83, 6.98],
        'B': [0, 0.1, 0.98, 1.35, 3.3, 1.97],
        'C': [0, 0.1, 4.28, 8.79, 7.6, 1.97],
    },
    880 : {
        'A': [0, 0.22, 1.02, 0.59, 0.82, 6.39],
        'B': [0, 0.1, 1.02, 2, 3.02, 6.43],
        'C': [0, 0.09, 3.94, 8.04, 11.94, 3.06]
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
motorSett("czasMiedzyImp", "int", "toUInt", checkFuns, motordefVals)
motorSett("maksIloscImp", "int", "toUInt", checkFuns, motordefVals)
motorSett("iloscImpBaza", "int", "toUInt", checkFuns, motordefVals)
motorSett("iloscImpSrodek", "int", "toUInt", checkFuns, motordefVals)

def addFiltrSett(fala, defFiltrVals):
    for i in range(0, 6):
        for c in ['A', 'B', 'C']:
            u.addFiltr(fala, c, i, "Filtr_", defFiltrVals[fala][c][i])
            
addFiltrSett(880, defFiltrVals)
addFiltrSett(655, defFiltrVals)
u.addSummFiltr(checkDoubleContent)

u.addDevice("zasilaczVendor", "Zasilacz/Vendor", 'QString("67b")')
u.addDevice("zasilaczProduct", "Zasilacz/Product", 'QString("23a3")')
u.addDevice("sterownikVendor", "Sterownik/Vendor", 'QString("2341")')
u.addDevice("sterownikProduct", "Sterownik/Product", 'QString("42")')
u.addDevideSummary(checkQStringContent)

u.addZakresy("minNapiecieCzujki", "int", "toUInt", "ParamentryBadania-Czujka/MinimalneNapiecie", '2.0', checkDoubleContent)
u.addZakresy("maxNapiecieCzujki", "int", "toUInt", "ParamentryBadania-Czujka/MaksymalneNapiecie", '30.0', checkDoubleContent)

u.addZakresy("minCzasStabCzujki", "int", "toUInt", "ParamentryBadania-Czujka/MinimalnyCzasStabilizacji", '10', checkUnsignedIntContent)
u.addZakresy("maxCzasStabCzujki", "int", "toUInt", "ParamentryBadania-Czujka/MaksymalnyCzasStabilizacji", '3600', checkUnsignedIntContent)

u.addZakresy("minPrzekrPradZasCzujki", "double", "toDouble", "ParamentryBadania-Alarm/MinimalnyPrzekroczonyPradZasilania", '4.0', checkDoubleContent)
u.addZakresy("maxPrzekrPradZasCzujki", "double", "toDouble", "ParamentryBadania-Alarm/MaksymalnyPrzekroczonyPradZasilania", '200.0', checkDoubleContent)

u.addZakresy("minCzasPoZmianieFiltra", "int", "toUInt", "ParamentryBadania-Filtry/MinimalnyCzasPomiedzyZmianami", '10', checkUnsignedIntContent)
u.addZakresy("maxCzasPoZmianieFiltra", "int", "toUInt", "ParamentryBadania-Filtry/MaksymalnyCzasPomiedzyZmianami", '3600', checkUnsignedIntContent)

#u.addZakresy("minCzasStabCzujki", "int", "toUInt", "ParamentryBadania-Filtry/MinimalnyCzasPomiedzyZmianami", '10', checkUnsignedIntContent)
#u.addZakresy("maxCzasStabCzujki", "int", "toUInt", "ParamentryBadania-Filtry/MaksymalnyCzasPomiedzyZmianami", '3600', checkUnsignedIntContent)

u.createHeader()
u.createSource()

#/*"67b"*/ || product != sd->getProduct() /*"23a3"*/)