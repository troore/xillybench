//-----------------------------------------------------------------------------
//
// (c) Copyright 2010-2011 Xilinx, Inc. All rights reserved.
//
// This file contains confidential and proprietary information
// of Xilinx, Inc. and is protected under U.S. and
// international copyright and other intellectual property
// laws.
//
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// Xilinx, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) Xilinx shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or Xilinx had been advised of the
// possibility of the same.
//
// CRITICAL APPLICATIONS
// Xilinx products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of Xilinx products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
//
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
//
//-----------------------------------------------------------------------------
// Project    : Series-7 Integrated Block for PCI Express
// File       : pcie_k7_8x_fast_cfg_init_cntr.v
// Version    : 1.6
//--
//-- Description:  PCIe Fast Configuration Init Counter
//--
//------------------------------------------------------------------------------
module pcie_k7_8x_fast_cfg_init_cntr #(
  parameter PATTERN_WIDTH = 8,
  parameter INIT_PATTERN  = 8'hA5,
  parameter INIT_PATTERN1 = 8'h12,
  parameter INIT_PATTERN2 = 8'h9a,
  parameter TCQ           = 1
) (
  input                          clk,
  input                          rst,
  output reg [PATTERN_WIDTH-1:0] pattern_o
);

//This logic constantly alternates between 2 values. The "KEEP"
// and registering of constant values is used to prevent optimization;
// it is important to preserve all the output bits so the 1st stage
// bitstream's pattern match logic has to check all the bits.
(* KEEP = "TRUE" *) reg [PATTERN_WIDTH-1:0] init_pattern1_reg    = 'h0;
(* KEEP = "TRUE" *) reg [PATTERN_WIDTH-1:0] init_pattern2_reg    = 'h0;
(* KEEP = "TRUE" *) reg                     init_toggle          = 1'b0;
(* KEEP = "TRUE" *) reg [PATTERN_WIDTH-1:0] init_pattern_bus_pre = 'h0;
always @(posedge clk) begin
    if(rst) begin
      init_pattern1_reg    <= #TCQ ~INIT_PATTERN1;
      init_pattern2_reg    <= #TCQ ~INIT_PATTERN2;
      init_toggle          <= #TCQ 'h0;
      init_pattern_bus_pre <= #TCQ 'h0;
      pattern_o            <= #TCQ 'h0;
    end else begin
      init_pattern1_reg    <= #TCQ INIT_PATTERN1;
      init_pattern2_reg    <= #TCQ INIT_PATTERN2;
      init_toggle          <= #TCQ !init_toggle;
      init_pattern_bus_pre <= #TCQ init_toggle ? init_pattern1_reg : init_pattern2_reg;
      pattern_o            <= #TCQ init_pattern_bus_pre;
    end
end

endmodule
