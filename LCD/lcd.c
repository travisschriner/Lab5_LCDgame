/*
 *  lcd.c
 *
 *  Created on: Oct 21, 2013
 *  Author: C15Travis.Schriner
 *  Description: This library allows
 *  me to interact with the LCD in the
 *  GeekBox. It gives me common functions
 *  that I would need.
 */

#include <msp430.h>
#include "lcd.h"

#define RS_MASK 0x40

char LCDDATA, LCDSEND, LCDCON;

void LCDDELAY1();
void LCDDELAY2();
void writeCommandNibble(char commandNibble);
void setSSHi();
void setSSLo();
void SPIsend(char byteToSend);
void LCDWRT4(char byte);
void LCDWRT8(char byteToSend);

//this is copied pretty much from Lab 3 example code
void initSPI(){

	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 |= UCCKPH|UCMSB|UCMST|UCSYNC;
	UCB0CTL1 |=	UCSSEL1;

	P1DIR	 |=	BIT0;
	P1SEL	 |=	BIT5;
	P1SEL2	 |=	BIT5;
	P1SEL	 |=	BIT7;
	P1SEL2	 |= BIT7;
	P1SEL	 |=	BIT6;
	P1SEL2	 |=	BIT6;
	UCB0CTL1 &=	~UCSWRST;
}

void LCDinit(){

	writeCommandNibble(0x03);

	writeCommandNibble(0x03);

    writeCommandNibble(0x03);

    writeCommandNibble(0x02);

    writeCommandByte(0x28);

    writeCommandByte(0x0C);

    writeCommandByte(0x01);

    writeCommandByte(0x06);

    writeCommandByte(0x01);

    writeCommandByte(0x02);

    SPIsend(0);
    LCDDELAY1();

}

void writeCommandNibble(char commandNibble){

    LCDCON &= ~RS_MASK;
    LCDWRT4(commandNibble);
    LCDDELAY2();
}

void writeCommandByte(char commandByte)
{
    LCDCON &= ~RS_MASK;
    LCDWRT8(commandByte);
    LCDDELAY2();
}

void writeDataByte(char dataByte){

    LCDCON |= RS_MASK;
    LCDWRT8(dataByte);
    LCDDELAY2();
}


void LCDclear(){

	writeCommandByte(1);
}

//40.5 us delay
void LCDDELAY1(){

	_delay_cycles(45);
}

//1.65 ms delay
void LCDDELAY2(){

	_delay_cycles(1817);
}

void cursorToLineTwo(){

	writeCommandByte(0xC0);
}

void cursorToLineOne(){
	writeCommandByte(1);

}

void writeChar(char asciiChar){
	LCDCON |= RS_MASK;
	LCDWRT8(asciiChar);
	LCDDELAY2();

}

void writeString(char * string){
	int i = 0;
	LCDCON |= RS_MASK;
	for (i =0; i < 8; i++){
		LCDWRT8(string[i]);
		LCDDELAY2();
	}



}

//courtesy of C2C Steinmiller...
void scrollString(char *string1, char *string2) {
        unsigned int i = 0;

        char *count1 = string1, *count2 = string2;

        while (1) {
                cursorToLineOne();
                char *currentChar = count1;

                for (i = 0; i < 8; i++) {
                     writeDataByte(*currentChar);        //send data in the string to be written

                     currentChar++;

                     if (*currentChar == 0)
                           currentChar = string1;
                }
                count1++;

                if (*count1 == 0) {
                     count1 = string1;
                }

                cursorToLineTwo();
                char *currentChar2 = count2;
                for (i = 0; i < 8; i++) {
                      writeDataByte(*currentChar2);        //send data in the string to be written

                      currentChar2++;

                      if (*currentChar2 == 0)
                      currentChar2 = string2;
                }
                count2++;

                if (*count2 == 0) {
                    count2 = string2;
                }

                __delay_cycles(600000);

                LCDclear();
        }
}



void setSSHi(){
	P1OUT |= BIT0;
}

void setSSLo(){
	P1OUT &= ~BIT0;
}

void SPIsend(char byteToSend){

volatile char readByte;

	setSSLo();

    UCB0TXBUF = byteToSend;

    while(!(UCB0RXIFG & IFG2)){
	        // wait until you've received a byte
	    }

    readByte = UCB0RXBUF;

    setSSHi();


}
void LCDWRT4(char byte){

	byte &= 0x0f;
	byte |= LCDCON;
	byte &= 0x7f;
	SPIsend(byte);
	LCDDELAY1();
	byte |= 0x80;
	SPIsend(byte);
	LCDDELAY1();
	byte &= 0x7f;
	SPIsend(byte);
	LCDDELAY1();
}

void LCDWRT8(char byteToSend){

	unsigned char sendByte = byteToSend;

	sendByte &= 0xF0;

	// rotate to the right 4 times
    sendByte = sendByte >> 4;

    LCDWRT4(sendByte);

    sendByte = byteToSend;

    sendByte &= 0x0F;

    LCDWRT4(sendByte);

}




