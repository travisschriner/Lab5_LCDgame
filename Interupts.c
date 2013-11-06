/*
 * Interupts.c
 *
 *  Created on: Nov 3, 2013
 *  Author: C15Travis.Schriner
 *  Description:
 *  	These are the functions
 *  	that will prove to be
 *  	irreplaceable for my
 *  	best game since PONG
 */

#include "Interupts.h"
#include "LCD/lcd.h"
#include <msp430.h>

unsigned char initPlayer()
{
        return 0x80;
}

void printPlayer(unsigned char player)
{
        writeCommandByte(player);
        writeDataByte('*');
}

void clearPlayer(unsigned char player)
{
        writeCommandByte(player);
        writeDataByte(' ');
}

#define ROW_MASK 0x40

unsigned char movePlayer(unsigned char player, unsigned char direction)
{

	//TODO needs to clear screen, update player position, print player,
	//reset timer
		clearPlayer(player);

        if(direction == BIT1){
        	if(player != 0x87 && player != 0xc7){
        		player++;
        	}

        }
        else if(direction == BIT2){
        	if(player != 0x80 && player != 0xc0){
        		player--;
        	}

        }
        else if(direction == BIT3){
        	player &= ~ROW_MASK;
        }
        else if(direction == BIT4){
        	player |= ROW_MASK;
        }

        printPlayer(player);

        return player;
}

char didPlayerWin(unsigned char player)
{
        return player == 0xC7;
}
