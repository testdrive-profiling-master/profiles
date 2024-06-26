# Introduction

'@<i>codegen@</i>' supports the following functions.

* Microsft WORD/Excel manipulation
* Git manipulation
* Lua code encryption & decription

 
## Libraries in use

* Lua 5.4.6 (@<link:https://lua.org>)
** Lua basic libraries (@<link:https://www.tutorialspoint.com/lua/lua_standard_libraries.htm>)
** LuaFileSystem (@<link:https://lunarmodules.github.io/luafilesystem>)
** hook.lua (@<link:https://github.com/vallentin/hook.lua>)
* LuaBridge 3.0 (@<link:https://github.com/kunitoki/LuaBridge3>)
* minizip (@<link:https://github.com/zlib-ng/minizip-ng>)
* SSL(@<link:https://github.com/openssl/openssl>)
* libgit2 (@<link:https://github.com/libgit2/libgit2>
* libz (@<link:https://www.zlib.net>)
* argtable3 (@<link:https://github.com/argtable/argtable3>)
* pugixml (@<link:https://github.com/zeux/pugixml>)
* ncurses (@<link:https://invisible-island.net/ncurses>)
* qrencode (@<link:https://github.com/fukuchi/libqrencode>)
* Clipper2 (@<link:https://github.com/AngusJohnson/Clipper2>)

 
## Command line

``` txt
> codegen

Code generator for TestDrive Profiling Master. v1.0 (build #166 date : Jun 20 2024)
Usage: codegen  [--help] [-e|--encrypt] [-d|--decrypt] [--trace] [-k|--key=key_code] lua_file ...

      --help                display this help and exit
  -e, --encrypt             Encrypt Lua script file
  -d, --decrypt             Decrypt Lua script file
      --trace               Enable traceback on Lua script
  -k, --key=key_code        Set key code for Encrypt/Decrypt
  lua_file                  Lua script file
```

 
### Run a Lua file

단순하게 lua 파일을 실행하려면, 아래와 같은 명령어로 실행한다.
```txt
> codegen lua_file
```

단 encypted 된 lua 파일의 경우 암호 키를 입력해야 실행할 수 있습니다.
```txt
> codegen -k key_code lua_file
```

또한 lua_file 에 argument 를 전달하려면, lua 파일명 뒤에 연달아 기술되어야 합니다.
```txt
> codegen lua_file arguments ...
```
 
### Encrypt Lua file

특정 키값으로 lua 파일을 암호하려면, 아래와 같은 명령어로 실행합니다.
```txt
> codegen -k key_code -e lua_file
```

 
### Decrypt Lua file

암호화된 lua 파일을 다시 암호를 해제하려면, 아래와 같은 명령어로 실행합니다.
```txt
> codegen -k key_code -d lua_file
```

 
## License

The source code implemented in codegen complies with the BSD license, \
and individual scripts or derivative works created by users are entirely owned by the user.
