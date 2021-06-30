
################################################################
# This is a generated script based on design: System
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

namespace eval _tcl {
proc get_script_folder {} {
   set script_path [file normalize [info script]]
   set script_folder [file dirname $script_path]
   return $script_folder
}
}
variable script_folder
set script_folder [_tcl::get_script_folder]

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2021.1
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   catch {common::send_gid_msg -ssname BD::TCL -id 2041 -severity "ERROR" "This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."}

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source System_script.tcl

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part xc7vx485tffg1761-2
   set_property BOARD_PART xilinx.com:vc707:part0:1.4 [current_project]
}


# CHANGE DESIGN NAME HERE
variable design_name
set design_name System

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design $design_name

# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      common::send_gid_msg -ssname BD::TCL -id 2001 -severity "INFO" "Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   common::send_gid_msg -ssname BD::TCL -id 2002 -severity "INFO" "Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   common::send_gid_msg -ssname BD::TCL -id 2003 -severity "INFO" "Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   common::send_gid_msg -ssname BD::TCL -id 2004 -severity "INFO" "Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

common::send_gid_msg -ssname BD::TCL -id 2005 -severity "INFO" "Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   catch {common::send_gid_msg -ssname BD::TCL -id 2006 -severity "ERROR" $errMsg}
   return $nRet
}


##################################################################
# MIG PRJ FILE TCL PROCs
##################################################################

