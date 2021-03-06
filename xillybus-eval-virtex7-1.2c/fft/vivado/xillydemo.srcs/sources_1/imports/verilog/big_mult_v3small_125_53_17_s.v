// ==============================================================
// RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2014.1
// Copyright (C) 2014 Xilinx Inc. All rights reserved.
// 
// ===========================================================

`timescale 1 ns / 1 ps 

module big_mult_v3small_125_53_17_s (
        ap_clk,
        ap_rst,
        ap_start,
        ap_done,
        ap_idle,
        ap_ready,
        a_V,
        b_V,
        ap_return
);

parameter    ap_const_logic_1 = 1'b1;
parameter    ap_const_logic_0 = 1'b0;
parameter    ap_ST_st1_fsm_0 = 4'b0000;
parameter    ap_ST_st2_fsm_1 = 4'b1;
parameter    ap_ST_st3_fsm_2 = 4'b10;
parameter    ap_ST_st4_fsm_3 = 4'b11;
parameter    ap_ST_st5_fsm_4 = 4'b100;
parameter    ap_ST_st6_fsm_5 = 4'b101;
parameter    ap_ST_st7_fsm_6 = 4'b110;
parameter    ap_ST_st8_fsm_7 = 4'b111;
parameter    ap_ST_st9_fsm_8 = 4'b1000;
parameter    ap_ST_st10_fsm_9 = 4'b1001;
parameter    ap_const_lv1_0 = 1'b0;
parameter    ap_const_lv4_0 = 4'b0000;
parameter    ap_const_lv4_1 = 4'b1;
parameter    ap_const_lv3_0 = 3'b000;
parameter    ap_const_lv64_0 = 64'b0000000000000000000000000000000000000000000000000000000000000000;
parameter    ap_const_lv64_7 = 64'b111;
parameter    ap_const_lv4_8 = 4'b1000;
parameter    ap_const_lv4_7 = 4'b111;
parameter    ap_const_lv7_10 = 7'b10000;
parameter    ap_const_lv7_7C = 7'b1111100;
parameter    ap_const_lv3_7 = 3'b111;
parameter    ap_const_lv32_11 = 32'b10001;
parameter    ap_const_lv32_45 = 32'b1000101;
parameter    ap_const_lv3_1 = 3'b1;
parameter    ap_const_lv32_77 = 32'b1110111;
parameter    ap_const_lv32_B1 = 32'b10110001;
parameter    ap_const_lv178_lc_1 = 178'b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000;
parameter    ap_true = 1'b1;

input   ap_clk;
input   ap_rst;
input   ap_start;
output   ap_done;
output   ap_idle;
output   ap_ready;
input  [124:0] a_V;
input  [52:0] b_V;
output  [177:0] ap_return;

reg ap_done;
reg ap_idle;
reg ap_ready;
reg[177:0] ap_return;
reg   [3:0] ap_CS_fsm = 4'b0000;
wire   [69:0] tmp_fu_196_p1;
reg   [69:0] tmp_reg_409;
wire   [3:0] i_4_fu_206_p2;
reg   [3:0] i_4_reg_417;
wire   [69:0] tmp_30_fu_261_p1;
reg   [69:0] tmp_30_reg_422;
wire   [0:0] exitcond5_fu_200_p2;
wire   [69:0] tmp_10_fu_265_p2;
reg   [69:0] tmp_10_reg_432;
wire   [63:0] tmp_12_fu_280_p1;
reg   [63:0] tmp_12_reg_440;
wire   [0:0] exitcond6_fu_274_p2;
wire   [3:0] i_5_fu_300_p2;
reg   [3:0] i_5_reg_455;
wire   [2:0] tmp_11_fu_333_p2;
reg   [2:0] tmp_11_reg_463;
wire   [0:0] exitcond_fu_327_p2;
wire   [177:0] p_Result_2_fu_376_p5;
reg   [2:0] pp_V_1_address0;
reg    pp_V_1_ce0;
reg    pp_V_1_we0;
wire   [69:0] pp_V_1_d0;
wire   [69:0] pp_V_1_q0;
reg   [2:0] pps_V_address0;
reg    pps_V_ce0;
reg    pps_V_we0;
reg   [69:0] pps_V_d0;
wire   [69:0] pps_V_q0;
reg   [3:0] i_reg_148;
reg   [3:0] i_1_reg_160;
reg   [177:0] p_Val2_s_reg_171;
reg   [2:0] i_2_reg_183;
wire   [63:0] tmp_9_fu_269_p1;
wire   [63:0] tmp_14_fu_295_p1;
wire   [63:0] tmp_16_fu_339_p1;
wire   [69:0] tmp_15_fu_320_p2;
wire   [2:0] tmp_29_fu_218_p1;
wire   [6:0] Lo_assign_fu_222_p3;
wire   [0:0] tmp_s_fu_212_p2;
wire   [6:0] tmp_8_fu_234_p2;
wire   [6:0] Ui_fu_240_p3;
wire   [31:0] tmp_70_cast_fu_230_p1;
wire   [31:0] Ui_4_cast_fu_248_p1;
wire   [124:0] p_Result_12_fu_252_p4;
wire   [52:0] tmp_10_fu_265_p1;
wire   [2:0] tmp_31_fu_285_p1;
wire   [2:0] tmp_13_fu_289_p2;
wire   [52:0] r_V_fu_306_p4;
wire   [69:0] r_V_5_fu_316_p1;
wire   [6:0] Li_fu_344_p4;
wire   [6:0] Ui_1_fu_358_p2;
wire   [16:0] tmp_33_fu_368_p1;
wire   [177:0] loc_V_fu_372_p1;
wire   [31:0] Li_cast2_fu_354_p1;
wire   [31:0] Hi_assign_cast_fu_364_p1;
wire   [58:0] tmp_32_fu_388_p1;
wire   [177:0] p_Result_s_fu_392_p5;
reg   [177:0] ap_return_preg = 178'b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000;
reg   [3:0] ap_NS_fsm;


big_mult_v3small_125_53_17_s_pp_V_1 #(
    .DataWidth( 70 ),
    .AddressRange( 8 ),
    .AddressWidth( 3 ))
pp_V_1_U(
    .clk( ap_clk ),
    .reset( ap_rst ),
    .address0( pp_V_1_address0 ),
    .ce0( pp_V_1_ce0 ),
    .we0( pp_V_1_we0 ),
    .d0( pp_V_1_d0 ),
    .q0( pp_V_1_q0 )
);

big_mult_v3small_125_53_17_s_pp_V_1 #(
    .DataWidth( 70 ),
    .AddressRange( 8 ),
    .AddressWidth( 3 ))
pps_V_U(
    .clk( ap_clk ),
    .reset( ap_rst ),
    .address0( pps_V_address0 ),
    .ce0( pps_V_ce0 ),
    .we0( pps_V_we0 ),
    .d0( pps_V_d0 ),
    .q0( pps_V_q0 )
);



/// the current state (ap_CS_fsm) of the state machine. ///
always @ (posedge ap_clk)
begin : ap_ret_ap_CS_fsm
    if (ap_rst == 1'b1) begin
        ap_CS_fsm <= ap_ST_st1_fsm_0;
    end else begin
        ap_CS_fsm <= ap_NS_fsm;
    end
end

/// ap_return_preg assign process. ///
always @ (posedge ap_clk)
begin : ap_ret_ap_return_preg
    if (ap_rst == 1'b1) begin
        ap_return_preg <= ap_const_lv178_lc_1;
    end else begin
        if ((ap_ST_st10_fsm_9 == ap_CS_fsm)) begin
            ap_return_preg <= p_Result_s_fu_392_p5;
        end
    end
end

/// assign process. ///
always @(posedge ap_clk)
begin
    if ((ap_ST_st7_fsm_6 == ap_CS_fsm)) begin
        i_1_reg_160 <= i_5_reg_455;
    end else if ((ap_ST_st5_fsm_4 == ap_CS_fsm)) begin
        i_1_reg_160 <= ap_const_lv4_1;
    end
end

/// assign process. ///
always @(posedge ap_clk)
begin
    if (((ap_ST_st6_fsm_5 == ap_CS_fsm) & ~(ap_const_lv1_0 == exitcond6_fu_274_p2))) begin
        i_2_reg_183 <= ap_const_lv3_0;
    end else if ((ap_ST_st9_fsm_8 == ap_CS_fsm)) begin
        i_2_reg_183 <= tmp_11_reg_463;
    end
end

/// assign process. ///
always @(posedge ap_clk)
begin
    if (((ap_ST_st1_fsm_0 == ap_CS_fsm) & ~(ap_start == ap_const_logic_0))) begin
        i_reg_148 <= ap_const_lv4_0;
    end else if ((ap_ST_st4_fsm_3 == ap_CS_fsm)) begin
        i_reg_148 <= i_4_reg_417;
    end
end

/// assign process. ///
always @(posedge ap_clk)
begin
    if ((ap_ST_st2_fsm_1 == ap_CS_fsm)) begin
        i_4_reg_417 <= i_4_fu_206_p2;
    end
end

/// assign process. ///
always @(posedge ap_clk)
begin
    if (((ap_ST_st6_fsm_5 == ap_CS_fsm) & (ap_const_lv1_0 == exitcond6_fu_274_p2))) begin
        i_5_reg_455 <= i_5_fu_300_p2;
        tmp_12_reg_440[0] <= tmp_12_fu_280_p1[0];
tmp_12_reg_440[1] <= tmp_12_fu_280_p1[1];
tmp_12_reg_440[2] <= tmp_12_fu_280_p1[2];
tmp_12_reg_440[3] <= tmp_12_fu_280_p1[3];
    end
end

/// assign process. ///
always @(posedge ap_clk)
begin
    if ((ap_ST_st9_fsm_8 == ap_CS_fsm)) begin
        p_Val2_s_reg_171 <= p_Result_2_fu_376_p5;
    end
end

/// assign process. ///
always @(posedge ap_clk)
begin
    if ((ap_ST_st3_fsm_2 == ap_CS_fsm)) begin
        tmp_10_reg_432 <= tmp_10_fu_265_p2;
    end
end

/// assign process. ///
always @(posedge ap_clk)
begin
    if ((ap_ST_st8_fsm_7 == ap_CS_fsm)) begin
        tmp_11_reg_463 <= tmp_11_fu_333_p2;
    end
end

/// assign process. ///
always @(posedge ap_clk)
begin
    if (((ap_ST_st2_fsm_1 == ap_CS_fsm) & (exitcond5_fu_200_p2 == ap_const_lv1_0))) begin
        tmp_30_reg_422 <= tmp_30_fu_261_p1;
    end
end

/// assign process. ///
always @(posedge ap_clk)
begin
    if (((ap_ST_st1_fsm_0 == ap_CS_fsm) & ~(ap_start == ap_const_logic_0))) begin
        tmp_reg_409[0] <= tmp_fu_196_p1[0];
tmp_reg_409[1] <= tmp_fu_196_p1[1];
tmp_reg_409[2] <= tmp_fu_196_p1[2];
tmp_reg_409[3] <= tmp_fu_196_p1[3];
tmp_reg_409[4] <= tmp_fu_196_p1[4];
tmp_reg_409[5] <= tmp_fu_196_p1[5];
tmp_reg_409[6] <= tmp_fu_196_p1[6];
tmp_reg_409[7] <= tmp_fu_196_p1[7];
tmp_reg_409[8] <= tmp_fu_196_p1[8];
tmp_reg_409[9] <= tmp_fu_196_p1[9];
tmp_reg_409[10] <= tmp_fu_196_p1[10];
tmp_reg_409[11] <= tmp_fu_196_p1[11];
tmp_reg_409[12] <= tmp_fu_196_p1[12];
tmp_reg_409[13] <= tmp_fu_196_p1[13];
tmp_reg_409[14] <= tmp_fu_196_p1[14];
tmp_reg_409[15] <= tmp_fu_196_p1[15];
tmp_reg_409[16] <= tmp_fu_196_p1[16];
tmp_reg_409[17] <= tmp_fu_196_p1[17];
tmp_reg_409[18] <= tmp_fu_196_p1[18];
tmp_reg_409[19] <= tmp_fu_196_p1[19];
tmp_reg_409[20] <= tmp_fu_196_p1[20];
tmp_reg_409[21] <= tmp_fu_196_p1[21];
tmp_reg_409[22] <= tmp_fu_196_p1[22];
tmp_reg_409[23] <= tmp_fu_196_p1[23];
tmp_reg_409[24] <= tmp_fu_196_p1[24];
tmp_reg_409[25] <= tmp_fu_196_p1[25];
tmp_reg_409[26] <= tmp_fu_196_p1[26];
tmp_reg_409[27] <= tmp_fu_196_p1[27];
tmp_reg_409[28] <= tmp_fu_196_p1[28];
tmp_reg_409[29] <= tmp_fu_196_p1[29];
tmp_reg_409[30] <= tmp_fu_196_p1[30];
tmp_reg_409[31] <= tmp_fu_196_p1[31];
tmp_reg_409[32] <= tmp_fu_196_p1[32];
tmp_reg_409[33] <= tmp_fu_196_p1[33];
tmp_reg_409[34] <= tmp_fu_196_p1[34];
tmp_reg_409[35] <= tmp_fu_196_p1[35];
tmp_reg_409[36] <= tmp_fu_196_p1[36];
tmp_reg_409[37] <= tmp_fu_196_p1[37];
tmp_reg_409[38] <= tmp_fu_196_p1[38];
tmp_reg_409[39] <= tmp_fu_196_p1[39];
tmp_reg_409[40] <= tmp_fu_196_p1[40];
tmp_reg_409[41] <= tmp_fu_196_p1[41];
tmp_reg_409[42] <= tmp_fu_196_p1[42];
tmp_reg_409[43] <= tmp_fu_196_p1[43];
tmp_reg_409[44] <= tmp_fu_196_p1[44];
tmp_reg_409[45] <= tmp_fu_196_p1[45];
tmp_reg_409[46] <= tmp_fu_196_p1[46];
tmp_reg_409[47] <= tmp_fu_196_p1[47];
tmp_reg_409[48] <= tmp_fu_196_p1[48];
tmp_reg_409[49] <= tmp_fu_196_p1[49];
tmp_reg_409[50] <= tmp_fu_196_p1[50];
tmp_reg_409[51] <= tmp_fu_196_p1[51];
tmp_reg_409[52] <= tmp_fu_196_p1[52];
    end
end

/// ap_done assign process. ///
always @ (ap_start or ap_CS_fsm)
begin
    if (((~(ap_const_logic_1 == ap_start) & (ap_ST_st1_fsm_0 == ap_CS_fsm)) | (ap_ST_st10_fsm_9 == ap_CS_fsm))) begin
        ap_done = ap_const_logic_1;
    end else begin
        ap_done = ap_const_logic_0;
    end
end

/// ap_idle assign process. ///
always @ (ap_start or ap_CS_fsm)
begin
    if ((~(ap_const_logic_1 == ap_start) & (ap_ST_st1_fsm_0 == ap_CS_fsm))) begin
        ap_idle = ap_const_logic_1;
    end else begin
        ap_idle = ap_const_logic_0;
    end
end

/// ap_ready assign process. ///
always @ (ap_CS_fsm)
begin
    if ((ap_ST_st10_fsm_9 == ap_CS_fsm)) begin
        ap_ready = ap_const_logic_1;
    end else begin
        ap_ready = ap_const_logic_0;
    end
end

/// ap_return assign process. ///
always @ (ap_CS_fsm or p_Result_s_fu_392_p5 or ap_return_preg)
begin
    if ((ap_ST_st10_fsm_9 == ap_CS_fsm)) begin
        ap_return = p_Result_s_fu_392_p5;
    end else begin
        ap_return = ap_return_preg;
    end
end

/// pp_V_1_address0 assign process. ///
always @ (ap_CS_fsm or tmp_12_fu_280_p1 or tmp_9_fu_269_p1)
begin
    if ((ap_ST_st4_fsm_3 == ap_CS_fsm)) begin
        pp_V_1_address0 = tmp_9_fu_269_p1;
    end else if ((ap_ST_st6_fsm_5 == ap_CS_fsm)) begin
        pp_V_1_address0 = tmp_12_fu_280_p1;
    end else if ((ap_ST_st2_fsm_1 == ap_CS_fsm)) begin
        pp_V_1_address0 = ap_const_lv64_0;
    end else begin
        pp_V_1_address0 = 'bx;
    end
end

/// pp_V_1_ce0 assign process. ///
always @ (ap_CS_fsm)
begin
    if (((ap_ST_st2_fsm_1 == ap_CS_fsm) | (ap_ST_st6_fsm_5 == ap_CS_fsm) | (ap_ST_st4_fsm_3 == ap_CS_fsm))) begin
        pp_V_1_ce0 = ap_const_logic_1;
    end else begin
        pp_V_1_ce0 = ap_const_logic_0;
    end
end

/// pp_V_1_we0 assign process. ///
always @ (ap_CS_fsm)
begin
    if ((ap_ST_st4_fsm_3 == ap_CS_fsm)) begin
        pp_V_1_we0 = ap_const_logic_1;
    end else begin
        pp_V_1_we0 = ap_const_logic_0;
    end
end

/// pps_V_address0 assign process. ///
always @ (ap_CS_fsm or tmp_12_reg_440 or exitcond_fu_327_p2 or tmp_14_fu_295_p1 or tmp_16_fu_339_p1)
begin
    if ((ap_ST_st7_fsm_6 == ap_CS_fsm)) begin
        pps_V_address0 = tmp_12_reg_440;
    end else if ((ap_ST_st5_fsm_4 == ap_CS_fsm)) begin
        pps_V_address0 = ap_const_lv64_0;
    end else if (((ap_ST_st8_fsm_7 == ap_CS_fsm) & ~(ap_const_lv1_0 == exitcond_fu_327_p2))) begin
        pps_V_address0 = ap_const_lv64_7;
    end else if (((ap_ST_st8_fsm_7 == ap_CS_fsm) & (ap_const_lv1_0 == exitcond_fu_327_p2))) begin
        pps_V_address0 = tmp_16_fu_339_p1;
    end else if ((ap_ST_st6_fsm_5 == ap_CS_fsm)) begin
        pps_V_address0 = tmp_14_fu_295_p1;
    end else begin
        pps_V_address0 = 'bx;
    end
end

/// pps_V_ce0 assign process. ///
always @ (ap_CS_fsm or exitcond_fu_327_p2)
begin
    if (((ap_ST_st6_fsm_5 == ap_CS_fsm) | ((ap_ST_st8_fsm_7 == ap_CS_fsm) & (ap_const_lv1_0 == exitcond_fu_327_p2)) | ((ap_ST_st8_fsm_7 == ap_CS_fsm) & ~(ap_const_lv1_0 == exitcond_fu_327_p2)) | (ap_ST_st5_fsm_4 == ap_CS_fsm) | (ap_ST_st7_fsm_6 == ap_CS_fsm))) begin
        pps_V_ce0 = ap_const_logic_1;
    end else begin
        pps_V_ce0 = ap_const_logic_0;
    end
end

/// pps_V_d0 assign process. ///
always @ (ap_CS_fsm or pp_V_1_q0 or tmp_15_fu_320_p2)
begin
    if ((ap_ST_st7_fsm_6 == ap_CS_fsm)) begin
        pps_V_d0 = tmp_15_fu_320_p2;
    end else if ((ap_ST_st5_fsm_4 == ap_CS_fsm)) begin
        pps_V_d0 = pp_V_1_q0;
    end else begin
        pps_V_d0 = 'bx;
    end
end

/// pps_V_we0 assign process. ///
always @ (ap_CS_fsm)
begin
    if (((ap_ST_st5_fsm_4 == ap_CS_fsm) | (ap_ST_st7_fsm_6 == ap_CS_fsm))) begin
        pps_V_we0 = ap_const_logic_1;
    end else begin
        pps_V_we0 = ap_const_logic_0;
    end
end
always @ (ap_start or ap_CS_fsm or exitcond5_fu_200_p2 or exitcond6_fu_274_p2 or exitcond_fu_327_p2)
begin
    case (ap_CS_fsm)
        ap_ST_st1_fsm_0 : 
            if (~(ap_start == ap_const_logic_0)) begin
                ap_NS_fsm = ap_ST_st2_fsm_1;
            end else begin
                ap_NS_fsm = ap_ST_st1_fsm_0;
            end
        ap_ST_st2_fsm_1 : 
            if (~(exitcond5_fu_200_p2 == ap_const_lv1_0)) begin
                ap_NS_fsm = ap_ST_st5_fsm_4;
            end else begin
                ap_NS_fsm = ap_ST_st3_fsm_2;
            end
        ap_ST_st3_fsm_2 : 
            ap_NS_fsm = ap_ST_st4_fsm_3;
        ap_ST_st4_fsm_3 : 
            ap_NS_fsm = ap_ST_st2_fsm_1;
        ap_ST_st5_fsm_4 : 
            ap_NS_fsm = ap_ST_st6_fsm_5;
        ap_ST_st6_fsm_5 : 
            if (~(ap_const_lv1_0 == exitcond6_fu_274_p2)) begin
                ap_NS_fsm = ap_ST_st8_fsm_7;
            end else begin
                ap_NS_fsm = ap_ST_st7_fsm_6;
            end
        ap_ST_st7_fsm_6 : 
            ap_NS_fsm = ap_ST_st6_fsm_5;
        ap_ST_st8_fsm_7 : 
            if (~(ap_const_lv1_0 == exitcond_fu_327_p2)) begin
                ap_NS_fsm = ap_ST_st10_fsm_9;
            end else begin
                ap_NS_fsm = ap_ST_st9_fsm_8;
            end
        ap_ST_st9_fsm_8 : 
            ap_NS_fsm = ap_ST_st8_fsm_7;
        ap_ST_st10_fsm_9 : 
            ap_NS_fsm = ap_ST_st1_fsm_0;
        default : 
            ap_NS_fsm = 'bx;
    endcase
end
assign Hi_assign_cast_fu_364_p1 = $unsigned(Ui_1_fu_358_p2);
assign Li_cast2_fu_354_p1 = $unsigned(Li_fu_344_p4);
assign Li_fu_344_p4 = {{{i_2_reg_183}, {ap_const_lv1_0}}, {i_2_reg_183}};
assign Lo_assign_fu_222_p3 = {{tmp_29_fu_218_p1}, {i_reg_148}};
assign Ui_1_fu_358_p2 = (Li_fu_344_p4 + ap_const_lv7_10);
assign Ui_4_cast_fu_248_p1 = $unsigned(Ui_fu_240_p3);
assign Ui_fu_240_p3 = ((tmp_s_fu_212_p2)? ap_const_lv7_7C: tmp_8_fu_234_p2);
assign exitcond5_fu_200_p2 = (i_reg_148 == ap_const_lv4_8? 1'b1: 1'b0);
assign exitcond6_fu_274_p2 = (i_1_reg_160 == ap_const_lv4_8? 1'b1: 1'b0);
assign exitcond_fu_327_p2 = (i_2_reg_183 == ap_const_lv3_7? 1'b1: 1'b0);
assign i_4_fu_206_p2 = (i_reg_148 + ap_const_lv4_1);
assign i_5_fu_300_p2 = (i_1_reg_160 + ap_const_lv4_1);
assign loc_V_fu_372_p1 = $unsigned(tmp_33_fu_368_p1);
wire [7-1:0] p_Result_12_fu_252_p4_vh_buf;
wire [7-1:0] p_Result_12_fu_252_p4_vl_buf;
wire [125-1:0] p_Result_12_fu_252_p4_vi_buf;
wire [125-1:0] p_Result_12_fu_252_p4_vo_buf;
wire [125-1:0] p_Result_12_fu_252_p4_vi_r;
wire [7:0] p_Result_12_fu_252_p4_length;
wire [125-1:0] p_Result_12_fu_252_p4_mask;

genvar ap_tvar_int_0;
generate
    for (ap_tvar_int_0 = 0; ap_tvar_int_0 < 125; ap_tvar_int_0 = ap_tvar_int_0 + 1) begin :p_Result_12_fu_252_p4_label0
        assign p_Result_12_fu_252_p4_vi_r[ap_tvar_int_0] = a_V[125-1-ap_tvar_int_0];
    end
endgenerate

assign p_Result_12_fu_252_p4_vh_buf = (Ui_4_cast_fu_248_p1 >= tmp_70_cast_fu_230_p1)? Ui_4_cast_fu_248_p1 : (125 -1- Ui_4_cast_fu_248_p1);
assign p_Result_12_fu_252_p4_vl_buf = (Ui_4_cast_fu_248_p1 >= tmp_70_cast_fu_230_p1)? tmp_70_cast_fu_230_p1 : (125 -1-tmp_70_cast_fu_230_p1);
assign p_Result_12_fu_252_p4_vi_buf = (Ui_4_cast_fu_248_p1 >= tmp_70_cast_fu_230_p1)? a_V : p_Result_12_fu_252_p4_vi_r;
assign p_Result_12_fu_252_p4_length = p_Result_12_fu_252_p4_vh_buf - p_Result_12_fu_252_p4_vl_buf + 1'b1;
assign p_Result_12_fu_252_p4_mask = ~({125{1'b1}} << p_Result_12_fu_252_p4_length);
assign p_Result_12_fu_252_p4 = (p_Result_12_fu_252_p4_vi_buf >> p_Result_12_fu_252_p4_vl_buf) & p_Result_12_fu_252_p4_mask;

wire [8-1:0] p_Result_2_fu_376_p5_vHi_buf;
wire [8-1:0] p_Result_2_fu_376_p5_vLo_buf;
wire [178-1:0] p_Result_2_fu_376_p5_v1_buf;
wire [178-1:0] p_Result_2_fu_376_p5_v1_buf_r;
wire [178-1:0] p_Result_2_fu_376_p5_v1_tmp;
wire [8-1:0] p_Result_2_fu_376_p5_v1_shift;
wire [178-1:0] p_Result_2_fu_376_p5_mask0;
wire [178-1:0] p_Result_2_fu_376_p5_mask1;
wire [178-1:0] p_Result_2_fu_376_p5_mask2;

genvar ap_tvar_int_1;
generate
    for (ap_tvar_int_1 = 0; ap_tvar_int_1 < 178; ap_tvar_int_1 = ap_tvar_int_1 + 1) begin :p_Result_2_fu_376_p5_label0
        assign p_Result_2_fu_376_p5_v1_buf_r[ap_tvar_int_1] = p_Result_2_fu_376_p5_v1_buf[178-1-ap_tvar_int_1];
    end
endgenerate

assign p_Result_2_fu_376_p5_vHi_buf = (Hi_assign_cast_fu_364_p1 >= Li_cast2_fu_354_p1)? Hi_assign_cast_fu_364_p1 : Li_cast2_fu_354_p1;
assign p_Result_2_fu_376_p5_vLo_buf = (Hi_assign_cast_fu_364_p1 >= Li_cast2_fu_354_p1)? Li_cast2_fu_354_p1 : Hi_assign_cast_fu_364_p1;
assign p_Result_2_fu_376_p5_v1_buf = loc_V_fu_372_p1;
assign p_Result_2_fu_376_p5_v1_shift = (Hi_assign_cast_fu_364_p1 >= Li_cast2_fu_354_p1)? Li_cast2_fu_354_p1 : (178-1-Li_cast2_fu_354_p1);
assign p_Result_2_fu_376_p5_v1_tmp = (Hi_assign_cast_fu_364_p1 >= Li_cast2_fu_354_p1)? (p_Result_2_fu_376_p5_v1_buf << p_Result_2_fu_376_p5_v1_shift) : (p_Result_2_fu_376_p5_v1_buf_r >> p_Result_2_fu_376_p5_v1_shift);
assign p_Result_2_fu_376_p5_mask0 = {{(178-1){1'b1}}, 1'b0} << p_Result_2_fu_376_p5_vHi_buf;
assign p_Result_2_fu_376_p5_mask1 = ~({(178){1'b1}} << p_Result_2_fu_376_p5_vLo_buf);
assign p_Result_2_fu_376_p5_mask2 = p_Result_2_fu_376_p5_mask0 | p_Result_2_fu_376_p5_mask1;
assign p_Result_2_fu_376_p5 = (p_Val2_s_reg_171 & p_Result_2_fu_376_p5_mask2) | (p_Result_2_fu_376_p5_v1_tmp & ~p_Result_2_fu_376_p5_mask2);

assign p_Result_s_fu_392_p5 = {{tmp_32_fu_388_p1}, {p_Val2_s_reg_171[32'd118 : 32'd0]}};
assign pp_V_1_d0 = tmp_10_reg_432;
assign r_V_5_fu_316_p1 = $unsigned(r_V_fu_306_p4);
assign r_V_fu_306_p4 = {{pps_V_q0[ap_const_lv32_45 : ap_const_lv32_11]}};
assign tmp_10_fu_265_p1 = tmp_reg_409;
assign tmp_10_fu_265_p2 = ($signed(tmp_30_reg_422) * $signed({{1'b0}, {tmp_10_fu_265_p1}}));
assign tmp_11_fu_333_p2 = (i_2_reg_183 + ap_const_lv3_1);
assign tmp_12_fu_280_p1 = $unsigned(i_1_reg_160);
assign tmp_13_fu_289_p2 = (tmp_31_fu_285_p1 + ap_const_lv3_7);
assign tmp_14_fu_295_p1 = $unsigned(tmp_13_fu_289_p2);
assign tmp_15_fu_320_p2 = (r_V_5_fu_316_p1 + pp_V_1_q0);
assign tmp_16_fu_339_p1 = $unsigned(i_2_reg_183);
assign tmp_29_fu_218_p1 = i_reg_148[2:0];
assign tmp_30_fu_261_p1 = p_Result_12_fu_252_p4[69:0];
assign tmp_31_fu_285_p1 = i_1_reg_160[2:0];
assign tmp_32_fu_388_p1 = pps_V_q0[58:0];
assign tmp_33_fu_368_p1 = pps_V_q0[16:0];
assign tmp_70_cast_fu_230_p1 = $unsigned(Lo_assign_fu_222_p3);
assign tmp_8_fu_234_p2 = (Lo_assign_fu_222_p3 + ap_const_lv7_10);
assign tmp_9_fu_269_p1 = $unsigned(i_reg_148);
assign tmp_fu_196_p1 = $unsigned(b_V);
assign tmp_s_fu_212_p2 = (i_reg_148 == ap_const_lv4_7? 1'b1: 1'b0);
always @ (posedge ap_clk)
begin
    tmp_reg_409[69:53] <= 17'b00000000000000000;
    tmp_12_reg_440[63:4] <= 60'b000000000000000000000000000000000000000000000000000000000000;
end



endmodule //big_mult_v3small_125_53_17_s

