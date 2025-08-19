#lua
LOGI("\t- System HAL : ")
System.Execute("mingw32-make", "-j dep", ".\\SystemHAL\\", {"error:", "] Error", "undefined reference to ", {"Build succeeded.",0}})
System.Execute("mingw32-make", "-j", ".\\SystemHAL\\", {"error:", "] Error", "undefined reference to ", {"Build succeeded.",0}})
LOGI("\t- System DDK : ")
System.Execute("mingw32-make", "-j dep", ".\\SystemDDK\\", {"error:", "] Error", "undefined reference to ", {"Build succeeded.",0}})
System.Execute("mingw32-make", "-j", ".\\SystemDDK\\", {"error:", "] Error", "undefined reference to ", {"Build succeeded.",0}})
