#include <msp430.h> 
#include "Interupts.h"
#include "LCD/lcd.h"

/*
 * Created By: Travis Schriner
 * Date 3 November 2013
 * Description:
 *	This code creates a game using
 *	the MSP430 microcontroller
 *	as well as the LCD display located
 *	on the geek box. It is the best
 *	game since PONG ;)
 *
 *	DOCUMENTATION: This frame was the given code from
 *	Capt. Branch
 */
void init_timer();
void init_buttons();
void gameWon();
void gameOver();

char flag = 0;
char GAMEON;
char BUTTON;
char HIT;



int main(void)
{
        WDTCTL = (WDTPW|WDTHOLD);


        //initiates game board and plater
        unsigned char player = initPlayer();
        initSPI();
        init_timer();
        init_buttons();
        LCDinit();
        LCDclear();
        printPlayer(player);

        HIT = 0;
        GAMEON = 1;
        init_timer();

        __enable_interrupt();

        while(1)
        {
        	//clears the screen again and prints player
        	LCDclear();
        	printPlayer(player);

        	//only works if GAMEON is 1. This is when I have not lost
        	while(GAMEON){
        		//tests if my P1 interrupt has been tripped.
        		if(HIT){
        			HIT = 0;
        			flag = 0;
        			player = movePlayer(player, BUTTON);
        		    TACTL |= TACLR;
        		}

        		//checks if I have won the game. If so, executes win logic and waits for a button to get pressed
        		if(didPlayerWin(player)){
        			GAMEON = 0;
        			gameWon();
        			player = initPlayer();
        			while(!GAMEON){

        			}
        			LCDclear();
        		}

        	//If I leave the GAMEON loop, this loop will execute that will execute gameOver() logic. Sits in holding pattern until button is hit.
        	}

        	if(!GAMEON){
        		gameOver();
        		player = initPlayer();
        		while(!GAMEON);
        	}

        }

        return 0;
}

//timer interrupt that will disable game after 2 seconds
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR()
{
    TACTL &= ~TAIFG;            // clear interrupt flag
    flag++;
    if (flag >= 4)
    {
    	GAMEON = 0;
    }
}

//code given from Capt Branch to test button and assign if button was hit and which one hit
void testAndRespondToButtonPush(char buttonToTest)
{
    if (buttonToTest & P1IFG)
    {
        if (buttonToTest & P1IES)
        {
        	GAMEON = 1;
	        P1IFG &= ~buttonToTest;                            // clear flag
	        BUTTON = buttonToTest;
	        HIT = 1;
        } else {
            __delay_cycles(1000);
        }

        P1IES ^= buttonToTest;
        P1IFG &= ~buttonToTest;
    }
}

//this is my interrupt logic for a button push.
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void){

	testAndRespondToButtonPush(BIT1);
	testAndRespondToButtonPush(BIT2);
	testAndRespondToButtonPush(BIT3);
	testAndRespondToButtonPush(BIT4);
}

//This sets up my timer interrupt and assigns bits appropriately.
void init_timer()
{
        // do timer initialization work

    TACTL &= ~(MC1|MC0);        // stop timer

    TACTL |= TACLR;             // clear TAR

    TACTL |= TASSEL1;           // configure for SMCLK - what's the frequency (roughly)?

    TACTL |= ID1|ID0;           // divide clock by 8 - what's the frequency of interrupt?

    TACTL &= ~TAIFG;            // clear interrupt flag

    TACTL |= MC1;               // set count mode to continuous

    TACTL |= TAIE;              // enable interrupt
}

//this sets up my buttons on P1 and sets bits accordingly
void init_buttons()
{
	 P1DIR &= ~(BIT1|BIT2|BIT3|BIT4);                // set buttons to input

	 P1IE |= BIT1|BIT2|BIT3|BIT4;                    // enable the interrupts
	 P1IES |= BIT1|BIT2|BIT3|BIT4;                   // configure interrupt to sense falling edges

	 P1REN |= BIT1|BIT2|BIT3|BIT4;                   // enable internal pull-up/pull-down network
	 P1OUT |= BIT1|BIT2|BIT3|BIT4;                   // configure as pull-up

	 P1IFG &= ~(BIT1|BIT2|BIT3|BIT4);                // clear flags


}

void gameWon(){
	LCDclear();
	cursorToLineOne();
	writeString("You     ");
	cursorToLineTwo();
	writeString("Win!!   ");

}

void gameOver(){

	LCDclear();
	cursorToLineOne();
	writeString("Game    ");
	cursorToLineTwo();
	writeString("Over!   ");

}
