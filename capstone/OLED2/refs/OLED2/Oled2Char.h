/************************************************************************/
/*																		*/
/*	Oled2Char.h	--	Interface Declarations for Oled2Char.c				*/
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
/*	Interface to Oled2Char.c												*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	06/01/2011(GeneA): created											*/
/*	07/5/2012(JordanR): adapted for the PmodOLED2						*/
/*																		*/
/************************************************************************/

#if !defined(OLED2CHAR_H)
#define OLED2CHAR_H

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					General Type Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Variable Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */

void	Oled2SetCursor(int xch, int ych);
void	Oled2GetCursor(int * pxcy, int * pych);
int		Oled2DefUserChar(char ch, uint8_t * pbDef);
void	Oled2SetCharUpdate(int f);
int		Oled2GetCharUpdate();
void	Oled2PutChar(char ch, uint8_t clr);
void	Oled2PutString(char * sz, uint8_t clr);

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
