#!/bin/sh

#
# PCI Express Endpoint NC Verilog Run Script
#

rm -rf INCA* work

mkdir work

ncvlog    -work work -define NCV \
          -define SIMULATION \
	  $XILINX/verilog/src/glbl.v \
          -file board.f \
          -incdir ../ -incdir ../tests -incdir ../dsport

ncelab -access +rwc -timescale 1ns/1ps \
work.board work.glbl
ncsim work.board
