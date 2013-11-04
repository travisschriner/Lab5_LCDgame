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
                /*
                 * while (game is on)
                 * {
                 *                 check if button is pushed (you don't want to block here, so don't poll!)
                 *                 if button is pushed:
                 *                         clear current player marker
                 *                         update player position based on direction
                 *                         print new player
                 *                         clear two second timer
                 *                         wait for button release (you can poll here)
                 * }
                 *
                 * print game over or you won, depending on game result
                 *
                 * wait for button press to begin new game (you can poll here)
                 * wait for release before starting again
                 */

        	while(GAMEON){


        		if(HIT){
        			HIT = 0;
        			movePlayer(player, BUTTON);
        		    TACTL |= TACLR;


        		}

        		if(didPlayerWin(player)){
        			gameWon();
        			player = initPlayer();
        		}

        	}

        	if(!GAMEON){
        		gameOver();
        		while(1);
        	}

        }

        return 0;
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR()
{
    TACTL &= ~TAIFG;            // clear interrupt flag
    GAMEON = 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void){

	 if (P1IFG & BIT1)
	    {
	        P1IFG &= ~BIT1;                            // clear flag
	       //TODO set button direction
	        BUTTON = 1;
	        HIT = 1;
	    }

	    if (P1IFG & BIT2)
	    {
	        P1IFG &= ~BIT2;                         // clear flag
	        //TODO set button direction
	        BUTTON = 2;
	        HIT = 1;
	    }

	    if (P1IFG & BIT3)
	    {
	        P1IFG &= ~BIT3;                         // clear P1.3 interrupt flag
	        //TODO set button direction
	        BUTTON = 3;
	        HIT = 1;
	    }
	    // might have to modify to a different pin if this interferes with my SPI
	    if (P1IFG & BIT4)
	   	{
	   	    P1IFG &= ~BIT4;                         // clear P1.4 interrupt flag
	   	    //TODO set button direction
	   	    BUTTON = 4;
	   	    HIT = 1;
	   	}

}
//
// YOUR TIMER A ISR GOES HERE
//

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

void init_buttons()
{
	 P1DIR &= ~(BIT1|BIT2|BIT3);                // set buttons to input

	 P1IE |= BIT1|BIT2|BIT3;                 // enable the interrupts
	 P1IES |= BIT1|BIT2|BIT3;                   // configure interrupt to sense falling edges

	 P1REN |= BIT1|BIT2|BIT3;                   // enable internal pull-up/pull-down network
	 P1OUT |= BIT1|BIT2|BIT3;                   // configure as pull-up

	 P1IFG &= ~(BIT1|BIT2|BIT3);                // clear flags


}

void gameWon(){
	LCDclear();
	cursorToLineOne();
	writeString("You");
	cursorToLineTwo();
	writeString("Win!!");

}

void gameOver(){
	GAMEON = 0;
	LCDclear();
	cursorToLineOne();
	writeString("Game");
	cursorToLineTwo();
	writeString("Over!");

}
