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
        switch (direction) {
                //
                // update player position based on direction of movement
                //
        }

        return player;
}

char didPlayerWin(unsigned char player)
{
        return player == 0xC7;
}
