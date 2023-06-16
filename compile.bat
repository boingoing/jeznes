@echo off

set name="jeznes"

set famitone_home=..\famitone5.0\
set nsf2data_home=%famitone_home%\nsf2data
set text2data_home=%famitone_home%\text2data
set CC65_HOME=..\cc65\bin\
set path=%path%;%CC65_HOME%;%nsf2data_home%;%text2data_home%

nsf2data5.exe music/sfx.nsf -ca65
cp music/sfx.s src/music/sfx.s

text2vol5.exe music/music.txt -ca65
cp music/music.s src/music/music.s

cc65 -Oirs src/%name%.c -o build/%name%.s -g --add-source
ca65 src/crt0.s -o build/crt0.o -g
ca65 build/%name%.s -o build/%name%.o -g

ld65 -C nrom_32k_vert.cfg -o build/%name%.nes build/crt0.o build/%name%.o nes.lib -Ln build/%name%.labels.txt --dbgfile build/%name%.dbg -v
