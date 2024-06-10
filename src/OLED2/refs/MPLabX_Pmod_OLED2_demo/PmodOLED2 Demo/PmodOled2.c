/************************************************************************/
/*																		*/
/*	PmodOled2.c	--	PmodOLED2 Driver Function Definitions				*/
/*																		*/
/************************************************************************/
/*	Author: 	Jordan Retz												*/
/*	Copyright 2012, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*		This module was adapted from PmodOLED.c written by Gene 		*/
/*	Apperson for the PmodOLED.											*/
/*																		*/
/*		This module contains driver function definitions for the 		*/
/*	PmodOLED2 library. It also contains hardcoded command values for	*/
/*	communicating the the display on the PmodOLED2.						*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	04/29/2011(GeneA): created PmodOLED driver module					*/
/*	06/19/2012(JordanR): Adapted from the PmodOLED library.				*/
/*																		*/
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <plib.h>
#include <GenericTypeDefs.h>

#include "PmodOled2.h"
#include "Oled2Char.h"
#include "Oled2Grph.h"
#include "FillPat.h"

#include "delay.h"

/* ------------------------------------------------------------ */
/*				General Type Definitions						*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

/* PmodOLED2 Display Controller Command Definitions
** A[7:0] = first data byte sent after command (on D[7:0] pins)
** B[7:0] = second data byte sent after command (on D[7:0] pins)
*/
#define cmdGrayScaleEnable		0x00	
#define cmdSetColumnAddress		0x15	//A[6:0] start address, B[6:0] end address
#define cmdWriteRAM				0x5C	
#define cmdReadRAM				0x5D	
#define cmdSetRowAddress		0x75	//A[6:0] start address, B[6:0] end address
#define cmdSetRemap				0xA0
#define cmdSetDisplayStartLine	0xA1
#define cmdSetDisplayOffset		0xA2	//A[6:0] sets vertical scroll
#define cmdSetDisplayModeOff	0xA4	//all pixels off at GS level 0
#define cmdSetDisplayModeOn		0xA5	//all pixels on at GS level 15
#define cmdSetDisplayModeNorm	0xA6	//reset
#define cmdSetDisplayModeInv	0xA7	//Invert Gray Scale display settings (GS0 = GS15)
#define cmdEnablePartialDisp	0xA8	//A[6:0] start address, B[6:0] end address
#define cmdDisablePartialDisp	0xA9
#define cmdFunctionSelect		0xAB	//A[0]=0 => select external Vdd, A[0] = 1 => Enable internal Vdd regulator [reset]
#define cmdSleep				0xAE	//display off
#define cmdWake					0xAF	//display on
#define cmdPhaseLength			0xB1	//A[3:0] is phase 1 period, A[7:4] is phase 2 period
#define cmdClkDividerOscFreq	0xB3	//A[3:0] divide  DCLK, A[7:4] set oscillator frequency
#define cmdDisplayEnhance		0xB4	//no external VSL (A[1:0] = 00), B[7:3] sets enchancement
#define cmdSetGPIO				0xB5	//A[3:2] = GPIO 0 , A[1:0] = GPIO 1 
#define cmdSetSecondPreCharge	0xB6	//A[3:0] sets the second pre charge period
#define cmdSetGrayScaleTable	0xB8	//An[7:0] where n = 1, 2, 3, .., 15. A1[7:0] sets GS1. Levels range from 0-180 dec as pulse width of DCLK
#define cmdSetDefaultGSTable	0xB9	//Sets the Gray Scale table to default values. Don't use
#define cmdSetPreChargeVoltage	0xBB	//A[4:0] sets precharge level as ( .2 (00000) to .6 (11111) )*Vcc
#define cmdSetVcomh				0xBE	//A[2:0] set Vcomh as ( .72 (000) to .86 (111) )*Vcc
#define cmdSetContrastCurrent	0xC1	//A[7:0]
#define	cmdMasterContrastCtrl	0xC7	//A[3:0] reduce output currents of all colors to n/16 n= 1, 2, ..., 15
#define cmdSetMuxRatio			0xCA	//A[6:0] sets MUX ratio from 16MUX (15d) to 128MUX (127d)
#define cmdDisplayEnhanceB1		0xD1	//A[5:4] and then send B2
#define cmdDisplayEnhanceB2		0x20
#define cmdSetLock				0xFD	//A[2] = 1, lock OLED controller from accepting command

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

