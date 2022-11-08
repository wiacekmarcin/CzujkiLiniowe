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
    i = raw_input('Podaj komende\n\t1. Welcone\n\t2. Set Silnik Konf\n\t3. Position\n\t4. Echo\n\t5. MoveHome\n\t6. Reset\n'
    '\t7. Echo\n\n\t0 - Wyjscie\n[1-5]:')
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


    	delayStep = int(raw_input('Podaj opoznienie krokow:'))
    	if delayStep < 0:
    		continue

	bytes = [ ((0x02 if en else  0x00) | ( 0x01 if rev else  0x00)),
        	  ((maxSteps >> 24) & 0xff),
	  	  ((maxSteps >> 16) & 0xff),
	          ((maxSteps >> 8) & 0xff),
	          (maxSteps  & 0xff),
	          ((baseSteps >> 8) & 0xff),
	          (baseSteps  & 0xff),
	          ((delayStep >> 8) & 0xff),
		  (delayStep  & 0xff)
		 ]
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
  	
    if i == '3':
        hash = crc8.crc8()
        dozo = int(raw_input("Podaj nr dozownika: "))

        step = int(raw_input("Podaj ilosc krokow: "))
        s = "%c%c%c%c%c%c" % (0x55, dozo, step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, step & 0xff)
        crc = hash.update(s)
        c = hash.hexdigest()
        print ("55%02x%02x%02x%02x%02x%s" % (dozo, step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, step & 0xff, c))
    if i == '4':
        hash.update("%c%c" % (0x71,0x00))
        c = hash.hexdigest()
        print ("7100%s" % c)
    if i == '5':
        dozo = int(raw_input("Podaj nr dozownika: "))
        hash.update("%c%c" % (0x91, dozo))
        c = hash.hexdigest()
        print ("91%02x%s" % (dozo, c))
    if i == '6':
        hash.update("%c" % 0xb0)
        c = hash.hexdigest()
        print ("b0%s" % c)
    if i == '7':
        hash.update("%c%c" % (0x71,0x01))
        c = hash.hexdigest()
        print ("7101%s" % c)
