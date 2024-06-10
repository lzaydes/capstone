/************************************************************************/
/*																		*/
/*	Oled2Grph.c	--	OLED Display Graphics Routines						*/
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
/*	This module contains the implementation of the graphics functions	*/
/*	for the OLED display driver.										*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	06/03/2011(GeneA): created											*/
/*	07/5/2012(JordanR): adapted for the PmodOLED2						*/
/*																		*/
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

extern "C" {
	#include <p32xxxx.h>
	#include <plib.h>
	#include <inttypes.h>
}

#include "Oled2Driver.h"
#include "FillPat.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

extern int		xcoOled2Cur;
extern int		ycoOled2Cur;
extern uint8_t *	pbOled2Cur;
extern uint8_t		rgbOled2DispBuf[];
extern uint8_t		rgbFillPat[];
extern int		bnOled2Cur;
extern uint8_t		clrOled2Cur;
extern uint8_t *	pbOled2PatCur;
extern uint8_t	*	pbOled2FontUser;
extern uint8_t *	pbOled2FontCur;
extern int		dxcoOled2FontCur;
extern int		dycoOled2FontCur;

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

uint8_t	(*pfnDoRop)(uint8_t bPix, uint8_t bDsp, uint8_t mskPix);
int		modOled2Cur;

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

void	Oled2MoveDown();
void	Oled2MoveUp();
void	Oled2MoveRight();
void	Oled2MoveLeft();
uint8_t	Oled2RopSet(uint8_t bPix, uint8_t bDsp, uint8_t mskPix);
uint8_t	Oled2RopOr(uint8_t bPix, uint8_t bDsp, uint8_t mskPix);
uint8_t	Oled2RopAnd(uint8_t bPix, uint8_t bDsp, uint8_t mskPix);
uint8_t	Oled2RopXor(uint8_t bPix, uint8_t bDsp, uint8_t mskPix);
int		Oled2ClampXco(int xco);
int		Oled2ClampYco(int yco);

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
/***	Oled2MoveTo
**
**	Parameters:
**		xco			- x coordinate
**		yco			- y coordinate
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set the current graphics drawing position.
*/

void
Oled2MoveTo(int xco, int yco)
	{

	/* Clamp the specified coordinates to the display surface
	*/
	xco = Oled2ClampXco(xco);
	yco = Oled2ClampYco(yco);

	/* Save the current position
	*/
	xcoOled2Cur = xco;
	ycoOled2Cur = yco;

	/* Compute the display access paramters corresponding to
	** the specified position.
	*/
	pbOled2Cur = &rgbOled2DispBuf[((yco) * ccolOled2Max/2) + xco/2];
	bnOled2Cur = (xco & 0x1) ? 0 : 4;

}

/* ------------------------------------------------------------ */
/***	Oled2GetPos
**
**	Parameters:
**		pxco	- variable to receive x coordinate
**		pyco	- variable to receive y coordinate
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Fetch the current graphics drawing position
*/

void
Oled2GetPos(int * pxco, int * pyco)
	{

	*pxco = xcoOled2Cur;
	*pyco = ycoOled2Cur;

}

/* ------------------------------------------------------------ */
/***	Oled2SetDrawColor
**
**	Parameters:
**		clr		- drawing color to set
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set the foreground color used for pixel draw operations.
*/

void
Oled2SetDrawColor(uint8_t clr)
	{

	clrOled2Cur = clr & 0x0F;

}

/* ------------------------------------------------------------ */
/***	Oled2GetStdPattern
**
**	Parameters:
**		ipat		- index to standard fill pattern
**
**	Return Value:
**		returns a pointer to the standard fill pattern
**
**	Errors:
**		returns pattern 0 if index out of range
**
**	Description:
**		Return a pointer to the byte array for the specified
**		standard fill pattern.
*/

uint8_t *
Oled2GetStdPattern(int ipat)
	{

	return rgbFillPat + 8*ipat;

}

/* ------------------------------------------------------------ */
/***	Oled2SetFillPattern
**
**	Parameters:
**		pbPat	- pointer to the fill pattern
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set a pointer to the current fill pattern to use. A fill
**		pattern is an array of 8 bytes.
*/

void
Oled2SetFillPattern(uint8_t * pbPat)
	{

	pbOled2PatCur = pbPat;

}

/* ------------------------------------------------------------ */
/***	Oled2SetDrawMode
**
**	Parameters:
**		mod		- drawing mode to select
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set the specified mode as the current drawing mode.
*/