/* Originally declared in CharFont0.c
*/
extern BYTE		rgbOled2Font0[];

/* Originally declared in OledChar.c
*/
extern BYTE		rgbOled2FontUser[];

extern int		xchOled2Max;
extern int		ychOled2Max;

/* Originally declared in OledFillPat.c
*/
extern BYTE		rgbFillPat[];

/* PmodOled2.c Global Variable Declarations
*/
BYTE rgbOled2GSTable[] = { oled2GS1, oled2GS2, oled2GS3, oled2GS4,
						   oled2GS5, oled2GS6, oled2GS7, oled2GS8, 
						   oled2GS9, oled2GS10, oled2GS11, oled2GS12,
						   oled2GS13, oled2GS14, oled2GS15 };

int		xcoOled2Cur;
int		ycoOled2Cur;

BYTE *	pbOled2Cur;			// address of byte corresponding to current location in buffer

BYTE	clrOled2Cur;		// drawing color to use.. 0x01-0x0F
BYTE *	pbOled2PatCur;		// current fill pattern
int		bnOled2Cur;			// bit number of byte corresponding to current location. Pixel
							// defs are 4 bits, so bnOled2Cur is 0 (left pixel) or 1 (right pixel)

BOOL	fOled2CharUpdate;

int		dxcoOled2FontCur;
int		dycoOled2FontCur;

BYTE *	pbOled2FontCur;
BYTE *	pbOled2FontUser;

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

/* This array is the offscreen frame buffer used for rendering.
** Display data is rendered into this offscreen buffer and then
** copied to the display.
*/
BYTE	rgbOled2DispBuf[cbOled2DispMax];

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

/* OLED2 Initialization and Termination
*/
void	Oled2HostInit();
void	Oled2DvrInit();
void	Oled2DevInit();
void	Oled2DispInit();

void	Oled2HostTerm();
void	Oled2DevTerm();

/* Parallel Master Port Communication
*/
void	PMPSendCmd(BYTE cmd);
void	PMPSendData(BYTE data);
void	PMPSendDataBuffer(const BYTE * pbBuf, WORD cbBuf);

/* ------------------------------------------------------------ */
/*				Procedure Declarations							*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/***	Oled2Init()
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
**		Initializes the OLED2 Driver and Display with the
**	correct procedure
*/

void Oled2Init() {
	
	/* Initialize the Parallel Master Port on the PIC32 to communicate
	** with the display
	*/
	Oled2HostInit();

	/* Initialize the memory variables to default settings
	*/
	Oled2DvrInit();

	/* Initialize the OLED2 display hardware
	*/
	Oled2DevInit();

	/* Clear the Display
	*/
	Oled2Clear();

}

/* ------------------------------------------------------------ */
/***	Oled2HostInit()
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
**		Set up the PIC32 device to communicate and drive the OLED2 display
**	The PmodOLED2 is set up to use 8080 Parallel Interface Communication Protocol.
**	Therefore, the Parallel Master Port on the PIC32 will be used.
**	
*/

void Oled2HostInit() {

	DelayInit();

	//Set up PMP in Master Mode 2
	IEC1CLR = 0x0004; 		// Disable PMP interrupt
	PMCONbits.ON = 0b0; 	//reset and disable PMP module
	PMCONSET = 0x0300;		//PMCON setup, no address multiplexing, PMRD/WR enabled, WR/RD strobes active low
	PMMODE = 0x02FF; 		//PMMODE setup, Master mode 2, WAITB = 4 * TPB, WAITM = 16 * TPB, WAITE = 4*TPB
	PMAENSET = 0x0000; 		//PMAEN setup, PMA<15:0> function as I/O pins
	PMCONbits.ON = 0b1; 	//enable PMP Master

	//Set power control pins as output and set supplies off
	PORTSetPinsDigitalOut(prtVddCtrl, bitVddCtrl);		//Vdd power control (0=off)
	PORTSetPinsDigitalOut(prtVccCtrl, bitVccCtrl);		//Vcc power control (0=off)
	PORTClearBits(prtVddCtrl, bitVddCtrl);
	PORTClearBits(prtVccCtrl, bitVccCtrl);


	//Set D/C, RES, R/W, CS, and RD as outputs
	PORTSetPinsDigitalOut(prtDataCmd, bitDataCmd);		//Data/Command# (D/C#)
	PORTSetBits(prtDataCmd, bitDataCmd);				//Configured as Data bus, clear bit for command bus
	PORTSetPinsDigitalOut(prtReset, bitReset);			//Reset# (RES#)
	PORTSetBits(prtReset, bitReset);					//When pulled low, the chip is reset		
	PORTSetPinsDigitalOut(prtSelect, bitSelect);		//ChipSelect# (CS#)
	PORTSetBits(prtSelect, bitSelect);					//display can communicate only when CS is low		
	PORTSetPinsDigitalOut(prtRead, bitRead);			//Read# (RD#)		
	PORTSetBits(prtRead, bitRead);						//Data read operation not initialized
	PORTSetPinsDigitalOut(prtWrite, bitWrite);			//Write# (WR#)				
	PORTSetBits(prtWrite, bitWrite);					//Data write operation not initialized

}

