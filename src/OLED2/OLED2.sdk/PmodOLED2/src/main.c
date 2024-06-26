#include <stdio.h>
#include "platform.h"
#include "sleep.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xuartlite.h"
#include "PmodGpio.h"

// Commands
/* A[7:0] = first data byte sent after command (on D[7:0] pins)
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
/*					General Type Declarations					*/
/* ------------------------------------------------------------ */
/* Gray Scale values for PmodOLED2. Gray Scale values range from 0 to 180.
** GS0 represents pixel OFF, and is always set at 0. GS1-15 are user
** definable, or, they can be drawn from the default Gray Scale Table.
*/
#define oled2GS1			40
#define oled2GS2			44
#define oled2GS3			48
#define oled2GS4			52
#define oled2GS5			56
#define oled2GS6			60
#define oled2GS7			68
#define oled2GS8			76
#define oled2GS9			84
#define oled2GS10			92
#define oled2GS11			102
#define oled2GS12			115
#define oled2GS13			135
#define oled2GS14			155
#define oled2GS15			180

// Pin definitions
// JB
#define DATA_PINS 0x3CF  // Pins 1-4, 7-10 (excluding 5, 6, 11, 12)
#define VCC_PINS_JB 0x820  // Pins 6 and 12
#define GND_PINS_JB 0x410  // Pins 5 and 11

// JC
#define VCI_PIN 0x1  // Pin 1
#define RESET_PIN 0x2  // Pin 2
#define VCC_BAT_PIN 0x4  // Pin 3
#define DC_PIN 0x40  // Pin 7
#define REN_PIN 0x80  // Pin 8
#define WREN_PIN 0x100  // Pin 9
#define VDD_LOGIC_PIN 0x200  // Pin 10
#define VCC_PINS_JC 0x820  // Pins 6 and 12
#define GND_PINS_JC 0x410  // Pins 5 and 11

// GPIO instances for monitoring ports JC and JB
PmodGPIO jb, jc;

// Display buffer
#define	cbOled2DispMax	8192		//max number of bytes in display buffer
uint8_t	rgbOled2DispBuf[cbOled2DispMax];

uint8_t rgbOled2GSTable[] = { oled2GS1, oled2GS2, oled2GS3, oled2GS4,
						   oled2GS5, oled2GS6, oled2GS7, oled2GS8,
						   oled2GS9, oled2GS10, oled2GS11, oled2GS12,
						   oled2GS13, oled2GS14, oled2GS15 };

void init();
void gpio_init();
void oled_init();
void send_command(uint8_t cmd);
void display_init();

void Oled2ClearBuffer();
void Oled2Clear();
void Oled2Update();

int main()
{
    init();
    oled_init();

    while(1)
    {
        continue;
    }

    return 0;
}

void init()
{
    gpio_init();
}

void gpio_init()
{
    // Initialize GPIO for JB and JC ports
    GPIO_begin(&jb, XPAR_PMODGPIO_0_AXI_LITE_GPIO_BASEADDR, DATA_PINS | VCC_PINS_JB | GND_PINS_JB);
    GPIO_begin(&jc, XPAR_PMODGPIO_1_AXI_LITE_GPIO_BASEADDR, VCI_PIN | RESET_PIN | VCC_BAT_PIN | DC_PIN | REN_PIN | WREN_PIN | VDD_LOGIC_PIN | VCC_PINS_JC | GND_PINS_JC);

    // Set VCC pins to high
    GPIO_setPin(&jb, 6, 1);  // VCC pin 6 on JB
    GPIO_setPin(&jb, 12, 1);  // VCC pin 12 on JB
    GPIO_setPin(&jc, 6, 1);  // VCC pin 6 on JC
    GPIO_setPin(&jc, 12, 1);  // VCC pin 12 on JC

    // Set GND pins to low
    GPIO_setPin(&jb, 5, 0);  // GND pin 5 on JB
    GPIO_setPin(&jb, 11, 0);  // GND pin 11 on JB
    GPIO_setPin(&jc, 5, 0);  // GND pin 5 on JC
    GPIO_setPin(&jc, 11, 0);  // GND pin 11 on JC
}

void oled_init()
{
    /* Display on sequence
     *
     * 1. Apply power to VCI
     * 2. Wait 300ms
     * 3. Send Display Off command (0xAE)
     * 4. Set DC Pin to select mode
     * 5. Clear screen
     * 6. Apply power to VCC
     * 7. Delay 100ms
     * 8. Send Display On command (0xAF)
     *
     */

    // Read & Write Enable
    GPIO_setPin(&jc, 7, 1);  // REN high
    GPIO_setPin(&jc, 8, 1);  // WREN high

    // Apply power to VCI
    GPIO_setPin(&jc, 0, 1);

    usleep(300000);  // Wait for 300ms

    // Display Off
    send_command(0xAE);

    // Set DC Pin to low to select command mode
    GPIO_setPin(&jc, 6, 0);

    // Set reset pin low for 100 us
    GPIO_setPin(&jc, 1, 0);
    usleep(150);
    GPIO_setPin(&jc, 1, 0);
    usleep(150);

    display_init();

    // Apply power to VCC
    GPIO_setPin(&jc, 2, 1);  // VCC_BAT_PIN high
    GPIO_setPin(&jc, 9, 1);  // VDD_LOGIC_PIN high
    usleep(100000);  // Wait for 100ms

    // Display On command
    send_command(0xAF);
}

