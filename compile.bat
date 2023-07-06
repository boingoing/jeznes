@echo off

setlocal

set name=jeznes

set CC65_HOME=..\cc65\bin\
set path=%path%;%CC65_HOME%

if not exist build mkdir build

cc65 src/%name%.c -o build/%name%.s -Oirs --target nes --debug-info --add-source
ca65 src/crt0.s -o build/crt0.o --target nes -g
ca65 build/%name%.s -o build/%name%.o --target nes -g

ld65 build/crt0.o build/%name%.o nes.lib -o build/%name%.nes -C nrom_32k_vert.cfg -Ln build/%name%.labels.txt --dbgfile build/%name%.dbg