/* ------------------------------------------------------------ */
/***	Oled2DvrInit()
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
**		Initialize the OLED2 software system
**	
*/

void Oled2DvrInit() {

	WORD	ib;

	/* Initialize the parameters for the default font
	*/
	dxcoOled2FontCur = cbOled2Char;
	dycoOled2FontCur = 8;
	pbOled2FontCur = rgbOled2Font0;		// rgbOledFont0 defined in CharFont0.c
	pbOled2FontUser = rgbOled2FontUser;	// rgbOled2FontUser defined in Oled2Char.c
	
	/* Reset the user defined characters
	*/
	for( ib = 0; ib < cbOled2Char; ib++) {
		rgbOled2FontUser[ib] = 0;
	}

	xchOled2Max = ccolOled2Max / dxcoOled2FontCur;	// max characters due to x display limitations
	ychOled2Max = crowOled2Max / dycoOled2FontCur; 	// max characters due to y display limitations

	/* Set the default character cursor position
	*/
	Oled2SetCursor(0, 0);		//set for top left corner of OLED2 display

	/* Set the default Gray Scale Level (GS15) and Fill Pattern
	*/
	clrOled2Cur = 0x0F;
	pbOled2PatCur = rgbFillPat;
	Oled2SetDrawMode(modOled2Set);

	/* Default the character routines to automatically
	** update the display
	*/
	fOled2CharUpdate = TRUE;

}

/* ------------------------------------------------------------ */
/***	Oled2DevInit()
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
**		Initialize the OELD2 Display controller with
**	the correct Power ON sequence.
**	
*/

void Oled2DevInit() {
	WORD delay = 0;
	/* Initialize procedure is as follows
	**	1. Power up Vci and Vddio
	**	2. Wait for 300 ms
	**	3. Send display Off command
	**	4. Set Reset pin LOW for 100 us, then HIGH
	**	5. Initialize (Steps based on Univision Data Sheet)
	**	6. Power on Vcc
	**	7. Wait for 100ms
	**	8. Send display ON command
	*/

	/* 1. Power up Vci and Vddio
	*/
	PORTSetBits(prtVddCtrl, bitVddCtrl);

	/* 2. Wait for Vci and Vddio to become stable (300 ms)
	*/
	delay = 300;
	DelayMs(delay);

	/* 3. Send display OFF command
	*/
	PMPSendCmd(cmdSleep);

	/* 4.Clear Reset Pin
	*/
	PORTClearBits(prtReset, bitReset);

	// 4a) Wait for at least 100 microseconds
	delay = 1;
	DelayMs(delay);

	// 4b) Set Reset Pin high
	PORTSetBits(prtReset, bitReset);

	// 4c) Wait for at least 100 microseconds
	delay = 1;
	DelayMs(delay);

	/* 5. Univision Display Initialization Steps
	*/
	Oled2DispInit();
	
	/* 6. Power on Vcc
	*/
	PORTSetBits(prtVccCtrl, bitVccCtrl);

	/* 7. Wait 100 milliseconds for Vcc to be stable
	*/
	delay = 100;
	DelayMs(delay);

	/* 8. Send display on command
	*/
	PMPSendCmd(cmdWake);

	Oled2Clear();

	/* 9. Wait 200 ms for SEG/COM to turn on
	*/
	delay = 200;
	DelayMs(delay);
	


}

