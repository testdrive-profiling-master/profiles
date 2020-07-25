variable script_path [file normalize [info script]]
variable curdir [file dirname $script_path]
cd $curdir
eval file delete -force System_wrapper.mcs
write_cfgmem -force -format MCS -size 128 -interface BPIx16 -loadbit "up 0x00000000 ../vc707_vivado/System.runs/impl_1/System_wrapper.bit" System_wrapper.mcs
eval file delete -force System_wrapper.prm
