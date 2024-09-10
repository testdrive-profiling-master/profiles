# Global functions

@<tbl:media/tables.xlsx;Globals>

 
## Require OS & Lua libraries

### Require OS package library

Checks the OS libraries or packages installed in OS and ensures that they are installed automatically if not installed.
Implement as follows using the [@<code>os.require@</code>] function.

```lua
os.require("mingw-w64-ucrt-x86_64-opencv")	-- == os.require_library("opencv")
os.require("git")	-- Verify installation of "git", and try to install if not
```
:::Note
The library you install must be the UCRT(Universal C RunTime) version. \
If you use the [@<code>os.require_library@</code>] function, prefix "mingw-w64-ucrt-x86_64-" is automatically added to the library name.

 
If you want to find the package name to install, search with [@<code>pacman -Sl@</code>] command.

```bash
> pacman -Sl | grep opencv
clangarm64 mingw-w64-clang-aarch64-gst-plugin-opencv 1.24.5-3
clangarm64 mingw-w64-clang-aarch64-opencv 4.10.0-2
clangarm64 mingw-w64-clang-aarch64-python-opencv 4.10.0-2
mingw64 mingw-w64-x86_64-gst-plugin-opencv 1.24.5-3
mingw64 mingw-w64-x86_64-opencv 4.10.0-2
mingw64 mingw-w64-x86_64-python-opencv 4.10.0-2
ucrt64 mingw-w64-ucrt-x86_64-gst-plugin-opencv 1.24.5-3
ucrt64 mingw-w64-ucrt-x86_64-opencv 4.10.0-2 [installed]
ucrt64 mingw-w64-ucrt-x86_64-python-opencv 4.10.0-2 [installed]
clang64 mingw-w64-clang-x86_64-gst-plugin-opencv 1.24.5-3
clang64 mingw-w64-clang-x86_64-opencv 4.10.0-2
clang64 mingw-w64-clang-x86_64-python-opencv 4.10.0-2
```

 
### Require Lua package library

Checks the module packages installed in Lua and ensures that they are installed automatically if not installed.
For this, you can use a modified [@<code>require@</code>] function.
When using the Lua original [@<code>require@</code>] function, the following error may occur if the requested package does not exist.
@<b>[Source : test.lua]@</b>
```lua
local socket = require("socket")
```
 
@<b>[Console]@</b>
```bash
> codegen test.lua
module 'socket' not found:
        no field package.preload['socket']
        no file 'D:/Project/TestDrive/bin/msys64/ucrt64\share\lua\5.4\socket.lua'
        no file 'D:/Project/TestDrive/bin/msys64/ucrt64\share\lua\5.4\socket\init.lua'
        no file 'D:/Project/TestDrive/bin/msys64/ucrt64\lib\lua\5.4\socket.lua'
        no file 'D:/Project/TestDrive/bin/msys64/ucrt64\lib\lua\5.4\socket\init.lua'
        no file '.\socket.lua'
        no file '.\socket\init.lua'
        no file 'D:/Project/Profiles/Common/bin/codegen/lib/socket.lua'
        no file 'D:/Project/TestDrive/bin/msys64/ucrt64\lib\lua\5.4\socket.dll'
        no file 'D:/Project/TestDrive/bin/msys64/ucrt64\lib\lua\5.4\loadall.dll'
        no file '.\socket.dll'
        no file 'D:/Project/Profiles/Common/bin/codegen/lib/socket.dll'
```
 
If you specify the package name in the second parameter of [@<code>require@</code>] function, \
it will be automatically installed as follows if it does not exist.
@<b>[Source : test.lua]@</b>
```lua
local socket = require("socket", "luasocket")
```
:::Note
The module name and its package name may be different.
 
@<b>[Console]@</b>
```bash
> codegen a.lua
*I: Try to install required 'luasocket' Lua package, please wait... Done.
```
 
To find the installable Lua package name, search with the [@<code>codegen lib search 'PACKAGE_NAME'@</code>] command.
```bash
> codegen lib search luasocket
Warning: falling back to wget - install luasec to get native HTTPS support

luasocket - Search results for Lua 5.4:
=======================================


Rockspecs and source rocks:
---------------------------

lua-resty-luasocket
   1.1.2-1 (rockspec) - https://luarocks.org
   1.1.2-1 (src) - https://luarocks.org
   1.1.1-1 (rockspec) - https://luarocks.org
   1.1.1-1 (src) - https://luarocks.org
   1.1.0-1 (rockspec) - https://luarocks.org
   1.1.0-1 (src) - https://luarocks.org
   1.0.1-1 (rockspec) - https://luarocks.org
   1.0.1-1 (src) - https://luarocks.org
   1.0.0-1 (rockspec) - https://luarocks.org
   1.0.0-1 (src) - https://luarocks.org

luasocket
   3.1.0-1 (rockspec) - https://luarocks.org
   3.1.0-1 (src) - https://luarocks.org
   3.0.0-1 (rockspec) - https://luarocks.org
   3.0.0-1 (src) - https://luarocks.org
   3.0rc1-2 (rockspec) - https://luarocks.org
   3.0rc1-2 (src) - https://luarocks.org
   3.0rc1-1 (rockspec) - https://luarocks.org
   3.0rc1-1 (src) - https://luarocks.org
   2.0.2-4 (rockspec) - https://luarocks.org
   2.0.2-4 (src) - https://luarocks.org
   2.0.2-3 (rockspec) - https://luarocks.org
   2.0.2-3 (src) - https://luarocks.org
   2.0.2-2 (rockspec) - https://luarocks.org
   2.0.2-2 (src) - https://luarocks.org
   2.0.2-1 (rockspec) - https://luarocks.org
   2.0.2-1 (src) - https://luarocks.org
   2.0.1-3 (rockspec) - https://luarocks.org
   2.0.1-3 (src) - https://luarocks.org
   2.0.1-2 (rockspec) - https://luarocks.org
   2.0.1-2 (src) - https://luarocks.org

luasocket-lanes
   3.0-3 (rockspec) - https://luarocks.org
   3.0-3 (src) - https://luarocks.org
   3.0-1 (rockspec) - https://luarocks.org
   3.0-1 (src) - https://luarocks.org

luasocket-unix
   2.0.2-2 (rockspec) - https://luarocks.org
   2.0.2-2 (src) - https://luarocks.org
   2.0.2-1 (rockspec) - https://luarocks.org
   2.0.2-1 (src) - https://luarocks.org
```
:::Note
To see the names of all installable Lua packages, use the [@<code>codegen lib search --all@</code>] command.