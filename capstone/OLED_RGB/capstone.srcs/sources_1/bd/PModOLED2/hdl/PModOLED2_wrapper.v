//Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2018.2 (win64) Build 2258646 Thu Jun 14 20:03:12 MDT 2018
//Date        : Mon Jun  3 14:44:49 2024
//Host        : CS152B-03 running 64-bit major release  (build 9200)
//Command     : generate_target PModOLED2_wrapper.bd
//Design      : PModOLED2_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module PModOLED2_wrapper
   (dip_switches_16bits_tri_i,
    jb_pin10_io,
    jb_pin1_io,
    jb_pin2_io,
    jb_pin3_io,
    jb_pin4_io,
    jb_pin7_io,
    jb_pin8_io,
    jb_pin9_io,
    jc_pin10_io,
    jc_pin1_io,
    jc_pin2_io,
    jc_pin3_io,
    jc_pin4_io,
    jc_pin7_io,
    jc_pin8_io,
    jc_pin9_io,
    reset,
    sys_clock,
    usb_uart_rxd,
    usb_uart_txd);
  input [15:0]dip_switches_16bits_tri_i;
  inout jb_pin10_io;
  inout jb_pin1_io;
  inout jb_pin2_io;
  inout jb_pin3_io;
  inout jb_pin4_io;
  inout jb_pin7_io;
  inout jb_pin8_io;
  inout jb_pin9_io;
  inout jc_pin10_io;
  inout jc_pin1_io;
  inout jc_pin2_io;
  inout jc_pin3_io;
  inout jc_pin4_io;
  inout jc_pin7_io;
  inout jc_pin8_io;
  inout jc_pin9_io;
  input reset;
  input sys_clock;
  input usb_uart_rxd;
  output usb_uart_txd;

  wire [15:0]dip_switches_16bits_tri_i;
  wire jb_pin10_i;
  wire jb_pin10_io;
  wire jb_pin10_o;
  wire jb_pin10_t;
  wire jb_pin1_i;
  wire jb_pin1_io;
  wire jb_pin1_o;
  wire jb_pin1_t;
  wire jb_pin2_i;
  wire jb_pin2_io;
  wire jb_pin2_o;
  wire jb_pin2_t;
  wire jb_pin3_i;
  wire jb_pin3_io;
  wire jb_pin3_o;
  wire jb_pin3_t;
  wire jb_pin4_i;
  wire jb_pin4_io;
  wire jb_pin4_o;
  wire jb_pin4_t;
  wire jb_pin7_i;
  wire jb_pin7_io;
  wire jb_pin7_o;
  wire jb_pin7_t;
  wire jb_pin8_i;
  wire jb_pin8_io;
  wire jb_pin8_o;
  wire jb_pin8_t;
  wire jb_pin9_i;
  wire jb_pin9_io;
  wire jb_pin9_o;
  wire jb_pin9_t;
  wire jc_pin10_i;
  wire jc_pin10_io;
  wire jc_pin10_o;
  wire jc_pin10_t;
  wire jc_pin1_i;
  wire jc_pin1_io;
  wire jc_pin1_o;
  wire jc_pin1_t;
  wire jc_pin2_i;
  wire jc_pin2_io;
  wire jc_pin2_o;
  wire jc_pin2_t;
  wire jc_pin3_i;
  wire jc_pin3_io;
  wire jc_pin3_o;
  wire jc_pin3_t;
  wire jc_pin4_i;
  wire jc_pin4_io;
  wire jc_pin4_o;
  wire jc_pin4_t;
  wire jc_pin7_i;
  wire jc_pin7_io;
  wire jc_pin7_o;
  wire jc_pin7_t;
  wire jc_pin8_i;
  wire jc_pin8_io;
  wire jc_pin8_o;
  wire jc_pin8_t;
  wire jc_pin9_i;
  wire jc_pin9_io;
  wire jc_pin9_o;
  wire jc_pin9_t;
  wire reset;
  wire sys_clock;
  wire usb_uart_rxd;
  wire usb_uart_txd;

  PModOLED2 PModOLED2_i
       (.dip_switches_16bits_tri_i(dip_switches_16bits_tri_i),
        .jb_pin10_i(jb_pin10_i),
        .jb_pin10_o(jb_pin10_o),
        .jb_pin10_t(jb_pin10_t),
        .jb_pin1_i(jb_pin1_i),
        .jb_pin1_o(jb_pin1_o),
        .jb_pin1_t(jb_pin1_t),
        .jb_pin2_i(jb_pin2_i),
        .jb_pin2_o(jb_pin2_o),
        .jb_pin2_t(jb_pin2_t),
        .jb_pin3_i(jb_pin3_i),
        .jb_pin3_o(jb_pin3_o),
        .jb_pin3_t(jb_pin3_t),
        .jb_pin4_i(jb_pin4_i),
        .jb_pin4_o(jb_pin4_o),
        .jb_pin4_t(jb_pin4_t),
        .jb_pin7_i(jb_pin7_i),
        .jb_pin7_o(jb_pin7_o),
        .jb_pin7_t(jb_pin7_t),
        .jb_pin8_i(jb_pin8_i),
        .jb_pin8_o(jb_pin8_o),
        .jb_pin8_t(jb_pin8_t),
        .jb_pin9_i(jb_pin9_i),
        .jb_pin9_o(jb_pin9_o),
        .jb_pin9_t(jb_pin9_t),
        .jc_pin10_i(jc_pin10_i),
        .jc_pin10_o(jc_pin10_o),
        .jc_pin10_t(jc_pin10_t),
        .jc_pin1_i(jc_pin1_i),
        .jc_pin1_o(jc_pin1_o),
        .jc_pin1_t(jc_pin1_t),
        .jc_pin2_i(jc_pin2_i),
        .jc_pin2_o(jc_pin2_o),
        .jc_pin2_t(jc_pin2_t),
        .jc_pin3_i(jc_pin3_i),
        .jc_pin3_o(jc_pin3_o),
        .jc_pin3_t(jc_pin3_t),
        .jc_pin4_i(jc_pin4_i),
        .jc_pin4_o(jc_pin4_o),
        .jc_pin4_t(jc_pin4_t),
        .jc_pin7_i(jc_pin7_i),
        .jc_pin7_o(jc_pin7_o),
        .jc_pin7_t(jc_pin7_t),
        .jc_pin8_i(jc_pin8_i),
        .jc_pin8_o(jc_pin8_o),
        .jc_pin8_t(jc_pin8_t),
        .jc_pin9_i(jc_pin9_i),
        .jc_pin9_o(jc_pin9_o),
        .jc_pin9_t(jc_pin9_t),
        .reset(reset),
        .sys_clock(sys_clock),
        .usb_uart_rxd(usb_uart_rxd),
        .usb_uart_txd(usb_uart_txd));
  IOBUF jb_pin10_iobuf
       (.I(jb_pin10_o),
        .IO(jb_pin10_io),
        .O(jb_pin10_i),
        .T(jb_pin10_t));
  IOBUF jb_pin1_iobuf
       (.I(jb_pin1_o),
        .IO(jb_pin1_io),
        .O(jb_pin1_i),
        .T(jb_pin1_t));
  IOBUF jb_pin2_iobuf
       (.I(jb_pin2_o),
        .IO(jb_pin2_io),
        .O(jb_pin2_i),
        .T(jb_pin2_t));
  IOBUF jb_pin3_iobuf
       (.I(jb_pin3_o),
        .IO(jb_pin3_io),
        .O(jb_pin3_i),
        .T(jb_pin3_t));
  IOBUF jb_pin4_iobuf
       (.I(jb_pin4_o),
        .IO(jb_pin4_io),
        .O(jb_pin4_i),
        .T(jb_pin4_t));
  IOBUF jb_pin7_iobuf
       (.I(jb_pin7_o),
        .IO(jb_pin7_io),
        .O(jb_pin7_i),
        .T(jb_pin7_t));
  IOBUF jb_pin8_iobuf
       (.I(jb_pin8_o),
        .IO(jb_pin8_io),
        .O(jb_pin8_i),
        .T(jb_pin8_t));
  IOBUF jb_pin9_iobuf
       (.I(jb_pin9_o),
        .IO(jb_pin9_io),
        .O(jb_pin9_i),
        .T(jb_pin9_t));
  IOBUF jc_pin10_iobuf
       (.I(jc_pin10_o),
        .IO(jc_pin10_io),
        .O(jc_pin10_i),
        .T(jc_pin10_t));
  IOBUF jc_pin1_iobuf
       (.I(jc_pin1_o),
        .IO(jc_pin1_io),
        .O(jc_pin1_i),
        .T(jc_pin1_t));
  IOBUF jc_pin2_iobuf
       (.I(jc_pin2_o),
        .IO(jc_pin2_io),
        .O(jc_pin2_i),
        .T(jc_pin2_t));
  IOBUF jc_pin3_iobuf
       (.I(jc_pin3_o),
        .IO(jc_pin3_io),
        .O(jc_pin3_i),
        .T(jc_pin3_t));
  IOBUF jc_pin4_iobuf
       (.I(jc_pin4_o),
        .IO(jc_pin4_io),
        .O(jc_pin4_i),
        .T(jc_pin4_t));
  IOBUF jc_pin7_iobuf
       (.I(jc_pin7_o),
        .IO(jc_pin7_io),
        .O(jc_pin7_i),
        .T(jc_pin7_t));
  IOBUF jc_pin8_iobuf
       (.I(jc_pin8_o),
        .IO(jc_pin8_io),
        .O(jc_pin8_i),
        .T(jc_pin8_t));
  IOBUF jc_pin9_iobuf
       (.I(jc_pin9_o),
        .IO(jc_pin9_io),
        .O(jc_pin9_i),
        .T(jc_pin9_t));
endmodule
