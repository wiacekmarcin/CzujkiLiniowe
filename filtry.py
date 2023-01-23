defFiltrValsdb = {
    655: {
        'A': [0, 1.0, 0.57, 0.44, 0.47, 18.36],
        'B': [0, 0.56, 0.47, 0.41, 7.69, 9.31],
        'C': [0, 2.84, 2.48, 4.13, 4.41, 6.22],
    },
    880 : {
        'A': [0, 0.16, 0.23, 0.29, 0.39, 13.46],
        'B': [0, 0.22, 0.4, 0.5, 6.0, 7.12],
        'C': [0, 0.99, 1.94, 2.96, 3.95, 4.81],
    }
}

fala = defFiltrValsdb[880]
ukladF = []
for a in fala['A']:
    for b in fala['B']:
        for c in fala['C']:
            suma = a + b + c
            if suma < 10:
                ssuma = "0%2.2f" % suma
            else:
                ssuma = "%2.2f" % suma
            key = "%s = %02.2f + %02.2f + %02.2f" % (ssuma, a, b, c)
            ukladF.append(key)
            
ukladF = sorted(ukladF)            
for u in ukladF:
    print u
