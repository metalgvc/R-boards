DEVICE     = attiny13
CLOCK      = 9600000
PROGRAMMER = usbasp-clone
PORT	   = usb
BAUD       = 9600
FILENAME   = main
COMPILE    = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

all: clean build flash

build:
	$(COMPILE) -c $(FILENAME).c -o $(FILENAME).o
	$(COMPILE) -o $(FILENAME).elf $(FILENAME).o
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

flash:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILENAME).hex:i 
	#avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -B 4 -U flash:w:$(FILENAME).hex:i

fuse:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U lfuse:w:0x6A:m -U hfuse:w:0xFF:m -U lock:w:0xFF:m

readfuse:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U hfuse:r:hfuse.txt:m
	cat hfuse.txt

clean:
	rm main.o
	rm main.elf
	rm main.hex