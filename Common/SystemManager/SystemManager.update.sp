#lua
System.Execute("mingw32-make", "clean", ".\\Document\\")
System.Execute("mingw32-make", "-j dep", ".\\Document\\", {"error:", "] Error", "undefined reference to ", {"Build succeeded.",0}})
System.Execute("mingw32-make", "-j", ".\\Document\\", {"error:", "] Error", "undefined reference to ", {"Build succeeded.",0}})
