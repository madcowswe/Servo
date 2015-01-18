
# (C) 2001-2015 Altera Corporation. All rights reserved.
# Your use of Altera Corporation's design tools, logic functions and 
# other software and tools, and its AMPP partner logic functions, and 
# any output files any of the foregoing (including device programming 
# or simulation files), and any associated documentation or information 
# are expressly subject to the terms and conditions of the Altera 
# Program License Subscription Agreement, Altera MegaCore Function 
# License Agreement, or other applicable license agreement, including, 
# without limitation, that your use is for the sole purpose of 
# programming logic devices manufactured by Altera and sold by Altera 
# or its authorized distributors. Please refer to the applicable 
# agreement for further details.

# ACDS 13.1 162 linux 2015.01.18.20:10:33

# ----------------------------------------
# vcs - auto-generated simulation script

# ----------------------------------------
# initialize variables
TOP_LEVEL_NAME="qsystem"
QSYS_SIMDIR="./../../"
QUARTUS_INSTALL_DIR="/home/icrs/altera/13.1/quartus/"
SKIP_FILE_COPY=0
SKIP_ELAB=0
SKIP_SIM=0
USER_DEFINED_ELAB_OPTIONS=""
USER_DEFINED_SIM_OPTIONS="+vcs+finish+100"
# ----------------------------------------
# overwrite variables - DO NOT MODIFY!
# This block evaluates each command line argument, typically used for 
# overwriting variables. An example usage:
#   sh <simulator>_setup.sh SKIP_ELAB=1 SKIP_SIM=1
for expression in "$@"; do
  eval $expression
  if [ $? -ne 0 ]; then
    echo "Error: This command line argument, \"$expression\", is/has an invalid expression." >&2
    exit $?
  fi
done

# ----------------------------------------
# initialize simulation properties - DO NOT MODIFY!
ELAB_OPTIONS=""
SIM_OPTIONS=""
if [[ `vcs -platform` != *"amd64"* ]]; then
  :
else
  :
fi

# ----------------------------------------
# copy RAM/ROM files to simulation directory
if [ $SKIP_FILE_COPY -eq 0 ]; then
  cp -f /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_onchip_memory2_0.hex ./
  cp -f /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_nios2_qsys_0_rf_ram_a.dat ./
  cp -f /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_nios2_qsys_0_rf_ram_b.hex ./
  cp -f /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_nios2_qsys_0_ociram_default_contents.mif ./
  cp -f /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_nios2_qsys_0_ociram_default_contents.dat ./
  cp -f /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_nios2_qsys_0_rf_ram_b.dat ./
  cp -f /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_nios2_qsys_0_rf_ram_b.mif ./
  cp -f /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_nios2_qsys_0_ociram_default_contents.hex ./
  cp -f /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_nios2_qsys_0_rf_ram_a.mif ./
  cp -f /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_nios2_qsys_0_rf_ram_a.hex ./
  cp -f /home/icrs/Servo/FPGA/software/ServoDrive/mem_init/hdl_sim/qsystem_onchip_memory2_0.dat ./
  cp -f /home/icrs/Servo/FPGA/software/ServoDrive/mem_init/qsystem_onchip_memory2_0.hex ./
fi

vcs -lca -timescale=1ps/1ps -sverilog +verilog2001ext+.v -ntb_opts dtm $ELAB_OPTIONS $USER_DEFINED_ELAB_OPTIONS \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/altera_primitives.v \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/220model.v \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/sgate.v \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/altera_mf.v \
  $QUARTUS_INSTALL_DIR/eda/sim_lib/altera_lnsim.sv \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/cycloneiii_atoms.v \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/altera_merlin_arbitrator.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_mm_interconnect_0_rsp_xbar_mux_001.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_mm_interconnect_0_rsp_xbar_mux.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_mm_interconnect_0_rsp_xbar_demux_002.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_mm_interconnect_0_cmd_xbar_mux_002.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_mm_interconnect_0_cmd_xbar_mux.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_mm_interconnect_0_cmd_xbar_demux_001.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_mm_interconnect_0_cmd_xbar_demux.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_mm_interconnect_0_id_router_002.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_mm_interconnect_0_id_router.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_mm_interconnect_0_addr_router_001.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_mm_interconnect_0_addr_router.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/altera_avalon_sc_fifo.v \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/altera_merlin_slave_agent.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/altera_merlin_burst_uncompressor.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/altera_merlin_master_agent.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/altera_merlin_slave_translator.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/altera_merlin_master_translator.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/altera_reset_controller.v \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/altera_reset_synchronizer.v \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_irq_mapper.sv \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_mm_interconnect_0.v \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_sysid_qsys_0.vo \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_jtag_uart_0.v \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_onchip_memory2_0.v \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_nios2_qsys_0_jtag_debug_module_wrapper.v \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_nios2_qsys_0_oci_test_bench.v \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_nios2_qsys_0_jtag_debug_module_sysclk.v \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_nios2_qsys_0.v \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_nios2_qsys_0_jtag_debug_module_tck.v \
  /home/icrs/Servo/FPGA/qsystem/simulation/submodules/qsystem_nios2_qsys_0_test_bench.v \
  /home/icrs/Servo/FPGA/qsystem/simulation/qsystem.v \
  -top $TOP_LEVEL_NAME
# ----------------------------------------
# simulate
if [ $SKIP_SIM -eq 0 ]; then
  ./simv $SIM_OPTIONS $USER_DEFINED_SIM_OPTIONS
fi
