//Copyright 1986-2014 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2014.4 (lin64) Build 1071353 Tue Nov 18 16:47:07 MST 2014
//Date        : Wed Dec 23 12:27:02 2015
//Host        : cecaraw running 64-bit openSUSE 13.2 (Harlequin) (x86_64)
//Command     : generate_target vivado_system_wrapper.bd
//Design      : vivado_system_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module vivado_system_wrapper
   (DDR_addr,
    DDR_ba,
    DDR_cas_n,
    DDR_ck_n,
    DDR_ck_p,
    DDR_cke,
    DDR_cs_n,
    DDR_dm,
    DDR_dq,
    DDR_dqs_n,
    DDR_dqs_p,
    DDR_odt,
    DDR_ras_n,
    DDR_reset_n,
    DDR_we_n,
    FIXED_IO_ddr_vrn,
    FIXED_IO_ddr_vrp,
    FIXED_IO_mio,
    FIXED_IO_ps_clk,
    FIXED_IO_ps_porb,
    FIXED_IO_ps_srstb,
    USBIND_0_port_indctl,
    USBIND_0_vbus_pwrfault,
    USBIND_0_vbus_pwrselect,
    clk_in,
    gpio_0_tri_io,
    user_addr,
    user_clk,
    user_irq,
    user_rd_data,
    user_rden,
    user_wr_data,
    user_wren,
    user_wstrb,
    vga_blue,
    vga_clk,
    vga_de,
    vga_green,
    vga_hsync,
    vga_red,
    vga_vsync,
    xillybus_M_AXI_araddr,
    xillybus_M_AXI_arburst,
    xillybus_M_AXI_arcache,
    xillybus_M_AXI_arlen,
    xillybus_M_AXI_arprot,
    xillybus_M_AXI_arready,
    xillybus_M_AXI_arsize,
    xillybus_M_AXI_arvalid,
    xillybus_M_AXI_awaddr,
    xillybus_M_AXI_awburst,
    xillybus_M_AXI_awcache,
    xillybus_M_AXI_awlen,
    xillybus_M_AXI_awprot,
    xillybus_M_AXI_awready,
    xillybus_M_AXI_awsize,
    xillybus_M_AXI_awvalid,
    xillybus_M_AXI_bready,
    xillybus_M_AXI_bresp,
    xillybus_M_AXI_bvalid,
    xillybus_M_AXI_rdata,
    xillybus_M_AXI_rlast,
    xillybus_M_AXI_rready,
    xillybus_M_AXI_rresp,
    xillybus_M_AXI_rvalid,
    xillybus_M_AXI_wdata,
    xillybus_M_AXI_wlast,
    xillybus_M_AXI_wready,
    xillybus_M_AXI_wstrb,
    xillybus_M_AXI_wvalid,
    xillybus_S_AXI_araddr,
    xillybus_S_AXI_arready,
    xillybus_S_AXI_arvalid,
    xillybus_S_AXI_awaddr,
    xillybus_S_AXI_awready,
    xillybus_S_AXI_awvalid,
    xillybus_S_AXI_bready,
    xillybus_S_AXI_bresp,
    xillybus_S_AXI_bvalid,
    xillybus_S_AXI_rdata,
    xillybus_S_AXI_rready,
    xillybus_S_AXI_rresp,
    xillybus_S_AXI_rvalid,
    xillybus_S_AXI_wdata,
    xillybus_S_AXI_wready,
    xillybus_S_AXI_wstrb,
    xillybus_S_AXI_wvalid,
    xillybus_bus_clk,
    xillybus_bus_rst_n,
    xillybus_host_interrupt);
  inout [14:0]DDR_addr;
  inout [2:0]DDR_ba;
  inout DDR_cas_n;
  inout DDR_ck_n;
  inout DDR_ck_p;
  inout DDR_cke;
  inout DDR_cs_n;
  inout [3:0]DDR_dm;
  inout [31:0]DDR_dq;
  inout [3:0]DDR_dqs_n;
  inout [3:0]DDR_dqs_p;
  inout DDR_odt;
  inout DDR_ras_n;
  inout DDR_reset_n;
  inout DDR_we_n;
  inout FIXED_IO_ddr_vrn;
  inout FIXED_IO_ddr_vrp;
  inout [53:0]FIXED_IO_mio;
  inout FIXED_IO_ps_clk;
  inout FIXED_IO_ps_porb;
  inout FIXED_IO_ps_srstb;
  output [1:0]USBIND_0_port_indctl;
  input USBIND_0_vbus_pwrfault;
  output USBIND_0_vbus_pwrselect;
  input clk_in;
  inout [55:0]gpio_0_tri_io;
  output [31:0]user_addr;
  output user_clk;
  input user_irq;
  input [31:0]user_rd_data;
  output user_rden;
  output [31:0]user_wr_data;
  output user_wren;
  output [3:0]user_wstrb;
  output [7:0]vga_blue;
  output vga_clk;
  output vga_de;
  output [7:0]vga_green;
  output vga_hsync;
  output [7:0]vga_red;
  output vga_vsync;
  input [31:0]xillybus_M_AXI_araddr;
  input [1:0]xillybus_M_AXI_arburst;
  input [3:0]xillybus_M_AXI_arcache;
  input [3:0]xillybus_M_AXI_arlen;
  input [2:0]xillybus_M_AXI_arprot;
  output xillybus_M_AXI_arready;
  input [2:0]xillybus_M_AXI_arsize;
  input xillybus_M_AXI_arvalid;
  input [31:0]xillybus_M_AXI_awaddr;
  input [1:0]xillybus_M_AXI_awburst;
  input [3:0]xillybus_M_AXI_awcache;
  input [3:0]xillybus_M_AXI_awlen;
  input [2:0]xillybus_M_AXI_awprot;
  output xillybus_M_AXI_awready;
  input [2:0]xillybus_M_AXI_awsize;
  input xillybus_M_AXI_awvalid;
  input xillybus_M_AXI_bready;
  output [1:0]xillybus_M_AXI_bresp;
  output xillybus_M_AXI_bvalid;
  output [63:0]xillybus_M_AXI_rdata;
  output xillybus_M_AXI_rlast;
  input xillybus_M_AXI_rready;
  output [1:0]xillybus_M_AXI_rresp;
  output xillybus_M_AXI_rvalid;
  input [63:0]xillybus_M_AXI_wdata;
  input xillybus_M_AXI_wlast;
  output xillybus_M_AXI_wready;
  input [7:0]xillybus_M_AXI_wstrb;
  input xillybus_M_AXI_wvalid;
  output [31:0]xillybus_S_AXI_araddr;
  input xillybus_S_AXI_arready;
  output xillybus_S_AXI_arvalid;
  output [31:0]xillybus_S_AXI_awaddr;
  input xillybus_S_AXI_awready;
  output xillybus_S_AXI_awvalid;
  output xillybus_S_AXI_bready;
  input [1:0]xillybus_S_AXI_bresp;
  input xillybus_S_AXI_bvalid;
  input [31:0]xillybus_S_AXI_rdata;
  output xillybus_S_AXI_rready;
  input [1:0]xillybus_S_AXI_rresp;
  input xillybus_S_AXI_rvalid;
  output [31:0]xillybus_S_AXI_wdata;
  input xillybus_S_AXI_wready;
  output [3:0]xillybus_S_AXI_wstrb;
  output xillybus_S_AXI_wvalid;
  output xillybus_bus_clk;
  output xillybus_bus_rst_n;
  input xillybus_host_interrupt;

  wire [14:0]DDR_addr;
  wire [2:0]DDR_ba;
  wire DDR_cas_n;
  wire DDR_ck_n;
  wire DDR_ck_p;
  wire DDR_cke;
  wire DDR_cs_n;
  wire [3:0]DDR_dm;
  wire [31:0]DDR_dq;
  wire [3:0]DDR_dqs_n;
  wire [3:0]DDR_dqs_p;
  wire DDR_odt;
  wire DDR_ras_n;
  wire DDR_reset_n;
  wire DDR_we_n;
  wire FIXED_IO_ddr_vrn;
  wire FIXED_IO_ddr_vrp;
  wire [53:0]FIXED_IO_mio;
  wire FIXED_IO_ps_clk;
  wire FIXED_IO_ps_porb;
  wire FIXED_IO_ps_srstb;
  wire [1:0]USBIND_0_port_indctl;
  wire USBIND_0_vbus_pwrfault;
  wire USBIND_0_vbus_pwrselect;
  wire clk_in;
  wire [0:0]gpio_0_tri_i_0;
  wire [1:1]gpio_0_tri_i_1;
  wire [10:10]gpio_0_tri_i_10;
  wire [11:11]gpio_0_tri_i_11;
  wire [12:12]gpio_0_tri_i_12;
  wire [13:13]gpio_0_tri_i_13;
  wire [14:14]gpio_0_tri_i_14;
  wire [15:15]gpio_0_tri_i_15;
  wire [16:16]gpio_0_tri_i_16;
  wire [17:17]gpio_0_tri_i_17;
  wire [18:18]gpio_0_tri_i_18;
  wire [19:19]gpio_0_tri_i_19;
  wire [2:2]gpio_0_tri_i_2;
  wire [20:20]gpio_0_tri_i_20;
  wire [21:21]gpio_0_tri_i_21;
  wire [22:22]gpio_0_tri_i_22;
  wire [23:23]gpio_0_tri_i_23;
  wire [24:24]gpio_0_tri_i_24;
  wire [25:25]gpio_0_tri_i_25;
  wire [26:26]gpio_0_tri_i_26;
  wire [27:27]gpio_0_tri_i_27;
  wire [28:28]gpio_0_tri_i_28;
  wire [29:29]gpio_0_tri_i_29;
  wire [3:3]gpio_0_tri_i_3;
  wire [30:30]gpio_0_tri_i_30;
  wire [31:31]gpio_0_tri_i_31;
  wire [32:32]gpio_0_tri_i_32;
  wire [33:33]gpio_0_tri_i_33;
  wire [34:34]gpio_0_tri_i_34;
  wire [35:35]gpio_0_tri_i_35;
  wire [36:36]gpio_0_tri_i_36;
  wire [37:37]gpio_0_tri_i_37;
  wire [38:38]gpio_0_tri_i_38;
  wire [39:39]gpio_0_tri_i_39;
  wire [4:4]gpio_0_tri_i_4;
  wire [40:40]gpio_0_tri_i_40;
  wire [41:41]gpio_0_tri_i_41;
  wire [42:42]gpio_0_tri_i_42;
  wire [43:43]gpio_0_tri_i_43;
  wire [44:44]gpio_0_tri_i_44;
  wire [45:45]gpio_0_tri_i_45;
  wire [46:46]gpio_0_tri_i_46;
  wire [47:47]gpio_0_tri_i_47;
  wire [48:48]gpio_0_tri_i_48;
  wire [49:49]gpio_0_tri_i_49;
  wire [5:5]gpio_0_tri_i_5;
  wire [50:50]gpio_0_tri_i_50;
  wire [51:51]gpio_0_tri_i_51;
  wire [52:52]gpio_0_tri_i_52;
  wire [53:53]gpio_0_tri_i_53;
  wire [54:54]gpio_0_tri_i_54;
  wire [55:55]gpio_0_tri_i_55;
  wire [6:6]gpio_0_tri_i_6;
  wire [7:7]gpio_0_tri_i_7;
  wire [8:8]gpio_0_tri_i_8;
  wire [9:9]gpio_0_tri_i_9;
  wire [0:0]gpio_0_tri_io_0;
  wire [1:1]gpio_0_tri_io_1;
  wire [10:10]gpio_0_tri_io_10;
  wire [11:11]gpio_0_tri_io_11;
  wire [12:12]gpio_0_tri_io_12;
  wire [13:13]gpio_0_tri_io_13;
  wire [14:14]gpio_0_tri_io_14;
  wire [15:15]gpio_0_tri_io_15;
  wire [16:16]gpio_0_tri_io_16;
  wire [17:17]gpio_0_tri_io_17;
  wire [18:18]gpio_0_tri_io_18;
  wire [19:19]gpio_0_tri_io_19;
  wire [2:2]gpio_0_tri_io_2;
  wire [20:20]gpio_0_tri_io_20;
  wire [21:21]gpio_0_tri_io_21;
  wire [22:22]gpio_0_tri_io_22;
  wire [23:23]gpio_0_tri_io_23;
  wire [24:24]gpio_0_tri_io_24;
  wire [25:25]gpio_0_tri_io_25;
  wire [26:26]gpio_0_tri_io_26;
  wire [27:27]gpio_0_tri_io_27;
  wire [28:28]gpio_0_tri_io_28;
  wire [29:29]gpio_0_tri_io_29;
  wire [3:3]gpio_0_tri_io_3;
  wire [30:30]gpio_0_tri_io_30;
  wire [31:31]gpio_0_tri_io_31;
  wire [32:32]gpio_0_tri_io_32;
  wire [33:33]gpio_0_tri_io_33;
  wire [34:34]gpio_0_tri_io_34;
  wire [35:35]gpio_0_tri_io_35;
  wire [36:36]gpio_0_tri_io_36;
  wire [37:37]gpio_0_tri_io_37;
  wire [38:38]gpio_0_tri_io_38;
  wire [39:39]gpio_0_tri_io_39;
  wire [4:4]gpio_0_tri_io_4;
  wire [40:40]gpio_0_tri_io_40;
  wire [41:41]gpio_0_tri_io_41;
  wire [42:42]gpio_0_tri_io_42;
  wire [43:43]gpio_0_tri_io_43;
  wire [44:44]gpio_0_tri_io_44;
  wire [45:45]gpio_0_tri_io_45;
  wire [46:46]gpio_0_tri_io_46;
  wire [47:47]gpio_0_tri_io_47;
  wire [48:48]gpio_0_tri_io_48;
  wire [49:49]gpio_0_tri_io_49;
  wire [5:5]gpio_0_tri_io_5;
  wire [50:50]gpio_0_tri_io_50;
  wire [51:51]gpio_0_tri_io_51;
  wire [52:52]gpio_0_tri_io_52;
  wire [53:53]gpio_0_tri_io_53;
  wire [54:54]gpio_0_tri_io_54;
  wire [55:55]gpio_0_tri_io_55;
  wire [6:6]gpio_0_tri_io_6;
  wire [7:7]gpio_0_tri_io_7;
  wire [8:8]gpio_0_tri_io_8;
  wire [9:9]gpio_0_tri_io_9;
  wire [0:0]gpio_0_tri_o_0;
  wire [1:1]gpio_0_tri_o_1;
  wire [10:10]gpio_0_tri_o_10;
  wire [11:11]gpio_0_tri_o_11;
  wire [12:12]gpio_0_tri_o_12;
  wire [13:13]gpio_0_tri_o_13;
  wire [14:14]gpio_0_tri_o_14;
  wire [15:15]gpio_0_tri_o_15;
  wire [16:16]gpio_0_tri_o_16;
  wire [17:17]gpio_0_tri_o_17;
  wire [18:18]gpio_0_tri_o_18;
  wire [19:19]gpio_0_tri_o_19;
  wire [2:2]gpio_0_tri_o_2;
  wire [20:20]gpio_0_tri_o_20;
  wire [21:21]gpio_0_tri_o_21;
  wire [22:22]gpio_0_tri_o_22;
  wire [23:23]gpio_0_tri_o_23;
  wire [24:24]gpio_0_tri_o_24;
  wire [25:25]gpio_0_tri_o_25;
  wire [26:26]gpio_0_tri_o_26;
  wire [27:27]gpio_0_tri_o_27;
  wire [28:28]gpio_0_tri_o_28;
  wire [29:29]gpio_0_tri_o_29;
  wire [3:3]gpio_0_tri_o_3;
  wire [30:30]gpio_0_tri_o_30;
  wire [31:31]gpio_0_tri_o_31;
  wire [32:32]gpio_0_tri_o_32;
  wire [33:33]gpio_0_tri_o_33;
  wire [34:34]gpio_0_tri_o_34;
  wire [35:35]gpio_0_tri_o_35;
  wire [36:36]gpio_0_tri_o_36;
  wire [37:37]gpio_0_tri_o_37;
  wire [38:38]gpio_0_tri_o_38;
  wire [39:39]gpio_0_tri_o_39;
  wire [4:4]gpio_0_tri_o_4;
  wire [40:40]gpio_0_tri_o_40;
  wire [41:41]gpio_0_tri_o_41;
  wire [42:42]gpio_0_tri_o_42;
  wire [43:43]gpio_0_tri_o_43;
  wire [44:44]gpio_0_tri_o_44;
  wire [45:45]gpio_0_tri_o_45;
  wire [46:46]gpio_0_tri_o_46;
  wire [47:47]gpio_0_tri_o_47;
  wire [48:48]gpio_0_tri_o_48;
  wire [49:49]gpio_0_tri_o_49;
  wire [5:5]gpio_0_tri_o_5;
  wire [50:50]gpio_0_tri_o_50;
  wire [51:51]gpio_0_tri_o_51;
  wire [52:52]gpio_0_tri_o_52;
  wire [53:53]gpio_0_tri_o_53;
  wire [54:54]gpio_0_tri_o_54;
  wire [55:55]gpio_0_tri_o_55;
  wire [6:6]gpio_0_tri_o_6;
  wire [7:7]gpio_0_tri_o_7;
  wire [8:8]gpio_0_tri_o_8;
  wire [9:9]gpio_0_tri_o_9;
  wire [0:0]gpio_0_tri_t_0;
  wire [1:1]gpio_0_tri_t_1;
  wire [10:10]gpio_0_tri_t_10;
  wire [11:11]gpio_0_tri_t_11;
  wire [12:12]gpio_0_tri_t_12;
  wire [13:13]gpio_0_tri_t_13;
  wire [14:14]gpio_0_tri_t_14;
  wire [15:15]gpio_0_tri_t_15;
  wire [16:16]gpio_0_tri_t_16;
  wire [17:17]gpio_0_tri_t_17;
  wire [18:18]gpio_0_tri_t_18;
  wire [19:19]gpio_0_tri_t_19;
  wire [2:2]gpio_0_tri_t_2;
  wire [20:20]gpio_0_tri_t_20;
  wire [21:21]gpio_0_tri_t_21;
  wire [22:22]gpio_0_tri_t_22;
  wire [23:23]gpio_0_tri_t_23;
  wire [24:24]gpio_0_tri_t_24;
  wire [25:25]gpio_0_tri_t_25;
  wire [26:26]gpio_0_tri_t_26;
  wire [27:27]gpio_0_tri_t_27;
  wire [28:28]gpio_0_tri_t_28;
  wire [29:29]gpio_0_tri_t_29;
  wire [3:3]gpio_0_tri_t_3;
  wire [30:30]gpio_0_tri_t_30;
  wire [31:31]gpio_0_tri_t_31;
  wire [32:32]gpio_0_tri_t_32;
  wire [33:33]gpio_0_tri_t_33;
  wire [34:34]gpio_0_tri_t_34;
  wire [35:35]gpio_0_tri_t_35;
  wire [36:36]gpio_0_tri_t_36;
  wire [37:37]gpio_0_tri_t_37;
  wire [38:38]gpio_0_tri_t_38;
  wire [39:39]gpio_0_tri_t_39;
  wire [4:4]gpio_0_tri_t_4;
  wire [40:40]gpio_0_tri_t_40;
  wire [41:41]gpio_0_tri_t_41;
  wire [42:42]gpio_0_tri_t_42;
  wire [43:43]gpio_0_tri_t_43;
  wire [44:44]gpio_0_tri_t_44;
  wire [45:45]gpio_0_tri_t_45;
  wire [46:46]gpio_0_tri_t_46;
  wire [47:47]gpio_0_tri_t_47;
  wire [48:48]gpio_0_tri_t_48;
  wire [49:49]gpio_0_tri_t_49;
  wire [5:5]gpio_0_tri_t_5;
  wire [50:50]gpio_0_tri_t_50;
  wire [51:51]gpio_0_tri_t_51;
  wire [52:52]gpio_0_tri_t_52;
  wire [53:53]gpio_0_tri_t_53;
  wire [54:54]gpio_0_tri_t_54;
  wire [55:55]gpio_0_tri_t_55;
  wire [6:6]gpio_0_tri_t_6;
  wire [7:7]gpio_0_tri_t_7;
  wire [8:8]gpio_0_tri_t_8;
  wire [9:9]gpio_0_tri_t_9;
  wire [31:0]user_addr;
  wire user_clk;
  wire user_irq;
  wire [31:0]user_rd_data;
  wire user_rden;
  wire [31:0]user_wr_data;
  wire user_wren;
  wire [3:0]user_wstrb;
  wire [7:0]vga_blue;
  wire vga_clk;
  wire vga_de;
  wire [7:0]vga_green;
  wire vga_hsync;
  wire [7:0]vga_red;
  wire vga_vsync;
  wire [31:0]xillybus_M_AXI_araddr;
  wire [1:0]xillybus_M_AXI_arburst;
  wire [3:0]xillybus_M_AXI_arcache;
  wire [3:0]xillybus_M_AXI_arlen;
  wire [2:0]xillybus_M_AXI_arprot;
  wire xillybus_M_AXI_arready;
  wire [2:0]xillybus_M_AXI_arsize;
  wire xillybus_M_AXI_arvalid;
  wire [31:0]xillybus_M_AXI_awaddr;
  wire [1:0]xillybus_M_AXI_awburst;
  wire [3:0]xillybus_M_AXI_awcache;
  wire [3:0]xillybus_M_AXI_awlen;
  wire [2:0]xillybus_M_AXI_awprot;
  wire xillybus_M_AXI_awready;
  wire [2:0]xillybus_M_AXI_awsize;
  wire xillybus_M_AXI_awvalid;
  wire xillybus_M_AXI_bready;
  wire [1:0]xillybus_M_AXI_bresp;
  wire xillybus_M_AXI_bvalid;
  wire [63:0]xillybus_M_AXI_rdata;
  wire xillybus_M_AXI_rlast;
  wire xillybus_M_AXI_rready;
  wire [1:0]xillybus_M_AXI_rresp;
  wire xillybus_M_AXI_rvalid;
  wire [63:0]xillybus_M_AXI_wdata;
  wire xillybus_M_AXI_wlast;
  wire xillybus_M_AXI_wready;
  wire [7:0]xillybus_M_AXI_wstrb;
  wire xillybus_M_AXI_wvalid;
  wire [31:0]xillybus_S_AXI_araddr;
  wire xillybus_S_AXI_arready;
  wire xillybus_S_AXI_arvalid;
  wire [31:0]xillybus_S_AXI_awaddr;
  wire xillybus_S_AXI_awready;
  wire xillybus_S_AXI_awvalid;
  wire xillybus_S_AXI_bready;
  wire [1:0]xillybus_S_AXI_bresp;
  wire xillybus_S_AXI_bvalid;
  wire [31:0]xillybus_S_AXI_rdata;
  wire xillybus_S_AXI_rready;
  wire [1:0]xillybus_S_AXI_rresp;
  wire xillybus_S_AXI_rvalid;
  wire [31:0]xillybus_S_AXI_wdata;
  wire xillybus_S_AXI_wready;
  wire [3:0]xillybus_S_AXI_wstrb;
  wire xillybus_S_AXI_wvalid;
  wire xillybus_bus_clk;
  wire xillybus_bus_rst_n;
  wire xillybus_host_interrupt;

