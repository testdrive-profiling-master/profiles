system.clear

system.msg "Bake current H/W model.\n"
system.call	".\\bake_hw.bat", "", "%PROJECT%System\\HDL\\", "Error:", "*E: ", "*I: ":0, "*W: ":W


system.msg "\n�� H/W bake sequence is completed!\n"