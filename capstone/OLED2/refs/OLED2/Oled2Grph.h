/************************************************************************/
/*																		*/
/*	Oled2Grph.h	--	Declarations for OLED Graphics Routines				*/
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
/*	Interface declarations for Oled2Grph.c								*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	06/03/2011(GeneA): created											*/
/*	07/5/2012(JordanR): adapted for the PmodOLED2						*/
/*																		*/
/************************************************************************/

#if !defined(OLED2GRPH_H)
#define	OLED2GRPH_H

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

void	Oled2SetDrawColor(uint8_t clr);
void	Oled2SetDrawMode(int mod);
int		Oled2GetDrawMode();
uint8_t *	Oled2GetStdPattern(int ipat);
void	Oled2SetFillPattern(uint8_t * pbPat);

void	Oled2MoveTo(int xco, int yco);
void	Oled2GetPos(int * pxco, int * pyco);
void	Oled2DrawPixel();
uint8_t	Oled2GetPixel();
void	Oled2LineTo(int xco, int yco);
void	Oled2DrawRect(int xco, int yco);
void	Oled2FillRect(int xco, int yco);
void	Oled2GetBmp(int dxco, int dyco, uint8_t * pbBmp);
void	Oled2PutBmp(int dxco, int dyco, uint8_t * pbBmp);
void	Oled2PutBmpFlipped(int dxco, int dyco, uint8_t * pbBmp);
void	Oled2DrawChar(char ch, uint8_t clr);
void	Oled2DrawString(char * sz, uint8_t clr);

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
