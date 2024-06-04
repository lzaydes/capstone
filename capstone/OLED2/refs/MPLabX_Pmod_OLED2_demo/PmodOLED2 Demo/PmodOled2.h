/************************************************************************/
/*																		*/
/*	PmodOled2.h	--	Header File for PmodOLED2 Driver					*/
/*																		*/
/************************************************************************/
/*	Author: 	Jordan Retz												*/
/*	Copyright 2012, Digilent Inc.										*/
/************************************************************************/
/*  File Description: 													*/
/*		This module was adapted from PmodOLED.h written by Gene 		*/
/*	Apperson for the PmodOLED.											*/
/*																		*/
/*		This file contains driver function declarations for the 		*/
/*	PmodOLED2 library.													*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	04/29/2011(GeneA): created PmodOLED driver file.					*/
/*	06/19/2012(JordanR): Adapted from PmodOLED library.					*/
/*																		*/
/************************************************************************/
#if !defined(PMODOLED2_INC)
#define PMODOLED2_INC

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#define cbOled2DispMax		8192	//max number of bytes in display buffer
#define ccolOled2Max		256		//256 pixels (columns), but, 1 byte = 2 pixels
									//therefore, 128 bytes = 1 row 
#define crowOled2Max		64		//number of display rows

#define cbOled2Char			8		//font glyph definition is 8 bytes long
#define chOled2UserMax		0x20	//number of character defs in user font table
#define cbOled2FontUser		(chOled2UserMax*cbOled2Char) //max number of bytes in user font

/* Graphics drawing modes.
*/
#define	modOled2Set		0
#define	modOled2Or		1
#define	modOled2And		2
#define	modOled2Xor		3

/* ------------------------------------------------------------ */
/*				General Type Declarations						*/
/* ------------------------------------------------------------ */
/* Gray Scale values for PmodOLED2. Gray Scale values range from 0 to 180.
** GS0 represents pixel OFF, and is always set at 0. GS1-15 are user 
** definable, or, they can be drawn from the default Gray Scale Table.
*/
#define oled2GS1			40
#define oled2GS2			44
#define oled2GS3			48
#define oled2GS4			52
#define oled2GS5			56
#define oled2GS6			60
#define oled2GS7			68
#define oled2GS8			76
#define oled2GS9			84
#define oled2GS10			92
#define oled2GS11			102
#define oled2GS12			115
#define oled2GS13			135
#define oled2GS14			155
#define oled2GS15			180

/* Pin definitions for access to OLED2 control signals on active (6/12/12) Cerebot Boards
*/
//Cerebot MX3cK
#if defined (__32MX320F128H__)
#define MX3CK
#define prtSelect	IOPORT_D
#define	prtDataCmd	IOPORT_D
#define	prtReset	IOPORT_F
#define	prtVccCtrl  IOPORT_F
#define	prtVddCtrl	IOPORT_D
#define prtRead		IOPORT_D
#define prtWrite	IOPORT_D

#define bitSelect	BIT_7
#define bitDataCmd	BIT_6
#define	bitReset	BIT_3
#define	bitVccCtrl	BIT_6
#define	bitVddCtrl	BIT_9
#define bitRead		BIT_5
#define bitWrite	BIT_4

//Cerebot MX4cK
#elif defined (__32MX460F512L__)
#define MX4CK
#define prtSelect	IOPORT_B
#define	prtDataCmd	IOPORT_B
#define	prtReset	IOPORT_G
#define	prtVccCtrl  IOPORT_G
#define	prtVddCtrl	IOPORT_G
#define prtRead		IOPORT_D
#define prtWrite	IOPORT_D

#define bitSelect	BIT_14
#define bitDataCmd	BIT_15
#define	bitReset	BIT_8
#define	bitVccCtrl	BIT_6
#define	bitVddCtrl	BIT_9
#define bitRead		BIT_5
#define bitWrite	BIT_4

//Cerebot MX7cK
#elif defined (__32MX795F512L__)
#define MX7CK
#define prtSelect	IOPORT_B
#define	prtDataCmd	IOPORT_B
#define	prtReset	IOPORT_G
#define	prtVccCtrl  IOPORT_D
#define	prtVddCtrl	IOPORT_C
#define prtRead		IOPORT_D
#define prtWrite	IOPORT_D

#define bitSelect	BIT_14
#define bitDataCmd	BIT_15
#define	bitReset	BIT_0
#define	bitVccCtrl	BIT_7
#define	bitVddCtrl	BIT_1
#define bitRead		BIT_5
#define bitWrite	BIT_4

//Unknown
#else
#error Unknown Board

#endif
/* ------------------------------------------------------------ */
/*				Local Type Declarations							*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Procedure Declarations							*/
/* ------------------------------------------------------------ */

void	Oled2Init();
void	Oled2Term();
void	Oled2Clear();
void	Oled2ClearBuffer();
void	Oled2Update();

void 	Oled2DisplayOn();
void	Oled2DisplayOff();

#endif