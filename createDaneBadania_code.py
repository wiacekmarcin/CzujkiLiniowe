import os

class UstawieniaCode:
    def __init__(self):
        self.className = "ParametryBadaniaGen";
        self.h_filename = "parametrybadania_gen.h"
        self.c_filename = "parametrybadania_gen.cpp"
        self.h_HEAD_2 = "\n};\n#endif"
        self.h_public_fun = []
        self.h_protected_fun = []
        self.h_private_fun = []
        self.h_private_values = []
        self.c_sources = []
        self.serout = []
        self.serin = []
        self.sserout = ''
        self.sserin = ''
        self.h_HEAD_1 = """
#ifndef PARAMETRYBADANIAGEN_H
#define PARAMETRYBADANIAGEN_H

#include <QString>
#include <QSettings>
#include <QObject>
#include <QDataStream>

class %(className)s
{
public:
    %(className)s();
    ~%(className)s();

protected:
    void load(QDataStream &in);
    void save(QDataStream &out) const;
    bool change;
public:    

    friend QDataStream &operator<<(QDataStream &, const %(className)s &);
    friend QDataStream &operator>>(QDataStream &, %(className)s &);
/********** GET SET **********/
""" % {'className' : self.className}
    
        
        
        self.c_include = """
#include "%(filename_h)s"

#include <QApplication>


QDataStream &operator<<(QDataStream &out, const %(className)s &d)
{
    QDataStream::FloatingPointPrecision prev = out.floatingPointPrecision();
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    d.save(out);
    out.setFloatingPointPrecision(prev);
    return out;
}

QDataStream &operator>>(QDataStream &in, %(className)s &d)
{
    QDataStream::FloatingPointPrecision prev = in.floatingPointPrecision();
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    d.load(in);
    in.setFloatingPointPrecision(prev);
    return in;
}

%(className)s::%(className)s() 
{
    change = false;
}

%(className)s::~%(className)s()
{
    
}
        
""" % {'className' : self.className,
        'filename_h' : self.h_filename,
        }
    
        self.static_c = """
void %(className)s::load(QDataStream &in)
{
    in  %(in)s ;
}

void %(className)s::save(QDataStream &out) const
{
    out %(out)s ;
}
"""
    
    def cName(self, val):
        return val[0].upper() + val[1:]
    
    def createHeader(self):
        hfile = open('/home/marcin/git.devel.marcin/czujkiLiniowe/CzujkiLinioweApp/'+self.h_filename, "w")
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
        cfile = open('/home/marcin/git.devel.marcin/czujkiLiniowe/CzujkiLinioweApp/'+self.c_filename, "w")
        cfile.write(self.c_include)
        cfile.write(self.static_c % { 'className' : self.className,
            'in'  : '\n\t\t'.join([' >> %s' % f for f in self.serin]),
            'out' : '\n\t\t'.join([' << %s' % f for f in self.serout])        
        })
       
        cfile.write("\n".join([f for f in self.c_sources]))
        cfile.close()
        
    def addValue(self, typeVal, localVal):

        self.h_private_values.append("%s %s;" % (typeVal, localVal))
        fget = "%s get%s() const" % (typeVal, self.cName(localVal))
        #fget2 = "QString get%sString() const" % (self.cName(nameVal))
        fset = "void set%s(const %s & %s)" % (self.cName(localVal), typeVal, localVal)
        #fset2 = "void set%sString(const QString & %s)" % (self.cName(nameVal), nameVal)
       
        fget_c = "%s %s::get%s() const" % (typeVal, self.className, self.cName(localVal))
        #fget2_c = "QString %s::get%sString() const" % (self.className, self.cName(nameVal))
        fset_c = "void %s::set%s(const %s & value)" % (self.className, self.cName(localVal), typeVal)
        #fset2_c = "void %s::set%s(const QString & value)" % (self.className, self.cName(nameVal))

        self.serout.append(localVal)
        self.serin.append(localVal)

        self.h_public_fun.append(fget + ";");
        self.h_public_fun.append(fset + ";");
        #self.h_public_fun.append(fget2 + ";");
        #self.h_public_fun.append(fset2 + ";");
        
        self.c_sources.append("%s\n{\n\treturn %s;\n}\n" % (fget_c, localVal))
        #self.c_sources.append("%s\n{\n\treturn %s;\n}" % (fget_c, localVal))
        
        #void Ustawienia::setRatioSilnik1(const QString & value) { m_ratioSilnik1 = value; settings.setValue("Ratio_1", QVariant::fromValue(m_ratioSilnik1)); }
        self.c_sources.append('%s\n{\n\t%s = value;\n\tchange = true;\n}\n' % (fset_c, localVal))



