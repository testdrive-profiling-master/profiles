#lua
LOG_CLEAR()

LOGI("Bake current H/W model.")
System.Execute(".\\bake_hw.bat", "", "%PROJECT%System\\HDL\\", {"Error:", "*E: ", {"*I: ",0}, {"*W: ", "warning"}})

LOGI("¢Â H/W bake sequence is completed!")
