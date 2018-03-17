avr-gcc -c -O2 -mmcu=atmega2560 -Wa,--gstabs -o cswitch.o cswitch.s
avr-gcc -Os -DF_CPU=16000000 -mmcu=atmega2560 -c active.c  -s cswitch.o -o active.o
avr-gcc -mmcu=atmega2560 active.o cswitch.o -o active.elf 
avr-objcopy -O ihex -R .eeprom active.elf active.hex
avrdude -v -p atmega2560 -c wiring -P /dev/cu.usbmodem1451 -b 115200 -D -U flash:w:active.hex