IOBUF gpio_0_tri_iobuf_0
       (.I(gpio_0_tri_o_0),
        .IO(gpio_0_tri_io[0]),
        .O(gpio_0_tri_i_0),
        .T(gpio_0_tri_t_0));
IOBUF gpio_0_tri_iobuf_1
       (.I(gpio_0_tri_o_1),
        .IO(gpio_0_tri_io[1]),
        .O(gpio_0_tri_i_1),
        .T(gpio_0_tri_t_1));
IOBUF gpio_0_tri_iobuf_10
       (.I(gpio_0_tri_o_10),
        .IO(gpio_0_tri_io[10]),
        .O(gpio_0_tri_i_10),
        .T(gpio_0_tri_t_10));
IOBUF gpio_0_tri_iobuf_11
       (.I(gpio_0_tri_o_11),
        .IO(gpio_0_tri_io[11]),
        .O(gpio_0_tri_i_11),
        .T(gpio_0_tri_t_11));
IOBUF gpio_0_tri_iobuf_12
       (.I(gpio_0_tri_o_12),
        .IO(gpio_0_tri_io[12]),
        .O(gpio_0_tri_i_12),
        .T(gpio_0_tri_t_12));
IOBUF gpio_0_tri_iobuf_13
       (.I(gpio_0_tri_o_13),
        .IO(gpio_0_tri_io[13]),
        .O(gpio_0_tri_i_13),
        .T(gpio_0_tri_t_13));
