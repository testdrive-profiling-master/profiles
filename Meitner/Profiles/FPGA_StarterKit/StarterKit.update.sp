#lua
System.Execute("make", "clean", ".\\Document\\")
System.Execute("make", "-j", ".\\Document\\", {"error:", "] Error", "undefined reference to ", {"Build succeeded.",0}})
