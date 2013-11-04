/*
 * lcd.h
 *
 *  Created on: Oct 21, 2013
 *  Author: C15Travis.Schriner
 *  Description: This is my Header File
 *  for my source file!!!!
 */

#ifndef LCD_H_
#define LCD_H_

void initSPI();

void LCDinit();

void LCDclear();

void cursorToLineTwo();

void cursorToLineOne();

void writeChar(char askiiChar);

void writeString(char * string);

void scrollString(char *string1, char *string2);

void writeCommandByte(char commandByte);

void writeDataByte(char dataByte);



#endif /* LCD_H_ */
