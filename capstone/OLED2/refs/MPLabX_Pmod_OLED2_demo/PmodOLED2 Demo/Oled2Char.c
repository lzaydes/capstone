/************************************************************************/
/*																		*/
/*	Oled2Char.c	--	Character Functionality Definitions for PmodOLED2	*/
/*																		*/
/************************************************************************/
/*	Author: 	Jordan Retz												*/
/*	Copyright 2012, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*		This module was adapted from OledChar.c written by Gene Apperson*/
/*	for the PmodOLED.													*/
/*																		*/
/*		This module contains function definitions for the character		*/
/*	"mode" of the PmodOLED2.											*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	06/01/2011(GeneA): created PmodOLED char module						*/
/*	06/19/2012(JordanR): Adapted from the PmodOLED library.				*/
/*																		*/
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "PmodOled2.h"
#include "Oled2Char.h"
#include "Oled2Grph.h"

/* ------------------------------------------------------------ */
/*				General Type Definitions						*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */
/* These variables are first declared in PmodOled2.c
*/
extern int		xcoOled2Cur;
extern int		ycoOled2Cur;

extern BYTE *	pbOled2Cur;

extern BOOL		fOled2CharUpdate;
extern BYTE		clrOled2Cur;

extern BYTE		rgbOled2DispBuf[];

extern int		dxcoOled2FontCur;
extern int		dycoOled2FontCur;

extern BYTE *	pbOled2FontCur;
extern BYTE *	pbOled2FontUser;

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */
int		xchOled2Cur;
int		ychOled2Cur;
	
int		xchOled2Max;
int		ychOled2Max;

BYTE	rgbOled2FontUser[cbOled2FontUser];

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */
void	Oled2DrawGlyph(char ch, BYTE clr);
void	Oled2AdvanceCursor();

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/***	Oled2SetCursor()
**
**	Parameters:
**		int xch - Desired x-coordinate of cursor
**		int ych - Desired y-coordinate of cursor
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set the character cursor position to the specified location.
**	If either the specified X or Y location is off the display, it is
**	clamped to be on the display.
*/

void Oled2SetCursor(int xch, int ych)	{

	/* Clamp the specified location to the display surface
	*/
	if(xch >= xchOled2Max) {
		xch = xchOled2Max - 1;
	}
	if(ych >= ychOled2Max) {
		ych = ychOled2Max - 1;
	}

	/* Save the given character location.
	*/
	xchOled2Cur = xch;
	ychOled2Cur = ych;

	/* Convert the character location to a frame buffer address.
	*/
	Oled2MoveTo(xch*dxcoOled2FontCur, ych*dycoOled2FontCur);
//	Oled2MoveTo(xch*dxcoOled2FontCur, ych*dycoOled2FontCur);
}

/* ------------------------------------------------------------ */
/***	Oled2GetCursor()
**
**	Parameters:
**		int * pxch - Pointer to variable to receive x-coordinate of cursor
**		int * pych - Pointer to variable to receive y-coordinate of cursor
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Gets the current cursor position.
*/

void Oled2GetCursor(int * pxch, int * pych) {

	*pxch = xchOled2Cur;
	*pych = ychOled2Cur;

}

/* ------------------------------------------------------------ */
/***	Oled2DefUserChar()
**
**	Parameters:
**		char ch 		- Character code to define (between 0x00 and 0x1F
**		BYTE * pbDef	- definition of character
**
**	Return Value:
**		TRUE if successful, FALSE if not
**
**	Errors:
**		none
**
**	Description:
**		Give a definition for the glyph of a specified user character
**	code. The character code must be in the range 0x00 - 0x1F.
**
*/

BOOL Oled2DefUserChar(char ch, BYTE * pbDef) {

	BYTE *	pb; 
	int	ib;
	BOOL	fSuccess = FALSE;

	if(ch < chOled2UserMax) {
		pb = pbOled2FontUser + ch * cbOled2Char;
		
		for(ib = 0; ib < cbOled2Char; ib++) {
			*pb++ = *pbDef++;
		}

		fSuccess = TRUE;
	}

	return fSuccess;
}

