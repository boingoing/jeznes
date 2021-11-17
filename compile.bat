@echo off

set name="jeznes"

set CC65_HOME=..\cc65\bin\
set path=%path%;%CC65_HOME%

cc65 -Oirs %name%.c --add-source
ca65 crt0.s
ca65 %name%.s -g

ld65 -C nrom_32k_vert.cfg -o %name%.nes crt0.o %name%.o nes.lib -Ln labels.txt --dbgfile %name%.dbg

move /Y labels.txt BUILD\ 1>NUL
move /Y %name%.s BUILD\ 1>NUL
move /Y %name%.nes BUILD\ 1>NUL
move /Y %name%.dbg BUILD\ 1>NUL
move /Y *.o BUILD\ 1>NUL

rem pause
rem BUILD\%name%.nes
