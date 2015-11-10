module xillydemo
  (
   input  PCIE_PERST_B_LS,
   input  PCIE_REFCLK_N,
   input  PCIE_REFCLK_P,
   input [7:0] PCIE_RX_N,
   input [7:0] PCIE_RX_P,
   output [3:0] GPIO_LED,
   output [7:0] PCIE_TX_N,
   output [7:0] PCIE_TX_P
   );
   // Clock and quiesce
   wire 	bus_clk;
   wire 	quiesce;
   
   // Memory array
   reg [7:0] 	demoarray[0:31];

   
   // Wires related to /dev/xillybus_mem_8
   wire       user_r_mem_8_rden;
   wire       user_r_mem_8_empty;
   reg [7:0]  user_r_mem_8_data;
   wire       user_r_mem_8_eof;
   wire       user_r_mem_8_open;
   wire       user_w_mem_8_wren;
   wire       user_w_mem_8_full;
   wire [7:0] user_w_mem_8_data;
   wire       user_w_mem_8_open;
   wire [4:0] user_mem_8_addr;
   wire       user_mem_8_addr_update;

   // Wires related to /dev/xillybus_read_32
   wire       user_r_read_32_rden;
   wire       user_r_read_32_empty;
   wire [31:0] user_r_read_32_data;
   wire        user_r_read_32_eof;
   wire        user_r_read_32_open;

   // Wires related to /dev/xillybus_read_8
   wire        user_r_read_8_rden;
   wire        user_r_read_8_empty;
   wire [7:0]  user_r_read_8_data;
   wire        user_r_read_8_eof;
   wire        user_r_read_8_open;

   // Wires related to /dev/xillybus_write_32
   wire        user_w_write_32_wren;
   wire        user_w_write_32_full;
   wire [31:0] user_w_write_32_data;
   wire        user_w_write_32_open;

   // Wires related to /dev/xillybus_write_8
   wire        user_w_write_8_wren;
   wire        user_w_write_8_full;
   wire [7:0]  user_w_write_8_data;
   wire        user_w_write_8_open;

   xillybus xillybus_ins (

			  // Ports related to /dev/xillybus_mem_8
			  // FPGA to CPU signals:
			  .user_r_mem_8_rden(user_r_mem_8_rden),
			  .user_r_mem_8_empty(user_r_mem_8_empty),
			  .user_r_mem_8_data(user_r_mem_8_data),
			  .user_r_mem_8_eof(user_r_mem_8_eof),
			  .user_r_mem_8_open(user_r_mem_8_open),

			  // CPU to FPGA signals:
			  .user_w_mem_8_wren(user_w_mem_8_wren),
			  .user_w_mem_8_full(user_w_mem_8_full),
			  .user_w_mem_8_data(user_w_mem_8_data),
			  .user_w_mem_8_open(user_w_mem_8_open),

			  // Address signals:
			  .user_mem_8_addr(user_mem_8_addr),
			  .user_mem_8_addr_update(user_mem_8_addr_update),


			  // Ports related to /dev/xillybus_read_32
			  // FPGA to CPU signals:
			  .user_r_read_32_rden(user_r_read_32_rden),
			  .user_r_read_32_empty(user_r_read_32_empty),
			  .user_r_read_32_data(user_r_read_32_data),
			  .user_r_read_32_eof(user_r_read_32_eof),
			  .user_r_read_32_open(user_r_read_32_open),

			  // Ports related to /dev/xillybus_write_32
			  // CPU to FPGA signals:
			  .user_w_write_32_wren(user_w_write_32_wren),
			  .user_w_write_32_full(user_w_write_32_full),
			  .user_w_write_32_data(user_w_write_32_data),
			  .user_w_write_32_open(user_w_write_32_open),

			  // Ports related to /dev/xillybus_read_8
			  // FPGA to CPU signals:
			  .user_r_read_8_rden(user_r_read_8_rden),
			  .user_r_read_8_empty(user_r_read_8_empty),
			  .user_r_read_8_data(user_r_read_8_data),
			  .user_r_read_8_eof(user_r_read_8_eof),
			  .user_r_read_8_open(user_r_read_8_open),

			  // Ports related to /dev/xillybus_write_8
			  // CPU to FPGA signals:
			  .user_w_write_8_wren(user_w_write_8_wren),
			  .user_w_write_8_full(user_w_write_8_full),
			  .user_w_write_8_data(user_w_write_8_data),
			  .user_w_write_8_open(user_w_write_8_open),


			  // Signals to top level
			  .PCIE_PERST_B_LS(PCIE_PERST_B_LS),
			  .PCIE_REFCLK_N(PCIE_REFCLK_N),
			  .PCIE_REFCLK_P(PCIE_REFCLK_P),
			  .PCIE_RX_N(PCIE_RX_N),
			  .PCIE_RX_P(PCIE_RX_P),
			  .GPIO_LED(GPIO_LED),
			  .PCIE_TX_N(PCIE_TX_N),
			  .PCIE_TX_P(PCIE_TX_P),
			  .bus_clk(bus_clk),
			  .quiesce(quiesce)
			  );

   // A simple inferred RAM
   always @(posedge bus_clk)
     begin
	if (user_w_mem_8_wren)
	  demoarray[user_mem_8_addr] <= user_w_mem_8_data;
	
	if (user_r_mem_8_rden)
	  user_r_mem_8_data <= demoarray[user_mem_8_addr];	  
     end

   assign  user_r_mem_8_empty = 0;
   assign  user_r_mem_8_eof = 0;
   assign  user_w_mem_8_full = 0;