void
Oled2SetDrawMode(int mod)
	{

	modOled2Cur = mod;

	switch(mod) {
		case	modOled2Set:
			pfnDoRop = Oled2RopSet;
			break;

		case	modOled2Or:
			pfnDoRop = Oled2RopOr;
			break;

		case	modOled2And:
			pfnDoRop = Oled2RopAnd;
			break;

		case	modOled2Xor:
			pfnDoRop = Oled2RopXor;
			break;

		default:
			modOled2Cur = modOled2Set;
			pfnDoRop = Oled2RopSet;
	}

}

/* ------------------------------------------------------------ */
/***	Oled2GetDrawMode
**
**	Parameters:
**		none

**	Return Value:
**		returns current drawing mode
**
**	Errors:
**		none
**
**	Description:
**		Get the current drawing mode
*/

int
Oled2GetDrawMode()
	{

	return modOled2Cur;

}

/* ------------------------------------------------------------ */
/***	Oled2DrawPixel
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
**		Set the pixel at the current drawing location to the
**		specified value.
*/

void
Oled2DrawPixel()
	{

	*pbOled2Cur = (*pfnDoRop)((clrOled2Cur << bnOled2Cur), *pbOled2Cur, (0xF << bnOled2Cur));

}

/* ------------------------------------------------------------ */
/***	Oled2GetPixel
**
**	Parameters:
**		none
**
**	Return Value:
**		returns pixel value at current drawing location
**
**	Errors:
**		none
**
**	Description:
**		Return the value of the pixel at the current drawing location
*/

uint8_t
Oled2GetPixel()
	{

	return (*pbOled2Cur & (0xF << bnOled2Cur)) >> bnOled2Cur;

}

/* ------------------------------------------------------------ */
/***	Oled2LineTo
**
**	Parameters:
**		xco			- x coordinate
**		yco			- y coordinate
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Draw a line from the current position to the specified
**		position.
*/

void
Oled2LineTo(int xco, int yco)
	{
	int		err;
	int		del;
	int		lim;
	int		cpx;
	int		dxco;
	int		dyco;
	void	(*pfnMajor)();
	void	(*pfnMinor)();

	/* Clamp the point to be on the display.
	*/
	xco = Oled2ClampXco(xco);
	yco = Oled2ClampYco(yco);

	/* Determine which octant the line occupies
	*/
	dxco = xco - xcoOled2Cur;
	dyco = yco - ycoOled2Cur;
	if (abs(dxco) >= abs(dyco)) {
		/* Line is x-major
		*/
		lim = abs(dxco);
		del = abs(dyco);
		if (dxco >= 0) {
			pfnMajor = Oled2MoveRight;
		}
		else {
			pfnMajor = Oled2MoveLeft;
		}

		if (dyco >= 0) {
			pfnMinor = Oled2MoveDown;
		}
		else {
			pfnMinor = Oled2MoveUp;
		}
	}
	else {
		/* Line is y-major
		*/
		lim = abs(dyco);
		del = abs(dxco);
		if (dyco >= 0) {
			pfnMajor = Oled2MoveDown;
		}
		else {
			pfnMajor = Oled2MoveUp;
		}

		if (dxco >= 0) {
			pfnMinor = Oled2MoveRight;
		}
		else {
			pfnMinor = Oled2MoveLeft;
		}
	}

	/* Render the line. The algorithm is:
	**		Write the current pixel
	**		Move one pixel on the major axis
	**		Add the minor axis delta to the error accumulator
	**		if the error accumulator is greater than the major axis delta
	**			Move one pixel in the minor axis
	**			Subtract major axis delta from error accumulator
	*/
	err = lim/2;
	cpx = lim;
	while (cpx > 0) {
		Oled2DrawPixel();
		(*pfnMajor)();
		err += del;
		if (err > lim) {
			err -= lim;
			(*pfnMinor)();
		}
		cpx -= 1;
	}

	/* Update the current location variables.
	*/
	xcoOled2Cur = xco;
	ycoOled2Cur = yco;		

}

/* ------------------------------------------------------------ */
/***	Oled2DrawRect
**
**	Parameters:
**		xco		- x coordinate of other corner
**		yco		- y coordinate of other corner
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Draw a rectangle bounded by the current location and
**		the specified location.
*/

