Please refer to the documentation on http://xillybus.com. This text is just
a starter.

Software environment
====================

The core was developed with ISE Release 14.2, but later versions apply as well.
Vivado 2014.1 can be used as well.

File outline
============

The bundle consists of four directories:

* core -- The binary of the Xillybus core is stored here
* instantiation templates -- Contains the instantiation template for the core
  in Verilog and VHDL
* verilog -- Contains the project file for the demo and the sources in Verilog
  (in the 'src' subdirectory)
* vhdl -- Contains the project file for the demo and the sources in VHDL (in
  the 'src' subdirectory)
* pcie_core -- Contains the XCO Coregen file for creating Xilinx' wrapper for
  PCIe. 
* vivado-essentials -- Deﬁnition ﬁles and build directories for the PCIe
  Integrated Block and general-purpose logic for use by Vivado.

The following two "IMPORTANT" notes relate to ISE only.

IMPORTANT: Before attempting to build the Xillybus bitfile, please open the
    project in the "pcie_core" directory (pcie_core.xise) and generate
    the one Coregen module there: Under "Design Utilities" click "Regenerate
    all cores". Then just close that project without any further action.
    As a side effect, this generated Verilog files, which are referenced by the
    main Xillybus FPGA project. This works around a slight bug in Xilinx'
    Coregen build system on certain ISE versions. 

IMPORTANT II: On some ISE versions, notably ISE 14.2, the build of the
    verilog/ directory may fail with an error saying

    ERROR:HDLCompiler:687 - "C:\try\xillybus-eval-virtex7-1.1\verilog\src\
          fifo_32x512_synth.v" Line 54: Illegal redeclaration of module
          <fifo_32x512>.

    (or similar). This is due to a bug in Xilinx' tools. To work around this,
    delete fifo_8x2048_synth.v and fifo_32x512_synth.v in the src/ directory,
    and restart "Regenerate Programming File".

Note that both 'src' directories also contain the UCF file for the KC705
evaluation kit. This file must be edited if another board is targeted, or if
configuration resistors have been added to or removed from the board. There
are also two XCO (Coregen) files.

If Vivado is used, vivado-essentials/xillydemo.xdc is the constraint file
in effect.

Also note that the vhdl directory contains Verilog files, but none of which
should need editing by user.

The interface with Xillybus takes place in the xillydemo.v or xillydemo.vhd
files in the respective 'src' subdirectories. This is the file to edit in
order to try Xillybus with your own data sources and sinks.

--------------------------------------------------

For further information about how to get started, run tests and hack the
code, please refer to the documentation in the site: http://xillybus.com
