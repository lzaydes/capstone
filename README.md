# capstone
Source code and documentation for my FPGA capstone at UCLA

##Background
This source code contains 2 Xilinx Vivado projects. The first is OLED2, which was an attempt to write a driver in C for the [Digilent PmodOLED2 screen](https://digilent.com/reference/pmodoled/pmodoled). This model is retired an no drivers exist to make it function on a Basys3 board. Additionally, documentation for this Pmod is almost nonexistent. Nonetheless we used documentation from the [SSD1322](https://www.hpinfotech.ro/SSD1322.pdf) display controller to find the hexadecimal commands for turning the display on, as well as the pin configurations. 

Due to timing constraints, and voltage constraints (Basys3 outputs only 3.3V to its Pmod ports, while the screen requires 5V), we had to scrap the project and start working on a different one.

The second project is OLEDRGB, which works with the [Digilent Pmod OLEDrgb screen), which is much smaller in comparison to the OLED2. This project implements an Etch a Sketch onto the Basys3 board as shown here:

[](capstone.MOV)

As in the video, one PModJSTK2 should be connected to _jc_, and one to _jxadc_. The PModOLEDrgb is connected via an extension cable to _ja_.

##Building the project
This project is targeted for Xilinx Vivado 2018.2. Upon opening the project the folders in the ip_catalog directory need to be imported into the block design. After this:
* Right click on the block design in the Sources window and select "Create HDL Wrapper"
* In the left window select "Generate Bitstream"
* Once the bitstream file is generated, go to File -> Export Hardware and **include bitstream** -> OK
* File -> Launch SDK
* Once the SDK is open, select File -> Import -> General -> Existing Projects into Workspace, and import **sketch** and **sketch_bsp** from the capstone.sdk directory
* With the Basys3 connected, select "Program FPGA" in the toolbar
* Once programmed, click Run -> Run As -> Run on hardware

  