void send_command(uint8_t cmd)
{
    xil_printf("Sending command %d\r\n", cmd);

    // Set DC pin to low for command mode
    GPIO_setPin(&jc, 6, 0);

    // Send command via DATA_PINS on JB
    for (int i = 0; i < 10; i++)
    {
    	if (i == 5 || i == 6)
    		continue;

        GPIO_setPin(&jb, (i > 6) ? i - 2 : i, (cmd >> i) & 0x01);
        xil_printf("Pin %d set to %d\r\n", i, (cmd >> i) & 0x01);
    }

    // Pulse the write enable pin (assume WREN_PIN is used for write enable)
    GPIO_setPin(&jc, 8, 1);
    usleep(1);  // Short pulse
    GPIO_setPin(&jc, 8, 0);

    // Set DC pin back to high for data mode
    GPIO_setPin(&jc, 6, 1);
}

void display_init()
{
	// 5a) Send command unlock
	send_command(cmdSetLock);
	send_command(0x12);

	// 5b) Send Sleep command
	send_command(cmdSleep);

	// 5c) Send Clock Divide Ratio and Oscillator Frequency
	send_command(cmdClkDividerOscFreq);
	send_command(0x91); //mid high oscillator frequency, DCLK = FpbCllk/2

	// 5d) Set Multiplex Ratio
	send_command(cmdSetMuxRatio);
	send_command(0x3F); //64MUX

	// 5e) Set Display Offset
	send_command(cmdSetDisplayOffset);
	send_command(0x00); //no offset

	// 5f) Set Display Start Line
	send_command(cmdSetDisplayStartLine);
	send_command(0x00); //start line is set at upper left corner

	// 5g) Set Remap and Dual COM Mode
	send_command(cmdSetRemap);
	send_command(0x14); //scan from COM[63] to COM[0], and enable nibble remap
					   // and horizontal address increment
	send_command(0x11); //Dual COM mode enabled

	// 5h) Set GPIO
	send_command(cmdSetGPIO);
	send_command(0x00); //pin high impedence, input disabled

	// 5i) Set Function Selection
	send_command(cmdFunctionSelect);
	send_command(0x01); //internal Vdd enabled

	// 5j) Set EnhancmentA
	send_command(cmdDisplayEnhance);
	send_command(0xA2); //there is no external VSL
	send_command(0xFD); //enhance GS quality

	// 5k) Set Contrast Current
	send_command(cmdSetContrastCurrent);
	send_command(0xDF); //High contrast current, reset is 7F

	// 5l) Set Master Contrast Current
	send_command(cmdMasterContrastCtrl);
	send_command(0x0F); //no change, reduce this value to change

	// 5m) Select Gray Scale Table
	send_command(cmdSetGrayScaleTable); //this will likely need to be user defined
	send_command_buffer(rgbOled2GSTable, 16);

	// 5n) Enable Gray Scale Table
	send_command(cmdGrayScaleEnable);

	// 5o) Set Phase Length
	send_command(cmdPhaseLength);
	send_command(0xE8); //phase 2 = 14 DCLKs, phase 1 = 15 DCLKS

	// 5p) EnhanceB Driving Scheme Capability
	send_command(cmdDisplayEnhanceB1);
	send_command(0x82); //reserved setting
	send_command(0x20);

	// 5q) Set Pre-Charge Voltage
	send_command(cmdSetPreChargeVoltage);
	send_command(0x1F); // Pre-charge voltage = .6Vcc

	// 5r) Set Second Pre-Charge Voltage
	send_command(cmdSetSecondPreCharge);
	send_command(0x08); // second precharge voltage = 8 DCLKS, reset

	// 5s) Set VCOMH Deselect Level
	send_command(cmdSetVcomh);
	send_command(0x07); // Vcomh = .86*Vcc

	// 5t) Set Display Mode
	send_command(cmdSetDisplayModeNorm); //normal reset mode

	// 5u) Clear Screen
	Oled2Clear();
}

Oled2ClearBuffer()
{
	int			ib;
	uint8_t *	pb;

	pb = rgbOled2DispBuf;

	/* Fill the memory buffer with 0x00
	*/
	for(ib = 0; ib < cbOled2DispMax; ib++)
		*pb++ = 0x00;

}

void Oled2Clear()
{
	Oled2ClearBuffer();
	Oled2Update();
}

void Oled2Update()
{
	/* Sets the Start and End column address in GDDRAM
	** When the col address pointer tries to increment
	** past the set End address, it will reset at the
	** set Start address.
	** Each Column is 4 SEGs (pixels) wide
	*/
	send_command(cmdSetColumnAddress);
	send_command(28);
	send_command(91);

	/* Sets the Start and End row address in GDDRAM
	** When the row address pointer tries to increment
	** past the set End address, it will reset at the
	** set Start address.
	** Each row is 1 COM (pixel)
	*/
	send_command(cmdSetRowAddress);
	send_command(0);
	send_command(63);

	/* Sends the display buffer. Must send command to enable
	** data to be written to GDDRAM
	*/
	send_command(cmdWriteRAM);
	send_command_buffer(rgbOled2DispBuf, cbOled2DispMax);

}

void send_command_buffer(const uint8_t* pbBuf, int cbBuf) {
	int ib;

	for(ib = 0; ib < cbBuf; ib++) {
		send_command(*pbBuf);
		pbBuf++;

		usleep(150);
	}
}
