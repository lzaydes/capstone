/************************************************************************/
/*																		*/
/*	main.c	--	Main Demo Setup and Test Procedure						*/
/*																		*/
/************************************************************************/
/*	Author: 	Jordan Retz												*/
/*	Copyright 2012, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	06/19/2012(JordanR): Created										*/
/*																		*/
/************************************************************************/
#include <plib.h>
#include <GenericTypeDefs.h>

#include "PmodOled2.h"

#include "delay.h"

/* Oscillator Settings
*/
#pragma config FNOSC		= PRIPLL	// Oscillator selection
#pragma config POSCMOD		= XT		// Primary oscillator mode
#pragma config FPLLIDIV 	= DIV_2		// PLL input divider
#pragma config FPLLMUL		= MUL_20	// PLL multiplier
#pragma config FPLLODIV 	= DIV_1		// PLL output divider
#pragma config FPBDIV		= DIV_1		// Peripheral bus clock divider
#pragma config FSOSCEN		= OFF		// Secondary oscillator enable

/* Clock control settings
*/
#pragma config IESO			= OFF		// Internal/external clock switchover
#pragma config FCKSM		= CSDCMD	// Clock switching (CSx)/Clock monitor (CMx)
#pragma config OSCIOFNC		= OFF		// Clock output on OSCO pin enable

/* USB Settings
*/
#if !defined (__32MX320F128H__)
#pragma config UPLLEN		= OFF		// USB PLL enable
#pragma config UPLLIDIV		= DIV_2		// USB PLL input divider
#endif

/* Other Peripheral Device settings
*/
#pragma config FWDTEN		= OFF		// Watchdog timer enable
#pragma config WDTPS		= PS1024	// Watchdog timer postscaler

/* Code Protection settings
*/
#pragma config CP			= OFF		// Code protection
#pragma config BWP			= OFF		// Boot flash write protect
#pragma config PWP			= OFF		// Program flash write protect

/* Debug settings
*/
#if defined MX7CK
	#pragma config ICESEL	= ICS_PGx1
#else
	#pragma config ICESEL	= ICS_PGx2
#endif

#pragma config DEBUG 		= OFF

/* ------------------------------------------------------------ */
/*				Local Definitions								*/
/* ------------------------------------------------------------ */
/* Logo Corner Position Definitions
*/
#define xlogo1		120
#define ylogo1		11

#define xlogo2		127
#define ylogo2		11

#define xlogo3		92
#define ylogo3		50

#define xlogo4		155
#define ylogo4		50

#define xlogo5		96
#define ylogo5		55

#define xlogo6		151
#define	ylogo6		55

#define xlogo7		120
#define	ylogo7		20

#define xlogo8		122
#define	ylogo8		25

#define xlogo9		109
#define	ylogo9		46

#define xlogo10		140
#define	ylogo10		46

#define xlogo11		146
#define	ylogo11		46

#define xlogo12		106
#define	ylogo12		50 

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */
void FullDemo();

BOOL CharTest();
BOOL CharTestASCII();
BOOL CharTestString();
BOOL CharTestUserChar();
BOOL CharTestDisplayMessage();

BOOL GraphTest();
BOOL GraphTestPixel();
BOOL GraphTestLineTo();
BOOL GraphTestDrawRect();
BOOL GraphTestDrawFillRect();
BOOL GraphTestPutBmp();
BOOL GraphTestGetBmp();
BOOL GraphTestDrawChar();

void Banner();
void DrawLogo(BYTE clr);

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/***	main()
**
**	Parameters:
**		none
**
**	Return Value:
**		0
**
**	Errors:
**		none
**
**	Description:
**		Sets up and runs test procedures for PmodOLED2
*/

int main() {

	Oled2Init();

	FullDemo();

	Oled2Term();

	return 0;
}

/* ------------------------------------------------------------ */
/***	FullDemo()
**
**	Parameters:
**		none
**
**	Return Value:
**		0
**
**	Errors:
**		none
**
**	Description:
**		Calls the OLED2 full demo functions
*/
void FullDemo() {
	
	Banner();

	CharTest();

	GraphTest();
}