IOBUF gpio_0_tri_iobuf_14
       (.I(gpio_0_tri_o_14),
        .IO(gpio_0_tri_io[14]),
        .O(gpio_0_tri_i_14),
        .T(gpio_0_tri_t_14));
IOBUF gpio_0_tri_iobuf_15
       (.I(gpio_0_tri_o_15),
        .IO(gpio_0_tri_io[15]),
        .O(gpio_0_tri_i_15),
        .T(gpio_0_tri_t_15));
IOBUF gpio_0_tri_iobuf_16
       (.I(gpio_0_tri_o_16),
        .IO(gpio_0_tri_io[16]),
        .O(gpio_0_tri_i_16),
        .T(gpio_0_tri_t_16));
IOBUF gpio_0_tri_iobuf_17
       (.I(gpio_0_tri_o_17),
        .IO(gpio_0_tri_io[17]),
        .O(gpio_0_tri_i_17),
        .T(gpio_0_tri_t_17));
IOBUF gpio_0_tri_iobuf_18
       (.I(gpio_0_tri_o_18),
        .IO(gpio_0_tri_io[18]),
        .O(gpio_0_tri_i_18),
        .T(gpio_0_tri_t_18));
IOBUF gpio_0_tri_iobuf_19
       (.I(gpio_0_tri_o_19),
        .IO(gpio_0_tri_io[19]),
        .O(gpio_0_tri_i_19),
        .T(gpio_0_tri_t_19));
