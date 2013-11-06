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
        	LCDclear();
        	printPlayer(player);
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
        		LCDclear();
        		printPlayer(player);


        		if(HIT){
        			HIT = 0;
        			flag = 0;
        			player = movePlayer(player, BUTTON);
        		    TACTL |= TACLR;
        		}

        		if(didPlayerWin(player)){
        			GAMEON = 0;
        			gameWon();
        			player = initPlayer();
        			while(!GAMEON){

        			}
        		}

        	}

        	if(!GAMEON){
        		gameOver();
        		player = initPlayer();
        		while(!GAMEON);
        	}

        }

        return 0;
}

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

void testAndRespondToButtonPush(char buttonToTest)
{
    if (buttonToTest & P1IFG)
    {
        if (buttonToTest & P1IES)
        {
        	GAMEON = 1;
	        P1IFG &= ~buttonToTest;                            // clear flag
	       //TODO set button direction
	        BUTTON = buttonToTest;
	        HIT = 1;

            //movePlayerInResponseToButtonPush(buttonToTest);
            //clearTimer();
        } else
        {
            __delay_cycles(1000);
        }

        P1IES ^= buttonToTest;
        P1IFG &= ~buttonToTest;
    }
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void){

	testAndRespondToButtonPush(BIT1);
	testAndRespondToButtonPush(BIT2);
	testAndRespondToButtonPush(BIT3);
	testAndRespondToButtonPush(BIT4);
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