// Wires related to HLS_wrapper
   wire [31:0] in_r_dout;
   wire        in_r_read;
   wire        hls_fifo_rd_en;
   reg 	       in_r_empty_n;
   wire [31:0] out_r_din;
   wire        out_r_full;
   wire        out_r_write;
   wire [7:0]  debug_out_din;
   wire        debug_out_full;
   wire        debug_out_write;

   fifo_32x512 fifo_to_function
     (
      .clk(bus_clk),
      .srst(!user_w_write_32_open),
      .din(user_w_write_32_data),
      .wr_en(user_w_write_32_wren),
      .rd_en(hls_fifo_rd_en),
      .dout(in_r_dout),
      .full(user_w_write_32_full),
      .empty(hls_fifo_empty)
      );

   assign  hls_fifo_rd_en = !hls_fifo_empty && (in_r_read || !in_r_empty_n);

   always @(posedge bus_clk)
     if (!user_w_write_32_open)
       in_r_empty_n <= 0;
     else if (hls_fifo_rd_en)
       in_r_empty_n <= 1;
     else if (in_r_read)
       in_r_empty_n <= 0;

   fifo_32x512 fifo_from_function
     (
      .clk(bus_clk),
      .srst(!user_r_read_32_open),
      .din(out_r_din),
      .wr_en(out_r_write),
      .rd_en(user_r_read_32_rden),
      .dout(user_r_read_32_data),
      .full(out_r_full),
      .empty(user_r_read_32_empty)
      );

   assign  user_r_read_32_eof = 0;

   fifo_8x2048 fifo_8
     (
      .clk(bus_clk),
      .srst(!user_r_read_8_open),
      .din(debug_out_din),
      .wr_en(debug_out_write),
      .rd_en(user_r_read_8_rden),
      .dout(user_r_read_8_data),
      .full(debug_out_full),
      .empty(user_r_read_8_empty)
      );

   assign  user_r_read_8_eof = 0;
   assign  user_w_write_8_full = 1; // Not used, hence always full

   xillybus_wrapper_0 HLS_wrapper
     (
      .ap_clk(bus_clk),
      .ap_rst(!user_w_write_32_open || !user_r_read_32_open),
      .debug_ready(!debug_out_full || !user_r_read_8_open),
      .debug_out(debug_out_din),
      .debug_out_ap_vld(debug_out_write),

      .in_r_dout(in_r_dout),
      .in_r_empty_n(in_r_empty_n),
      .in_r_read(in_r_read),
      .out_r_din(out_r_din),
      .out_r_full_n(!out_r_full),
      .out_r_write(out_r_write)
      );

endmodule
