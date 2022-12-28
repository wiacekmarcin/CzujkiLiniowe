#ifndef MAIN_MINI_H_
#define MAIN_MINI_H_

#include <Arduino.h>
void phex(uint8_t b);
void setBusy(bool busy);
uint8_t getAddress();

#define BUSYPIN A0
#define MOVEPIN 5

#define ENPIN  7
#define DIRPIN  8
#define PULSEPIN 9

#define KRANCPIN 2
#define STOPPIN 3

#define DEBUGPIN 4

#define AD3 10
#define AD2 A3
#define AD1 A2
#define AD0 A1


void setStopSoft();

#endif /* MAIN_MINI_H_ */
