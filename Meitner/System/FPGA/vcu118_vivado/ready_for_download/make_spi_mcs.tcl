file copy -force ../System.runs/impl_1/system_design_wrapper.bit .
write_cfgmem -force -format MCS -size 64 -interface SPIx8 -loadbit "up 0x00000000 system_design_wrapper.bit" system_design.mcs