void
Oled2DrawRect(int xco, int yco)
	{
	int		xco1;
	int		yco1;

	/* Clamp the point to be on the display.
	*/
	xco = Oled2ClampXco(xco);
	yco = Oled2ClampYco(yco);

	xco1 = xcoOled2Cur;
	yco1 = ycoOled2Cur;
	Oled2LineTo(xco, yco1);
	Oled2LineTo(xco, yco);
	Oled2LineTo(xco1, yco);
	Oled2LineTo(xco1, yco1);
}

/* ------------------------------------------------------------ */
/***	Oled2FillRect
**
**	Parameters:
**		xco		- x coordinate of other corner
**		yco		- y coordinate of other corner
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Fill a rectangle bounded by the current location and
**		the specified location.
*/

void
Oled2FillRect(int xco, int yco)
	{
	int		xcoLeft;
	int		xcoRight;
	int		ycoTop;
	int		ycoBottom;
	int		ibPat;
	uint8_t *	pbCur;
	uint8_t *	pbLeft;
	int		xcoCur;
	uint8_t	bTmp;
	uint8_t	mskPat;
	uint8_t	bnPatCur;
	int		ib;


	/* Clamp the point to be on the display.
	*/
	xco = Oled2ClampXco(xco);
	yco = Oled2ClampYco(yco);

	/* Set up the four sides of the rectangle.
	*/
	if (xcoOled2Cur < xco) {
		xcoLeft = xcoOled2Cur;
		xcoRight = xco;
	}
	else {
		xcoLeft = xco;
		xcoRight = xcoOled2Cur;
	}

	if (ycoOled2Cur < yco) {
		ycoTop = ycoOled2Cur;
		ycoBottom = yco;
	}
	else {
		ycoTop = yco;
		ycoBottom = ycoOled2Cur;
	}

	/* Set Pattern byte index to pattern definition 
	** length - 1
	*/
	ibPat = ciptnVals - 1;

	while (ycoTop <= ycoBottom) {
		/* Compute the address of the left edge of the rectangle for this
		** stripe across the rectangle.
		*/
		pbLeft = &rgbOled2DispBuf[((ycoTop) * ccolOled2Max/2) + xcoLeft/2];									
	
		xcoCur = xcoLeft;
		pbCur = pbLeft;

		/* Set Pattern bit index to 7, pattern byte index
		** to pattern definition length - 1. This will rotate
		** the pattern definition by 90 degrees, if the pattern
		** was defined correctly. 
		*/
		ib = 7;

		/* Loop through all of the bytes horizontally making up this stripe
		** of the rectangle.
		*/
		while (xcoCur <= xcoRight) {
			bnPatCur = (xcoCur & 0x1) ? 0 : 4;
			if (*(pbOled2PatCur+ibPat) & (1 << ib)) {
				*pbCur = (*pfnDoRop)((clrOled2Cur << bnPatCur), *pbCur, (0xF << bnPatCur));
			}
			else {
				*pbCur = (*pfnDoRop)(0, *pbCur, (0xF << bnPatCur));
			}
		
			xcoCur += 1;
			pbCur += (bnPatCur == 0) ? 1 : 0;
			ib -= 1;
			if (ib < 0) {
				ib = 7;
			}
		}

		/* Advance to the next horizontal stripe.
		*/
		ycoTop += 1;
		ibPat--;
		if(ibPat < 0) {
			ibPat = ciptnVals - 1;
		}

	}

}

/* ------------------------------------------------------------ */
/***	Oled2GetBmp
**
**	Parameters:
**		dxco		- width of bitmap
**		dyco		- height of bitmap
**		pbBits		- pointer to the bitmap bits	
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This routine will get the bits corresponding to the
**		rectangle implied by the current location and the
**		specified width and height. The buffer specified
**		by pbBits must be large enough to hold the resulting
**		bytes.
*/

