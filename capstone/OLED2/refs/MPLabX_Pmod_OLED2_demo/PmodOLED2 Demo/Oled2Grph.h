/************************************************************************/
/*																		*/
/*	Oled2Grph.h	--	Graphics Function Declarations for PmodOLED2		*/
/*																		*/
/************************************************************************/
/*	Author: 	Jordan Retz												*/
/*	Copyright 2012, Digilent Inc.										*/
/************************************************************************/
/*  File Description: 													*/
/*		This file was adapted from OledGraph.h written by Gene 			*/
/*	Apperson for the PmodOLED.											*/
/*																		*/
/*		This file contains function declarations for the graphic "mode"	*/
/*	of the PmodOLED2.													*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	06/03/2011(GeneA): created PmodOLED graph file						*/
/*	06/19/2012(JordanR): Adapted from PmodOLED library					*/
/*																		*/
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <GenericTypeDefs.h>

/* ------------------------------------------------------------ */
/*				General Type Definitions						*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Procedure Declarations							*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

void	Oled2SetDrawColor(BYTE clr);
void	Oled2SetDrawMode(int mod);
int		Oled2GetDrawMode();
BYTE *	Oled2GetStdPattern(int ipat);
void	Oled2SetFillPattern(BYTE * pbPat);

void	Oled2MoveTo(int xco, int yco);
void	Oled2GetPos(int * pxco, int * pyco);
void	Oled2DrawPixel();
BYTE	Oled2GetPixel();
void	Oled2LineTo(int xco, int yco);
void	Oled2DrawRect(int xco, int yco);
void	Oled2FillRect(int xco, int yco);
void	Oled2GetBmp(int dxco, int dyco, BYTE * pbBmp);
void	Oled2PutBmp(int dxco, int dyco, BYTE * pbBmp);
void	Oled2PutBmpFlipped(int dxco, int dyco, BYTE * pbBits);
void	Oled2DrawChar(char ch, BYTE clr);
void	Oled2DrawString(char * sz, BYTE clr);