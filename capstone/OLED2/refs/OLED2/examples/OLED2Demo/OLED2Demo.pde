
/************************************************************************/
/*									*/
/*  OLED2Demo  --  OLED2 Display demo                                     */
/*									*/
/************************************************************************/
/*  Author: 	Oliver Jones						*/
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
/*  06/01/2011(GeneA): created						*/
/*  08/04/2011(GeneA): prepare first release				*/
/*  07/10/2012(JordanR): adapted for PmodOLED2                          */
/*									*/
/************************************************************************/

/************************************************************************/
/*  Board Support:							*/
/*									*/
/*  chipKit Uno with Pmod Shield:     Header JA	                	*/
/*   (Note: To use this, download and deploy the Board Variant for      */
/*          chipKIT UNO32 W/Pmod Shield from the Digilent website)      */              
/*  Cerebot Mx3ck:                    Header JA                         */
/*  Cerebot Mx4ck:                    Header JA                         */
/*  Cerebot Mx7ck:                    Header JB                         */
/*                                                                      */
/*  Press BTN1 to start demo                                            */
/************************************************************************/

#include <OLED2.h>

Oled2Class OLED2;

void setup()
{
  pinMode(42, INPUT);
  pinMode(43, INPUT);
}

void loop()
{
  int irow;
  int ib;
  int c;
  
  /* Init OLED2
  ** Protect the OLED2 by calling the begin() function
  ** after ending. It will contain the correct power on sequence
  ** press button 1 on the MX4cK
  */
  while(digitalRead(42) == LOW);
  
  OLED2.begin(); 
  c = 0;

  //Clear the virtual buffer
  OLED2.clearBuffer();
  
  //Chosing Fill pattern 0
  OLED2.setFillPattern(OLED2.getStdPattern(0));
  //Turn automatic updating off
  OLED2.setCharUpdate(0);
  
  //Draw a rectangle over wrting then slide the rectagle
  //down slowly displaying all writing
  for (irow = 0; irow < OLED2.rowMax; irow++)
  {
    OLED2.clearBuffer();
    OLED2.setCursor(0, 0);
    OLED2.putString("PmodOLED2", 15);
    OLED2.setCursor(0, 1);
    OLED2.putString("by Digilent", 15);
    OLED2.setCursor(0, 2);
    OLED2.putString("Simple Demo", 15);
    
    OLED2.moveTo(0, irow);
    OLED2.drawFillRect(127,31);
    OLED2.moveTo(0, irow);
    OLED2.drawLine(127,irow);
    OLED2.updateDisplay();
    delay(100);
  }
  
  delay(1000);
  
  // Blink the display a bit.
  OLED2.displayOff();
  delay(500);
  OLED2.displayOn();
  delay(500);
  
  OLED2.displayOff();
  delay(500);
  OLED2.displayOn();
  delay(500);

  OLED2.displayOff();
  delay(500);
  OLED2.displayOn();
  delay(500);

  delay(2000);
  
  // Now erase the characters from the display
  for (irow = OLED2.rowMax-1; irow >= 0; irow--) {
    OLED2.setDrawColor(15);
    OLED2.setDrawMode(OLED2.modeSet);
    OLED2.moveTo(0,irow);
    OLED2.drawLine(127,irow);
    OLED2.updateDisplay();
    delay(25);
    OLED2.setDrawMode(OLED2.modeXor);
    OLED2.moveTo(0, irow);
    OLED2.drawLine(127, irow);
    OLED2.updateDisplay();
  }
  
  delay(1000);  

  // Draw a rectangle in center of screen
  // Display the 8 different patterns availible
  OLED2.setDrawMode(OLED2.modeSet);

  for(ib = 1; ib < 8; ib++)
  {
    OLED2.clearBuffer();
    
    OLED2.setFillPattern(OLED2.getStdPattern(ib));
    OLED2.moveTo(0, 0);
    OLED2.drawFillRect(76, 27);
    OLED2.drawRect(76, 27);
    OLED2.updateDisplay();
    
    delay(1000);
  }

  OLED2.clear();       
  
  /* Protect the OLED2 by calling the end() function
  ** which will contain the correct power off sequence
  ** press button 2 on the MX4cK
  */
  while(c = 0) {
     if(digitalRead(43) == HIGH) {
        OLED2.end();
        c = 1;
     } 
  }
}