IOBUF gpio_0_tri_iobuf_2
       (.I(gpio_0_tri_o_2),
        .IO(gpio_0_tri_io[2]),
        .O(gpio_0_tri_i_2),
        .T(gpio_0_tri_t_2));
IOBUF gpio_0_tri_iobuf_20
       (.I(gpio_0_tri_o_20),
        .IO(gpio_0_tri_io[20]),
        .O(gpio_0_tri_i_20),
        .T(gpio_0_tri_t_20));
IOBUF gpio_0_tri_iobuf_21
       (.I(gpio_0_tri_o_21),
        .IO(gpio_0_tri_io[21]),
        .O(gpio_0_tri_i_21),
        .T(gpio_0_tri_t_21));
IOBUF gpio_0_tri_iobuf_22
       (.I(gpio_0_tri_o_22),
        .IO(gpio_0_tri_io[22]),
        .O(gpio_0_tri_i_22),
        .T(gpio_0_tri_t_22));
IOBUF gpio_0_tri_iobuf_23
       (.I(gpio_0_tri_o_23),
        .IO(gpio_0_tri_io[23]),
        .O(gpio_0_tri_i_23),
        .T(gpio_0_tri_t_23));
IOBUF gpio_0_tri_iobuf_24
       (.I(gpio_0_tri_o_24),
        .IO(gpio_0_tri_io[24]),
        .O(gpio_0_tri_i_24),
        .T(gpio_0_tri_t_24));
