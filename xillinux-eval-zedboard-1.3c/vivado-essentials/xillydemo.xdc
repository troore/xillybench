create_clock -name gclk -period 10 [get_ports "clk_100"]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets "clk_100"]

# Vivado constraints unrelated clocks. So set false paths.
set_false_path -from [get_clocks clk_fpga_1] -to [get_clocks vga_clk_ins/*]
set_false_path -from [get_clocks vga_clk_ins/*] -to [get_clocks clk_fpga_1]

# The VGA outputs are turned into an analog voltage by virtue of a resistor
# network, so the flip flops driving these must sit in the IOBs to minimize
# timing skew. The RTL code should handle this, but the constraint below
# is there to fail if something goes wrong about this.
set_output_delay 5.5 [get_ports {vga*}]

set_property -dict "PACKAGE_PIN Y9 IOSTANDARD LVCMOS33" [get_ports "clk_100"]
set_property -dict "PACKAGE_PIN T22 IOSTANDARD LVCMOS33" [get_ports "GPIO_LED[0]"]
set_property -dict "PACKAGE_PIN T21 IOSTANDARD LVCMOS33" [get_ports "GPIO_LED[1]"]
set_property -dict "PACKAGE_PIN U22 IOSTANDARD LVCMOS33" [get_ports "GPIO_LED[2]"]
set_property -dict "PACKAGE_PIN U21 IOSTANDARD LVCMOS33" [get_ports "GPIO_LED[3]"]
set_property -dict "PACKAGE_PIN Y21 IOSTANDARD LVCMOS33" [get_ports "vga4_blue[0]"]
set_property -dict "PACKAGE_PIN Y20 IOSTANDARD LVCMOS33" [get_ports "vga4_blue[1]"]
set_property -dict "PACKAGE_PIN AB20 IOSTANDARD LVCMOS33" [get_ports "vga4_blue[2]"]
set_property -dict "PACKAGE_PIN AB19 IOSTANDARD LVCMOS33" [get_ports "vga4_blue[3]"]
set_property -dict "PACKAGE_PIN AB22 IOSTANDARD LVCMOS33" [get_ports "vga4_green[0]"]
set_property -dict "PACKAGE_PIN AA22 IOSTANDARD LVCMOS33" [get_ports "vga4_green[1]"]
set_property -dict "PACKAGE_PIN AB21 IOSTANDARD LVCMOS33" [get_ports "vga4_green[2]"]
set_property -dict "PACKAGE_PIN AA21 IOSTANDARD LVCMOS33" [get_ports "vga4_green[3]"]
set_property -dict "PACKAGE_PIN V20 IOSTANDARD LVCMOS33" [get_ports "vga4_red[0]"]
set_property -dict "PACKAGE_PIN U20 IOSTANDARD LVCMOS33" [get_ports "vga4_red[1]"]
set_property -dict "PACKAGE_PIN V19 IOSTANDARD LVCMOS33" [get_ports "vga4_red[2]"]
set_property -dict "PACKAGE_PIN V18 IOSTANDARD LVCMOS33" [get_ports "vga4_red[3]"]
set_property -dict "PACKAGE_PIN Y19 IOSTANDARD LVCMOS33" [get_ports "vga_vsync"]
set_property -dict "PACKAGE_PIN AA19 IOSTANDARD LVCMOS33" [get_ports "vga_hsync"]

# IMPORTANT: Since four LEDs are taken by the Xillybus IP core, the pin
# placement doesn't match the one given by Digilent.

# GPIO pin to reset the USB OTG PHY

set_property -dict "PACKAGE_PIN G17 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[0]"]

# On-board OLED

set_property -dict "PACKAGE_PIN U11 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[1]"]
set_property -dict "PACKAGE_PIN U12 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[2]"]
set_property -dict "PACKAGE_PIN U9 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[3]"]
set_property -dict "PACKAGE_PIN U10 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[4]"]
set_property -dict "PACKAGE_PIN AB12 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[5]"]
set_property -dict "PACKAGE_PIN AA12 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[6]"]

# On-board LEDs. Note that only for LEDs are allocated, as opposed to
# Digilent's eight, and all placements that follow are shifted by four.
# There was no other choice, as the tools don't allow unplaced PS GPIO pins.

set_property -dict "PACKAGE_PIN V22 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[7]"]
set_property -dict "PACKAGE_PIN W22 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[8]"]
set_property -dict "PACKAGE_PIN U19 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[9]"]
set_property -dict "PACKAGE_PIN U14 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[10]"]

# On-board Slide Switches

set_property -dict "PACKAGE_PIN F22 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[11]"]
set_property -dict "PACKAGE_PIN G22 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[12]"]
set_property -dict "PACKAGE_PIN H22 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[13]"]
set_property -dict "PACKAGE_PIN F21 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[14]"]
set_property -dict "PACKAGE_PIN H19 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[15]"]
set_property -dict "PACKAGE_PIN H18 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[16]"]
set_property -dict "PACKAGE_PIN H17 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[17]"]
set_property -dict "PACKAGE_PIN M15 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[18]"]

# On-board Left, Right, Up, Down, and Select Pushbuttons

set_property -dict "PACKAGE_PIN N15 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[19]"]
set_property -dict "PACKAGE_PIN R18 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[20]"]
set_property -dict "PACKAGE_PIN T18 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[21]"]
set_property -dict "PACKAGE_PIN R16 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[22]"]
set_property -dict "PACKAGE_PIN P16 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[23]"]

# Pmod JA

set_property -dict "PACKAGE_PIN Y11 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[24]"]
set_property -dict "PACKAGE_PIN AA11 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[25]"]
set_property -dict "PACKAGE_PIN Y10 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[26]"]
set_property -dict "PACKAGE_PIN AA9 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[27]"]
set_property -dict "PACKAGE_PIN AB11 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[28]"]
set_property -dict "PACKAGE_PIN AB10 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[29]"]
set_property -dict "PACKAGE_PIN AB9 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[30]"]
set_property -dict "PACKAGE_PIN AA8 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[31]"]

# Pmod JB

set_property -dict "PACKAGE_PIN W12 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[32]"]
set_property -dict "PACKAGE_PIN W11 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[33]"]
set_property -dict "PACKAGE_PIN V10 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[34]"]
set_property -dict "PACKAGE_PIN W8 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[35]"]
set_property -dict "PACKAGE_PIN V12 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[36]"]
set_property -dict "PACKAGE_PIN W10 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[37]"]
set_property -dict "PACKAGE_PIN V9 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[38]"]
set_property -dict "PACKAGE_PIN V8 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[39]"]

# Pmod JC

set_property -dict "PACKAGE_PIN AB7 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[40]"]
set_property -dict "PACKAGE_PIN AB6 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[41]"]
set_property -dict "PACKAGE_PIN Y4 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[42]"]
set_property -dict "PACKAGE_PIN AA4 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[43]"]
set_property -dict "PACKAGE_PIN R6 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[44]"]
set_property -dict "PACKAGE_PIN T6 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[45]"]
set_property -dict "PACKAGE_PIN T4 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[46]"]
set_property -dict "PACKAGE_PIN U4 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[47]"]

# Pmod JD

set_property -dict "PACKAGE_PIN V7 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[48]"]
set_property -dict "PACKAGE_PIN W7 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[49]"]
set_property -dict "PACKAGE_PIN V5 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[50]"]
set_property -dict "PACKAGE_PIN V4 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[51]"]
set_property -dict "PACKAGE_PIN W6 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[52]"]
set_property -dict "PACKAGE_PIN W5 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[53]"]
set_property -dict "PACKAGE_PIN U6 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[54]"]
set_property -dict "PACKAGE_PIN U5 IOSTANDARD LVCMOS33" [get_ports "PS_GPIO[55]"]

# Pin for detecting USB OTG over-current condition

set_property -dict "PACKAGE_PIN L16 IOSTANDARD LVCMOS33" [get_ports "otg_oc"]

# Pins connected to sound chip
set_property -dict "PACKAGE_PIN AB1 IOSTANDARD LVCMOS33" [get_ports "smbus_addr[0]"]
set_property -dict "PACKAGE_PIN Y5 IOSTANDARD LVCMOS33" [get_ports "smbus_addr[1]"]
set_property -dict "PACKAGE_PIN AB4 IOSTANDARD LVCMOS33" [get_ports "smb_sclk"]
set_property -dict "PACKAGE_PIN AB5 IOSTANDARD LVCMOS33" [get_ports "smb_sdata"]

set_property -dict "PACKAGE_PIN Y8 IOSTANDARD LVCMOS33" [get_ports "audio_dac"]
set_property -dict "PACKAGE_PIN AA7 IOSTANDARD LVCMOS33" [get_ports "audio_adc"]
set_property -dict "PACKAGE_PIN AA6 IOSTANDARD LVCMOS33" [get_ports "audio_bclk"]
set_property -dict "PACKAGE_PIN Y6 IOSTANDARD LVCMOS33" [get_ports "audio_lrclk"]
set_property -dict "PACKAGE_PIN AB2 IOSTANDARD LVCMOS33" [get_ports "audio_mclk"]
