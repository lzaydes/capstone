# MPLAB IDE generated this makefile for use with GNU make.
# Project: PmodOLED2_Library.mcp
# Date: Wed Jul 11 22:00:17 2012

AS = pic32-as.exe
CC = pic32-gcc.exe
LD = pic32-ld.exe
AR = pic32-ar.exe
HX = pic32-bin2hex.exe
RM = rm

PmodOLED2_Library.hex : PmodOLED2_Library.elf
	$(HX) "PmodOLED2_Library.elf"

PmodOLED2_Library.elf : main.o Oled2Char.o PmodOled2.o Oled2Grph.o FillPat.o ChrFont0.o delay.o PmodOled2Uart.o
	$(CC) -mdebugger -mprocessor=32MX460F512L "main.o" "Oled2Char.o" "PmodOled2.o" "Oled2Grph.o" "FillPat.o" "ChrFont0.o" "delay.o" "PmodOled2Uart.o" -o"PmodOLED2_Library.elf" -Wl,-L"C:\Program Files (x86)\Microchip\MPLAB C32 Suite\lib",-L"C:\Program Files (x86)\Microchip\MPLAB C32 Suite\pic32mx\lib",--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=__ICD2RAM=1,-Map="PmodOLED2_Library.map"

main.o : main.c PmodOled2.h Oled2Char.h Oled2Grph.h delay.h
	$(CC) -mprocessor=32MX460F512L -x c -c "main.c" -o"main.o" -MMD -MF"main.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g

Oled2Char.o : Oled2Char.c PmodOled2.h Oled2Char.h Oled2Grph.h
	$(CC) -mprocessor=32MX460F512L -x c -c "Oled2Char.c" -o"Oled2Char.o" -MMD -MF"Oled2Char.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g

PmodOled2.o : PmodOled2.c PmodOled2.h Oled2Char.h Oled2Grph.h FillPat.h delay.h
	$(CC) -mprocessor=32MX460F512L -x c -c "PmodOled2.c" -o"PmodOled2.o" -MMD -MF"PmodOled2.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g

Oled2Grph.o : Oled2Grph.c Oled2Grph.h PmodOled2.h FillPat.h
	$(CC) -mprocessor=32MX460F512L -x c -c "Oled2Grph.c" -o"Oled2Grph.o" -MMD -MF"Oled2Grph.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g

FillPat.o : FillPat.c
	$(CC) -mprocessor=32MX460F512L -x c -c "FillPat.c" -o"FillPat.o" -MMD -MF"FillPat.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g

ChrFont0.o : ChrFont0.c
	$(CC) -mprocessor=32MX460F512L -x c -c "ChrFont0.c" -o"ChrFont0.o" -MMD -MF"ChrFont0.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g

delay.o : delay.c delay.h
	$(CC) -mprocessor=32MX460F512L -x c -c "delay.c" -o"delay.o" -MMD -MF"delay.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g

PmodOled2Uart.o : PmodOled2Uart.c PmodOled2.h PmodOled2Uart.h
	$(CC) -mprocessor=32MX460F512L -x c -c "PmodOled2Uart.c" -o"PmodOled2Uart.o" -MMD -MF"PmodOled2Uart.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g

clean : 
	$(RM) "main.o" "Oled2Char.o" "PmodOled2.o" "Oled2Grph.o" "FillPat.o" "ChrFont0.o" "delay.o" "PmodOled2Uart.o" "PmodOLED2_Library.elf" "PmodOLED2_Library.hex"