/*********************************************************************************
//////////////////////////////////////////////
//				BANNER DISPLAY				//
//////////////////////////////////////////////
*********************************************************************************/
/* ------------------------------------------------------------ */
/***	Banner()
**
**	Parameters:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Displays a Digilent banner 
*/
void Banner() {
	int ib;
	int c;
	char sz1[] = {'P', 'm', 'o', 'd', 'O', 'L', 'E', 'D', '2', '\0'};
	char sz2[] = {'B', 'y', ' ', 'D', 'i', 'g', 'i', 'l', 'e', 'n', 't', '\0'};

	/* Surrounding Fill
	*/
	Oled2SetDrawColor(15);

	Oled2MoveTo(0,0);
	Oled2DrawRect(ccolOled2Max - 1, crowOled2Max - 1);

	Oled2MoveTo(0,0);
	Oled2SetFillPattern(Oled2GetStdPattern(1));
	Oled2FillRect(ccolOled2Max - 1, crowOled2Max - 1);

	DelayMs(1000);

	for(ib = 15; ib >= 0; ib--) {
	
		Oled2SetDrawColor(ib);
	
		Oled2MoveTo(0,0);
		Oled2SetFillPattern(Oled2GetStdPattern(1));
		Oled2FillRect(ccolOled2Max - 1, crowOled2Max - 1);

		Oled2SetDrawColor(15);
	
		Oled2MoveTo(0,0);
		Oled2DrawRect(ccolOled2Max - 1, crowOled2Max - 1);

		DrawLogo(15);

		Oled2Update();
	}	

	DelayMs(1500);

	for(ib = 15; ib > 7; ib--) {
		DrawLogo(ib);

		Oled2Update();
	}

	/* DrawBanner
	*/
	Oled2SetCursor(12, 2);

	Oled2PutString(sz1, 15);

	Oled2SetCursor(11, 4);

	Oled2PutString(sz2, 15);


	/* Update Display
	*/
	Oled2Update();

	DelayMs(1000);

	Oled2SetFillPattern(Oled2GetStdPattern(0));	

	for(ib = crowOled2Max - 1; ib >= 0; ib--) {

		Oled2MoveTo(0, 0);
		Oled2DrawRect(ccolOled2Max - 1, ib);

		if(ib < crowOled2Max - 1) {
			Oled2MoveTo(0, crowOled2Max - 1);
			Oled2FillRect(ccolOled2Max - 1, ib+1);
		}

		Oled2Update();
	}

	Oled2Clear();

	DelayMs(1000);	
}

/* ------------------------------------------------------------ */
/***	DrawLogo()
**
**	Parameters:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Draws the Digilent Logo to the middle of the OLED2 display
*/
void DrawLogo(BYTE clr) {

	Oled2SetDrawColor(clr);

	/* Outline
	*/
	Oled2MoveTo(xlogo1, ylogo1);

	Oled2LineTo(xlogo2, ylogo2);

	Oled2LineTo(xlogo4, ylogo4);

	Oled2LineTo(xlogo6, ylogo6);

	Oled2LineTo(xlogo5, ylogo5);

	Oled2LineTo(xlogo3, ylogo3);

	Oled2LineTo(xlogo1, ylogo1);

	Oled2LineTo(xlogo11, ylogo11);

	Oled2LineTo(xlogo10, ylogo10);

	Oled2LineTo(xlogo7, ylogo7);

	Oled2LineTo(xlogo5, ylogo5);

	Oled2MoveTo(xlogo10, ylogo10);

	Oled2LineTo(xlogo9, ylogo9);

	Oled2LineTo(xlogo8, ylogo8);

	Oled2LineTo(xlogo12, ylogo12);

	Oled2LineTo(xlogo4, ylogo4);

}

/*********************************************************************************
//////////////////////////////////////////////
//				CHAR TESTS					//
//////////////////////////////////////////////
*********************************************************************************/
/* ------------------------------------------------------------ */
/***	CharTest()
**
**	Parameters:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Calls Character Mode functions to test and demonstrate their
**	usage.
*/
BOOL CharTest() {

	CharTestDisplayMessage();

	DelayMs(4000);
	Oled2Clear();

	CharTestASCII();

	DelayMs(1000);
	Oled2Clear();
	
	CharTestString();

	DelayMs(1000);

	CharTestUserChar();

	DelayMs(1000);
	Oled2Clear();

	return TRUE;

}

/* ------------------------------------------------------------ */
/***	CharTestASCII()
**
**	Parameters:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Write the entire ASCII character table to the display
*/
BOOL CharTestASCII() {
	char ch = ' ';
	BYTE clr = 15;

	Oled2SetCursor(0,0);

	for(ch = ' '; ch <= '~'; ch++) {
		Oled2PutChar(ch, clr);
	}

	return TRUE;
}