/* ------------------------------------------------------------ */
/***	Oled2SetCharUpdate()
**
**	Parameters:
**		BOOL f - enable/disable automatic display update
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set the character update mode. If this function
**	sets fOled2CharUpdate = TRUE, then the display will 
**	automatically updated without having to call Oled2Update().
**
*/

void Oled2SetCharUpdate(BOOL f) {
	
	fOled2CharUpdate = f;

}

/* ------------------------------------------------------------ */
/***	Oled2GetCharUpdate()
**
**	Parameters:
**		none
**
**	Return Value:
**		TRUE if successful, FALSE if not
**
**	Errors:
**		none
**
**	Description:
**		Returns the status of the fOled2CharUpdate flag
**
*/

BOOL OledGetCharUpdate() {

	return fOled2CharUpdate;

}

/* ------------------------------------------------------------ */
/***	Oled2PutChar()
**
**	Parameters:
**		char ch - character to be written to display
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Writes the desired character to the display buffer,
**	advances the cursor, and updates the display if 
**	fOled2CharUpdate = TRUE.
**
*/

void Oled2PutChar(char ch, BYTE clr) {
	
	Oled2DrawGlyph(ch, clr);
	Oled2AdvanceCursor();

	if (fOled2CharUpdate) {
		Oled2Update();
	}

}	

/* ------------------------------------------------------------ */
/***	Oled2PutString()
**
**	Parameters:
**		char * sz - pointer to null terminated string
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Write the desired null terminated character string to
**	the display buffer and advance the cursor. Update the 
**	display if the fOled2CharUpdate = TRUE.
**
*/

void Oled2PutString(char * sz, BYTE clr) {
	
	while(*sz != '\0') {
		Oled2DrawGlyph(*sz, clr);
		Oled2AdvanceCursor();
		sz++;
	}

	if(fOled2CharUpdate) {
		Oled2Update();
	}

}

/* ------------------------------------------------------------ */
/***	Oled2DrawGlyph()
**
**	Parameters:
**		char ch - character code to draw
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Renders the specified character into the display buffer
**	at the current character cursor location. Does not change
**	character cursor location or display buffer position.
**
*/

void Oled2DrawGlyph(char ch, BYTE clr) {

	BYTE *	pbFont;
	BYTE *	pbDisp;
	int	iy;
	int	ix;

	if((ch & 0x80) != 0) {
		return;
	}

	Oled2SetDrawColor(clr);

	/* Access User Font Definitions
	*/
	if(ch < chOled2UserMax) {
		pbFont = pbOled2FontUser + ch*cbOled2Char;
	}
	/* Access Pre-defined Font Definitions
	*/
	else if((ch & 0x80) == 0) {
		pbFont = pbOled2FontCur + (ch-chOled2UserMax) * cbOled2Char;
	}

	pbDisp = pbOled2Cur;

	/* Written by Michael T. Alexander; "Rotates" the glyph
	** definition by 90 degrees and write GS Level nibble 
	** data to each byte in the display buffer. One byte of 
	** data in the display buffer contains GS Level data 
	** for two row-adjacent pixels on the display.
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
/***	Oled2AdvanceCursor()
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
**		Advance the character cursor by one character location.
**	wrap at the end of a line, and back to the top at the end
**	of the display.
**
*/

void Oled2AdvanceCursor() {
	
	xchOled2Cur++;
	if(xchOled2Cur >= xchOled2Max) {
		xchOled2Cur = 0;
		ychOled2Cur++;
	}

	if (ychOled2Cur >= ychOled2Max) {
		ychOled2Cur = 0;
	}

	Oled2SetCursor(xchOled2Cur, ychOled2Cur);

}