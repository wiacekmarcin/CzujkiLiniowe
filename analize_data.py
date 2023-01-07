import os
import datetime
first_1 = None
period_min = None
period_max = None
period_maxPos = None
period_maxT =    [None, None, None, None, None, None, None, None, None, None, None, None,None, None, None, None, None, None, None, None, None, None, None, None]
period_maxPosT = [None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None]
cntT = 15    
edge = 0
last_1 = None
first_line = False
for f in file('digital.csv'):
    if not first_line:
        first_line = True
        continue
    t, bst = f.split(',')
    bst = int(bst)
    t = float(t)
    #print (t)
    #print(bst)
    if first_1 is None and bst == 1:
        first_1 = t
    if bst:
        prev_1 = t
        continue
        
    if first_1:
        current_period = t - prev_1
        current_period = current_period * 1000000
        last_1 = t

        if period_min is None or period_min > current_period:
            period_min = current_period
        elif period_max is None or period_max < current_period:
            period_maxT.insert(0, current_period)
            period_maxT = period_maxT[0:-1]
            period_maxPosT.insert(0, t)
            period_maxPosT = period_maxPosT[0:-1]
            period_max = current_period
            period_maxPos = t
        elif period_maxT[cntT-1] is None or period_maxT[cntT-1] < current_period:
            for i in range(cntT):
                if period_maxT[i] is None or period_maxT[i] < current_period:
                    period_maxT.insert(i, current_period)
                    period_maxT = period_maxT[0:-1]
                    period_maxPosT.insert(i, t)
                    period_maxPosT = period_maxPosT[0:-1]
                    break                
for p in range(cntT):
    print(period_maxT[p], period_maxPosT[p])
    
