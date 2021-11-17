@echo off

set name="jeznes"

set CC65_HOME=..\cc65\bin\
set path=%path%;%CC65_HOME%

cc65 -Oirs %name%.c --add-source
ca65 crt0.s
ca65 %name%.s -g

ld65 -C nrom_32k_vert.cfg -o %name%.nes crt0.o %name%.o nes.lib -Ln labels.txt --dbgfile %name%.dbg

del *.o

move /Y labels.txt BUILD\
move /Y %name%.s BUILD\
move /Y %name%.nes BUILD\
move /Y %name%.dbg BUILD\

rem pause
rem BUILD\%name%.nes