/* ------------------------------------------------------------ */
/***	CharTestString()
**
**	Parameters:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Write a string to the Display
*/
BOOL CharTestString() {
	Oled2SetCursor(0, 0);

	char ch[] = {'P', 'm', 'o', 'd', 'O', 'L', 'E', 'D', '2', '\0'};

	Oled2PutString(ch, 15);


	return TRUE;

}

/* ------------------------------------------------------------ */
/***	CharTestUserChar()
**
**	Parameters:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Defines a "user character" (trademark symbol), and writes
**	it to the display
*/
BOOL CharTestUserChar() {
	char myChar1 = 0x00;
	BYTE myTMChar[] = {0x02, 0x0E, 0x02, 0x0C, 0x04, 0x08, 0x04, 0x0C};

	Oled2DefUserChar(myChar1, myTMChar);

	Oled2PutChar(myChar1, 15);

	return TRUE;
}

/* ------------------------------------------------------------ */
/***	CharTestDisplayMessage()
**
**	Parameters:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Displays a multi line message
**	
*/
BOOL CharTestDisplayMessage() {
	char myChar2 = 0x01;
	BYTE myRChar[] = {0x3C, 0x42, 0xFD, 0x95, 0xB5, 0xCD, 0x42, 0x3C};
	char myChar1 = 0x00;
	BYTE myTMChar[] = {0x02, 0x0E, 0x02, 0x0C, 0x04, 0x08, 0x04, 0x0C};
	char myString1[] = {'H', 'e', 'l', 'l', 'o', ' ', 'f', 'r', 'o', 'm', ' ', 'D', 'i', 'g', 'i', 'l', 'e', 'n', 't', '\0'};
	char myString2[] = {'T', 'h', 'i', 's', ' ', 'i', 's', ' ', 't', 'h', 'e', ' ', 'P', 'm', 'o', 'd', 'O', 'L', 'E', 'D', '2', ' ', 'D', 'e', 'm', 'o', '\0'};

	Oled2SetCursor(0,0);
	Oled2Clear();

	Oled2DefUserChar(myChar2, myRChar);

	Oled2PutString(myString1, 15);
	Oled2PutChar(myChar2, 15);
	Oled2PutChar('!', 15);
	Oled2SetCursor(0, 2);
	Oled2PutString(myString2, 15);
	Oled2PutChar(0x00, 15);

	return TRUE;
}

/*********************************************************************************
//////////////////////////////////////////////
//				GRAPH TESTS					//
//////////////////////////////////////////////
*********************************************************************************/
/* ------------------------------------------------------------ */
/***	GraphTest()
**
**	Parameters:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Calls Graphic Mode functions to test and demonstrate their
**	usage.
*/
BOOL GraphTest() {

	GraphTestPixel();

	DelayMs(1000);
	
	GraphTestLineTo();

	DelayMs(1000);
	Oled2Clear();

	DelayMs(1000);
	Oled2Clear();

	GraphTestDrawRect();

	DelayMs(1000);
	Oled2Clear();

	GraphTestDrawFillRect();

	DelayMs(1000);
	Oled2Clear();

	GraphTestPutBmp();

	DelayMs(1000);

	GraphTestGetBmp();

	DelayMs(1000);
	Oled2Clear();

	GraphTestDrawChar();

	DelayMs(1000);
	Oled2ClearBuffer();

	return TRUE;
}

/* ------------------------------------------------------------ */
/***	GraphTestPixel()
**
**	Parameters:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Turns on a single pixel in the middle of the display
**	
*/
BOOL GraphTestPixel() {

	Oled2MoveTo(ccolOled2Max/2 - 1 , crowOled2Max/2 - 1);
	
	Oled2DrawPixel();
	Oled2Update();

	return TRUE;
}

/* ------------------------------------------------------------ */
/***	GraphTestLineTo()
**
**	Parameters:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Draws Lines to the display
**	
*/
BOOL GraphTestLineTo() {

	Oled2MoveTo(0,0);

	Oled2LineTo(ccolOled2Max/2 - 1 , crowOled2Max/2 - 1);

	Oled2Update();

	Oled2MoveTo(ccolOled2Max, 0);

	Oled2LineTo(ccolOled2Max/2 - 1 , crowOled2Max/2 - 1);

	Oled2Update();

	return TRUE;
}

