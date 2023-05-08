@echo off

set name="jeznes"

set CC65_HOME=..\cc65\bin\
set path=%path%;%CC65_HOME%

cc65 -Oirs src/%name%.c -o build/%name%.s -g --add-source
ca65 src/crt0.s -o build/crt0.o -g
ca65 build/%name%.s -o build/%name%.o -g

ld65 -C nrom_32k_vert.cfg -o build/%name%.nes build/crt0.o build/%name%.o nes.lib -Ln build/%name%.labels.txt --dbgfile build/%name%.dbg
