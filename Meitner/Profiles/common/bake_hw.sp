system.clear

system.msg "Bake current H/W model.\n"
system.call	".\\bake_hw.bat", "", "..\\..\\System\\HDL\\", "Error:", "*E: ", "*I: ":0


system.msg "\n¢Â H/W bake sequence is completed!\n"