/* ------------------------------------------------------------ */
/***	Oled2DispInit()
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
**		This function can be modified by the user. This function
**	will initialize the display for their desired functionality by 
**	sending appropriate command and data bytes. The available demo 
**	will contain this generic initialize sequence.
**		
*/
void Oled2DispInit() {
	/* 5. Univision Initialization Steps
	*/
	// 5a) Send command unlock
	PMPSendCmd(cmdSetLock);
	PMPSendData(0x12);
	
	// 5b) Send Sleep command
	PMPSendCmd(cmdSleep);

	// 5c) Send Clock Divide Ratio and Oscillator Frequency
	PMPSendCmd(cmdClkDividerOscFreq);
	PMPSendData(0x91); //mid high oscillator frequency, DCLK = FpbCllk/2

	// 5d) Set Multiplex Ratio
	PMPSendCmd(cmdSetMuxRatio);
	PMPSendData(0x3F); //64MUX

	// 5e) Set Display Offset
	PMPSendCmd(cmdSetDisplayOffset);
	PMPSendData(0x00); //no offset

	// 5f) Set Display Start Line
	PMPSendCmd(cmdSetDisplayStartLine);
	PMPSendData(0x00); //start line is set at upper left corner

	// 5g) Set Remap and Dual COM Mode
	PMPSendCmd(cmdSetRemap);
	PMPSendData(0x14); //scan from COM[63] to COM[0], and enable nibble remap
					   // and horizontal address increment
	PMPSendData(0x11); //Dual COM mode enabled

	// 5h) Set GPIO
	PMPSendCmd(cmdSetGPIO);
	PMPSendData(0x00); //pin high impedence, input disabled

	// 5i) Set Function Selection
	PMPSendCmd(cmdFunctionSelect);
	PMPSendData(0x01); //internal Vdd enabled

	// 5j) Set EnhancmentA
	PMPSendCmd(cmdDisplayEnhance);
	PMPSendData(0xA2); //there is no external VSL
	PMPSendData(0xFD); //enhance GS quality

	// 5k) Set Contrast Current
	PMPSendCmd(cmdSetContrastCurrent);
	PMPSendData(0xDF); //High contrast current, reset is 7F

	// 5l) Set Master Contrast Current
	PMPSendCmd(cmdMasterContrastCtrl);
	PMPSendData(0x0F); //no change, reduce this value to change

	// 5m) Select Gray Scale Table
	PMPSendCmd(cmdSetGrayScaleTable); //this will likely need to be user defined
	PMPSendDataBuffer(rgbOled2GSTable, 16);

	// 5n) Enable Gray Scale Table
	PMPSendCmd(cmdGrayScaleEnable);

	// 5o) Set Phase Length
	PMPSendCmd(cmdPhaseLength);
	PMPSendData(0xE8); //phase 2 = 14 DCLKs, phase 1 = 15 DCLKS

	// 5p) EnhanceB Driving Scheme Capability
	PMPSendCmd(cmdDisplayEnhanceB1);
	PMPSendData(0x82); //reserved setting
	PMPSendData(0x20);

	// 5q) Set Pre-Charge Voltage
	PMPSendCmd(cmdSetPreChargeVoltage);
	PMPSendData(0x1F); // Pre-charge voltage = .6Vcc

	// 5r) Set Second Pre-Charge Voltage
	PMPSendCmd(cmdSetSecondPreCharge);
	PMPSendData(0x08); // second precharge voltage = 8 DCLKS, reset

	// 5s) Set VCOMH Deselect Level
	PMPSendCmd(cmdSetVcomh);
	PMPSendData(0x07); // Vcomh = .86*Vcc

	// 5t) Set Display Mode
	PMPSendCmd(cmdSetDisplayModeNorm); //normal reset mode
	
	// 5u) Clear Screen
	Oled2Clear();
	
}

/* ------------------------------------------------------------ */
/***	Oled2Term()
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
**		Powers off the OLED2 in the correct sequence.
**	
*/

void Oled2Term() {
	/* The following steps were taken from the SSD1322 data sheet.
	*/
	
	/* 1. Send the display off command
	*/
	PMPSendCmd(cmdSleep);
	
	/* 2. Power off VCC.
	*/
	PORTClearBits(prtVccCtrl, bitVccCtrl);
	DelayMs(200); // data sheet says wait at least 100 ms
	
	/* 3. Power off VCI and VDDIO
	*/
	PORTClearBits(prtVddCtrl, bitVddCtrl);

}

