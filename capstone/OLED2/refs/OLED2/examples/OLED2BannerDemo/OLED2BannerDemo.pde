
/************************************************************************/
/*									*/
/*  OLED2BannerDemo  --  OLED2 Display demo                             */
/*									*/
/************************************************************************/
/*  Author: 	Jordan Retz 						*/
/*  Copyright 2011, Digilent Inc.					*/
/************************************************************************/
/*
  This program is free software; you can redistribute it and/or
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
/*  Module Description: 						*/
/*									*/
/*  This program demonstrates the basic operation of the OLED2 graphics  */
/*  display.  It illustrates the  initialization of the display and     */
/*  some basic character and graphic operations.                        */
/*									*/
/************************************************************************/
/*  Revision History:							*/
/*									*/
/*  07/12/2012(JordanR): created                                        */
/*									*/
/************************************************************************/

/************************************************************************/
/*  Board Support:							*/
/*				      Connect J1/J2 on the PmodOLED2 to	*/
/*                                    the Pmod headers shown below,     */
/*                                    depending on your board           */
/*                                   ---------------------------------  */
/*  chipKit Uno with Pmod Shield:     Header JA/JB               	*/
/*   (Note: To use this, download and deploy the Board Variant for      */
/*          chipKIT UNO32 W/Pmod Shield from the Digilent website)      */              
/*  Cerebot Mx3ck:                    Header JA/JB                      */
/*  Cerebot Mx4ck:                    Header JA/JB                      */
/*  Cerebot Mx7ck:                    Header JB/JC                      */
/*                                                                      */
/*  Press BTN1 to start the demo                                        */
/************************************************************************/

/* ------------------------------------------------------------ */
/*		Include File Definitions	                */
/* ------------------------------------------------------------ */

#include <OLED2.h>
#include <GenericTypeDefs.h>

#include "Board_Defs.h"
#include "Oled2Driver.h"

/* ------------------------------------------------------------ */
/*		Local Type Definitions	                	*/
/* ------------------------------------------------------------ */
/* Coordinates of corners for the Digilent Logo
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
/*                Global Variables                              */
/* ------------------------------------------------------------ */
Oled2Class OLED2;

/* ------------------------------------------------------------ */
/*                Forward Declarations                          */
/* ------------------------------------------------------------ */
void DrawLogo(byte clr);

/* ------------------------------------------------------------ */
/*                Procedure Definitions                         */
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/***  void setup()
**
**  Parameters:
**    none
**
**  Return Value:
**    none
**
**  Errors:
**    none
**
**  Description:
**    Initialize the system.
*/

void setup() {
  
#if defined PIN_BTN1
    pinMode(PIN_BTN1, INPUT);
#endif
  
}

/* ------------------------------------------------------------ */
/***  void loop();
**
**  Parameters:
**    none
**
**  Return Value:
**    none
**
**  Errors:
**    none
**
**  Description:
**    Main application loop.
*/

void loop() {
    
  int ib;
  char sz1[] = {'P', 'm', 'o', 'd', 'O', 'L', 'E', 'D', '2', '\0'};
  char sz2[] = {'B', 'y', ' ', 'D', 'i', 'g', 'i', 'l', 'e', 'n', 't', '\0'};

  /* Init OLED2
  ** Protect the OLED2 by calling the begin() function
  ** after ending. It will contain the correct power on sequence
  */
#ifdef PIN_BTN1
  while(digitalRead(PIN_BTN1) == LOW);
#endif  
  OLED2.begin(); 
  
  /* Surrounding Fill
  */
  OLED2.setDrawColor(15);

  /* Draw a border on the display
  */
  OLED2.moveTo(0,0);
  OLED2.drawRect(ccolOled2Max - 1, crowOled2Max - 1);

  /* Turn all pixels on
  */
  OLED2.moveTo(0,0);
  OLED2.setFillPattern(OLED2.getStdPattern(1));
  OLED2.drawFillRect(ccolOled2Max - 1, crowOled2Max - 1);

  delay(1000);

  /* This loop starts with all pixels on at the highest
  ** gray scale level. Then it fades out, leaving only the
  ** Digilent logo.
  */
  for(ib = 15; ib >= 0; ib--) {
	
    OLED2.setDrawColor(ib);
	
    OLED2.moveTo(0,0);
    OLED2.setFillPattern(OLED2.getStdPattern(1));
    OLED2.drawFillRect(ccolOled2Max - 1, crowOled2Max - 1);

    OLED2.setDrawColor(15);
	
    OLED2.moveTo(0,0);
    OLED2.drawRect(ccolOled2Max - 1, crowOled2Max - 1);

    DrawLogo(15);

    OLED2.updateDisplay();

    delay(100);
  }	

  delay(1500);

  /* Fade the Logo slightly
  */
  for(ib = 15; ib > 7; ib--) {
    DrawLogo(ib);

    OLED2.updateDisplay();
  }

  /* Draw Banner String
  */
  OLED2.setCursor(12, 2);

  OLED2.putString(sz1, 15);

  OLED2.setCursor(11, 4);

  OLED2.putString(sz2, 15);


  /* Update Display
  */
  OLED2.updateDisplay();

  delay(1000);

  /* Clear Fill Pattern
  */
  OLED2.setFillPattern(OLED2.getStdPattern(0));	

  /* Swipe Away Banner
  */
  for(ib = crowOled2Max - 1; ib >= 0; ib--) {

    OLED2.moveTo(0, 0);
    OLED2.drawRect(ccolOled2Max - 1, ib);

    /* The drawFillRect is set as a blank rectangle. As it
    ** is drawn it will cover the Banner and appear to
    ** swipe it away
    */ 
    if(ib < crowOled2Max - 1) {
      OLED2.moveTo(0, crowOled2Max - 1);
      OLED2.drawFillRect(ccolOled2Max - 1, ib+1);
    }
    OLED2.updateDisplay();
  }

  OLED2.clear();       
  
  /* Protect the OLED2 by calling the end() function
  ** which will contain the correct power off sequence
  */
  OLED2.end();
  
#ifndef PIN_BTN1
  delay(2000); //give time to power off the board
#endif
}

/* ------------------------------------------------------------ */
/***  void DrawLogo();
**
**  Parameters:
**    clr - gray scale level
**
**  Return Value:
**  none
**
**  Errors:
**  none
**
**  Description:
**    Draws the Digilent Logo to the display buffer. 
*/

void DrawLogo(byte clr) {

  OLED2.setDrawColor(clr);

  /* Outline
  */
  OLED2.moveTo(xlogo1, ylogo1);

  OLED2.drawLine(xlogo2, ylogo2);

  OLED2.drawLine(xlogo4, ylogo4);

  OLED2.drawLine(xlogo6, ylogo6);

  OLED2.drawLine(xlogo5, ylogo5);

  OLED2.drawLine(xlogo3, ylogo3);

  OLED2.drawLine(xlogo1, ylogo1);

  OLED2.drawLine(xlogo11, ylogo11);

  OLED2.drawLine(xlogo10, ylogo10);

  OLED2.drawLine(xlogo7, ylogo7);

  OLED2.drawLine(xlogo5, ylogo5);

  OLED2.moveTo(xlogo10, ylogo10);

  OLED2.drawLine(xlogo9, ylogo9);

  OLED2.drawLine(xlogo8, ylogo8);

  OLED2.drawLine(xlogo12, ylogo12);

  OLED2.drawLine(xlogo4, ylogo4);

}
  
  