IOBUF gpio_0_tri_iobuf_25
       (.I(gpio_0_tri_o_25),
        .IO(gpio_0_tri_io[25]),
        .O(gpio_0_tri_i_25),
        .T(gpio_0_tri_t_25));
IOBUF gpio_0_tri_iobuf_26
       (.I(gpio_0_tri_o_26),
        .IO(gpio_0_tri_io[26]),
        .O(gpio_0_tri_i_26),
        .T(gpio_0_tri_t_26));
IOBUF gpio_0_tri_iobuf_27
       (.I(gpio_0_tri_o_27),
        .IO(gpio_0_tri_io[27]),
        .O(gpio_0_tri_i_27),
        .T(gpio_0_tri_t_27));
IOBUF gpio_0_tri_iobuf_28
       (.I(gpio_0_tri_o_28),
        .IO(gpio_0_tri_io[28]),
        .O(gpio_0_tri_i_28),
        .T(gpio_0_tri_t_28));
IOBUF gpio_0_tri_iobuf_29
       (.I(gpio_0_tri_o_29),
        .IO(gpio_0_tri_io[29]),
        .O(gpio_0_tri_i_29),
        .T(gpio_0_tri_t_29));
IOBUF gpio_0_tri_iobuf_3
       (.I(gpio_0_tri_o_3),
        .IO(gpio_0_tri_io[3]),
        .O(gpio_0_tri_i_3),
        .T(gpio_0_tri_t_3));
IOBUF gpio_0_tri_iobuf_30
       (.I(gpio_0_tri_o_30),
        .IO(gpio_0_tri_io[30]),
        .O(gpio_0_tri_i_30),
        .T(gpio_0_tri_t_30));
IOBUF gpio_0_tri_iobuf_31
       (.I(gpio_0_tri_o_31),
        .IO(gpio_0_tri_io[31]),
        .O(gpio_0_tri_i_31),
        .T(gpio_0_tri_t_31));
IOBUF gpio_0_tri_iobuf_32
       (.I(gpio_0_tri_o_32),
        .IO(gpio_0_tri_io[32]),
        .O(gpio_0_tri_i_32),
        .T(gpio_0_tri_t_32));
IOBUF gpio_0_tri_iobuf_33
       (.I(gpio_0_tri_o_33),
        .IO(gpio_0_tri_io[33]),
        .O(gpio_0_tri_i_33),
        .T(gpio_0_tri_t_33));