void
Oled2GetBmp(int dxco, int dyco, uint8_t * pbBits)
	{
	int		xcoLeft;
	int		xcoRight;
	int		ycoTop;
	int		ycoBottom;
	uint8_t *	pbDspCur;
	uint8_t *	pbDspLeft;
	uint8_t *	pbBmpCur;
	uint8_t *	pbBmpLeft;
	int		xcoDispCur;
	int		xcoBmpCur;
	uint8_t	mskDisp;
	uint8_t	mskBmp;
	int		bnDisp;
	int		bnBmp;
	uint8_t	clrOled2Temp;

	/* Set up the four sides of the source rectangle.
	*/
	xcoLeft = xcoOled2Cur;
	xcoRight = xcoLeft + dxco;
	if (xcoRight >= ccolOled2Max) {
		xcoRight = ccolOled2Max - 1;
	}

	ycoTop = ycoOled2Cur;
	ycoBottom = ycoTop + dyco;
	if (ycoBottom >= crowOled2Max) {
		ycoBottom = crowOled2Max - 1;
	}

	pbDspLeft = &rgbOled2DispBuf[((ycoTop) * ccolOled2Max/2) + xcoLeft/2];
	pbBmpLeft = pbBits;

	while (ycoTop <= ycoBottom) {
											
		xcoDispCur = xcoLeft;
		xcoBmpCur = 0;
		pbDspCur = pbDspLeft;
		pbBmpCur = pbBmpLeft;

		/* Loop through all of the bytes horizontally making up this stripe
		** of the rectangle.
		*/
		while (xcoDispCur <= xcoRight) {
			bnDisp = (xcoDispCur & 0x1) ? 0 : 4;
			bnBmp = (xcoBmpCur & 0x1) ? 0 : 4;

			mskDisp = 0xF << bnDisp;
			mskBmp = 0xF << bnBmp;

			clrOled2Temp = ((*pbDspCur) & mskDisp) >> bnDisp;

			*pbBmpCur &= ~mskBmp;

			*pbBmpCur |= clrOled2Temp << bnBmp;

			xcoDispCur += 1;
			xcoBmpCur += 1;
			pbBmpCur += (bnBmp == 0) ? 1 : 0;
			pbDspCur += (bnDisp == 0) ? 1 : 0;
		}

		/* Advance to the next horizontal stripe.
		*/
		ycoTop += 1;
		pbDspLeft += ccolOled2Max/2;
		pbBmpLeft += dxco/2 + (dxco & 0x1 ? 1 : 0);

	}

}

/* ------------------------------------------------------------ */
/***	Oled2PutBmp
**
**	Parameters:
**		dxco		- width of bitmap
**		dyco		- height of bitmap
**		pbBits		- pointer to the bitmap bits	
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This routine will put the specified bitmap into the display
**		buffer at the current location.
*/

void
Oled2PutBmp(int dxco, int dyco, uint8_t * pbBits)
	{
	int		xcoLeft;
	int		xcoRight;
	int		ycoTop;
	int		ycoBottom;
	uint8_t *	pbDspCur;
	uint8_t *	pbDspLeft;
	uint8_t *	pbBmpCur;
	uint8_t *	pbBmpLeft;
	int		xcoDispCur;
	uint8_t	mskDisp;
	uint8_t	mskBmp;
	int		bnDisp;
	int		bnBmp;
	uint8_t	clrOled2Temp;
	int		xcoBmpCur;

	/* Set up the four sides of the destination rectangle.
	*/
	xcoLeft = xcoOled2Cur;
	xcoRight = xcoLeft + dxco;
	if (xcoRight >= ccolOled2Max) {
		xcoRight = ccolOled2Max - 1;
	}

	ycoTop = ycoOled2Cur;
	ycoBottom = ycoTop + dyco;
	if (ycoBottom >= crowOled2Max) {
		ycoBottom = crowOled2Max - 1;
	}


	pbDspLeft = &rgbOled2DispBuf[((ycoTop) * ccolOled2Max/2) + xcoLeft/2];
	pbBmpLeft = pbBits;
	clrOled2Temp = clrOled2Cur;

	while (ycoTop <= ycoBottom) {
						
		xcoDispCur = xcoLeft;
		xcoBmpCur = 0;
		pbDspCur = pbDspLeft;
		pbBmpCur = pbBmpLeft;

		/* Loop through all of the bytes horizontally making up this stripe
		** of the rectangle.
		*/
		while (xcoDispCur <= xcoRight) {
			bnDisp = (xcoDispCur & 0x1) ? 0 : 4;
			bnBmp = (xcoBmpCur & 0x1) ? 0 : 4;

			mskDisp = 0xF << bnDisp;
			mskBmp = 0xF << bnBmp;

			clrOled2Temp = (*pbBmpCur & mskBmp) >> bnBmp;

			*pbDspCur = (*pfnDoRop)((clrOled2Temp << bnDisp), *pbDspCur, mskDisp);
			xcoDispCur += 1;
			xcoBmpCur += 1;
			pbBmpCur += (bnBmp == 0) ? 1 : 0;
			pbDspCur += (bnDisp == 0) ? 1 : 0;
		}	

		/* Advance to the next horizontal stripe.
		*/
		ycoTop += 1;
		pbDspLeft += ccolOled2Max/2;
		pbBmpLeft += dxco/2 + (dxco & 0x1 ? 1 : 0);

	}

}

