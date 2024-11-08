@echo off
echo -----------------------------------------
echo -- arguments show
echo -----------------------------------------
codegen script.lua

echo -----------------------------------------
echo -- arguments test
echo -----------------------------------------
codegen script.lua -p -f a.c b.c