IOBUF gpio_0_tri_iobuf_34
       (.I(gpio_0_tri_o_34),
        .IO(gpio_0_tri_io[34]),
        .O(gpio_0_tri_i_34),
        .T(gpio_0_tri_t_34));
IOBUF gpio_0_tri_iobuf_35
       (.I(gpio_0_tri_o_35),
        .IO(gpio_0_tri_io[35]),
        .O(gpio_0_tri_i_35),
        .T(gpio_0_tri_t_35));
IOBUF gpio_0_tri_iobuf_36
       (.I(gpio_0_tri_o_36),
        .IO(gpio_0_tri_io[36]),
        .O(gpio_0_tri_i_36),
        .T(gpio_0_tri_t_36));
IOBUF gpio_0_tri_iobuf_37
       (.I(gpio_0_tri_o_37),
        .IO(gpio_0_tri_io[37]),
        .O(gpio_0_tri_i_37),
        .T(gpio_0_tri_t_37));
IOBUF gpio_0_tri_iobuf_38
       (.I(gpio_0_tri_o_38),
        .IO(gpio_0_tri_io[38]),
        .O(gpio_0_tri_i_38),
        .T(gpio_0_tri_t_38));
IOBUF gpio_0_tri_iobuf_39
       (.I(gpio_0_tri_o_39),
        .IO(gpio_0_tri_io[39]),
        .O(gpio_0_tri_i_39),
        .T(gpio_0_tri_t_39));
IOBUF gpio_0_tri_iobuf_4
       (.I(gpio_0_tri_o_4),
        .IO(gpio_0_tri_io[4]),
        .O(gpio_0_tri_i_4),
        .T(gpio_0_tri_t_4));
IOBUF gpio_0_tri_iobuf_40
       (.I(gpio_0_tri_o_40),
        .IO(gpio_0_tri_io[40]),
        .O(gpio_0_tri_i_40),
        .T(gpio_0_tri_t_40));
IOBUF gpio_0_tri_iobuf_41
       (.I(gpio_0_tri_o_41),
        .IO(gpio_0_tri_io[41]),
        .O(gpio_0_tri_i_41),
        .T(gpio_0_tri_t_41));
IOBUF gpio_0_tri_iobuf_42
       (.I(gpio_0_tri_o_42),
        .IO(gpio_0_tri_io[42]),
        .O(gpio_0_tri_i_42),
        .T(gpio_0_tri_t_42));
IOBUF gpio_0_tri_iobuf_43
       (.I(gpio_0_tri_o_43),
        .IO(gpio_0_tri_io[43]),
        .O(gpio_0_tri_i_43),
        .T(gpio_0_tri_t_43));
IOBUF gpio_0_tri_iobuf_44
       (.I(gpio_0_tri_o_44),
        .IO(gpio_0_tri_io[44]),
        .O(gpio_0_tri_i_44),
        .T(gpio_0_tri_t_44));
IOBUF gpio_0_tri_iobuf_45
       (.I(gpio_0_tri_o_45),
        .IO(gpio_0_tri_io[45]),
        .O(gpio_0_tri_i_45),
        .T(gpio_0_tri_t_45));
IOBUF gpio_0_tri_iobuf_46
       (.I(gpio_0_tri_o_46),
        .IO(gpio_0_tri_io[46]),
        .O(gpio_0_tri_i_46),
        .T(gpio_0_tri_t_46));
IOBUF gpio_0_tri_iobuf_47
       (.I(gpio_0_tri_o_47),
        .IO(gpio_0_tri_io[47]),
        .O(gpio_0_tri_i_47),
        .T(gpio_0_tri_t_47));
IOBUF gpio_0_tri_iobuf_48
       (.I(gpio_0_tri_o_48),
        .IO(gpio_0_tri_io[48]),
        .O(gpio_0_tri_i_48),
        .T(gpio_0_tri_t_48));
IOBUF gpio_0_tri_iobuf_49
       (.I(gpio_0_tri_o_49),
        .IO(gpio_0_tri_io[49]),
        .O(gpio_0_tri_i_49),
        .T(gpio_0_tri_t_49));
IOBUF gpio_0_tri_iobuf_5
       (.I(gpio_0_tri_o_5),
        .IO(gpio_0_tri_io[5]),
        .O(gpio_0_tri_i_5),
        .T(gpio_0_tri_t_5));
IOBUF gpio_0_tri_iobuf_50
       (.I(gpio_0_tri_o_50),
        .IO(gpio_0_tri_io[50]),
        .O(gpio_0_tri_i_50),
        .T(gpio_0_tri_t_50));
IOBUF gpio_0_tri_iobuf_51
       (.I(gpio_0_tri_o_51),
        .IO(gpio_0_tri_io[51]),
        .O(gpio_0_tri_i_51),
        .T(gpio_0_tri_t_51));
IOBUF gpio_0_tri_iobuf_52
       (.I(gpio_0_tri_o_52),
        .IO(gpio_0_tri_io[52]),
        .O(gpio_0_tri_i_52),
        .T(gpio_0_tri_t_52));
IOBUF gpio_0_tri_iobuf_53
       (.I(gpio_0_tri_o_53),
        .IO(gpio_0_tri_io[53]),
        .O(gpio_0_tri_i_53),
        .T(gpio_0_tri_t_53));
IOBUF gpio_0_tri_iobuf_54
       (.I(gpio_0_tri_o_54),
        .IO(gpio_0_tri_io[54]),
        .O(gpio_0_tri_i_54),
        .T(gpio_0_tri_t_54));
IOBUF gpio_0_tri_iobuf_55
       (.I(gpio_0_tri_o_55),
        .IO(gpio_0_tri_io[55]),
        .O(gpio_0_tri_i_55),
        .T(gpio_0_tri_t_55));
IOBUF gpio_0_tri_iobuf_6
       (.I(gpio_0_tri_o_6),
        .IO(gpio_0_tri_io[6]),
        .O(gpio_0_tri_i_6),
        .T(gpio_0_tri_t_6));
IOBUF gpio_0_tri_iobuf_7
       (.I(gpio_0_tri_o_7),
        .IO(gpio_0_tri_io[7]),
        .O(gpio_0_tri_i_7),
        .T(gpio_0_tri_t_7));