/* ------------------------------------------------------------ */
/***	Oled2PutBmpFlipped
**
**	Parameters:
**		dxco		- width of bitmap
**		dyco		- height of bitmap
**		pbBits		- pointer to the bitmap bits	
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This routine will put the specified bitmap, flipped about the
**	vertical axis, into the display	buffer at the current location.
*/

void Oled2PutBmpFlipped(int dxco, int dyco, uint8_t * pbBits) {

	int		xcoLeft;
	int		xcoRight;
	int		ycoTop;
	int		ycoBottom;
	uint8_t *	pbDspCur;
	uint8_t *	pbDspRight;
	uint8_t *	pbBmpCur;
	uint8_t *	pbBmpLeft;
	int		xcoDispCur;
	uint8_t	mskDisp;
	uint8_t	mskBmp;
	int		bnDisp;
	int		bnBmp;
	uint8_t	clrOled2Temp;
	int		xcoBmpCur;

	/* Set up the four sides of the destination rectangle.
	*/
	xcoLeft = xcoOled2Cur;
	xcoRight = xcoLeft + dxco;
	if (xcoRight >= ccolOled2Max) {
		xcoRight = ccolOled2Max - 1;
	}

	ycoTop = ycoOled2Cur;
	ycoBottom = ycoTop + dyco;
	if (ycoBottom >= crowOled2Max) {
		ycoBottom = crowOled2Max - 1;
	}


	pbDspRight = &rgbOled2DispBuf[((ycoTop) * ccolOled2Max/2) + xcoRight/2];
	pbBmpLeft = pbBits;
	clrOled2Temp = clrOled2Cur;

	while (ycoTop <= ycoBottom) {
						
		xcoDispCur = xcoRight;
		xcoBmpCur = 0;
		pbDspCur = pbDspRight;
		pbBmpCur = pbBmpLeft;

		/* Loop through all of the bytes horizontally making up this stripe
		** of the rectangle.
		*/
		while (xcoDispCur >= xcoLeft) {
			bnDisp = (xcoDispCur & 0x1) ? 0 : 4;
			bnBmp = (xcoBmpCur & 0x1) ? 0 : 4;

			mskDisp = 0xF << bnDisp;
			mskBmp = 0xF << bnBmp;

			clrOled2Temp = (*pbBmpCur & mskBmp) >> bnBmp;

			*pbDspCur = (*pfnDoRop)((clrOled2Temp << bnDisp), *pbDspCur, mskDisp);
			xcoDispCur -= 1;
			xcoBmpCur += 1;
			pbBmpCur += (bnBmp == 0) ? 1 : 0;
			pbDspCur -= (bnDisp == 4) ? 1 : 0;
		}	

		/* Advance to the next horizontal stripe.
		*/
		ycoTop += 1;
		pbDspRight += ccolOled2Max/2;
		pbBmpLeft += dxco/2 + (dxco & 0x1 ? 1 : 0);

	}

}

/* ------------------------------------------------------------ */
/***	Oled2DrawChar
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
Oled2DrawChar(char ch, uint8_t clr)
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

	xcoOled2Cur += dxcoOled2FontCur;

	Oled2MoveTo(xcoOled2Cur, ycoOled2Cur);

}

/* ------------------------------------------------------------ */
/***	Oled2DrawString
**
**	Parameters:
**		sz		- pointer to the null terminated string
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
Oled2DrawString(char * sz, uint8_t clr)
	{

	while (*sz != '\0') {
		Oled2DrawChar(*sz, clr);
		sz += 1;
	}
}

/* ------------------------------------------------------------ */
/*				Internal Support Routines						*/
/* ------------------------------------------------------------ */
/***	Oled2RopSet
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

uint8_t
Oled2RopSet(uint8_t bPix, uint8_t bDsp, uint8_t mskPix)
	{

	return (bDsp & ~mskPix) | (bPix & mskPix);

}

/* ------------------------------------------------------------ */
/***	Oled2RopOr
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

uint8_t
Oled2RopOr(uint8_t bPix, uint8_t bDsp, uint8_t mskPix)
	{

	return bDsp | (bPix & mskPix);

}

/* ------------------------------------------------------------ */
/***	Oled2RopAnd
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

uint8_t
Oled2RopAnd(uint8_t bPix, uint8_t bDsp, uint8_t mskPix)
	{

	return bDsp & (bPix & mskPix);

}

/* ------------------------------------------------------------ */
/***	Oled2RopXor
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

uint8_t
Oled2RopXor(uint8_t bPix, uint8_t bDsp, uint8_t mskPix)
	{

	return bDsp ^ (bPix & mskPix);

}

/* ------------------------------------------------------------ */
/***	Oled2MoveUp
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
**		Updates global variables related to current position on the
**		display.
*/

