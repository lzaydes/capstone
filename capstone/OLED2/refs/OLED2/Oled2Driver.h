/************************************************************************/
/*																		*/
/*	Oled2Driver.h -- Interface Declarations for OLED Display Driver 		*/
/*																		*/
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
/************************************************************************/
/*  File Description:													*/
/*																		*/
/*	Interface to Oled2Driver.c											*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	04/29/2011(GeneA): created											*/
/*	07/5/2012(JordanR): adapted for the PmodOLED2						*/
/*																		*/
/************************************************************************/

#if !defined(OLED2DRIVER_INC)
#define	OLED2DRIVER_INC

#include "Board_Defs.h"

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

#define	cbOled2DispMax	8192		//max number of bytes in display buffer

#define	ccolOled2Max	256		//number of display columns
#define	crowOled2Max	64		//number of display rows

#define	cbOled2Char		8		//font glyph definitions is 8 bytes long
#define	chOled2UserMax	0x20	//number of character defs in user font table
#define	cbOled2FontUser	(chOled2UserMax*cbOled2Char)

/* Graphics drawing modes.
*/
#define	modOled2Set		0
#define	modOled2Or		1
#define	modOled2And		2
#define	modOled2Xor		3

/* ------------------------------------------------------------ */
/*					General Type Declarations					*/
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

//Cerebot MX3cK, or, chipKIT Uno w/Pmod Sheild
#if defined (__32MX320F128H__)
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
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*					Variable Declarations						*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */

void	Oled2Init();
void	Oled2Term();
void	Oled2DisplayOn();
void	Oled2DisplayOff();
void	Oled2Clear();
void	Oled2ClearBuffer();
void	Oled2Update();

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