u = UstawieniaCode()

u.addValue("QString", "numerZlecenia")
u.addValue("QString", "numerTestu")
u.addValue("QString", "osobaOdpowiedzialna")
u.addValue("QString", "uwagi")
u.addValue("QString", "haslo")
u.addValue("bool", "zasilanieCzujekCentrala")
u.addValue("QString", "zasilanieCzujekTypCentrali")
u.addValue("bool", "zasilanieCzujekZasilaczZewnetrzny")
u.addValue("unsigned int", "napiecieZasilaniaCzujki_mV")

u.addValue("unsigned int", "czasStabilizacjiCzujki_s")
u.addValue("unsigned int", "czasStabilizacjiPoResecie_s")
u.addValue("bool", "wyzwalanieAlarmuPrzekaznikiem")
u.addValue("bool", "wyzwalanieAlarmuPradem")
u.addValue("QString", "przekroczeniePraduZasilania_mA")
u.addValue("unsigned int", "dlugoscFaliFiltrow")
u.addValue("unsigned int", "czasPomZmianaTlumenia_s");
u.addValue("bool", "systemOdbiornikNadajnik");
u.addValue("QString", "producentCzujki");
u.addValue("QString", "rozstawienieMinCzujki");
u.addValue("QString", "rozstawienieMaxCzujki");
u.addValue("QString", "maksKatowaNieWspolPionowaNadajnika");
u.addValue("QString", "maksKatowaNieWspolPoziomaNadajnika");
u.addValue("QString", "maksKatowaNieWspolPionowaOdbiornika");
u.addValue("QString", "maksKatowaNieWspolPoziomaOdbiornika");
u.addValue("unsigned short", "iloscCzujek");
u.addValue("bool", "testOdtwarzalnosci");

u.addValue("QString", "nazwaNumerTransmitter");
u.addValue("QString", "nazwaNumerReceiver");
u.addValue("QString", "nazwaTransmitter");
u.addValue("QString", "nazwaReceiver");
u.addValue("QString", "nazwaTransmitter_a");
u.addValue("QString", "nazwaReceiver_a");
u.addValue("QString", "typTransmitter");
u.addValue("QString", "typReceiver");

u.addValue("float", "odtwarzalnoscCmaxCrep")
u.addValue("float", "odtwarzalnoscCrepCmin")
u.addValue("float", "odtwarzalnoscCmin")
u.addValue("float", "powtarzalnoscCmaxCmin")
u.addValue("float", "drogaoptycznaCmaxCmin")
u.addValue("float", "szybkieZmianyWartoscTlumnikaA")
u.addValue("float", "szybkieZmianyWartoscTlumnikaB")
u.addValue("float", "wartoscTlumienieDlaKataNieWspolosiowosci")
u.addValue("float", "niewspolosiowoscMinimalnyKatProducentMierzony")
u.addValue("float", "rozproszoneSwiatloCmaxCmin")
u.addValue("float", "tolerancjaNapieciaZasilaniaCmaxCmin")
u.addValue("float", "minimalneNapieciaTolerancjaNapiecia")
u.addValue("float", "maksymalneNapieciaTolerancjaNapiecia")
u.addValue("unsigned int","minimalnyCzasOczekiwaniaPowtarzalnosc1Test")
u.addValue("unsigned int","maksymalnyCzasOczekiwaniaPowtarzalnosc1Test")

u.createHeader()
u.createSource()


#/*"67b"*/ || product != sd->getProduct() /*"23a3"*/)