/* ------------------------------------------------------------ */
/***	Oled2Clear()
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
**		Clears the display buffer, and then update the display
**	
*/

void Oled2Clear() {

	Oled2ClearBuffer();
	Oled2Update();

}

/* ------------------------------------------------------------ */
/***	Oled2ClearBuffer()
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
**		Clears the display buffer.
**	
*/

void Oled2ClearBuffer() {
	
	WORD	ib;
	BYTE *	pb;

	pb = rgbOled2DispBuf;

	/* Fill the memory buffer with 0x00
	*/
	for(ib = 0; ib < cbOled2DispMax; ib++) {
		*pb++ = 0x00;
	}
	
}

/* ------------------------------------------------------------ */
/***	Oled2Update()
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
**		Updates the OLED2 display with the contents of the display
**	buffer
**	
*/

void Oled2Update() {

	/* Sets the Start and End column address in GDDRAM
	** When the col address pointer tries to increment
	** past the set End address, it will reset at the 
	** set Start address.
	** Each Column is 4 SEGs (pixels) wide
	*/
	PMPSendCmd(cmdSetColumnAddress);
	PMPSendData(28);
	PMPSendData(91);
	
	/* Sets the Start and End row address in GDDRAM
	** When the row address pointer tries to increment
	** past the set End address, it will reset at the 
	** set Start address.
	** Each row is 1 COM (pixel)
	*/
	PMPSendCmd(cmdSetRowAddress);
	PMPSendData(0);
	PMPSendData(63);

	/* Sends the display buffer. Must send command to enable
	** data to be written to GDDRAM
	*/
	PMPSendCmd(cmdWriteRAM);
	PMPSendDataBuffer(rgbOled2DispBuf, cbOled2DispMax);	

}

/* ------------------------------------------------------------ */
/***	Oled2DisplayOn()
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
**		Un-blank the display
**		
*/

void Oled2DisplayOn() {

	PMPSendCmd(cmdWake);
	
}

/* ------------------------------------------------------------ */
/***	Oled2DisplayOff()
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
**		Blank the display
**		
*/

void Oled2DisplayOff() {

	PMPSendCmd(cmdSleep);
	
}

/* ------------------------------------------------------------ */
/***	PMPSendCmd()
**
**	Parameters:
**		BYTE cmd - command to be sent
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Send the specified byte to OLED2 as a command
**		
*/

void PMPSendCmd(BYTE cmd) {
	
	// Clear D/C pin to configure Display controller to receive command
	PORTClearBits(prtDataCmd, bitDataCmd);

	// Chip select line low, so that Display controller will configure
	PORTClearBits(prtSelect, bitSelect);

	// Write command to PMP data line and wait for task completion
	PMDIN = cmd;

	while(PMMODEbits.BUSY) {
		continue;
	}

	// Chip select line high, Controller no longer configured
	PORTSetBits(prtSelect, bitSelect);

	//	Reset D/C pin for data
	PORTSetBits(prtDataCmd, bitDataCmd);
}

/* ------------------------------------------------------------ */
/***	PMPSendData()
**
**	Parameters:
**		BYTE data - data to be sent
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Send the specified byte to OLED2 as a data
**		
*/

void PMPSendData(BYTE data) {
	
	// Set D/C pin to configure Display controller to receive data
	PORTSetBits(prtDataCmd, bitDataCmd);

	// Chip select line low, so that Display controller will configure
	PORTClearBits(prtSelect, bitSelect);

	// Write data to PMP data line and wait for task completion
	PMDIN = data;

	while(PMMODEbits.BUSY) {
		continue;
	}

	// Chip select line high, Controller no longer configured
	PORTSetBits(prtSelect, bitSelect);
}

/* ------------------------------------------------------------ */
/***	PMPSendDataBuffer()
**
**	Parameters:
**		BYTE* pbBuf - pointer to the display buffer
**		WORD cbBuf - size of buffer
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This function will send the data stored in the display buffer
** 	to the OLED2 GDDRAM. Only send the display buffer through this 
**		
*/
void PMPSendDataBuffer(const BYTE* pbBuf, WORD cbBuf) {
	int ib;

	for(ib = 0; ib < cbBuf; ib++) {
		PMPSendData(*pbBuf);
		pbBuf++;

		while(PMMODEbits.BUSY) {
			continue;
		}
	}
}