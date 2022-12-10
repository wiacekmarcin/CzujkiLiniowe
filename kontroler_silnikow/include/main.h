#ifndef MAIN_MINI_H_
#define MAIN_MINI_H_
//dx dx dx dx dx d3 d2 d1
typedef enum _debugWorkMode
{
	CHECKKRANC = 0, //brak zworek
	KATOWA_POZ = 1, //d3 pierwsza od procka
	KOLOWA = 2, //d2 druga od procka
	KATOWA_PION = 3, //d3,d2 pierwsza, druga od procka
	POZIOMA = 4, //d3 trzecia od procka
	PIONOWA = 7, //wszystkie zworki
	UNKNOWN = 8
} DebugWorkMode;

#endif /* MAIN_MINI_H_ */
