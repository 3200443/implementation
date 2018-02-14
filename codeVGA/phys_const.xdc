set_property PACKAGE_PIN Y9 [get_ports clk_inp]
set_property PACKAGE_PIN AA19 [get_ports HSYNC_outp]
set_property PACKAGE_PIN Y19 [get_ports VSYNC_outp]
set_property PACKAGE_PIN AB19 [get_ports {data_B_outp[3]}]
set_property PACKAGE_PIN AB20 [get_ports {data_B_outp[2]}]
set_property PACKAGE_PIN Y20 [get_ports {data_B_outp[1]}]
set_property PACKAGE_PIN Y21 [get_ports {data_B_outp[0]}]
set_property PACKAGE_PIN AA21 [get_ports {data_G_outp[3]}]
set_property PACKAGE_PIN AB21 [get_ports {data_G_outp[2]}]
set_property PACKAGE_PIN AA22 [get_ports {data_G_outp[1]}]
set_property PACKAGE_PIN AB22 [get_ports {data_G_outp[0]}]
set_property PACKAGE_PIN V18 [get_ports {data_R_outp[3]}]
set_property PACKAGE_PIN V19 [get_ports {data_R_outp[2]}]
set_property PACKAGE_PIN U20 [get_ports {data_R_outp[1]}]
set_property PACKAGE_PIN V20 [get_ports {data_R_outp[0]}]

set_property PACKAGE_PIN F22 [get_ports reset_inp]

# ----------------------------------------------------------------------------
# IOSTANDARD Constraints
#
# Note that these IOSTANDARD constraints are applied to all IOs currently
# assigned within an I/O bank.  If these IOSTANDARD constraints are 
# evaluated prior to other PACKAGE_PIN constraints being applied, then 
# the IOSTANDARD specified will likely not be applied properly to those 
# pins.  Therefore, bank wide IOSTANDARD constraints should be placed 
# within the XDC file in a location that is evaluated AFTER all 
# PACKAGE_PIN constraints within the target bank have been evaluated.
#
# Un-comment one or more of the following IOSTANDARD constraints according to
# the bank pin assignments that are required within a design.
# ---------------------------------------------------------------------------- 

# Note that the bank voltage for IO Bank 33 is fixed to 3.3V on ZedBoard. 
set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 33]];

# Set the bank voltage for IO Bank 34 to 1.8V by default.
# set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 34]];
# set_property IOSTANDARD LVCMOS25 [get_ports -of_objects [get_iobanks 34]];
# set_property IOSTANDARD LVCMOS18 [get_ports -of_objects [get_iobanks 34]];

# Set the bank voltage for IO Bank 35 to 1.8V by default.
# set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 35]];
# set_property IOSTANDARD LVCMOS25 [get_ports -of_objects [get_iobanks 35]];
set_property IOSTANDARD LVCMOS18 [get_ports -of_objects [get_iobanks 35]];

# Note that the bank voltage for IO Bank 13 is fixed to 3.3V on ZedBoard. 
set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 13]];
