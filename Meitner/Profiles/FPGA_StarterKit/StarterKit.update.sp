#lua
System.Execute("mingw32-make", "clean", ".\\Document\\")
System.Execute("mingw32-make", "-j", ".\\Document\\", {"error:", "] Error", "undefined reference to ", {"Build succeeded.",0}})
