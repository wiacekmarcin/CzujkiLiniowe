#include "workmode.hpp"
#include <limits.h>

WorkMode::WorkMode()
{
    pinMode(DEBUGPIN, INPUT);
	pinMode(DBG3, INPUT);
	pinMode(DBG2, INPUT);
	pinMode(DBG1, INPUT);
    mode = UNKNOWN;
}

void WorkMode::init() 
{
    Serial.begin(115200);
    debugMode = digitalRead(DEBUGPIN) == LOW;
	mode = conv2DebugWorkMode(digitalRead(DBG3), digitalRead(DBG2), digitalRead(DBG1));
    print();
}
WorkMode::~WorkMode()
{

}

/**
 * M1 DIR=1 w prawo do nadajnika kroki 782(dla dir=0) / 778 (dla dir=1)
 * M2 DIR=1 do góry kroki 650
 * M3 DIR=1 przeciwnie do wskazowek zegara kroki 29825
 * M4 DIR=1 przeciwnie do wskazowek zegara kroki 29835
 * M5 DIR=1 ze wskazowkami zegara kroki 29750
 * M6 DIR=1 w prawo do najnika kroki 19930
 * M7 DIR=1 do gory 150000 krokow malo
 * M8 DIR=1 w prawo od odbiornika 1307 / 973 / 996
 * M9 DIR=1 dp dolu Kroki 826(dla dir=0) / 797(dla dir=1) 2 wdrugim przypadku 802 do 831
 */
uint32_t WorkMode::getDelayImp() const
{
	switch (mode)
	{
	case PIONOWA:		return 50;
	case KATOWA_POZ:
	case KATOWA_PION:	return 1000;
	case KOLOWA:		return 60;
	case POZIOMA:		return 250;
	default:    		return UINT_MAX;
	}
}

uint32_t WorkMode::getMaxSteps() const
{
	switch (mode)
	{
	case PIONOWA:		return 250000;
	case KATOWA_POZ:
	case KATOWA_PION:	return 1500;
	case KOLOWA:		return 32000;
	case POZIOMA:		return 25000;
	default:    		return 0;
	}
}

void WorkMode::print() const
{
	switch (mode)
	{
	case PIONOWA:		Serial.print("Wozek Gora/Dol"); break;
	case KATOWA_POZ:    Serial.print("Ramie Lewo/Prawo"); break;
	case KATOWA_PION:	Serial.print("Ramie Gora/Dol"); break;
	case KOLOWA:		Serial.print("Tarcza Filtra"); break;
	case POZIOMA:		Serial.print("Wozek Lewo/Prawo"); break;
	default:    		Serial.print("Nieznane"); break;
	}
}

WorkMode::WorkModeEnum WorkMode::conv2DebugWorkMode(uint8_t d3, uint8_t d2, uint8_t d1)
{
	uint8_t b = 0;
	b += (d3 == LOW) ? 0x01 : 0x00;
	b += (d2 == LOW) ? 0x02 : 0x00;
	b += (d1 == LOW) ? 0x04 : 0x00;
#ifdef MAINDEBUG
	Serial.print("d3=");
	Serial.print(d3);
	Serial.print(" d2=");
	Serial.print(d2);
	Serial.print(" d1=");
	Serial.print(d1);
	Serial.print(" d=");
	Serial.println(isDebugMode);
#endif
	switch (b)
	{
	case PIONOWA:	    	return PIONOWA;
	case KATOWA_PION:		return KATOWA_PION;
	case KATOWA_POZ:		return KATOWA_POZ;
	case KOLOWA:    		return KOLOWA;
	case POZIOMA:   		return POZIOMA;
	case CHECKKRANC:		return CHECKKRANC;
	default:        		return UNKNOWN;
	}
}

void debugModeFun(const WorkMode & mode, uint8_t KRANCPIN, uint8_t DIRPIN, uint8_t PULSEPIN)
{
    unsigned long prevTime = millis();
    unsigned long actTime = millis();
    unsigned long timeDelay = 0;
    uint32_t middleSteps = 0;
    uint32_t steps = 0;
    bool debugDir = false;

    uint32_t maxSteps = mode.getMaxSteps();
    uint32_t impDelay = mode.getDelayImp();

    do {
	if (mode.getMode() == WorkMode::CHECKKRANC)
	{
		Serial.print("Czujnik krancowki = ");
		Serial.println(digitalRead(KRANCPIN) == LOW ? "Tak" : "Nie");
		delay(500);
		prevTime = millis();
		continue;
	}

	if (steps == 0xffffffff)
	{
		Serial.print("Rozpoczynam prace. Kierunek dir=");
		Serial.println(debugDir ? HIGH : LOW);
		Serial.print("maxSteps=");
		Serial.print(maxSteps);
		Serial.print(" impDelay=");
		Serial.print(impDelay);
		steps = 0;
		continue;
	}

	if (steps > maxSteps)
	{
		debugDir = !debugDir;
		digitalWrite(DIRPIN, debugDir ? HIGH : LOW);
		Serial.print("Nie wykryto krancowki. Steps > ");
		Serial.println(maxSteps);
		Serial.print("Zmieniam kierunek na dir=");
		Serial.println(debugDir ? HIGH : LOW);
		steps = 0;
		if (mode.getMode() == WorkMode::PIONOWA)
		{
			//timeDelay += 5000;
            delay(5000);
		}
		continue;
	}

	if (digitalRead(KRANCPIN) == LOW)
	{
		actTime = millis();
		Serial.print("\nWykryto krancowke po ");
		Serial.print(steps);
		Serial.print(" krokach. Czas [ms]=");
		Serial.println(actTime - prevTime - timeDelay);
		middleSteps = steps / 2;
		steps = 0;
		debugDir = !debugDir;
		digitalWrite(DIRPIN, debugDir ? HIGH : LOW);
		
        delay(3000);
        timeDelay = 0;
		prevTime = millis();

		while (digitalRead(KRANCPIN) == LOW)
		{
			digitalWrite(PULSEPIN, HIGH);
			delayMicroseconds(impDelay);
			digitalWrite(PULSEPIN, LOW);
			delayMicroseconds(impDelay);
		}
		Serial.print("Zmiana kierunku dir=");
		Serial.println(debugDir);
	}

	if (false && (mode.getMode() == WorkMode::KATOWA_PION || mode.getMode() == WorkMode::KATOWA_POZ) && steps == middleSteps && steps > 0)
	{
		Serial.print("Srodek (kroki= ");
		Serial.print(steps);
		Serial.println(")");
		timeDelay += 10000;
		delay(10000);
	}
	digitalWrite(PULSEPIN, HIGH);
	delayMicroseconds(impDelay);
	digitalWrite(PULSEPIN, LOW);
	delayMicroseconds(impDelay);
	++steps;
	if ((mode.getMode() == WorkMode::KATOWA_PION || mode.getMode() == WorkMode::KATOWA_POZ)  && steps % 50 == 0)
	{
		Serial.print(".");
	}

	if ((mode.getMode() == WorkMode::PIONOWA || mode.getMode() == WorkMode::POZIOMA) && steps % 10000 == 0)
	{
		Serial.print(".");
	}

    if ((mode.getMode() == WorkMode::KOLOWA ) && steps % 1000 == 0)
	{
		Serial.print(".");
	}
    } while(true);
}


