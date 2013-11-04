/*
 * Interupts.h
 *
 *  Created on: Nov 3, 2013
 *  Author: C15Travis.Schriner
 *  Description:
 *  	This is the header file for
 *  	my best game since PONG
 *
 *
 *  SOURCE: This code is from Capt. Branch's given header
 */

#ifndef INTERUPTS_H_
#define INTERUPTS_H_


#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define ROW_MASK 0x40

#define NUM_MINES 2

//
// Initializes player to starting position on board.
//
unsigned char initPlayer();

//
// Prints the player marker (*) at the player location passed in.
//
void printPlayer(unsigned char player);

//
// Clears the location passed in.
//
void clearPlayer(unsigned char player);

//
// Given a player's current position and a direction, returns an updated player position.
//
unsigned char movePlayer(unsigned char player, unsigned char direction);

//
// Returns true if the position passed in is the winning game position.
//
char didPlayerWin(unsigned char player);

/*
 * Functions which may prove useful in achieving A-Functionality.
 *
char didPlayerHitMine(unsigned char player, unsigned char mines[NUM_MINES]);
void generateMines(unsigned char mines[NUM_MINES]);
void printMines(unsigned char mines[NUM_MINES]);
*/

#endif /* INTERUPTS_H_ */
