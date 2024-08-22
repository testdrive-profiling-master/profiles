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
* LuaRocks package manager (@<link:https://luarocks.org>)
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

```txt
> codegen

Code generator for TestDrive Profiling Master. v1.0 (build #190 date : Jul 29 2024)
Usage: codegen  [--help] [-e|--encrypt] [-d|--decrypt] [--trace] [-k|--key=key_code] lua_file ...

      --help                display this help and exit
  -e, --encrypt             Encrypt Lua script file
  -d, --decrypt             Decrypt Lua script file
      --trace               Enable traceback on Lua script
  -k, --key=key_code        Set key code for Encrypt/Decrypt
  lua_file                  Lua script file
```

 
### Run a Lua file

To simply run the lua file, run the following command.
```txt
> codegen lua_file
```
 
However, in the case of an encrypted lua file, you must enter the encryption key to run it.
```txt
> codegen -k key_code lua_file
```
or you have to register the private_key for encryption as like below.
```txt
> TestDrive_LM register [private_key]
```

 
And, to pass an argument to lua_file, it must be written after the lua file name.
```txt
> codegen lua_file arguments ...
```

 
### Encrypt Lua file

To encrypt a lua file with a specific key value, run the command below.
```txt
> codegen -k key_code -e lua_file
```

 
### Decrypt Lua file

To decrypt the encrypted lua file again, run the command below.
```txt
> codegen -k key_code -d lua_file
```
 
:::NoteHeading
However, if the password begins with the '@~' character, decryption is not possible and only execution is allowed.

 
## Package managing

codegen provides the LuaRocks environment. \
You can enter luarocks directly as a command, \
or use the [@<code>codegen lib@</code>] command. Additionally, \
a modified [@<code>require@</code>] Lua fuction is provided to enable automatic installation.

:::NoteHeading
For more information, see '@<bookmark:@Require Lua package library>'.

 
## License

The source code implemented in codegen complies with the BSD license, \
and individual scripts or derivative works created by users are entirely owned by the user.