void
Oled2MoveUp()
	{

	/* Stay at bit position, but move up one row
	** 256 columns, 2 columns = 1 byte
	*/
	pbOled2Cur -= ccolOled2Max/2;

	/* If we have gone off the top of the display,
	** go back down.
	*/
	if(pbOled2Cur < rgbOled2DispBuf) {
		pbOled2Cur += ccolOled2Max/2;
	}
}

/* ------------------------------------------------------------ */
/***	Oled2MoveDown
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
**		Updates global variables related to current position on the
**		display.
*/

void
Oled2MoveDown()
	{

	/* Stay at bit position, but move down one row
	** 256 columns, 2 columns = 1 byte
	*/
	pbOled2Cur += ccolOled2Max/2;

	/* If we have gone off the bottom of the display,
	** go back up.
	*/
	if(pbOled2Cur >= rgbOled2DispBuf + cbOled2DispMax) {
		pbOled2Cur -= ccolOled2Max/2;
	}
}

/* ------------------------------------------------------------ */
/***	Oled2MoveLeft
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
**		Updates global variables related to current position on the
**		display.
*/

void
Oled2MoveLeft()
	{

	/* Are we at the left edge of the display already?
	*/
	if (((pbOled2Cur - rgbOled2DispBuf) & (ccolOled2Max-1)) == 0 && bnOled2Cur == 4) {
		return;
	}

	/* Go left one pixel position (4 bits) in the current byte.
	*/
	bnOled2Cur += 4;

	/* If we have gone off the end of the current byte
	** move to next byte
	*/
	if (bnOled2Cur > 4) {
		bnOled2Cur = 0;
		pbOled2Cur -= 1;

		/* If we have gonne off the top left
		** of the display, move back
		*/
		if(pbOled2Cur < rgbOled2DispBuf) {
			pbOled2Cur += 1;
		}
	}

}

/* ------------------------------------------------------------ */
/***	Oled2MoveRight
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
**		Updates global variables related to current position on the
**		display.
*/

void
Oled2MoveRight()
	{

	/* Are we at the right edge of the display already?
	*/
	if (((pbOled2Cur-rgbOled2DispBuf) & (ccolOled2Max-1)) == (ccolOled2Max-1) && bnOled2Cur == 0) {
		return;
	}

	/* Go left one pixel position (4 bits) in the current byte.
	*/
	bnOled2Cur -= 4;

	/* If we have gone off the end of the current byte
	** move to next byte
	*/
	if (bnOled2Cur < 0) {
		bnOled2Cur = 4;
		pbOled2Cur += 1;

		/* If we have gonne off the bottom right 
		** of the display, move back
		*/
		if(pbOled2Cur >= rgbOled2DispBuf + cbOled2DispMax) {
			pbOled2Cur -= 1;
		}
	}

}

/* ------------------------------------------------------------ */
/***	Oled2ClampXco
**
**	Parameters:
**		xco		- x value to clamp
**
**	Return Value:
**		Returns clamped x value
**
**	Errors:
**		none
**
**	Description:
**		This routine forces the x value to be on the display.
*/

int
Oled2ClampXco(int xco)
	{
	if (xco < 0) {
		xco = 0;
	}
	if (xco >= ccolOled2Max) {
		xco = ccolOled2Max-1;
	}

	return xco;

}

/* ------------------------------------------------------------ */
/***	Oled2ClampYco
**
**	Parameters:
**		yco		- y value to clamp
**
**	Return Value:
**		Returns clamped y value
**
**	Errors:
**		none
**
**	Description:
**		This routine forces the y value to be on the display.
*/

int
Oled2ClampYco(int yco)
	{
	if (yco < 0) {
		yco = 0;
	}
	if (yco >= crowOled2Max) {
		yco = crowOled2Max-1;
	}

	return yco;

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

