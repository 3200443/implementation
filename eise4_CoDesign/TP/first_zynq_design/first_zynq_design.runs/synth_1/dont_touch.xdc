# This file is automatically generated.
# It contains project source information necessary for synthesis and implementation.

# Block Designs: bd/first_zynq_system/first_zynq_system.bd
set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==first_zynq_system || ORIG_REF_NAME==first_zynq_system}]

# IP: bd/first_zynq_system/ip/first_zynq_system_processing_system7_0_0/first_zynq_system_processing_system7_0_0.xci
set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==first_zynq_system_processing_system7_0_0 || ORIG_REF_NAME==first_zynq_system_processing_system7_0_0}]

# IP: bd/first_zynq_system/ip/first_zynq_system_axi_gpio_0_0/first_zynq_system_axi_gpio_0_0.xci
set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==first_zynq_system_axi_gpio_0_0 || ORIG_REF_NAME==first_zynq_system_axi_gpio_0_0}]

# IP: bd/first_zynq_system/ip/first_zynq_system_processing_system7_0_axi_periph_0/first_zynq_system_processing_system7_0_axi_periph_0.xci
set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==first_zynq_system_processing_system7_0_axi_periph_0 || ORIG_REF_NAME==first_zynq_system_processing_system7_0_axi_periph_0}]

# IP: bd/first_zynq_system/ip/first_zynq_system_rst_processing_system7_0_100M_0/first_zynq_system_rst_processing_system7_0_100M_0.xci
set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==first_zynq_system_rst_processing_system7_0_100M_0 || ORIG_REF_NAME==first_zynq_system_rst_processing_system7_0_100M_0}]

# IP: bd/first_zynq_system/ip/first_zynq_system_auto_pc_0/first_zynq_system_auto_pc_0.xci
set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==first_zynq_system_auto_pc_0 || ORIG_REF_NAME==first_zynq_system_auto_pc_0}]

# XDC: bd/first_zynq_system/ip/first_zynq_system_processing_system7_0_0/first_zynq_system_processing_system7_0_0.xdc
set_property DONT_TOUCH TRUE [get_cells [split [join [get_cells -hier -filter {REF_NAME==first_zynq_system_processing_system7_0_0 || ORIG_REF_NAME==first_zynq_system_processing_system7_0_0}] {/inst }]/inst ]]

# XDC: bd/first_zynq_system/ip/first_zynq_system_axi_gpio_0_0/first_zynq_system_axi_gpio_0_0_board.xdc
set_property DONT_TOUCH TRUE [get_cells [split [join [get_cells -hier -filter {REF_NAME==first_zynq_system_axi_gpio_0_0 || ORIG_REF_NAME==first_zynq_system_axi_gpio_0_0}] {/U0 }]/U0 ]]

# XDC: bd/first_zynq_system/ip/first_zynq_system_axi_gpio_0_0/first_zynq_system_axi_gpio_0_0_ooc.xdc

# XDC: bd/first_zynq_system/ip/first_zynq_system_axi_gpio_0_0/first_zynq_system_axi_gpio_0_0.xdc
#dup# set_property DONT_TOUCH TRUE [get_cells [split [join [get_cells -hier -filter {REF_NAME==first_zynq_system_axi_gpio_0_0 || ORIG_REF_NAME==first_zynq_system_axi_gpio_0_0}] {/U0 }]/U0 ]]

# XDC: bd/first_zynq_system/ip/first_zynq_system_rst_processing_system7_0_100M_0/first_zynq_system_rst_processing_system7_0_100M_0_board.xdc
#dup# set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==first_zynq_system_rst_processing_system7_0_100M_0 || ORIG_REF_NAME==first_zynq_system_rst_processing_system7_0_100M_0}]

# XDC: bd/first_zynq_system/ip/first_zynq_system_rst_processing_system7_0_100M_0/first_zynq_system_rst_processing_system7_0_100M_0.xdc
#dup# set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==first_zynq_system_rst_processing_system7_0_100M_0 || ORIG_REF_NAME==first_zynq_system_rst_processing_system7_0_100M_0}]

# XDC: bd/first_zynq_system/ip/first_zynq_system_rst_processing_system7_0_100M_0/first_zynq_system_rst_processing_system7_0_100M_0_ooc.xdc

# XDC: bd/first_zynq_system/ip/first_zynq_system_auto_pc_0/first_zynq_system_auto_pc_0_ooc.xdc

# XDC: bd/first_zynq_system/first_zynq_system_ooc.xdc
