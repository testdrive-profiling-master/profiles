@echo off
@echo Encryption... [sample.lua to encrypted.lua]
rm -f encrypted.lua
cp sample.lua encrypted.lua
codegen --encrypt --key="no secret" encrypted.lua

@echo Decryption... [encrypted.lua to decrypted.lua]
rm -f decrypted.lua
cp encrypted.lua decrypted.lua
codegen --decrypt --key="no secret" decrypted.lua
pause