proc write_mig_file_System_mig_7series_0_0 { str_mig_prj_filepath } {

   file mkdir [ file dirname "$str_mig_prj_filepath" ]
   set mig_prj_file [open $str_mig_prj_filepath  w+]

   puts $mig_prj_file {<?xml version='1.0' encoding='UTF-8'?>}
   puts $mig_prj_file {<!-- IMPORTANT: This is an internal file that has been generated by the MIG software. Any direct editing or changes made to this file may result in unpredictable behavior or data corruption. It is strongly advised that users do not edit the contents of this file. Re-run the MIG GUI with the required settings if any of the options provided below need to be altered. -->}
   puts $mig_prj_file {<Project NoOfControllers="1" >}
   puts $mig_prj_file {    <ModuleName>System_mig_7series_0_0</ModuleName>}
   puts $mig_prj_file {    <dci_inouts_inputs>1</dci_inouts_inputs>}
   puts $mig_prj_file {    <dci_inputs>1</dci_inputs>}
   puts $mig_prj_file {    <Debug_En>OFF</Debug_En>}
   puts $mig_prj_file {    <DataDepth_En>1024</DataDepth_En>}
   puts $mig_prj_file {    <LowPower_En>ON</LowPower_En>}
   puts $mig_prj_file {    <XADC_En>Enabled</XADC_En>}
   puts $mig_prj_file {    <TargetFPGA>xc7vx485t-ffg1761/-2</TargetFPGA>}
   puts $mig_prj_file {    <Version>3.0</Version>}
   puts $mig_prj_file {    <SystemClock>Differential</SystemClock>}
   puts $mig_prj_file {    <ReferenceClock>Use System Clock</ReferenceClock>}
   puts $mig_prj_file {    <SysResetPolarity>ACTIVE HIGH</SysResetPolarity>}
   puts $mig_prj_file {    <BankSelectionFlag>FALSE</BankSelectionFlag>}
   puts $mig_prj_file {    <InternalVref>0</InternalVref>}
   puts $mig_prj_file {    <dci_hr_inouts_inputs>50 Ohms</dci_hr_inouts_inputs>}
   puts $mig_prj_file {    <dci_cascade>0</dci_cascade>}
   puts $mig_prj_file {    <Controller number="0" >}
   puts $mig_prj_file {        <MemoryDevice>DDR3_SDRAM/sodimms/MT8JTF12864HZ-1G6</MemoryDevice>}
   puts $mig_prj_file {        <TimePeriod>1250</TimePeriod>}
   puts $mig_prj_file {        <VccAuxIO>2.0V</VccAuxIO>}
   puts $mig_prj_file {        <PHYRatio>4:1</PHYRatio>}
   puts $mig_prj_file {        <InputClkFreq>200</InputClkFreq>}
   puts $mig_prj_file {        <UIExtraClocks>0</UIExtraClocks>}
   puts $mig_prj_file {        <MMCM_VCO>800</MMCM_VCO>}
   puts $mig_prj_file {        <MMCMClkOut0> 5.000</MMCMClkOut0>}
   puts $mig_prj_file {        <MMCMClkOut1>1</MMCMClkOut1>}
   puts $mig_prj_file {        <MMCMClkOut2>1</MMCMClkOut2>}
   puts $mig_prj_file {        <MMCMClkOut3>1</MMCMClkOut3>}
   puts $mig_prj_file {        <MMCMClkOut4>1</MMCMClkOut4>}
   puts $mig_prj_file {        <DataWidth>64</DataWidth>}
   puts $mig_prj_file {        <DeepMemory>1</DeepMemory>}
   puts $mig_prj_file {        <DataMask>1</DataMask>}
   puts $mig_prj_file {        <ECC>Disabled</ECC>}
   puts $mig_prj_file {        <Ordering>Normal</Ordering>}
   puts $mig_prj_file {        <CustomPart>FALSE</CustomPart>}
   puts $mig_prj_file {        <NewPartName></NewPartName>}
   puts $mig_prj_file {        <RowAddress>14</RowAddress>}
   puts $mig_prj_file {        <ColAddress>10</ColAddress>}
   puts $mig_prj_file {        <BankAddress>3</BankAddress>}
   puts $mig_prj_file {        <MemoryVoltage>1.5V</MemoryVoltage>}
   puts $mig_prj_file {        <C0_MEM_SIZE>1073741824</C0_MEM_SIZE>}
   puts $mig_prj_file {        <UserMemoryAddressMap>ROW_BANK_COLUMN</UserMemoryAddressMap>}
   puts $mig_prj_file {        <PinSelection>}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="A20" SLEW="" name="ddr3_addr[0]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="B21" SLEW="" name="ddr3_addr[10]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="B17" SLEW="" name="ddr3_addr[11]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="A15" SLEW="" name="ddr3_addr[12]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="A21" SLEW="" name="ddr3_addr[13]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="B19" SLEW="" name="ddr3_addr[1]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="C20" SLEW="" name="ddr3_addr[2]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="A19" SLEW="" name="ddr3_addr[3]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="A17" SLEW="" name="ddr3_addr[4]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="A16" SLEW="" name="ddr3_addr[5]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="D20" SLEW="" name="ddr3_addr[6]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="C18" SLEW="" name="ddr3_addr[7]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="D17" SLEW="" name="ddr3_addr[8]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="C19" SLEW="" name="ddr3_addr[9]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="D21" SLEW="" name="ddr3_ba[0]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="C21" SLEW="" name="ddr3_ba[1]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="D18" SLEW="" name="ddr3_ba[2]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="K17" SLEW="" name="ddr3_cas_n" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15" PADName="G18" SLEW="" name="ddr3_ck_n[0]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15" PADName="H19" SLEW="" name="ddr3_ck_p[0]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="K19" SLEW="" name="ddr3_cke[0]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="J17" SLEW="" name="ddr3_cs_n[0]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="M13" SLEW="" name="ddr3_dm[0]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="K15" SLEW="" name="ddr3_dm[1]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="F12" SLEW="" name="ddr3_dm[2]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="A14" SLEW="" name="ddr3_dm[3]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="C23" SLEW="" name="ddr3_dm[4]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="D25" SLEW="" name="ddr3_dm[5]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="C31" SLEW="" name="ddr3_dm[6]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="F31" SLEW="" name="ddr3_dm[7]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="N14" SLEW="" name="ddr3_dq[0]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="H13" SLEW="" name="ddr3_dq[10]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="J13" SLEW="" name="ddr3_dq[11]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="L16" SLEW="" name="ddr3_dq[12]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="L15" SLEW="" name="ddr3_dq[13]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="H14" SLEW="" name="ddr3_dq[14]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="J15" SLEW="" name="ddr3_dq[15]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="E15" SLEW="" name="ddr3_dq[16]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="E13" SLEW="" name="ddr3_dq[17]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="F15" SLEW="" name="ddr3_dq[18]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="E14" SLEW="" name="ddr3_dq[19]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="N13" SLEW="" name="ddr3_dq[1]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="G13" SLEW="" name="ddr3_dq[20]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="G12" SLEW="" name="ddr3_dq[21]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="F14" SLEW="" name="ddr3_dq[22]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="G14" SLEW="" name="ddr3_dq[23]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="B14" SLEW="" name="ddr3_dq[24]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="C13" SLEW="" name="ddr3_dq[25]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="B16" SLEW="" name="ddr3_dq[26]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="D15" SLEW="" name="ddr3_dq[27]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="D13" SLEW="" name="ddr3_dq[28]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="E12" SLEW="" name="ddr3_dq[29]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="L14" SLEW="" name="ddr3_dq[2]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="C16" SLEW="" name="ddr3_dq[30]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="D16" SLEW="" name="ddr3_dq[31]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="A24" SLEW="" name="ddr3_dq[32]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="B23" SLEW="" name="ddr3_dq[33]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="B27" SLEW="" name="ddr3_dq[34]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="B26" SLEW="" name="ddr3_dq[35]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="A22" SLEW="" name="ddr3_dq[36]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="B22" SLEW="" name="ddr3_dq[37]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="A25" SLEW="" name="ddr3_dq[38]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="C24" SLEW="" name="ddr3_dq[39]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="M14" SLEW="" name="ddr3_dq[3]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="E24" SLEW="" name="ddr3_dq[40]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="D23" SLEW="" name="ddr3_dq[41]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="D26" SLEW="" name="ddr3_dq[42]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="C25" SLEW="" name="ddr3_dq[43]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="E23" SLEW="" name="ddr3_dq[44]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="D22" SLEW="" name="ddr3_dq[45]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="F22" SLEW="" name="ddr3_dq[46]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="E22" SLEW="" name="ddr3_dq[47]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="A30" SLEW="" name="ddr3_dq[48]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="D27" SLEW="" name="ddr3_dq[49]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="M12" SLEW="" name="ddr3_dq[4]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="A29" SLEW="" name="ddr3_dq[50]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="C28" SLEW="" name="ddr3_dq[51]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="D28" SLEW="" name="ddr3_dq[52]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="B31" SLEW="" name="ddr3_dq[53]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="A31" SLEW="" name="ddr3_dq[54]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="A32" SLEW="" name="ddr3_dq[55]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="E30" SLEW="" name="ddr3_dq[56]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="F29" SLEW="" name="ddr3_dq[57]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="F30" SLEW="" name="ddr3_dq[58]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="F27" SLEW="" name="ddr3_dq[59]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="N15" SLEW="" name="ddr3_dq[5]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="C30" SLEW="" name="ddr3_dq[60]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="E29" SLEW="" name="ddr3_dq[61]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="F26" SLEW="" name="ddr3_dq[62]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="D30" SLEW="" name="ddr3_dq[63]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="M11" SLEW="" name="ddr3_dq[6]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="L12" SLEW="" name="ddr3_dq[7]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="K14" SLEW="" name="ddr3_dq[8]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15_T_DCI" PADName="K13" SLEW="" name="ddr3_dq[9]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="M16" SLEW="" name="ddr3_dqs_n[0]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="J12" SLEW="" name="ddr3_dqs_n[1]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="G16" SLEW="" name="ddr3_dqs_n[2]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="C14" SLEW="" name="ddr3_dqs_n[3]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="A27" SLEW="" name="ddr3_dqs_n[4]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="E25" SLEW="" name="ddr3_dqs_n[5]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="B29" SLEW="" name="ddr3_dqs_n[6]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="E28" SLEW="" name="ddr3_dqs_n[7]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="N16" SLEW="" name="ddr3_dqs_p[0]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="K12" SLEW="" name="ddr3_dqs_p[1]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="H16" SLEW="" name="ddr3_dqs_p[2]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="C15" SLEW="" name="ddr3_dqs_p[3]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="A26" SLEW="" name="ddr3_dqs_p[4]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="F25" SLEW="" name="ddr3_dqs_p[5]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="B28" SLEW="" name="ddr3_dqs_p[6]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="DIFF_SSTL15_T_DCI" PADName="E27" SLEW="" name="ddr3_dqs_p[7]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="H20" SLEW="" name="ddr3_odt[0]" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="E20" SLEW="" name="ddr3_ras_n" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="LVCMOS15" PADName="C29" SLEW="" name="ddr3_reset_n" IN_TERM="" />}
   puts $mig_prj_file {            <Pin VCCAUX_IO="HIGH" IOSTANDARD="SSTL15" PADName="F20" SLEW="" name="ddr3_we_n" IN_TERM="" />}
   puts $mig_prj_file {        </PinSelection>}
   puts $mig_prj_file {        <System_Clock>}
   puts $mig_prj_file {            <Pin PADName="E19/E18(CC_P/N)" Bank="38" name="sys_clk_p/n" />}
   puts $mig_prj_file {        </System_Clock>}
   puts $mig_prj_file {        <System_Control>}
   puts $mig_prj_file {            <Pin PADName="No connect" Bank="Select Bank" name="sys_rst" />}
   puts $mig_prj_file {            <Pin PADName="No connect" Bank="Select Bank" name="init_calib_complete" />}
   puts $mig_prj_file {            <Pin PADName="No connect" Bank="Select Bank" name="tg_compare_error" />}
   puts $mig_prj_file {        </System_Control>}
   puts $mig_prj_file {        <TimingParameters>}
   puts $mig_prj_file {            <Parameters twtr="7.5" trrd="6" trefi="7.8" tfaw="30" trtp="7.5" tcke="5" trfc="110" trp="13.75" tras="35" trcd="13.75" />}
   puts $mig_prj_file {        </TimingParameters>}
   puts $mig_prj_file {        <mrBurstLength name="Burst Length" >8 - Fixed</mrBurstLength>}
   puts $mig_prj_file {        <mrBurstType name="Read Burst Type and Length" >Sequential</mrBurstType>}
   puts $mig_prj_file {        <mrCasLatency name="CAS Latency" >11</mrCasLatency>}
   puts $mig_prj_file {        <mrMode name="Mode" >Normal</mrMode>}
   puts $mig_prj_file {        <mrDllReset name="DLL Reset" >No</mrDllReset>}
   puts $mig_prj_file {        <mrPdMode name="DLL control for precharge PD" >Slow Exit</mrPdMode>}
   puts $mig_prj_file {        <emrDllEnable name="DLL Enable" >Enable</emrDllEnable>}
   puts $mig_prj_file {        <emrOutputDriveStrength name="Output Driver Impedance Control" >RZQ/7</emrOutputDriveStrength>}
   puts $mig_prj_file {        <emrMirrorSelection name="Address Mirroring" >Disable</emrMirrorSelection>}
   puts $mig_prj_file {        <emrCSSelection name="Controller Chip Select Pin" >Enable</emrCSSelection>}
   puts $mig_prj_file {        <emrRTT name="RTT (nominal) - On Die Termination (ODT)" >RZQ/6</emrRTT>}
   puts $mig_prj_file {        <emrPosted name="Additive Latency (AL)" >0</emrPosted>}
   puts $mig_prj_file {        <emrOCD name="Write Leveling Enable" >Disabled</emrOCD>}
   puts $mig_prj_file {        <emrDQS name="TDQS enable" >Enabled</emrDQS>}
   puts $mig_prj_file {        <emrRDQS name="Qoff" >Output Buffer Enabled</emrRDQS>}
   puts $mig_prj_file {        <mr2PartialArraySelfRefresh name="Partial-Array Self Refresh" >Full Array</mr2PartialArraySelfRefresh>}
   puts $mig_prj_file {        <mr2CasWriteLatency name="CAS write latency" >8</mr2CasWriteLatency>}
   puts $mig_prj_file {        <mr2AutoSelfRefresh name="Auto Self Refresh" >Enabled</mr2AutoSelfRefresh>}
   puts $mig_prj_file {        <mr2SelfRefreshTempRange name="High Temparature Self Refresh Rate" >Normal</mr2SelfRefreshTempRange>}
   puts $mig_prj_file {        <mr2RTTWR name="RTT_WR - Dynamic On Die Termination (ODT)" >Dynamic ODT off</mr2RTTWR>}
   puts $mig_prj_file {        <PortInterface>AXI</PortInterface>}
   puts $mig_prj_file {        <AXIParameters>}
   puts $mig_prj_file {            <C0_C_RD_WR_ARB_ALGORITHM>RD_PRI_REG</C0_C_RD_WR_ARB_ALGORITHM>}
   puts $mig_prj_file {            <C0_S_AXI_ADDR_WIDTH>30</C0_S_AXI_ADDR_WIDTH>}
   puts $mig_prj_file {            <C0_S_AXI_DATA_WIDTH>512</C0_S_AXI_DATA_WIDTH>}
   puts $mig_prj_file {            <C0_S_AXI_ID_WIDTH>3</C0_S_AXI_ID_WIDTH>}
   puts $mig_prj_file {            <C0_S_AXI_SUPPORTS_NARROW_BURST>1</C0_S_AXI_SUPPORTS_NARROW_BURST>}
   puts $mig_prj_file {        </AXIParameters>}
   puts $mig_prj_file {    </Controller>}
   puts $mig_prj_file {</Project>}

   close $mig_prj_file
}
# End of write_mig_file_System_mig_7series_0_0()