IOBUF gpio_0_tri_iobuf_8
       (.I(gpio_0_tri_o_8),
        .IO(gpio_0_tri_io[8]),
        .O(gpio_0_tri_i_8),
        .T(gpio_0_tri_t_8));
IOBUF gpio_0_tri_iobuf_9
       (.I(gpio_0_tri_o_9),
        .IO(gpio_0_tri_io[9]),
        .O(gpio_0_tri_i_9),
        .T(gpio_0_tri_t_9));
vivado_system vivado_system_i
       (.DDR_addr(DDR_addr),
        .DDR_ba(DDR_ba),
        .DDR_cas_n(DDR_cas_n),
        .DDR_ck_n(DDR_ck_n),
        .DDR_ck_p(DDR_ck_p),
        .DDR_cke(DDR_cke),
        .DDR_cs_n(DDR_cs_n),
        .DDR_dm(DDR_dm),
        .DDR_dq(DDR_dq),
        .DDR_dqs_n(DDR_dqs_n),
        .DDR_dqs_p(DDR_dqs_p),
        .DDR_odt(DDR_odt),
        .DDR_ras_n(DDR_ras_n),
        .DDR_reset_n(DDR_reset_n),
        .DDR_we_n(DDR_we_n),
        .FIXED_IO_ddr_vrn(FIXED_IO_ddr_vrn),
        .FIXED_IO_ddr_vrp(FIXED_IO_ddr_vrp),
        .FIXED_IO_mio(FIXED_IO_mio),
        .FIXED_IO_ps_clk(FIXED_IO_ps_clk),
        .FIXED_IO_ps_porb(FIXED_IO_ps_porb),
        .FIXED_IO_ps_srstb(FIXED_IO_ps_srstb),
        .GPIO_0_tri_i({gpio_0_tri_i_55,gpio_0_tri_i_54,gpio_0_tri_i_53,gpio_0_tri_i_52,gpio_0_tri_i_51,gpio_0_tri_i_50,gpio_0_tri_i_49,gpio_0_tri_i_48,gpio_0_tri_i_47,gpio_0_tri_i_46,gpio_0_tri_i_45,gpio_0_tri_i_44,gpio_0_tri_i_43,gpio_0_tri_i_42,gpio_0_tri_i_41,gpio_0_tri_i_40,gpio_0_tri_i_39,gpio_0_tri_i_38,gpio_0_tri_i_37,gpio_0_tri_i_36,gpio_0_tri_i_35,gpio_0_tri_i_34,gpio_0_tri_i_33,gpio_0_tri_i_32,gpio_0_tri_i_31,gpio_0_tri_i_30,gpio_0_tri_i_29,gpio_0_tri_i_28,gpio_0_tri_i_27,gpio_0_tri_i_26,gpio_0_tri_i_25,gpio_0_tri_i_24,gpio_0_tri_i_23,gpio_0_tri_i_22,gpio_0_tri_i_21,gpio_0_tri_i_20,gpio_0_tri_i_19,gpio_0_tri_i_18,gpio_0_tri_i_17,gpio_0_tri_i_16,gpio_0_tri_i_15,gpio_0_tri_i_14,gpio_0_tri_i_13,gpio_0_tri_i_12,gpio_0_tri_i_11,gpio_0_tri_i_10,gpio_0_tri_i_9,gpio_0_tri_i_8,gpio_0_tri_i_7,gpio_0_tri_i_6,gpio_0_tri_i_5,gpio_0_tri_i_4,gpio_0_tri_i_3,gpio_0_tri_i_2,gpio_0_tri_i_1,gpio_0_tri_i_0}),
        .GPIO_0_tri_o({gpio_0_tri_o_55,gpio_0_tri_o_54,gpio_0_tri_o_53,gpio_0_tri_o_52,gpio_0_tri_o_51,gpio_0_tri_o_50,gpio_0_tri_o_49,gpio_0_tri_o_48,gpio_0_tri_o_47,gpio_0_tri_o_46,gpio_0_tri_o_45,gpio_0_tri_o_44,gpio_0_tri_o_43,gpio_0_tri_o_42,gpio_0_tri_o_41,gpio_0_tri_o_40,gpio_0_tri_o_39,gpio_0_tri_o_38,gpio_0_tri_o_37,gpio_0_tri_o_36,gpio_0_tri_o_35,gpio_0_tri_o_34,gpio_0_tri_o_33,gpio_0_tri_o_32,gpio_0_tri_o_31,gpio_0_tri_o_30,gpio_0_tri_o_29,gpio_0_tri_o_28,gpio_0_tri_o_27,gpio_0_tri_o_26,gpio_0_tri_o_25,gpio_0_tri_o_24,gpio_0_tri_o_23,gpio_0_tri_o_22,gpio_0_tri_o_21,gpio_0_tri_o_20,gpio_0_tri_o_19,gpio_0_tri_o_18,gpio_0_tri_o_17,gpio_0_tri_o_16,gpio_0_tri_o_15,gpio_0_tri_o_14,gpio_0_tri_o_13,gpio_0_tri_o_12,gpio_0_tri_o_11,gpio_0_tri_o_10,gpio_0_tri_o_9,gpio_0_tri_o_8,gpio_0_tri_o_7,gpio_0_tri_o_6,gpio_0_tri_o_5,gpio_0_tri_o_4,gpio_0_tri_o_3,gpio_0_tri_o_2,gpio_0_tri_o_1,gpio_0_tri_o_0}),
        .GPIO_0_tri_t({gpio_0_tri_t_55,gpio_0_tri_t_54,gpio_0_tri_t_53,gpio_0_tri_t_52,gpio_0_tri_t_51,gpio_0_tri_t_50,gpio_0_tri_t_49,gpio_0_tri_t_48,gpio_0_tri_t_47,gpio_0_tri_t_46,gpio_0_tri_t_45,gpio_0_tri_t_44,gpio_0_tri_t_43,gpio_0_tri_t_42,gpio_0_tri_t_41,gpio_0_tri_t_40,gpio_0_tri_t_39,gpio_0_tri_t_38,gpio_0_tri_t_37,gpio_0_tri_t_36,gpio_0_tri_t_35,gpio_0_tri_t_34,gpio_0_tri_t_33,gpio_0_tri_t_32,gpio_0_tri_t_31,gpio_0_tri_t_30,gpio_0_tri_t_29,gpio_0_tri_t_28,gpio_0_tri_t_27,gpio_0_tri_t_26,gpio_0_tri_t_25,gpio_0_tri_t_24,gpio_0_tri_t_23,gpio_0_tri_t_22,gpio_0_tri_t_21,gpio_0_tri_t_20,gpio_0_tri_t_19,gpio_0_tri_t_18,gpio_0_tri_t_17,gpio_0_tri_t_16,gpio_0_tri_t_15,gpio_0_tri_t_14,gpio_0_tri_t_13,gpio_0_tri_t_12,gpio_0_tri_t_11,gpio_0_tri_t_10,gpio_0_tri_t_9,gpio_0_tri_t_8,gpio_0_tri_t_7,gpio_0_tri_t_6,gpio_0_tri_t_5,gpio_0_tri_t_4,gpio_0_tri_t_3,gpio_0_tri_t_2,gpio_0_tri_t_1,gpio_0_tri_t_0}),
        .USBIND_0_port_indctl(USBIND_0_port_indctl),
        .USBIND_0_vbus_pwrfault(USBIND_0_vbus_pwrfault),
        .USBIND_0_vbus_pwrselect(USBIND_0_vbus_pwrselect),
        .clk_in(clk_in),
        .user_addr(user_addr),
        .user_clk(user_clk),
        .user_irq(user_irq),
        .user_rd_data(user_rd_data),
        .user_rden(user_rden),
        .user_wr_data(user_wr_data),
        .user_wren(user_wren),
        .user_wstrb(user_wstrb),
        .vga_blue(vga_blue),
        .vga_clk(vga_clk),
        .vga_de(vga_de),
        .vga_green(vga_green),
        .vga_hsync(vga_hsync),
        .vga_red(vga_red),
        .vga_vsync(vga_vsync),
        .xillybus_M_AXI_araddr(xillybus_M_AXI_araddr),
        .xillybus_M_AXI_arburst(xillybus_M_AXI_arburst),
        .xillybus_M_AXI_arcache(xillybus_M_AXI_arcache),
        .xillybus_M_AXI_arlen(xillybus_M_AXI_arlen),
        .xillybus_M_AXI_arprot(xillybus_M_AXI_arprot),
        .xillybus_M_AXI_arready(xillybus_M_AXI_arready),
        .xillybus_M_AXI_arsize(xillybus_M_AXI_arsize),
        .xillybus_M_AXI_arvalid(xillybus_M_AXI_arvalid),
        .xillybus_M_AXI_awaddr(xillybus_M_AXI_awaddr),
        .xillybus_M_AXI_awburst(xillybus_M_AXI_awburst),
        .xillybus_M_AXI_awcache(xillybus_M_AXI_awcache),
        .xillybus_M_AXI_awlen(xillybus_M_AXI_awlen),
        .xillybus_M_AXI_awprot(xillybus_M_AXI_awprot),
        .xillybus_M_AXI_awready(xillybus_M_AXI_awready),
        .xillybus_M_AXI_awsize(xillybus_M_AXI_awsize),
        .xillybus_M_AXI_awvalid(xillybus_M_AXI_awvalid),
        .xillybus_M_AXI_bready(xillybus_M_AXI_bready),
        .xillybus_M_AXI_bresp(xillybus_M_AXI_bresp),
        .xillybus_M_AXI_bvalid(xillybus_M_AXI_bvalid),
        .xillybus_M_AXI_rdata(xillybus_M_AXI_rdata),
        .xillybus_M_AXI_rlast(xillybus_M_AXI_rlast),
        .xillybus_M_AXI_rready(xillybus_M_AXI_rready),
        .xillybus_M_AXI_rresp(xillybus_M_AXI_rresp),
        .xillybus_M_AXI_rvalid(xillybus_M_AXI_rvalid),
        .xillybus_M_AXI_wdata(xillybus_M_AXI_wdata),
        .xillybus_M_AXI_wlast(xillybus_M_AXI_wlast),
        .xillybus_M_AXI_wready(xillybus_M_AXI_wready),
        .xillybus_M_AXI_wstrb(xillybus_M_AXI_wstrb),
        .xillybus_M_AXI_wvalid(xillybus_M_AXI_wvalid),
        .xillybus_S_AXI_araddr(xillybus_S_AXI_araddr),
        .xillybus_S_AXI_arready(xillybus_S_AXI_arready),
        .xillybus_S_AXI_arvalid(xillybus_S_AXI_arvalid),
        .xillybus_S_AXI_awaddr(xillybus_S_AXI_awaddr),
        .xillybus_S_AXI_awready(xillybus_S_AXI_awready),
        .xillybus_S_AXI_awvalid(xillybus_S_AXI_awvalid),
        .xillybus_S_AXI_bready(xillybus_S_AXI_bready),
        .xillybus_S_AXI_bresp(xillybus_S_AXI_bresp),
        .xillybus_S_AXI_bvalid(xillybus_S_AXI_bvalid),
        .xillybus_S_AXI_rdata(xillybus_S_AXI_rdata),
        .xillybus_S_AXI_rready(xillybus_S_AXI_rready),
        .xillybus_S_AXI_rresp(xillybus_S_AXI_rresp),
        .xillybus_S_AXI_rvalid(xillybus_S_AXI_rvalid),
        .xillybus_S_AXI_wdata(xillybus_S_AXI_wdata),
        .xillybus_S_AXI_wready(xillybus_S_AXI_wready),
        .xillybus_S_AXI_wstrb(xillybus_S_AXI_wstrb),
        .xillybus_S_AXI_wvalid(xillybus_S_AXI_wvalid),
        .xillybus_bus_clk(xillybus_bus_clk),
        .xillybus_bus_rst_n(xillybus_bus_rst_n),
        .xillybus_host_interrupt(xillybus_host_interrupt));
endmodule
