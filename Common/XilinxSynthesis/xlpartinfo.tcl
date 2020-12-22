#
# Source this TCL file in Vivado to generate the partinfo data base for Sysgen
#

#
# Name : dsp_get_all_families
# Description : Iterates through all the parts and returns all the unique families
# return : A tcl list of all families supported
# 
proc dsp_get_all_families { } {
   set parts [get_parts]
   set families [list]
   foreach part $parts {
        set family [get_property FAMILY $part]
        if { [lsearch -exact $families $family] == -1 } {
            lappend families $family
        }
   }
   return $families
}

proc dsp_get_all_boards { } {
   set boards [get_board_parts -quiet]
   return $boards
}

proc dsp_get_devices_from_family { family } {
   set parts [eval "get_parts -filter {FAMILY == $family}"]
   set devices [list]
   foreach part $parts {
        set device [get_property DEVICE $part]
        if { [lsearch -exact $devices $device] == -1 } {
            lappend devices $device
        }
   }
    if {$family == "everesteaes1"} {
     set devices [string tolower $devices]
   }
   return $devices
}


proc dsp_write_device_list_mapping { family fileID } {
    set devices [dsp_get_devices_from_family $family]
    puts $fileID "    '$family' => \["
    foreach device $devices {
        puts $fileID "      '$device',"
    }
    puts $fileID "    \]," 
}

proc dsp_write_part_family { family fileID } {
   puts $fileID "    '$family' => {"
   set parts [eval "get_parts -filter {FAMILY == $family}"]
   set devices [list]
   foreach part $parts {
        set device [get_property DEVICE $part]
        if { [lsearch -exact $devices $device] == -1 } {
            lappend devices $device
        }
   }
   foreach device $devices {
       set pkgs [list]
       set spds [list]
       set tmps [list]
       set tmp_spds [list]
       set revs [list]
       set PkgSpeedmap [dict create]
       
       set parts [eval "get_parts -filter {DEVICE == $device}"]
       
       foreach part $parts {

           set pkg [get_property PACKAGE $part -quiet]
           set spd [get_property SPEED $part -quiet] 
           set rev [get_property REVISION $part -quiet] 
           set tmp [string tolower [get_property TEMPERATURE_GRADE_LETTER $part -quiet] ]
           set family [get_property FAMILY $part -quiet]
           if {$tmp == "" && $family == "everesteaes1"} {
	       set tmp "e"
	   }

           if { [lsearch -exact $pkgs $pkg] == -1 } {
               lappend pkgs $pkg
           }

           if { [lsearch -exact $spds $spd] == -1 } {
               lappend spds $spd
           }

           if { [lsearch -exact $tmps $tmp] == -1 } {
               lappend tmps $tmp
           }

           if { [lsearch -exact $revs $rev] == -1 } {
               lappend revs $rev
           }

           if { [string first "$spd-$tmp-$rev" $part] > 0} {
               set tmp_spd "$spd-$tmp-$rev"
           } elseif { [string first "$spd-$tmp" $part] > 0} {
               set tmp_spd "$spd-$tmp"
           } else {
               set tmp_spd "$spd"
           }
           if { [lsearch -exact $tmp_spds $tmp_spd] == -1 } {
               lappend tmp_spds $tmp_spd
           }

           if { [dict exists $PkgSpeedmap $pkg] } {
                set grade [dict get $PkgSpeedmap $pkg]
                lappend grade $tmp_spd
                dict set PkgSpeedmap $pkg $grade 
           } else {
                dict set PkgSpeedmap $pkg $tmp_spd 
           }
       }

       if {$family == "everesteaes1"} {
          set device [string tolower $device]
       }
       puts $fileID "      '$device' => {"
       puts $fileID "        'PackageSpeedMap' => {"

       foreach key [dict keys $PkgSpeedmap] {
          set value [dict get $PkgSpeedmap $key]
          puts $fileID "          '$key' => \["
          foreach item $value {
            puts $fileID "            '$item',"
          }
          puts $fileID "          \],"
       }
       
       puts $fileID "        },"
       puts $fileID "        'pkgs' => \["
       foreach pkg $pkgs {
           puts $fileID "          '$pkg',"
       }
       puts $fileID "        \],"
       puts $fileID "        'spds' => \["
       foreach spd $spds {
           puts $fileID "          '$spd',"
       }
       puts $fileID "        \],"
       puts $fileID "        'tmps' => \["
       foreach tmp $tmps {
           puts $fileID "          '$tmp',"
       }
       puts $fileID "        \],"
       puts $fileID "        'tmp_spds' => \["
       foreach tmp_spd $tmp_spds {
           puts $fileID "          '$tmp_spd',"
       }
       puts $fileID "        \],"
       puts $fileID "      },"
   }
   puts $fileID "    },"
}