##################################################################
# DESIGN PROCs
##################################################################



# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  variable script_folder
  variable design_name

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2090 -severity "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2091 -severity "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports
  set ddr3 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:ddrx_rtl:1.0 ddr3 ]

  set pcie [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:pcie_7x_mgt_rtl:1.0 pcie ]

  set pcie_diff_clock [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 pcie_diff_clock ]

  set sys_clk [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 sys_clk ]
  set_property -dict [ list \
   CONFIG.FREQ_HZ {200000000} \
   ] $sys_clk


  # Create ports
  set HDMI_CLK [ create_bd_port -dir O HDMI_CLK ]
  set HDMI_DATA [ create_bd_port -dir O -from 35 -to 0 HDMI_DATA ]
  set HDMI_DE [ create_bd_port -dir O HDMI_DE ]
  set HDMI_HSYNC [ create_bd_port -dir O HDMI_HSYNC ]
  set HDMI_INTR [ create_bd_port -dir I HDMI_INTR ]
  set HDMI_SPDIF [ create_bd_port -dir O HDMI_SPDIF ]
  set HDMI_VSYNC [ create_bd_port -dir O HDMI_VSYNC ]
  set I2C_SCL [ create_bd_port -dir IO I2C_SCL ]
  set I2C_SDA [ create_bd_port -dir IO I2C_SDA ]
  set I2C_nRST [ create_bd_port -dir O I2C_nRST ]
  set pcie_perstn [ create_bd_port -dir I -type rst pcie_perstn ]
  set sys_rst [ create_bd_port -dir I -type rst sys_rst ]
  set_property -dict [ list \
   CONFIG.POLARITY {ACTIVE_HIGH} \
 ] $sys_rst

  # Create instance: AXI_PCIe, and set properties
  set AXI_PCIe [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_pcie:2.9 AXI_PCIe ]
  set_property -dict [ list \
   CONFIG.AXIBAR2PCIEBAR_0 {0xD0000000} \
   CONFIG.AXIBAR2PCIEBAR_1 {0xD2000000} \
   CONFIG.AXIBAR_AS_0 {true} \
   CONFIG.AXIBAR_AS_1 {true} \
   CONFIG.AXIBAR_NUM {2} \
   CONFIG.BAR0_SCALE {Megabytes} \
   CONFIG.BAR0_SIZE {1} \
   CONFIG.BAR1_ENABLED {true} \
   CONFIG.BAR1_SCALE {Gigabytes} \
   CONFIG.BAR1_SIZE {1} \
   CONFIG.BAR1_TYPE {Memory} \
   CONFIG.BAR_64BIT {true} \
   CONFIG.DEVICE_ID {0x7022} \
   CONFIG.MAX_LINK_SPEED {5.0_GT/s} \
   CONFIG.M_AXI_DATA_WIDTH {128} \
   CONFIG.NO_OF_LANES {X4} \
   CONFIG.NUM_MSI_REQ {3} \
   CONFIG.PCIEBAR2AXIBAR_1 {0x80000000} \
   CONFIG.SUBSYSTEM_ID {0x2D3D} \
   CONFIG.SUBSYSTEM_VENDOR_ID {0xC10E} \
   CONFIG.S_AXI_DATA_WIDTH {128} \
   CONFIG.S_AXI_SUPPORTS_NARROW_BURST {true} \
   CONFIG.XLNX_REF_BOARD {VC707} \
 ] $AXI_PCIe

  # Create instance: DDR3_SDRAM, and set properties
  set DDR3_SDRAM [ create_bd_cell -type ip -vlnv xilinx.com:ip:mig_7series:4.2 DDR3_SDRAM ]

  # Generate the PRJ File for MIG
  set str_mig_folder [get_property IP_DIR [ get_ips [ get_property CONFIG.Component_Name $DDR3_SDRAM ] ] ]
  set str_mig_file_name mig_a.prj
  set str_mig_file_path ${str_mig_folder}/${str_mig_file_name}

  write_mig_file_System_mig_7series_0_0 $str_mig_file_path

  set_property -dict [ list \
   CONFIG.BOARD_MIG_PARAM {Custom} \
   CONFIG.RESET_BOARD_INTERFACE {Custom} \
   CONFIG.XML_INPUT_FILE {mig_a.prj} \
 ] $DDR3_SDRAM

  # Create instance: MSI_Generator_0, and set properties
  set MSI_Generator_0 [ create_bd_cell -type ip -vlnv xilinx.com:user:MSI_Generator:1.0 MSI_Generator_0 ]
  set_property -dict [ list \
   CONFIG.INTERRUPT_SIZE {2} \
 ] $MSI_Generator_0

  # Create instance: PCIe_diff_clk_I, and set properties
  set PCIe_diff_clk_I [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_ds_buf:2.2 PCIe_diff_clk_I ]
  set_property -dict [ list \
   CONFIG.C_BUF_TYPE {IBUFDSGTE} \
   CONFIG.DIFF_CLK_IN_BOARD_INTERFACE {Custom} \
 ] $PCIe_diff_clk_I

  # Create instance: axi_apb_bridge_0, and set properties
  set axi_apb_bridge_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_apb_bridge:3.0 axi_apb_bridge_0 ]
  set_property -dict [ list \
   CONFIG.C_APB_NUM_SLAVES {2} \
 ] $axi_apb_bridge_0

  # Create instance: bus_reset_0, and set properties
  set bus_reset_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 bus_reset_0 ]
  set_property -dict [ list \
   CONFIG.C_EXT_RST_WIDTH {16} \
 ] $bus_reset_0

  # Create instance: dut_wrapper_0, and set properties
  set dut_wrapper_0 [ create_bd_cell -type ip -vlnv xilinx.com:user:dut_wrapper:1.0 dut_wrapper_0 ]

  # Create instance: from_pcie, and set properties
  set from_pcie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 from_pcie ]
  set_property -dict [ list \
   CONFIG.ENABLE_ADVANCED_OPTIONS {1} \
   CONFIG.M01_HAS_REGSLICE {1} \
   CONFIG.NUM_MI {3} \
   CONFIG.SYNCHRONIZATION_STAGES {2} \
   CONFIG.XBAR_DATA_WIDTH {128} \
 ] $from_pcie

  # Create instance: hdmi_controller_0, and set properties
  set hdmi_controller_0 [ create_bd_cell -type ip -vlnv xilinx.com:user:hdmi_controller:1.0 hdmi_controller_0 ]
  set_property -dict [ list \
   CONFIG.C_M_AXI_ID_WIDTH {1} \
 ] $hdmi_controller_0

  # Create instance: pcie_reset_0, and set properties
  set pcie_reset_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 pcie_reset_0 ]
  set_property -dict [ list \
   CONFIG.C_EXT_RST_WIDTH {16} \
 ] $pcie_reset_0

  # Create instance: to_memory, and set properties
  set to_memory [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 to_memory ]
  set_property -dict [ list \
   CONFIG.ENABLE_ADVANCED_OPTIONS {0} \
   CONFIG.M00_HAS_REGSLICE {0} \
   CONFIG.M01_HAS_REGSLICE {0} \
   CONFIG.M02_HAS_REGSLICE {3} \
   CONFIG.NUM_MI {2} \
   CONFIG.NUM_SI {3} \
   CONFIG.S00_HAS_DATA_FIFO {1} \
   CONFIG.S00_HAS_REGSLICE {0} \
   CONFIG.S01_HAS_DATA_FIFO {1} \
   CONFIG.S02_HAS_DATA_FIFO {1} \
   CONFIG.SYNCHRONIZATION_STAGES {2} \
 ] $to_memory

  # Create instance: util_vector_logic_0, and set properties
  set util_vector_logic_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_vector_logic:2.0 util_vector_logic_0 ]
  set_property -dict [ list \
   CONFIG.C_SIZE {1} \
 ] $util_vector_logic_0

  # Create interface connections
  connect_bd_intf_net -intf_net AXI_PCIe_M_AXI [get_bd_intf_pins AXI_PCIe/M_AXI] [get_bd_intf_pins from_pcie/S00_AXI]
  connect_bd_intf_net -intf_net AXI_PCIe_pcie_7x_mgt [get_bd_intf_ports pcie] [get_bd_intf_pins AXI_PCIe/pcie_7x_mgt]
  connect_bd_intf_net -intf_net DDR3_SDRAM_DDR3 [get_bd_intf_ports ddr3] [get_bd_intf_pins DDR3_SDRAM/DDR3]
  connect_bd_intf_net -intf_net SYS_CLK_1 [get_bd_intf_ports sys_clk] [get_bd_intf_pins DDR3_SDRAM/SYS_CLK]
  connect_bd_intf_net -intf_net axi4_0_M00_AXI [get_bd_intf_pins DDR3_SDRAM/S_AXI] [get_bd_intf_pins to_memory/M00_AXI]
  connect_bd_intf_net -intf_net axi_apb_bridge_0_APB_M [get_bd_intf_pins axi_apb_bridge_0/APB_M] [get_bd_intf_pins hdmi_controller_0/S_APB]
  connect_bd_intf_net -intf_net axi_apb_bridge_0_APB_M2 [get_bd_intf_pins axi_apb_bridge_0/APB_M2] [get_bd_intf_pins dut_wrapper_0/S_APB]
  connect_bd_intf_net -intf_net dut_wrapper_0_M_AXI [get_bd_intf_pins dut_wrapper_0/M_AXI] [get_bd_intf_pins to_memory/S02_AXI]
  connect_bd_intf_net -intf_net from_pcie_M00_AXI [get_bd_intf_pins from_pcie/M00_AXI] [get_bd_intf_pins to_memory/S00_AXI]
  connect_bd_intf_net -intf_net from_pcie_M01_AXI [get_bd_intf_pins axi_apb_bridge_0/AXI4_LITE] [get_bd_intf_pins from_pcie/M01_AXI]
  connect_bd_intf_net -intf_net from_pcie_M02_AXI [get_bd_intf_pins AXI_PCIe/S_AXI_CTL] [get_bd_intf_pins from_pcie/M02_AXI]
  connect_bd_intf_net -intf_net hdmi_controller_0_M_AXI [get_bd_intf_pins hdmi_controller_0/M_AXI] [get_bd_intf_pins to_memory/S01_AXI]
  connect_bd_intf_net -intf_net pcie_diff_clk_1 [get_bd_intf_ports pcie_diff_clock] [get_bd_intf_pins PCIe_diff_clk_I/CLK_IN_D]
  connect_bd_intf_net -intf_net to_memory_M01_AXI [get_bd_intf_pins AXI_PCIe/S_AXI] [get_bd_intf_pins to_memory/M01_AXI]

  # Create port connections
  connect_bd_net -net ARESETN_1 [get_bd_pins from_pcie/ARESETN] [get_bd_pins pcie_reset_0/interconnect_aresetn]
  connect_bd_net -net AXI_PCIe_INTX_MSI_Grant [get_bd_pins AXI_PCIe/INTX_MSI_Grant] [get_bd_pins MSI_Generator_0/MSI_GRANT]
  connect_bd_net -net AXI_PCIe_MSI_enable [get_bd_pins AXI_PCIe/MSI_enable] [get_bd_pins MSI_Generator_0/MSI_EN]
  connect_bd_net -net AXI_PCIe_axi_aclk_out [get_bd_pins AXI_PCIe/axi_aclk_out] [get_bd_pins MSI_Generator_0/CLK] [get_bd_pins from_pcie/ACLK] [get_bd_pins from_pcie/S00_ACLK] [get_bd_pins to_memory/M01_ACLK]
  connect_bd_net -net AXI_PCIe_axi_ctl_aclk_out [get_bd_pins AXI_PCIe/axi_ctl_aclk_out] [get_bd_pins from_pcie/M02_ACLK] [get_bd_pins pcie_reset_0/slowest_sync_clk]
  connect_bd_net -net AXI_PCIe_mmcm_lock [get_bd_pins AXI_PCIe/mmcm_lock] [get_bd_pins util_vector_logic_0/Op2]
  connect_bd_net -net DDR3_SDRAM_mmcm_locked [get_bd_pins DDR3_SDRAM/mmcm_locked] [get_bd_pins util_vector_logic_0/Op1]
  connect_bd_net -net DDR3_SDRAM_ui_clk [get_bd_pins DDR3_SDRAM/ui_clk] [get_bd_pins axi_apb_bridge_0/s_axi_aclk] [get_bd_pins bus_reset_0/slowest_sync_clk] [get_bd_pins dut_wrapper_0/ACLK] [get_bd_pins from_pcie/M00_ACLK] [get_bd_pins from_pcie/M01_ACLK] [get_bd_pins hdmi_controller_0/ACLK] [get_bd_pins to_memory/ACLK] [get_bd_pins to_memory/M00_ACLK] [get_bd_pins to_memory/S00_ACLK] [get_bd_pins to_memory/S01_ACLK] [get_bd_pins to_memory/S02_ACLK]
  connect_bd_net -net DDR3_SDRAM_ui_clk_sync_rst [get_bd_pins DDR3_SDRAM/ui_clk_sync_rst] [get_bd_pins bus_reset_0/ext_reset_in] [get_bd_pins pcie_reset_0/ext_reset_in]
  connect_bd_net -net HDMI_INTR_1 [get_bd_ports HDMI_INTR] [get_bd_pins hdmi_controller_0/HDMI_INTR]
  connect_bd_net -net MSI_Generator_0_MSI_NUM [get_bd_pins AXI_PCIe/MSI_Vector_Num] [get_bd_pins MSI_Generator_0/MSI_NUM]
  connect_bd_net -net MSI_Generator_0_MSI_REQ [get_bd_pins AXI_PCIe/INTX_MSI_Request] [get_bd_pins MSI_Generator_0/MSI_REQ]
  connect_bd_net -net Net [get_bd_ports I2C_SCL] [get_bd_pins hdmi_controller_0/I2C_SCL]
  connect_bd_net -net Net1 [get_bd_ports I2C_SDA] [get_bd_pins hdmi_controller_0/I2C_SDA]
  connect_bd_net -net PCIe_diff_clk_I_IBUF_OUT [get_bd_pins AXI_PCIe/REFCLK] [get_bd_pins PCIe_diff_clk_I/IBUF_OUT]
  connect_bd_net -net S00_ARESETN_1 [get_bd_pins AXI_PCIe/axi_aresetn] [get_bd_pins MSI_Generator_0/nRST] [get_bd_pins from_pcie/M02_ARESETN] [get_bd_pins from_pcie/S00_ARESETN] [get_bd_pins pcie_reset_0/peripheral_aresetn] [get_bd_pins to_memory/M01_ARESETN]
  connect_bd_net -net dut_wrapper_0_INTR [get_bd_pins MSI_Generator_0/INTR_1] [get_bd_pins dut_wrapper_0/INTR]
  connect_bd_net -net hdmi_controller_0_HDMI_CLK [get_bd_ports HDMI_CLK] [get_bd_pins hdmi_controller_0/HDMI_CLK]
  connect_bd_net -net hdmi_controller_0_HDMI_DATA [get_bd_ports HDMI_DATA] [get_bd_pins hdmi_controller_0/HDMI_DATA]
  connect_bd_net -net hdmi_controller_0_HDMI_DE [get_bd_ports HDMI_DE] [get_bd_pins hdmi_controller_0/HDMI_DE]
  connect_bd_net -net hdmi_controller_0_HDMI_HSYNC [get_bd_ports HDMI_HSYNC] [get_bd_pins hdmi_controller_0/HDMI_HSYNC]
  connect_bd_net -net hdmi_controller_0_HDMI_SPDIF [get_bd_ports HDMI_SPDIF] [get_bd_pins hdmi_controller_0/HDMI_SPDIF]
  connect_bd_net -net hdmi_controller_0_HDMI_VSYNC [get_bd_ports HDMI_VSYNC] [get_bd_pins hdmi_controller_0/HDMI_VSYNC]
  connect_bd_net -net hdmi_controller_0_I2C_nRST [get_bd_ports I2C_nRST] [get_bd_pins hdmi_controller_0/I2C_nRST]
  connect_bd_net -net hdmi_controller_0_INTR [get_bd_pins MSI_Generator_0/INTR_0] [get_bd_pins hdmi_controller_0/INTR]
  connect_bd_net -net pcie_perstn_1 [get_bd_ports pcie_perstn] [get_bd_pins bus_reset_0/aux_reset_in] [get_bd_pins pcie_reset_0/aux_reset_in]
  connect_bd_net -net reset_1 [get_bd_ports sys_rst] [get_bd_pins DDR3_SDRAM/sys_rst]
  connect_bd_net -net rst_DDR3_SDRAM_200M_interconnect_aresetn [get_bd_pins bus_reset_0/interconnect_aresetn] [get_bd_pins to_memory/ARESETN]
  connect_bd_net -net rst_DDR3_SDRAM_200M_peripheral_aresetn [get_bd_pins DDR3_SDRAM/aresetn] [get_bd_pins axi_apb_bridge_0/s_axi_aresetn] [get_bd_pins bus_reset_0/peripheral_aresetn] [get_bd_pins dut_wrapper_0/nRST] [get_bd_pins from_pcie/M00_ARESETN] [get_bd_pins from_pcie/M01_ARESETN] [get_bd_pins hdmi_controller_0/nRST] [get_bd_pins to_memory/M00_ARESETN] [get_bd_pins to_memory/S00_ARESETN] [get_bd_pins to_memory/S01_ARESETN] [get_bd_pins to_memory/S02_ARESETN]
  connect_bd_net -net util_vector_logic_0_Res [get_bd_pins bus_reset_0/dcm_locked] [get_bd_pins pcie_reset_0/dcm_locked] [get_bd_pins util_vector_logic_0/Res]

  # Create address segments
  assign_bd_address -offset 0x40000000 -range 0x02000000 -target_address_space [get_bd_addr_spaces AXI_PCIe/M_AXI] [get_bd_addr_segs AXI_PCIe/S_AXI/BAR0] -force
  assign_bd_address -offset 0x42000000 -range 0x02000000 -target_address_space [get_bd_addr_spaces AXI_PCIe/M_AXI] [get_bd_addr_segs AXI_PCIe/S_AXI/BAR1] -force
  assign_bd_address -offset 0x00000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces AXI_PCIe/M_AXI] [get_bd_addr_segs AXI_PCIe/S_AXI_CTL/CTL0] -force
  assign_bd_address -offset 0x80000000 -range 0x40000000 -target_address_space [get_bd_addr_spaces AXI_PCIe/M_AXI] [get_bd_addr_segs DDR3_SDRAM/memmap/memaddr] -force
  assign_bd_address -offset 0x00020000 -range 0x00010000 -target_address_space [get_bd_addr_spaces AXI_PCIe/M_AXI] [get_bd_addr_segs dut_wrapper_0/S_APB/S_APB] -force
  assign_bd_address -offset 0x00010000 -range 0x00010000 -target_address_space [get_bd_addr_spaces AXI_PCIe/M_AXI] [get_bd_addr_segs hdmi_controller_0/S_APB/S_APB] -force


  # Restore current instance
  current_bd_instance $oldCurInst

  validate_bd_design
  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


