/************************************************************************/
/*																		*/
/*	Oled2Char.h	--	Header File for Character Functionality in PmodOLED2*/
/*																		*/
/************************************************************************/
/*	Author: 	Jordan Retz												*/
/*	Copyright 2012, Digilent Inc.										*/
/************************************************************************/
/*  File Description: 													*/
/*		This module was adapted from OledChar.h written by Gene Apperson*/
/*	for the PmodOLED.													*/
/*																		*/
/*		This file contains function declarations for the character		*/
/*	"mode" of the PmodOLED2.											*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	06/01/2011(GeneA): created PmodOLED char file						*/
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

void	Oled2SetCursor(int xch, int ych);
void	Oled2GetCursor(int * pxcy, int * pych);
BOOL	Oled2DefUserChar(char ch, BYTE * pbDef);
void	Oled2SetCharUpdate(BOOL f);
BOOL	Oled2GetCharUpdate();
void	Oled2PutChar(char ch, BYTE clr);
void	Oled2PutString(char * sz, BYTE clr);

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/***	TitleOfFunction()
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
**		
*/
