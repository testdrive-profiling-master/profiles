system.msg "\t- System HAL : "
system.call	"mingw32-make", "-j dep", ".\\SystemHAL\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0
system.call	"mingw32-make", "-j", ".\\SystemHAL\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0
system.msg "\t- System DDK : "
system.call	"mingw32-make", "-j dep", ".\\SystemDDK\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0
system.call	"mingw32-make", "-j", ".\\SystemDDK\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0
