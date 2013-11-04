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

unsigned char movePlayer(unsigned char player, unsigned char direction)
{

	//TODO needs to clear screen, update player position, print player,
	//reset timer
	LCDclear();

        if(direction == 1){
        	player ++;
        }
        else if(direction == 2){
        	player--;
        }
        else if(direction == 3){
        	player += 100;
        }
        else if(direction == 4){
        	player -= 100;
        }

        printPlayer(player);



        return player;
}

char didPlayerWin(unsigned char player)
{
        return player == 0xC7;
}
