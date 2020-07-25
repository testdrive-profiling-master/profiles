system.msg "\t- System HAL : "
system.call	"mingw32-make", "-j", ".\\SystemHAL\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0
system.msg "\t- System DDK : "
system.call	"mingw32-make", "-j", ".\\SystemDDK\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0
