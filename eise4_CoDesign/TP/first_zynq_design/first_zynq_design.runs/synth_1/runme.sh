#!/bin/sh

# 
# Vivado(TM)
# runme.sh: a Vivado-generated Runs Script for UNIX
# Copyright 1986-2015 Xilinx, Inc. All Rights Reserved.
# 

if [ -z "$PATH" ]; then
  PATH=/opt/Xilinx/SDK/2015.4/bin:/opt/Xilinx/Vivado/2015.4/ids_lite/ISE/bin/lin64:/opt/Xilinx/Vivado/2015.4/bin
else
  PATH=/opt/Xilinx/SDK/2015.4/bin:/opt/Xilinx/Vivado/2015.4/ids_lite/ISE/bin/lin64:/opt/Xilinx/Vivado/2015.4/bin:$PATH
fi
export PATH

if [ -z "$LD_LIBRARY_PATH" ]; then
  LD_LIBRARY_PATH=/opt/Xilinx/Vivado/2015.4/ids_lite/ISE/lib/lin64
else
  LD_LIBRARY_PATH=/opt/Xilinx/Vivado/2015.4/ids_lite/ISE/lib/lin64:$LD_LIBRARY_PATH
fi
export LD_LIBRARY_PATH

HD_PWD='/nfs/home/sasl/eleves/ei-se/3300195/eise4_CoDesign/TP/first_zynq_design/first_zynq_design.runs/synth_1'
cd "$HD_PWD"

HD_LOG=runme.log
/bin/touch $HD_LOG

ISEStep="./ISEWrap.sh"
EAStep()
{
     $ISEStep $HD_LOG "$@" >> $HD_LOG 2>&1
     if [ $? -ne 0 ]
     then
         exit
     fi
}

EAStep vivado -log first_zynq_system_wrapper.vds -m64 -mode batch -messageDb vivado.pb -notrace -source first_zynq_system_wrapper.tcl
