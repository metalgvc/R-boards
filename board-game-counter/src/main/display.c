//  7-segment 4-digits indicator attiny13 with two 74HC595
// 
//  display.c 
//
//  Created by metal gvc on 2020-02-20
//

#define DISP_DATA_PIN PB0
#define DISP_CLK_PIN PB2
#define DISP_LTCH_PIN PB1

// segment/block number from right to left
#define DISP_SEGM_1 1
#define DISP_SEGM_2 4
#define DISP_SEGM_3 3
#define DISP_SEGM_4 2

#define DISP_MAP_DOT 10
#define DISP_MAP_OFF 11

// 74HC595 pin - Display part
// 0 - A
// 1 - F
// 2 - B
// 3 - G
// 4 - C
// 5 - point
// 6 - E
// 7 - D


// digit - LED names
// 1 - B,C
// 2 - A,B,G,E,D
// 3 - A,B,G,C,D
// 4 - F,G,B,C
// 5 - A,F,G,C,D
// 6 - A,F,G,E,C,D
// 7 - A,B,C
// 8 - A,B,C,D,E,F,G
// 9 - A,B,C,D,F,G
// 0 - A,B,C,D,E,F
// . - point

// display number map
char dispMap[12] = {
    0b11101011, // 0
    0b00101000, // 1
    0b10110011, // 2
    0b10111001, // 3
    0b01111000, // 4
    0b11011001, // 5
    0b11011011, // 6
    0b10101000, // 7
    0b11111011, // 8
    0b11111001, // 9
    0b00000100, // .
    0b00000000  // OFF
};

void displ_show(char dispMapDigit, char blck){

    PORTB &= ~(1<<DISP_LTCH_PIN);  // latch to 0
    PORTB &= ~(1<<DISP_CLK_PIN);   // 0

    // block (second 74HC595)
    for (char i = 0; i < 4; i++) {
        if ((blck-1) == i) {
            PORTB |= (1<<DISP_DATA_PIN); // send 1
        } else {
            PORTB &= ~(1<<DISP_DATA_PIN); // send 0
        }

        // clock
        PORTB |= 1<<DISP_CLK_PIN; // 1
        //_delay_ms(10);
        PORTB &= ~(1<<DISP_CLK_PIN); // 0
    }

    // digit (first 74HC595)
    for (char i = 0; i < 8; i++) {
        if ( (dispMapDigit>>i)&1 ) {
            PORTB |= (1<<DISP_DATA_PIN); // send 1
        } else {
            PORTB &= ~(1<<DISP_DATA_PIN); // send 0
        }

        // clock
        PORTB |= 1<<DISP_CLK_PIN; // 1
        //_delay_ms(10);
        PORTB &= ~(1<<DISP_CLK_PIN); // 0
    }

    PORTB |= 1<<DISP_LTCH_PIN; // latch
    // _delay_ms(10);
}


void displ_show_num(int number){
    if (number > 9999) {
        number = 9999;
    } else if (number < 0) {
        number = 0;
    }

    int displNum = number%10;
    displ_show(dispMap[displNum], DISP_SEGM_1);

    number = (number/10);
    displNum = number%10;
    if (displNum == 0 && number < 10) { displNum = DISP_MAP_OFF; }
    displ_show(dispMap[displNum], DISP_SEGM_2);

    number = (number/10);
    displNum = number%10;
    if (displNum == 0 && number < 10) { displNum = DISP_MAP_OFF; }
    displ_show(dispMap[displNum], DISP_SEGM_3);

    number = (number/10);
    displNum = number%10;
    if (displNum == 0 && number < 10) { displNum = DISP_MAP_OFF; }
    displ_show(dispMap[displNum], DISP_SEGM_4);
}

