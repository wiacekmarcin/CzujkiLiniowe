import sys
import crc8
import serial

ser=serial.Serial(
    port='/dev/ttyMega',
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)



if ser.isOpen():
    print(ser.name + ' is open...')
         

while True:
    i = raw_input('Podaj komende\n\t1. Welcone\n\t2. Set Silnik Konf\n\t3. MoveHome\n\t4. MovePos\n\t0 - Wyjscie\n[1-5]:')
    hash = crc8.crc8()
    if i =='0':
        sys.exit(0)
    if i == '1':
        bytes = [0x10, 0x00]
        for c in bytes:
                hash.update(chr(c))
    	str = "%s" % ' '.join([ hex(c) for c in bytes])
    	bytes += hash.digest()
        sc = hash.hexdigest()
        print ("%s 0x%s" % (str,sc))
        ser.write(bytes)
       	data_raw = [hex(ord(b)) for b in  ser.readline()]
        print(data_raw)
        
    if i == '2':
    	m = int(raw_input('Wybierz silnik [1-9]:'))
    	if m < 1 or m > 9 :
		    continue
    	opt = 0
    	en = True
    	
    	rev = raw_input('Czy obrocic obroty [tn]:').strip()

    	if rev != 't' and rev != 'n':
		    continue

    	maxSteps = int(raw_input('Podaj maksymalna ilosc obrotow :'))
    	if maxSteps < 0:
    		continue

    	baseSteps = int(raw_input('Podaj ilosc krokow w bazie:'))
    	if baseSteps < 0:
    		continue


    	middleStep = int(raw_input('Podaj ilosc krokow do srodka:'))
    	if middleStep < 0:
    		continue

        bytes = [ 
			  ((maxSteps >> 24) & 0xff),
    	  	  ((maxSteps >> 16) & 0xff),
	          ((maxSteps >> 8) & 0xff),
	          (maxSteps  & 0xff),
	          ((baseSteps >> 24) & 0xff),
    	  	  ((baseSteps >> 16) & 0xff),
	          ((baseSteps >> 8) & 0xff),
	          (baseSteps  & 0xff),
			  ((middleStep >> 24) & 0xff),
    	  	  ((middleStep >> 16) & 0xff),
	          ((middleStep >> 8) & 0xff),
	          (middleStep  & 0xff)
		]
        opt = 1 if rev == 't' else 0
        cmd = [0x80 | len(bytes), (m  << 4 ) | opt] + bytes
    		
        hash = crc8.crc8()

        for c in cmd:
            hash.update(chr(c))
    	str = "%s" % ' '.join([ hex(c) for c in cmd])
    	cmd += hash.digest()
        sc = hash.hexdigest()
        print ("%s 0x%s" % (str,sc))
        ser.write(cmd)
       	data_raw = [hex(ord(b)) for b in  ser.readline()]
        print(data_raw)
  	
    if i == '3' or i == '4':
        m = int(raw_input('Wybierz silnik [1-9]:'))
        if m < 1 or m > 9 :
            continue
        speed = int(raw_input("Podaj predkosc: "))
    	opt = 0
        steps = 0
    	if i == '4':
    		steps = int(raw_input("Podaj ilosc krokow: "))
    	else:
    	    opt = 0x01
    	    
    	bytes = [ ((steps >> 24) & 0xff),
        	  	  ((steps >> 16) & 0xff),
    	          ((steps >> 8) & 0xff),
    	          (steps  & 0xff),
				  ((speed >> 24) & 0xff),
        	  	  ((speed >> 16) & 0xff),
    	          ((speed >> 8) & 0xff),
    	          (speed  & 0xff)]    
        cmd = [0xa0 | len(bytes), (m << 4 ) | opt] + bytes
        hash = crc8.crc8()

        for c in cmd:
            hash.update(chr(c))
    	str = "%s" % ' '.join([ hex(c) for c in cmd])
    	cmd += hash.digest()
        sc = hash.hexdigest()
        print ("%s 0x%s" % (str,sc))
        ser.write(cmd)
       	data_raw = [hex(ord(b)) for b in  ser.readline()]
        print(data_raw)

    
