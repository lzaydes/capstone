/************************************************************************/
/*																		*/
/*	Oled2Char.c	--	Character Output Routines for OLED Display			*/
/*																		*/
/************************************************************************/
/*	Author: 	Gene Apperson											*/
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
/*  Module Description: 												*/
/*																		*/
/*	This module contains the implementations of the 'character mode'	*/
/*	functions. These functions treat the graphics display as a 4 row	*/
/*	by 16 column character display.										*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	06/01/2011(GeneA): created											*/
/*	07/5/2012(JordanR): adapted for the PmodOLED2						*/
/*																		*/
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

extern "C" {
	#include <inttypes.h>
}

#include "Oled2Driver.h"
#include "Oled2Char.h"
#include "Oled2Grph.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

extern int		xcoOled2Cur;
extern int		ycoOled2Cur;

extern uint8_t *	pbOled2Cur;
extern uint8_t		mskOled2Cur;
extern int		bnOled2Cur;
extern int		fOled2CharUpdate;

extern uint8_t		rgbOled2DispBuf[];

extern uint8_t		clrOled2Cur;

extern int		dxcoOled2FontCur;
extern int		dycoOled2FontCur;

extern	uint8_t *	pbOled2FontCur;
extern	uint8_t *	pbOled2FontUser;

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

int		xchOled2Cur;
int		ychOled2Cur;

int		xchOled2Max;
int		ychOled2Max;

uint8_t *	pbOled2FontExt;

uint8_t	rgbOled2FontUser[cbOled2FontUser];

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

void	Oled2DrawGlyph(char ch, uint8_t clr);
void	Oled2AdvanceCursor();

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
/***	Oled2SetCursor
**
**	Parameters:
**		xch			- horizontal character position
**		ych			- vertical character position
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set the character cursor position to the specified location.
**		If either the specified X or Y location is off the display, it
**		is clamped to be on the display.
*/

void
Oled2SetCursor(int xch, int ych)
	{

	/* Clamp the specified location to the display surface
	*/
	if (xch >= xchOled2Max) {
		xch = xchOled2Max-1;
	}

	if (ych >= ychOled2Max) {
		ych = ychOled2Max-1;
	}

	/* Save the given character location.
	*/
	xchOled2Cur = xch;
	ychOled2Cur = ych;

	/* Convert the character location to a frame buffer address.
	*/
	Oled2MoveTo(xch*dxcoOled2FontCur, ych*dycoOled2FontCur);

}

/* ------------------------------------------------------------ */
/***	Oled2GetCursor
**
**	Parameters:
**		pxch		- pointer to variable to receive horizontal position
**		pych		- pointer to variable to receive vertical position
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Fetch the current cursor position
*/

void
Oled2GetCursor( int * pxch, int * pych)
	{

	*pxch = xchOled2Cur;
	*pych = ychOled2Cur;

}

/* ------------------------------------------------------------ */
/***	Oled2DefUserChar
**
**	Parameters:
**		ch		- character code to define
**		pbDef	- definition for the character
**
**	Return Value:
**		none
**
**	Errors:
**		Returns TRUE if successful, FALSE if not
**
**	Description:
**		Give a definition for the glyph for the specified user
**		character code. User definable character codes are in
**		the range 0x00 - 0x1F. If the code specified by ch is
**		outside this range, the function returns false.
*/

int
Oled2DefUserChar(char ch, uint8_t * pbDef)
	{
	uint8_t *	pb;
	int		ib;

	if (ch < chOled2UserMax) {
		pb = pbOled2FontUser + ch * cbOled2Char;
		for (ib = 0; ib < cbOled2Char; ib++) {
			*pb++ = *pbDef++;
		}
		return 1;
	}
	else {
		return 0;
	}

	}

/* ------------------------------------------------------------ */
/***	Oled2SetCharUpdate
**
**	Parameters:
**		f		- enable/disable automatic update
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set the character update mode. This determines whether
**		or not the display is automatically updated after a
**		character or string is drawn. A non-zero value turns
**		automatic updating on.
*/

