#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "main/display.c"

#define BTNS_ADC_PIN PB4

// get out last specified bits from ADCH
#define ADC_ACCURACY 4 //  highest -> lower accurancy

#define BTNS_IS_PRESSED 1
#define BTNS_IS_RELEASED 0

//#define F_CPU 9600000UL

volatile unsigned char ADCResult = 0;
volatile char btnsState = BTNS_IS_RELEASED;
char prevBtnState = BTNS_IS_RELEASED;

int currDisplNumber = 0;


void displ_show(char dispMapDigit, char blck);

// ADC interrupt handler
ISR(ADC_vect) {
   if ((ADCH>>ADC_ACCURACY) != (ADCResult>>ADC_ACCURACY)) {
      ADCResult = ADCH;

      if (ADCResult > 0x0f) {
      	btnsState = BTNS_IS_PRESSED;
      } else {
      	btnsState = BTNS_IS_RELEASED;
      }
   }
}

int init(void) {

	// clock prescaller 0
	//CLKPR &= ~(1<<CLKPS0) & ~(1<<CLKPS1) & ~(1<<CLKPS2) & ~(1<<CLKPS3);

	// set display data pins as output
	DDRB |= (1<<DISP_DATA_PIN) | (1<<DISP_LTCH_PIN) | (1<<DISP_CLK_PIN);

	// ADC pin - buttons pin as input
	DDRB &= ~(1<<BTNS_ADC_PIN);

	// set B0..B4 pins -> 0
	PORTB = 0x00;

	// ADC settings -------------------------------------------------------------
	// ADLAR - left adjust the result (read from ADCH)
	ADMUX |= (1<<ADLAR);
	ADMUX &= ~(1<<REFS0); // Vcc voltage reference

	// set PB4 input as ADC
	ADMUX &= ~(1<<MUX0);
	ADMUX |= (1<<MUX1);

	// ADATE - enable autotrigger
	// ADIE - enable ADC interrupts
	// ADPSx - set prescaller (F_CPU/128)
	ADCSRA |= (1<<ADATE) | (1<<ADIE) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);

	// set ADC autotrigger source 000 - free running mode
	ADCSRB &= ~(1<<ADTS0) & ~(1<<ADTS1) & ~(1<<ADTS2);

	// disable digital buffer for ADC2 pin
	DIDR0 |= (1<<ADC2D);

	// enable ADC & start first conversion
	ADCSRA |= (1<<ADEN) | (1<<ADSC);
	// -- end ADC settings ------------------------------------------------------

	// enable all interrupts
	sei(); 

	return 1;
}

int check_btns(void){

	// 60 120 190 255

	if (ADCResult < 80) {
		currDisplNumber = currDisplNumber - 10;
	} else if (ADCResult < 140) {
		currDisplNumber = currDisplNumber - 1;
	} else if (ADCResult < 220) {
		currDisplNumber = currDisplNumber + 1;
	} else {
		currDisplNumber = currDisplNumber + 10;
	}

	if (currDisplNumber < 0) {
		currDisplNumber = 0;
	} else if (currDisplNumber > 9999) {
		currDisplNumber = 9999;
	}

	return 1;
}

int main(void) {

	init();

	while (1) {
		
		// debounce delay
		if (prevBtnState != btnsState) {
			_delay_ms(3);
		}
		
		if (prevBtnState != btnsState) {
			prevBtnState = btnsState;

			if (btnsState == BTNS_IS_PRESSED) {
				check_btns();
			}
		}

		//displ_show_num(ADCResult);
		displ_show_num(currDisplNumber);
	}

	return 1;
}