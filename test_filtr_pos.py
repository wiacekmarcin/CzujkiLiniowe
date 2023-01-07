globalPos = 0
maxSteps = 360


def setMove(pos):
    global globalPos
    global maxSteps
    downVal = 6
    downVal = 6               
    print ("-----")
    print("globalPos = %d" % globalPos)
    print("pos = %d" % pos)
    d1 = 0
    d2 = 0
    d3 = 0
    d4 = 0
    diff = 0
    if (pos == globalPos):
        return 
    elif pos > globalPos:
        d1 = pos - globalPos;
        d2 = globalPos - pos + maxSteps;
        if d2 > d1:
            diff = 1
            newPosition = pos
            upVal = d1 - downVal
            steps = d1
        else:
            diff = -1
            newPosition = pos
            upVal = d2 - downVal
            steps = d2
    elif pos < globalPos:
        d3 = globalPos - pos
        d4 = pos - globalPos + maxSteps
        if d3 > d4:
            newPosition = pos;
            diff = 1;
            upVal = d4 - downVal;
            steps = d4
        else:
            diff -= 1;
            newPosition = pos;
            upVal = d3 - downVal;
            steps = d3

    print("d1=%d" % d1)
    print("d2=%d" % d2)
    print("d3=%d" % d3)
    print("d4=%d" % d4)
    print("diff=%d" % diff)
    print("newPosition = %d" % newPosition)
    print("steps = %d" % steps)
    print("upVal = %d" % upVal)

    globalPos = newPosition

setMove(60)	
setMove(180)
setMove(300)
setMove(60)	
setMove(300)
setMove(180)
