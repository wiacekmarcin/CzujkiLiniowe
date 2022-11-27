

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
    
    //load();
}

%(className)s::~%(className)s()
{
    save();
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
        cfile.write("\n}\n\nbool %s::check()\n{\n" % self.className)
        cfile.write("\n".join(["\tif (!%s) return false;" % f for f in self.c_check]))
        cfile.write("\n\treturn true;\n}\n\n")
        
        cfile.write("\n\n".join([f for f in self.c_sources]))
        cfile.close()
        
    def addMotor(self, nrMotor, nameVal, typeVal, path, convF):
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
        self.h_protected_fun.append
        
        settPath = path + "%d" % nrMotor + "/" + self.cName(nameVal)
        
        #m_ratioSilnik1 = settings.value("Ratio_1", m_ratioSilnik1).toString();
        self.c_load.append('%s = %s(settings.value("%s").toString());' % (localVal, convF, settPath))
        
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
        
def checkNazwaContent() :
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
checkFuns["nazwa"] = checkNazwaContent
checkFuns["przelozenie"] = checkDoubleContent
checkFuns["maksPredkosc"] = checkDoubleContent
checkFuns["odwrocObroty"] = checkBoolContent
checkFuns["opoznienieImp"] = checkUnsignedIntContent
checkFuns["maksIloscKrokow"] = checkUnsignedIntContent

u = UstawieniaCode()
def motorSett(nazwa, typ, fun, checkFuns): 
    for i in range(1,10):
        u.addMotor(i, nazwa, typ, "Silnik-", fun)
    u.addSummMotor(nazwa, typ, fun, checkFuns)

motorSett("nazwa", "QString", "toQString", checkFuns)
motorSett("przelozenie", "double", "toDouble", checkFuns)
motorSett("maksPredkosc", "double", "toDouble", checkFuns)
motorSett("odwrocObroty", "bool", "toBool", checkFuns)
motorSett("opoznienieImp", "int", "toUInt", checkFuns)
motorSett("maksIloscKrokow", "int", "toUInt", checkFuns)
u.createHeader()
u.createSource()