void
Oled2SetCharUpdate(int f)
	{

	fOled2CharUpdate = (f != 0) ? 1 : 0;

}

/* ------------------------------------------------------------ */
/***	Oled2GetCharUpdate
**
**	Parameters:
**		none
**
**	Return Value:
**		returns current character update mode
**
**	Errors:
**		none
**
**	Description:
**		Return the current character update mode.
*/

int
Oled2GetCharUpdate()
	{

	return fOled2CharUpdate;

}

/* ------------------------------------------------------------ */
/***	Oled2PutChar
**
**	Parameters:
**		ch			- character to write to display
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Write the specified character to the display at the current
**		cursor position and advance the cursor.
*/

void
Oled2PutChar(char ch, uint8_t clr)
	{

	Oled2DrawGlyph(ch, clr);
	Oled2AdvanceCursor();
	if (fOled2CharUpdate) {
		Oled2Update();
	}

}

/* ------------------------------------------------------------ */
/***	Oled2PutString
**
**	Parameters:
**		sz		- pointer to the null terminated string
**		clr		- desired gray scale level
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Write the specified null terminated character string to the
**		display and advance the cursor.
*/

void
Oled2PutString(char * sz, uint8_t clr)
	{

	while (*sz != '\0') {
		Oled2DrawGlyph(*sz, clr);
		Oled2AdvanceCursor();
		sz += 1;
	}

	if (fOled2CharUpdate) {
		Oled2Update();
	}

}

/* ------------------------------------------------------------ */
/***	Oled2DrawGlyph
**
**	Parameters:
**		ch		- character code of character to draw
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Renders the specified character into the display buffer
**		at the current character cursor location. This does not
**		affect the current character cursor location or the 
**		current drawing position in the display buffer.
*/

void
Oled2DrawGlyph(char ch, uint8_t clr)
	{
	uint8_t *	pbFont;
	uint8_t *	pbDisp;
	int		ib;
	int		ix;
	int		iy;

	if ((ch & 0x80) != 0) {
		return;
	}
	
	Oled2SetDrawColor(clr);

	if (ch < chOled2UserMax) {
		pbFont = pbOled2FontUser + ch*cbOled2Char;
	}
	else if ((ch & 0x80) == 0) {
		pbFont = pbOled2FontCur + (ch-chOled2UserMax) * cbOled2Char;
	}

	pbDisp = pbOled2Cur;

	/* Written by Michael T. Alexander; "Rotates" the glyph
	** definition by 90 degrees and write GS Level nibble 
	** data to each byte in the display buffer. One byte of 
	** data in the display buffer contains GS Level data 
	** for two row adjacent pixels on the display.
	*/
	for ( iy = 0; iy < dycoOled2FontCur; iy++ ) {
		for ( ix = 0; ix < dxcoOled2FontCur; ix++ ) {
			if ( pbFont[ix] & ( 1 << iy ) ) {
				pbDisp[ix/2] |= ( ix & 1 ) ? ( 0x0F & clrOled2Cur ) : ( 0xF0 & ( clrOled2Cur << 4 ) );
			}
			else {		
				pbDisp[ix/2] &= ( ix & 1 ) ? 0xF0 : 0x0F;
			}	
		}	
		pbDisp += 128;
	}

}

/* ------------------------------------------------------------ */
/***	Oled2AdvanceCursor
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Advance the character cursor by one character location,
**		wrapping at the end of line and back to the top at the
**		end of the display.
*/

void
Oled2AdvanceCursor()
	{

	xchOled2Cur += 1;
	if (xchOled2Cur >= xchOled2Max) {
		xchOled2Cur = 0;
		ychOled2Cur += 1;
	}
	if (ychOled2Cur >= ychOled2Max) {
		ychOled2Cur = 0;
	}

	Oled2SetCursor(xchOled2Cur, ychOled2Cur);

}

/* ------------------------------------------------------------ */
/***	ProcName
**
**	Parameters:
**
**	Return Value:
**
**	Errors:
**
**	Description:
**
*/

/* ------------------------------------------------------------ */

/************************************************************************/