/* ------------------------------------------------------------ */
/***	GraphTestDrawRect()
**
**	Parameters:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Draws a Rectangle on the display
**	
*/
BOOL GraphTestDrawRect() {
	/* Normal Rectangle
	*/ 

	Oled2MoveTo(0, 0);

	Oled2DrawRect(100, 63);

	Oled2Update();

	return TRUE;
}

/* ------------------------------------------------------------ */
/***	GraphTestDrawFillRect()
**
**	Parameters:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Draws a rectangular fill pattern to the display. Cycles
**	through all provided patterns
*/
BOOL GraphTestDrawFillRect() {
	/* Fill Rectangle
	*/ 

	/* Blank
	*/
	Oled2MoveTo(0, 0);

	Oled2SetFillPattern(Oled2GetStdPattern(0));

	Oled2FillRect(76, 27);

	Oled2Update();

	/* Fill
	*/
	Oled2MoveTo(0, 0);

	Oled2SetFillPattern(Oled2GetStdPattern(1));

	Oled2FillRect(76, 27);

	Oled2Update();

	DelayMs(1000);

	/* Checkers
	*/
	Oled2MoveTo(0, 0);

	Oled2SetFillPattern(Oled2GetStdPattern(2));

	Oled2FillRect(76, 27);

	Oled2Update();

	DelayMs(1000);

	/* Speck Open
	*/
	Oled2MoveTo(0, 0);

	Oled2SetFillPattern(Oled2GetStdPattern(3));

	Oled2FillRect(76, 27);

	Oled2Update();

	DelayMs(1000);

	/* Speck Tight
	*/
	Oled2MoveTo(0, 0);

	Oled2SetFillPattern(Oled2GetStdPattern(4));

	Oled2FillRect(76, 27);

	Oled2Update();

	DelayMs(1000);
	
	/* Circles Open
	*/
	Oled2MoveTo(0, 0);

	Oled2SetFillPattern(Oled2GetStdPattern(5));

	Oled2FillRect(76, 27);

	Oled2Update();

	DelayMs(1000);

	/* Circles Bar
	*/
	Oled2MoveTo(0, 0);

	Oled2SetFillPattern(Oled2GetStdPattern(6));

	Oled2FillRect(76, 27);

	Oled2Update();

	DelayMs(1000);

	/* Carrots
	*/
	Oled2MoveTo(0, 0);

	Oled2SetFillPattern(Oled2GetStdPattern(7));

	Oled2FillRect(76, 27);

	Oled2Update();

	DelayMs(1000);



	return TRUE;
}

/* ------------------------------------------------------------ */
/***	GraphTestDrawChar()
**
**	Parameters:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Uses the Graphic Mode DrawChar/String. If an MX4cK/MX7ck is being
**	used, this functions prompts the user to send a BMP image file.
*/
BOOL GraphTestDrawChar() {

	char sz[] = {'B', 'C', 'D', '\0'};

	Oled2DrawChar('A', 15);

	Oled2Update();

	Oled2MoveTo(25, 25);

	Oled2DrawString(sz, 15);

	Oled2Update();

	return TRUE;
}

/* ------------------------------------------------------------ */
/***	GraphTestPutBmp()
**
**	Parameters:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Draws a bit map to the display (smiley face)
**	
*/
BOOL GraphTestPutBmp() {
	BYTE rgbBmp0[] = {
		0x00, 0x0F, 0x00, 0xF0, 0x00,
		0x00, 0x0F, 0x00, 0xF0, 0x00,
		0x00, 0x0F, 0x00, 0xF0, 0x00,
		0x00, 0x0F, 0x00, 0xF0, 0x00,
		0x00, 0x0F, 0x00, 0xF0, 0x00,
		0xF0, 0x00, 0x00, 0x00, 0x0F,
		0x0F, 0x00, 0x00, 0x00, 0xF0,
		0x00, 0xFF, 0xFF, 0xFF, 0x00
	};

	Oled2MoveTo(1,0);

	Oled2PutBmp(9, 7, rgbBmp0);

	Oled2Update();

	return TRUE;
}

/* ------------------------------------------------------------ */
/***	GraphTestPutBmp()
**
**	Parameters:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Gets the image drawn by GraphTestPutBmp() from the display
**	and copies it a few pixels below.
*/
BOOL GraphTestGetBmp() {
	BYTE rgbBmp1[80];

	Oled2MoveTo(1, 0);
	
	Oled2GetBmp(9, 7, rgbBmp1);

	Oled2MoveTo(1, 19);

	Oled2PutBmp(9, 7, rgbBmp1);

	Oled2Update();

	return TRUE;
}