proc dsp_write_board_data { board fileID } {
    set vendor_name [get_property VENDOR_NAME [get_boards -of_objects $board]]
    set board_part_version [get_property FILE_VERSION  [get_boards -of_objects $board]]
    set board_name [get_property BOARD_NAME [get_boards -of_objects $board]]
    set board_part_name [get_property PART_NAME $board]
    set board_revisions [get_property COMPATIBLE_BOARD_REVISIONS [get_boards -of_objects $board]]
    set display_name [get_property DISPLAY_NAME [get_boards -of_objects $board]]
    
    regsub -all {:} $board {_} board
    regsub -all {\.} $board {_} board
    regsub -all {\-} $board {_} board
    puts $fileID "    '$board' => {"
    puts $fileID "      'Vendor' => '$vendor_name',"
    puts $fileID "      'Version' => '$board_part_version',"
    puts $fileID "      'BoardName' => '$board_name',"
    puts $fileID "      'BoardPartName' => '$board_part_name',"
    puts $fileID "      'DisplayName' => '$display_name',"
    puts $fileID "      'CompatiblieBoardRevisions' => \["
    foreach board_revision $board_revisions {
        set rev [lindex $board_revision 1]
        puts $fileID "        '$rev',"
    }
    puts $fileID "      \],"
    puts $fileID "    },"
}

proc main { filename } {
    if { [ catch { set fileID [open $filename "w+"] } err ] } {
       error "can not open file $filename for writing"
    }
    set families [dsp_get_all_families]
    set sw_version [version -short]
    
    puts $fileID "{"
    puts $fileID "  'BuildNumber' => '$sw_version',"
    puts $fileID "  'DeviceListMapping' => {"
    foreach family $families {
        dsp_write_device_list_mapping $family $fileID
    }
    puts $fileID "  },"
    puts $fileID "  'Families' => {"
    foreach family $families {
        dsp_write_part_family $family $fileID
    }
    puts $fileID "  },"
    puts $fileID "  'PartnameMapping' => {"
    puts $fileID "    'artix7' => 'Artix7',"
    puts $fileID "    'artix7l' => 'Artix7 Low Voltage',"
    puts $fileID "    'aartix7' => 'Automotive Artix7',"
    puts $fileID "    'aspartan7' => 'Automotive Spartan7',"
    puts $fileID "    'azynq' => 'Automotive Zynq',"
    puts $fileID "    'azynquplus' => 'Automotive Zynq UltraScale+',"
    puts $fileID "    'everesteaes1' => 'Everest Early Access ES1',"
    puts $fileID "    'kintex7' => 'Kintex7',"
    puts $fileID "    'kintex7l' => 'Kintex7 Low Voltage',"
    puts $fileID "    'kintexu' => 'Kintex UltraScale',"
    puts $fileID "    'kintexuplus' => 'Kintex UltraScale+',"
    puts $fileID "    'qartix7' => 'Defense-Grade Artix7',"
    puts $fileID "    'qkintex7' => 'Defense-Grade Kintex7',"
    puts $fileID "    'qkintex7l' => 'Defense-Grade Kintex7 Low Voltage',"
    puts $fileID "    'qkintexu' => 'Defense-Grade Kintex UltraScale',"
    puts $fileID "    'qkintexuplus' => 'Defense-Grade Kintex UltraScale+',"
    puts $fileID "    'qvirtex7' => 'Defense-Grade Virtex7',"
    puts $fileID "    'qvirtexuplus' => 'Defense-Grade Virtex UltraScale+',"
    puts $fileID "    'qzynq' => 'Defense-Grade Zynq',"
    puts $fileID "    'qzynquplus' => 'Defense-Grade Zynq UltraScale+',"
    puts $fileID "    'qzynquplusRFSOC' => 'Defense-Grade Zynq UltraScale+ RFSoC',"
    puts $fileID "    'spartan7' => 'Spartan7',"
    puts $fileID "    'virtex7' => 'Virtex7',"
    puts $fileID "    'virtexu' => 'Virtex UltraScale',"
    puts $fileID "    'virtexuplus' => 'Virtex UltraScale+',"
    puts $fileID "    'virtexuplusHBMes1' => 'Virtex UltraScale+ HBM',"
    puts $fileID "    'virtexuplus58ges1' => 'Virtex UltraScale+ 58G',"
    puts $fileID "    'zynq' => 'Zynq',"
    puts $fileID "    'zynquplus' => 'Zynq UltraScale+',"
    puts $fileID "    'zynquplusRFSOC' => 'Zynq UltraScale+ RFSoC',"
    puts $fileID "  },"
    puts $fileID "  'Version' => '$sw_version',"

    if { [ catch { set boards [dsp_get_all_boards] } err ] } { }
    puts $fileID "  'BoardParts' => {"
    foreach board $boards {
        dsp_write_board_data $board $fileID
    }
    puts $fileID "  },"
    puts $fileID "}"
    close $fileID
}

if { $argc == 1} {
   set file_name [lindex $argv 0]
   main $file_name
} else {
   error "Please enter only one argument i.e. filename"
   exit
}

exit
