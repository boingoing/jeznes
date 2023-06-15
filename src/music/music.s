;this file for FamiTone5 library generated by text2vol5 tool
;edited by Doug Fraker, 2019, to add volume column, all notes, 
;duty envelopes, and effects 1xx,2xx,3xx,4xx,Qxx,Rxx


music_music_data:
	.byte 2
	.word @instruments
	.word @samples-3
	.word @song0ch0,@song0ch1,@song0ch2,@song0ch3,@song0ch4,307,256 ; blues
	.word @song1ch0,@song1ch1,@song1ch2,@song1ch3,@song1ch4,307,256 ; jazz

@instruments:
	.word @env1,@env0,@env0,@env8
	.word @env2,@env0,@env0,@env9
	.word @env3,@env0,@env0,@env0
	.word @env4,@env0,@env0,@env0
	.word @env5,@env7,@env0,@env9
	.word @env6,@env0,@env0,@env0
	.word @env1,@env0,@env3,@env8
	.word @env1,@env0,@env3,@env8
	.word @env1,@env0,@env3,@env8
	.word @env1,@env0,@env3,@env8
	.word @env1,@env0,@env3,@env8
	.word @env1,@env0,@env3,@env8
	.word @env1,@env0,@env3,@env8
	.word @env1,@env0,@env3,@env8
	.word @env1,@env0,@env3,@env8
	.word @env1,@env0,@env3,@env8
	.word @env1,@env0,@env3,@env8

@samples:
@env0:
	.byte $c0,$00,$00
@env1:
	.byte $ca,$00,$00
@env2:
	.byte $c8,$cc,$cd,$00,$02
@env3:
	.byte $cf,$00,$00
@env4:
	.byte $ce,$cc,$c9,$c0,$c0,$c0,$00,$05
@env5:
	.byte $cf,$cf,$ce,$cd,$ca,$c6,$c0,$00,$06
@env6:
	.byte $ce,$ce,$cd,$ca,$c6,$c0,$c0,$c0,$00,$07
@env7:
	.byte $c0,$bf,$c0,$00,$02
@env8:
	.byte $c2,$00,$00
@env9:
	.byte $c1,$00,$00


; blues
@song0ch0:
	.byte $fb,$02
@song0ch0loop:
@ref0:
	.byte $5f,$82,$00,$8d,$79,$82,$24,$85,$8c,$25,$8d,$82,$20,$85,$8c,$21
	.byte $8d,$27,$85,$00,$8d,$23,$85,$24,$8d,$82,$20,$85,$8c,$21,$8d,$28
	.byte $85,$00,$8d,$82,$29,$85,$8c,$2a,$8d,$82,$2d,$85
@ref1:
	.byte $79,$00,$8d,$2b,$85,$8c,$2a,$8d,$82,$24,$85,$8c,$25,$8d,$82,$27
	.byte $85,$8c,$28,$85,$25,$85,$20,$85,$21,$8d,$82,$1f,$85,$8c,$1e,$8d
	.byte $1b,$a5,$6c,$10,$8f
@ref2:
	.byte $79,$00,$8d,$5f,$82,$23,$85,$8c,$24,$8d,$82,$20,$85,$8c,$21,$8d
	.byte $27,$85,$00,$8d,$82,$29,$85,$8c,$2a,$8d,$82,$27,$85,$8c,$2a,$8d
	.byte $2d,$85,$00,$8d,$82,$29,$85,$8c,$2a,$8d,$2d,$85
@ref3:
	.byte $79,$2b,$8d,$2a,$85,$24,$8d,$25,$85,$27,$8d,$28,$85,$25,$8d,$82
	.byte $20,$85,$8c,$21,$8d,$82,$1f,$85,$8c,$1e,$8d,$22,$9d,$6c,$05,$8f
	.byte $5f,$28,$85
@ref4:
	.byte $79,$00,$8d,$82,$25,$85,$8c,$26,$8d,$82,$2a,$85,$8c,$2d,$8d,$2a
	.byte $85,$00,$8d,$82,$25,$85,$8c,$26,$8d,$82,$28,$85,$8c,$29,$8d,$2c
	.byte $85,$00,$8d,$82,$29,$85,$8c,$2a,$8d,$2d,$85
@ref5:
	.byte $79,$00,$8d,$82,$2b,$85,$8c,$2a,$8d,$82,$24,$85,$8c,$25,$8d,$82
	.byte $27,$85,$8c,$28,$85,$25,$85,$20,$85,$21,$8d,$82,$1f,$85,$8c,$1e
	.byte $8d,$21,$a5,$6c,$07,$8f
@ref6:
	.byte $5f,$00,$8d,$79,$82,$24,$85,$8c,$25,$8d,$82,$20,$85,$8c,$21,$8d
	.byte $27,$85,$00,$8d,$23,$85,$24,$8d,$82,$20,$85,$8c,$21,$8d,$28,$85
	.byte $00,$8d,$82,$29,$85,$8c,$2a,$8d,$82,$2d,$85
	.byte $ff,$1b
	.word @ref1
	.byte $ff,$20
	.word @ref2
	.byte $ff,$1b
	.word @ref3
	.byte $ff,$20
	.word @ref4
	.byte $ff,$1b
	.word @ref5
	.byte $ff,$20
	.word @ref6
	.byte $ff,$1b
	.word @ref1
	.byte $ff,$20
	.word @ref2
	.byte $ff,$1b
	.word @ref3
	.byte $ff,$20
	.word @ref4
	.byte $ff,$1b
	.word @ref5
@ref18:
	.byte $5f,$00,$f9,$c3
@ref19:
	.byte $5f,$00,$f9,$c3
@ref20:
	.byte $5f,$00,$f9,$c3
@ref21:
	.byte $5f,$00,$f9,$c3
@ref22:
	.byte $5f,$00,$f9,$c3
@ref23:
	.byte $5f,$00,$f9,$c3
	.byte $fd
	.word @song0ch0loop

; blues
@song0ch1:
@song0ch1loop:
@ref24:
	.byte $7a,$80,$25,$85,$00,$8d,$25,$85,$00,$8d,$25,$85,$00,$8d,$25,$85
	.byte $21,$85,$23,$85,$24,$85,$00,$8d,$24,$85,$00,$8d,$24,$85,$00,$85
	.byte $21,$85,$24,$85,$00,$85,$21,$85
@ref25:
	.byte $7a,$25,$85,$00,$8d,$25,$85,$00,$8d,$25,$85,$00,$8d,$25,$85,$00
	.byte $8d,$25,$85,$00,$8d,$25,$85,$00,$8d,$25,$85,$00,$8d,$25,$85,$00
	.byte $85,$29,$85
@ref26:
	.byte $7a,$2a,$85,$00,$8d,$2a,$85,$00,$8d,$2a,$85,$00,$8d,$2a,$85,$00
	.byte $85,$29,$85,$2a,$85,$00,$8d,$2a,$85,$00,$8d,$2a,$85,$00,$8d,$2a
	.byte $85,$27,$85,$21,$85
@ref27:
	.byte $7a,$25,$85,$00,$8d,$25,$85,$00,$8d,$25,$85,$00,$8d,$25,$85,$00
	.byte $8d,$29,$85,$00,$8d,$29,$85,$00,$8d,$28,$85,$00,$8d,$28,$85,$00
	.byte $8d
@ref28:
	.byte $7a,$26,$85,$00,$8d,$26,$85,$00,$8d,$2a,$85,$00,$8d,$2a,$85,$00
	.byte $85,$25,$85,$26,$85,$00,$8d,$26,$85,$00,$85,$28,$85,$2c,$85,$00
	.byte $85,$28,$85,$2f,$85,$2c,$85,$28,$85
@ref29:
	.byte $7a,$25,$85,$00,$8d,$25,$85,$00,$8d,$28,$85,$00,$8d,$28,$85,$00
	.byte $8d,$24,$85,$00,$8d,$24,$85,$00,$8d,$26,$85,$00,$8d,$26,$85,$23
	.byte $85,$20,$85
@ref30:
	.byte $7a,$25,$85,$00,$8d,$25,$85,$00,$8d,$25,$85,$00,$8d,$25,$85,$21
	.byte $85,$23,$85,$24,$85,$00,$8d,$24,$85,$00,$8d,$24,$85,$00,$85,$21
	.byte $85,$24,$85,$00,$85,$21,$85
	.byte $ff,$22
	.word @ref25
	.byte $ff,$24
	.word @ref26
	.byte $ff,$20
	.word @ref27
	.byte $ff,$28
	.word @ref28
	.byte $ff,$22
	.word @ref29
	.byte $ff,$26
	.word @ref30
	.byte $ff,$22
	.word @ref25
	.byte $ff,$24
	.word @ref26
	.byte $ff,$20
	.word @ref27
	.byte $ff,$28
	.word @ref28
	.byte $ff,$22
	.word @ref29
	.byte $ff,$26
	.word @ref30
	.byte $ff,$22
	.word @ref25
	.byte $ff,$24
	.word @ref26
	.byte $ff,$20
	.word @ref27
	.byte $ff,$28
	.word @ref28
	.byte $ff,$22
	.word @ref29
	.byte $fd
	.word @song0ch1loop

; blues
@song0ch2:
@song0ch2loop:
@ref48:
	.byte $84,$21,$85,$00,$8d,$21,$85,$00,$8d,$21,$85,$00,$8d,$21,$85,$1f
	.byte $85,$25,$85,$26,$85,$00,$8d,$26,$85,$00,$8d,$27,$85,$00,$8d,$27
	.byte $85,$26,$85,$27,$85
@ref49:
	.byte $28,$85,$25,$85,$1f,$85,$21,$85,$00,$8d,$21,$85,$00,$8d,$21,$85
	.byte $00,$8d,$21,$85,$00,$8d,$21,$85,$00,$8d,$27,$85,$00,$85,$21,$85
	.byte $27,$85,$00,$85,$21,$85
@ref50:
	.byte $26,$85,$00,$8d,$26,$85,$00,$8d,$26,$85,$00,$8d,$26,$85,$24,$85
	.byte $26,$85,$27,$85,$00,$8d,$27,$85,$00,$8d,$27,$85,$00,$8d,$27,$85
	.byte $21,$85,$27,$85
@ref51:
	.byte $21,$85,$1f,$85,$20,$85,$21,$85,$00,$8d,$21,$85,$00,$8d,$21,$85
	.byte $00,$8d,$25,$85,$00,$8d,$25,$85,$20,$85,$29,$85,$2a,$85,$00,$8d
	.byte $2a,$85,$25,$85,$22,$85
@ref52:
	.byte $23,$85,$1e,$85,$21,$85,$23,$85,$00,$8d,$23,$85,$00,$8d,$23,$85
	.byte $00,$85,$27,$85,$28,$85,$00,$8d,$28,$85,$26,$85,$23,$85,$20,$85
	.byte $00,$85,$1f,$85,$20,$85,$00,$8d
@ref53:
	.byte $21,$85,$00,$8d,$21,$85,$1f,$85,$23,$85,$24,$85,$00,$8d,$24,$85
	.byte $00,$85,$22,$85,$23,$85,$26,$85,$21,$85,$23,$85,$00,$8d,$28,$85
	.byte $00,$8d,$28,$85,$26,$85,$23,$85
@ref54:
	.byte $21,$85,$00,$8d,$21,$85,$00,$8d,$21,$85,$00,$8d,$21,$85,$1f,$85
	.byte $25,$85,$26,$85,$00,$8d,$26,$85,$00,$8d,$27,$85,$00,$8d,$27,$85
	.byte $26,$85,$27,$85
	.byte $ff,$26
	.word @ref49
	.byte $ff,$24
	.word @ref50
	.byte $ff,$26
	.word @ref51
	.byte $ff,$28
	.word @ref52
	.byte $ff,$28
	.word @ref53
	.byte $ff,$24
	.word @ref54
	.byte $ff,$26
	.word @ref49
	.byte $ff,$24
	.word @ref50
	.byte $ff,$26
	.word @ref51
	.byte $ff,$28
	.word @ref52
	.byte $ff,$28
	.word @ref53
	.byte $ff,$24
	.word @ref54
	.byte $ff,$26
	.word @ref49
	.byte $ff,$24
	.word @ref50
	.byte $ff,$26
	.word @ref51
	.byte $ff,$28
	.word @ref52
	.byte $ff,$28
	.word @ref53
	.byte $fd
	.word @song0ch2loop

; blues
@song0ch3:
@song0ch3loop:
@ref72:
	.byte $f9,$c5
@ref73:
	.byte $f9,$c5
@ref74:
	.byte $f9,$c5
@ref75:
	.byte $f9,$c5
@ref76:
	.byte $74,$88,$01,$95,$74,$01,$95,$75,$01,$95,$75,$01,$95,$75,$01,$95
	.byte $76,$01,$95,$76,$01,$95,$76,$01,$95
@ref77:
	.byte $77,$01,$95,$77,$01,$95,$77,$01,$95,$78,$01,$95,$78,$01,$95,$78
	.byte $01,$95,$79,$01,$95,$79,$01,$8d,$79,$8a,$05,$85
@ref78:
	.byte $79,$88,$01,$8d,$01,$85,$8a,$05,$8d,$88,$01,$85,$01,$95,$8a,$05
	.byte $8d,$88,$01,$85,$01,$8d,$01,$85,$8a,$05,$8d,$88,$01,$95,$01,$85
	.byte $8a,$05,$8d,$88,$01,$85
@ref79:
	.byte $8f,$01,$85,$8a,$05,$8d,$88,$01,$85,$01,$8d,$01,$85,$8a,$05,$85
	.byte $88,$10,$85,$78,$02,$85,$79,$01,$8d,$01,$85,$8a,$05,$8d,$88,$01
	.byte $95,$01,$85,$8a,$05,$8d,$88,$01,$85
@ref80:
	.byte $79,$01,$8d,$01,$85,$8a,$05,$8d,$88,$01,$85,$01,$95,$8a,$05,$8d
	.byte $88,$01,$85,$01,$8d,$01,$85,$8a,$05,$8d,$88,$01,$95,$01,$85,$8a
	.byte $05,$8d,$88,$01,$85
@ref81:
	.byte $79,$01,$8d,$01,$85,$8a,$05,$8d,$88,$01,$85,$01,$8d,$01,$85,$8a
	.byte $05,$8d,$88,$01,$85,$01,$8d,$01,$85,$8a,$05,$8d,$88,$01,$95,$01
	.byte $85,$8a,$05,$8d,$88,$01,$85
@ref82:
	.byte $8f,$01,$85,$8a,$05,$8d,$88,$01,$85,$01,$95,$8a,$05,$8d,$88,$01
	.byte $85,$01,$8d,$01,$85,$8a,$05,$8d,$88,$01,$95,$01,$85,$8a,$05,$8d
	.byte $88,$01,$85
@ref83:
	.byte $79,$8f,$01,$85,$8a,$05,$8d,$88,$01,$85,$01,$8d,$01,$85,$8a,$05
	.byte $85,$88,$01,$85,$01,$85,$01,$8d,$01,$85,$8a,$05,$8d,$88,$01,$95
	.byte $77,$86,$09,$85,$79,$88,$01,$8d,$8a,$05,$85
@ref84:
	.byte $79,$88,$01,$8d,$8a,$05,$85,$75,$86,$09,$8d,$79,$88,$01,$85,$8a
	.byte $05,$8d,$88,$01,$85,$75,$01,$85,$86,$09,$81,$72,$09,$71,$81,$75
	.byte $09,$81,$71,$09,$81,$79,$88,$01,$8d,$8a,$05,$85,$88,$01,$8d,$75
	.byte $86,$09,$81,$71,$09,$81,$79,$8a,$05,$95,$88,$01,$8d,$8a,$05,$85
@ref85:
	.byte $79,$88,$01,$8d,$75,$86,$09,$81,$72,$09,$81,$79,$88,$01,$8d,$01
	.byte $85,$8a,$05,$8d,$75,$86,$09,$81,$72,$09,$81,$79,$88,$01,$8d,$8a
	.byte $05,$85,$88,$01,$8d,$01,$85,$8a,$05,$8d,$88,$01,$85,$75,$01,$85
	.byte $86,$09,$81,$72,$09,$81,$79,$88,$01,$85,$8a,$05,$8d,$88,$01,$85
@ref86:
	.byte $79,$01,$85,$75,$86,$09,$81,$72,$09,$81,$79,$8a,$05,$85,$75,$86
	.byte $09,$8d,$79,$88,$01,$85,$8a,$05,$8d,$88,$01,$85,$75,$01,$85,$86
	.byte $09,$81,$72,$09,$81,$79,$09,$85,$88,$01,$8d,$8a,$05,$85,$88,$01
	.byte $8d,$75,$86,$09,$81,$72,$09,$81,$79,$8a,$05,$95,$88,$01,$8d,$8a
	.byte $05,$85
	.byte $ff,$28
	.word @ref85
@ref88:
	.byte $79,$01,$8d,$8a,$05,$85,$75,$86,$09,$8d,$79,$88,$01,$85,$8a,$05
	.byte $8d,$88,$01,$85,$75,$01,$85,$86,$09,$81,$72,$09,$71,$81,$75,$09
	.byte $81,$71,$09,$81,$79,$88,$01,$8d,$8a,$05,$85,$88,$01,$8d,$75,$86
	.byte $09,$81,$71,$09,$81,$79,$8a,$05,$95,$88,$01,$8d,$8a,$05,$85
	.byte $ff,$26
	.word @ref84
	.byte $ff,$1c
	.word @ref78
	.byte $ff,$1f
	.word @ref79
	.byte $ff,$1c
	.word @ref80
	.byte $ff,$1e
	.word @ref81
	.byte $ff,$1b
	.word @ref82
@ref95:
	.byte $79,$8f,$01,$85,$8a,$05,$8d,$88,$01,$85,$01,$8d,$01,$85,$8a,$05
	.byte $85,$88,$01,$85,$01,$85,$01,$8d,$01,$85,$8a,$05,$8d,$88,$01,$95
	.byte $79,$01,$85,$8a,$05,$8d,$05,$81,$76,$86,$0d,$81
	.byte $fd
	.word @song0ch3loop

; blues
@song0ch4:
@song0ch4loop:
@ref96:
	.byte $f9,$c5
@ref97:
	.byte $f9,$c5
@ref98:
	.byte $f9,$c5
@ref99:
	.byte $f9,$c5
@ref100:
	.byte $f9,$c5
@ref101:
	.byte $f9,$c5
@ref102:
	.byte $f9,$c5
@ref103:
	.byte $f9,$c5
@ref104:
	.byte $f9,$c5
@ref105:
	.byte $f9,$c5
@ref106:
	.byte $f9,$c5
@ref107:
	.byte $f9,$c5
@ref108:
	.byte $f9,$c5
@ref109:
	.byte $f9,$c5
@ref110:
	.byte $f9,$c5
@ref111:
	.byte $f9,$c5
@ref112:
	.byte $f9,$c5
@ref113:
	.byte $f9,$c5
@ref114:
	.byte $f9,$c5
@ref115:
	.byte $f9,$c5
@ref116:
	.byte $f9,$c5
@ref117:
	.byte $f9,$c5
@ref118:
	.byte $f9,$c5
@ref119:
	.byte $f9,$c5
	.byte $fd
	.word @song0ch4loop


; jazz
@song1ch0:
	.byte $fb,$02
@song1ch0loop:
@ref120:
	.byte $7a,$8e,$21,$89,$00,$89,$21,$89,$00,$89,$28,$a1
@ref121:
	.byte $21,$89,$00,$89,$21,$89,$00,$89,$28,$a1
	.byte $ff,$0a
	.word @ref121
@ref123:
	.byte $21,$89,$00,$89,$21,$89,$00,$89,$7a,$90,$25,$89,$28,$89,$2a,$89
@ref124:
	.byte $7a,$2d,$89,$2a,$95,$79,$97,$78,$97
@ref125:
	.byte $77,$97,$76,$8b,$75,$8b,$74,$8b,$73,$87,$7a,$2f,$85,$2d,$85
@ref126:
	.byte $2a,$85,$2d,$85,$2a,$85,$25,$a1,$79,$97
@ref127:
	.byte $78,$8b,$77,$8b,$76,$8b,$6c,$04,$8b,$7a,$5f,$25,$89,$28,$89,$2a
	.byte $89
@ref128:
	.byte $7a,$2d,$89,$2f,$89,$2d,$89,$2f,$95,$2d,$95
@ref129:
	.byte $2f,$95,$79,$8b,$6c,$04,$8b,$7a,$5f,$2d,$89,$2a,$89,$23,$89
@ref130:
	.byte $25,$95,$79,$97,$78,$97,$77,$8b
@ref131:
	.byte $8b,$76,$97,$75,$97,$74,$8f,$73,$83,$72,$81,$71,$81
@ref132:
	.byte $7a,$34,$8d,$2f,$85,$78,$81,$77,$81,$76,$81,$74,$81,$7a,$9e,$2f
	.byte $85,$78,$81,$77,$81,$76,$81,$74,$81,$7a,$90,$2f,$95,$79,$8b
@ref133:
	.byte $8b,$78,$97,$77,$97,$6c,$04,$8b,$7a,$5f,$2f,$89
@ref134:
	.byte $2c,$a1,$25,$ad
@ref135:
	.byte $79,$97,$78,$97,$77,$8b,$76,$85,$75,$85,$7a,$2f,$89
@ref136:
	.byte $7a,$34,$85,$78,$81,$77,$81,$76,$81,$74,$81,$7a,$34,$85,$78,$81
	.byte $77,$81,$76,$81,$74,$81,$7a,$34,$85,$78,$81,$77,$81,$76,$81,$74
	.byte $81,$7a,$34,$a1
@ref137:
	.byte $79,$8b,$78,$8b,$77,$8b,$6c,$04,$8b,$7a,$5f,$2f,$9d,$6c,$04,$83
@ref138:
	.byte $5f,$25,$95,$79,$97,$78,$97,$77,$8b
@ref139:
	.byte $8b,$76,$8b,$75,$8b,$74,$6c,$04,$8b,$7a,$5f,$25,$89,$28,$89,$2a
	.byte $89
@ref140:
	.byte $7a,$2d,$99,$6c,$03,$87,$5f,$2a,$95,$79,$97
@ref141:
	.byte $78,$97,$77,$97,$76,$87,$75,$83,$74,$83,$73,$83,$72,$83,$7a,$5f
	.byte $2f,$89
@ref142:
	.byte $2d,$95,$6c,$04,$8b,$5f,$25,$95,$79,$97
@ref143:
	.byte $78,$97,$77,$8b,$76,$6c,$04,$8b,$7a,$5f,$25,$89,$28,$89,$2a,$89
@ref144:
	.byte $7a,$2d,$8d,$2f,$8d,$2d,$8d,$2f,$8d,$2d,$8d,$2f,$81
@ref145:
	.byte $91,$6b,$01,$81,$6b,$02,$81,$5f,$85,$6c,$02,$81,$6c,$01,$81,$5f
	.byte $81,$5f,$87,$25,$28,$2a,$6b,$01,$2d,$5f,$a1
@ref146:
	.byte $6c,$01,$83,$6c,$02,$8b,$5f,$9e,$2a,$91,$79,$97,$78,$97
@ref147:
	.byte $77,$97,$76,$97,$75,$91,$74,$85,$73,$85,$72,$85
	.byte $ff,$0a
	.word @ref120
	.byte $ff,$0a
	.word @ref121
	.byte $ff,$0a
	.word @ref121
@ref151:
	.byte $21,$89,$00,$89,$21,$89,$00,$89,$25,$a1
@ref152:
	.byte $7a,$21,$89,$76,$9c,$31,$89,$7a,$8e,$21,$89,$00,$89,$28,$a1
@ref153:
	.byte $21,$89,$76,$9c,$31,$89,$7a,$8e,$21,$89,$00,$89,$28,$a1
	.byte $ff,$0a
	.word @ref153
	.byte $ff,$0a
	.word @ref153
	.byte $ff,$0a
	.word @ref153
	.byte $ff,$0a
	.word @ref153
	.byte $ff,$0a
	.word @ref153
@ref159:
	.byte $21,$89,$76,$9c,$31,$89,$7a,$8e,$21,$89,$76,$9c,$2c,$89,$8e,$28
	.byte $a1
@ref160:
	.byte $7a,$24,$89,$76,$9c,$34,$89,$7a,$8e,$24,$89,$76,$9c,$30,$89,$7a
	.byte $9a,$28,$a1
@ref161:
	.byte $7a,$8e,$24,$89,$76,$9c,$34,$89,$7a,$8e,$24,$89,$76,$9c,$30,$89
	.byte $7a,$9a,$28,$a1
@ref162:
	.byte $7a,$8e,$21,$89,$76,$9c,$31,$89,$7a,$8e,$21,$89,$76,$9c,$2c,$89
	.byte $7a,$9a,$28,$a1
	.byte $ff,$0a
	.word @ref162
	.byte $ff,$0a
	.word @ref161
	.byte $ff,$0a
	.word @ref161
	.byte $ff,$0a
	.word @ref162
@ref167:
	.byte $7a,$8e,$21,$89,$76,$9c,$2c,$89,$7a,$8e,$21,$89,$76,$9c,$2d,$89
	.byte $7a,$9a,$29,$a1
@ref168:
	.byte $7a,$8e,$21,$89,$78,$9c,$31,$89,$7a,$8e,$21,$89,$00,$89,$7a,$28
	.byte $a1
@ref169:
	.byte $21,$89,$00,$89,$7a,$21,$89,$78,$9c,$31,$89,$7a,$8e,$28,$a1
@ref170:
	.byte $7a,$21,$89,$78,$9c,$31,$89,$7a,$8e,$21,$89,$00,$89,$7a,$28,$a1
@ref171:
	.byte $79,$21,$89,$00,$89,$79,$21,$89,$77,$9c,$31,$89,$79,$8e,$28,$a1
@ref172:
	.byte $78,$21,$89,$76,$9c,$31,$89,$78,$8e,$21,$89,$00,$89,$78,$28,$a1
@ref173:
	.byte $77,$21,$89,$00,$89,$77,$21,$89,$75,$9c,$31,$89,$77,$8e,$28,$a1
@ref174:
	.byte $76,$21,$89,$74,$9c,$31,$89,$76,$8e,$21,$89,$00,$89,$76,$28,$a1
@ref175:
	.byte $75,$21,$89,$00,$89,$75,$21,$89,$73,$9c,$31,$89,$74,$8e,$28,$89
	.byte $00,$95
@ref176:
	.byte $00,$89,$72,$9c,$34,$95,$72,$30,$ad
@ref177:
	.byte $8b,$73,$34,$95,$73,$30,$ad
@ref178:
	.byte $8b,$74,$31,$95,$74,$2c,$ad
@ref179:
	.byte $8b,$75,$31,$95,$75,$2c,$ad
@ref180:
	.byte $8b,$76,$34,$95,$76,$30,$ad
@ref181:
	.byte $8b,$77,$34,$95,$77,$30,$ad
@ref182:
	.byte $8b,$78,$31,$95,$78,$2c,$ad
@ref183:
	.byte $8b,$78,$2c,$95,$78,$2d,$89,$7c,$8e,$25,$89,$28,$89,$2a,$89
	.byte $ff,$0a
	.word @ref170
	.byte $ff,$0a
	.word @ref169
	.byte $ff,$0a
	.word @ref170
@ref187:
	.byte $7a,$21,$89,$00,$89,$7a,$21,$89,$78,$9c,$31,$89,$7a,$8e,$28,$a1
	.byte $ff,$0a
	.word @ref170
@ref189:
	.byte $7a,$21,$89,$78,$00,$89,$7a,$21,$89,$78,$9c,$31,$89,$7a,$8e,$28
	.byte $a1
@ref190:
	.byte $7a,$21,$89,$78,$9c,$31,$89,$7a,$8e,$21,$89,$78,$00,$89,$7a,$28
	.byte $a1
@ref191:
	.byte $7a,$21,$89,$78,$00,$89,$7a,$21,$89,$78,$9c,$00,$89,$7a,$90,$25
	.byte $89,$28,$89,$2a,$89
	.byte $ff,$06
	.word @ref124
	.byte $ff,$09
	.word @ref125
	.byte $ff,$09
	.word @ref126
	.byte $ff,$0a
	.word @ref127
	.byte $ff,$0a
	.word @ref128
	.byte $ff,$0a
	.word @ref129
	.byte $ff,$05
	.word @ref130
	.byte $ff,$07
	.word @ref131
	.byte $ff,$11
	.word @ref132
	.byte $ff,$06
	.word @ref133
@ref202:
	.byte $2c,$a1,$25,$ad
	.byte $ff,$07
	.word @ref135
	.byte $ff,$14
	.word @ref136
	.byte $ff,$07
	.word @ref137
	.byte $ff,$05
	.word @ref138
	.byte $ff,$0a
	.word @ref139
	.byte $ff,$06
	.word @ref140
	.byte $ff,$09
	.word @ref141
	.byte $ff,$06
	.word @ref142
	.byte $ff,$09
	.word @ref143
	.byte $ff,$0c
	.word @ref144
	.byte $ff,$0d
	.word @ref145
	.byte $ff,$06
	.word @ref146
@ref215:
	.byte $77,$97,$76,$97,$78,$6b,$01,$90,$2d,$5f,$a1
@ref216:
	.byte $6c,$01,$83,$6c,$02,$8b,$5f,$9e,$2a,$91,$77,$97,$76,$97
@ref217:
	.byte $75,$97,$74,$97,$76,$6b,$01,$90,$2d,$5f,$a1
@ref218:
	.byte $6c,$01,$83,$6c,$02,$8b,$5f,$9e,$2a,$c1
@ref219:
	.byte $78,$6c,$3f,$a9,$00,$a7
	.byte $fd
	.word @song1ch0loop

; jazz
@song1ch1:
@song1ch1loop:
@ref220:
	.byte $7f,$8e,$25,$89,$00,$89,$25,$89,$00,$89,$2c,$a1
@ref221:
	.byte $25,$89,$00,$89,$25,$89,$00,$89,$2c,$a1
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
@ref224:
	.byte $7f,$25,$89,$00,$89,$25,$89,$00,$89,$2c,$a1
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
@ref231:
	.byte $25,$89,$00,$89,$25,$89,$00,$89,$26,$a1
@ref232:
	.byte $2b,$89,$00,$89,$2b,$89,$00,$89,$2f,$a1
	.byte $ff,$0a
	.word @ref232
@ref234:
	.byte $28,$89,$00,$89,$28,$89,$00,$89,$2c,$a1
	.byte $ff,$0a
	.word @ref234
	.byte $ff,$0a
	.word @ref232
	.byte $ff,$0a
	.word @ref232
	.byte $ff,$0a
	.word @ref234
@ref239:
	.byte $28,$89,$00,$89,$28,$89,$00,$89,$2d,$a1
	.byte $ff,$0a
	.word @ref224
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref224
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
@ref252:
	.byte $7f,$25,$89,$79,$9c,$38,$89,$7f,$8e,$25,$89,$00,$89,$2c,$a1
@ref253:
	.byte $25,$89,$79,$9c,$38,$89,$7f,$8e,$25,$89,$00,$89,$2c,$a1
	.byte $ff,$0a
	.word @ref253
	.byte $ff,$0a
	.word @ref253
@ref256:
	.byte $25,$89,$7a,$9c,$38,$89,$7f,$8e,$25,$89,$00,$89,$2c,$a1
	.byte $ff,$0a
	.word @ref256
	.byte $ff,$0a
	.word @ref256
@ref259:
	.byte $25,$89,$7a,$9c,$38,$89,$7f,$8e,$25,$89,$7a,$9c,$31,$89,$8e,$26
	.byte $a1
@ref260:
	.byte $7f,$2b,$89,$7a,$9c,$3b,$89,$7f,$8e,$2b,$89,$7a,$9c,$37,$89,$7f
	.byte $9a,$2f,$a1
@ref261:
	.byte $7f,$8e,$2b,$89,$7a,$9c,$3b,$89,$7f,$8e,$2b,$89,$7a,$9c,$37,$89
	.byte $7f,$9a,$2f,$a1
@ref262:
	.byte $7f,$8e,$28,$89,$7a,$9c,$38,$89,$7f,$8e,$28,$89,$7a,$9c,$31,$89
	.byte $7f,$9a,$2c,$a1
	.byte $ff,$0a
	.word @ref262
	.byte $ff,$0a
	.word @ref261
	.byte $ff,$0a
	.word @ref261
	.byte $ff,$0a
	.word @ref262
@ref267:
	.byte $7f,$8e,$28,$89,$7a,$9c,$31,$89,$7f,$8e,$28,$89,$7a,$9c,$31,$89
	.byte $7f,$9a,$2d,$a1
@ref268:
	.byte $7d,$8e,$25,$89,$7a,$9c,$38,$89,$7d,$8e,$25,$89,$00,$89,$7d,$2c
	.byte $a1
@ref269:
	.byte $7c,$25,$89,$00,$89,$7c,$25,$89,$79,$9c,$38,$89,$7c,$8e,$2c,$a1
@ref270:
	.byte $7b,$25,$89,$78,$9c,$38,$89,$7b,$8e,$25,$89,$00,$89,$7b,$2c,$a1
@ref271:
	.byte $7a,$25,$89,$00,$89,$7a,$25,$89,$77,$9c,$38,$89,$7a,$8e,$2c,$a1
@ref272:
	.byte $7b,$25,$89,$76,$9c,$38,$89,$7b,$8e,$25,$89,$00,$89,$7b,$2c,$a1
@ref273:
	.byte $7a,$25,$89,$00,$89,$7a,$25,$89,$75,$9c,$38,$89,$7a,$8e,$2c,$a1
@ref274:
	.byte $79,$25,$89,$74,$9c,$38,$89,$79,$8e,$25,$89,$00,$89,$79,$2c,$a1
@ref275:
	.byte $78,$25,$89,$00,$89,$78,$25,$89,$73,$9c,$38,$89,$77,$8e,$2c,$89
	.byte $00,$95
@ref276:
	.byte $00,$89,$72,$9c,$3b,$95,$72,$37,$ad
@ref277:
	.byte $8b,$73,$3b,$95,$73,$37,$ad
@ref278:
	.byte $8b,$74,$38,$95,$74,$31,$ad
@ref279:
	.byte $8b,$75,$38,$95,$75,$31,$ad
@ref280:
	.byte $8b,$76,$3b,$95,$76,$37,$ad
@ref281:
	.byte $8b,$77,$3b,$95,$77,$37,$ad
@ref282:
	.byte $8b,$78,$38,$95,$78,$31,$ad
@ref283:
	.byte $8b,$79,$31,$95,$79,$31,$89,$7e,$8e,$19,$89,$1c,$89,$1e,$89
@ref284:
	.byte $7f,$25,$89,$7c,$9c,$38,$89,$7f,$8e,$25,$89,$00,$89,$2c,$a1
@ref285:
	.byte $25,$89,$00,$89,$7f,$25,$89,$7c,$9c,$38,$89,$7f,$8e,$2c,$a1
@ref286:
	.byte $25,$89,$7c,$9c,$38,$89,$7f,$8e,$25,$89,$00,$89,$2c,$a1
	.byte $ff,$0a
	.word @ref285
@ref288:
	.byte $7f,$25,$89,$7c,$9c,$38,$89,$7f,$8e,$25,$89,$7c,$00,$89,$7f,$2c
	.byte $a1
@ref289:
	.byte $7f,$25,$89,$7c,$00,$89,$7f,$25,$89,$7c,$9c,$38,$89,$7f,$8e,$2c
	.byte $a1
	.byte $ff,$0a
	.word @ref288
@ref291:
	.byte $7f,$25,$89,$7c,$00,$89,$7f,$25,$89,$7c,$9c,$00,$89,$7f,$8e,$2c
	.byte $a1
	.byte $ff,$0a
	.word @ref224
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref231
	.byte $ff,$0a
	.word @ref232
	.byte $ff,$0a
	.word @ref232
	.byte $ff,$0a
	.word @ref234
	.byte $ff,$0a
	.word @ref234
	.byte $ff,$0a
	.word @ref232
	.byte $ff,$0a
	.word @ref232
	.byte $ff,$0a
	.word @ref234
	.byte $ff,$0a
	.word @ref239
	.byte $ff,$0a
	.word @ref224
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref224
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref221
	.byte $ff,$0a
	.word @ref224
	.byte $ff,$0a
	.word @ref221
@ref318:
	.byte $25,$d1
@ref319:
	.byte $00,$d1
	.byte $fd
	.word @song1ch1loop

; jazz
@song1ch2:
@song1ch2loop:
@ref320:
	.byte $92,$1e,$89,$00,$89,$1e,$89,$00,$89,$25,$a1
@ref321:
	.byte $1e,$89,$00,$89,$1e,$89,$00,$89,$25,$a1
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
@ref331:
	.byte $1e,$89,$00,$89,$1e,$89,$00,$89,$1f,$a1
@ref332:
	.byte $24,$89,$00,$89,$24,$89,$00,$89,$28,$a1
	.byte $ff,$0a
	.word @ref332
	.byte $ff,$0a
	.word @ref151
	.byte $ff,$0a
	.word @ref151
	.byte $ff,$0a
	.word @ref332
	.byte $ff,$0a
	.word @ref332
	.byte $ff,$0a
	.word @ref151
	.byte $ff,$0a
	.word @ref151
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
@ref351:
	.byte $1e,$89,$00,$89,$1e,$89,$00,$89,$19,$89,$1c,$89,$1e,$89
@ref352:
	.byte $21,$89,$1e,$a1,$25,$a1
@ref353:
	.byte $1e,$89,$00,$89,$1e,$89,$00,$89,$25,$91,$23,$85,$21,$85
@ref354:
	.byte $1e,$85,$21,$85,$1e,$85,$19,$95,$25,$a1
	.byte $ff,$0e
	.word @ref351
@ref356:
	.byte $21,$89,$23,$89,$21,$89,$23,$95,$21,$95
@ref357:
	.byte $1e,$89,$00,$89,$1e,$89,$00,$89,$21,$89,$1e,$89,$17,$89
@ref358:
	.byte $19,$95,$1e,$89,$00,$89,$25,$a1
	.byte $ff,$0a
	.word @ref331
@ref360:
	.byte $28,$8d,$23,$85,$24,$85,$23,$87,$00,$83,$23,$a1
@ref361:
	.byte $24,$89,$00,$89,$24,$89,$00,$89,$28,$99,$23,$85
@ref362:
	.byte $20,$95,$21,$89,$1c,$ad
@ref363:
	.byte $21,$89,$00,$89,$21,$89,$a0,$00,$89,$92,$25,$95,$23,$89
@ref364:
	.byte $28,$89,$a0,$00,$81,$92,$28,$85,$24,$85,$28,$87,$00,$83,$28,$a1
@ref365:
	.byte $24,$89,$a0,$00,$89,$92,$24,$89,$a0,$00,$89,$92,$28,$95,$23,$89
@ref366:
	.byte $20,$89,$a0,$00,$89,$92,$21,$89,$1c,$ad
@ref367:
	.byte $21,$89,$a0,$00,$89,$92,$21,$89,$a0,$00,$89,$92,$25,$89,$1c,$89
	.byte $1e,$89
@ref368:
	.byte $21,$89,$a0,$00,$89,$92,$21,$89,$1e,$89,$25,$a1
@ref369:
	.byte $1e,$89,$00,$89,$1e,$89,$a0,$00,$89,$92,$25,$95,$23,$89
@ref370:
	.byte $21,$89,$a0,$00,$89,$92,$21,$89,$19,$89,$25,$a1
@ref371:
	.byte $1e,$89,$00,$89,$1e,$89,$a0,$00,$89,$92,$19,$89,$1c,$89,$1e,$89
@ref372:
	.byte $21,$8d,$23,$8d,$21,$8d,$23,$8d,$21,$91
@ref373:
	.byte $1e,$89,$00,$89,$1e,$89,$a0,$00,$89,$92,$21,$95,$1c,$89
@ref374:
	.byte $1e,$89,$a0,$00,$89,$92,$1e,$89,$00,$89,$25,$a1
@ref375:
	.byte $1e,$89,$00,$89,$1e,$89,$a0,$00,$89,$92,$25,$a1
@ref376:
	.byte $28,$89,$a0,$23,$81,$92,$23,$85,$24,$85,$23,$81,$a0,$1f,$87,$00
	.byte $92,$23,$a1
@ref377:
	.byte $24,$89,$a0,$23,$89,$92,$24,$89,$a0,$1f,$89,$92,$28,$99,$23,$85
@ref378:
	.byte $20,$89,$a0,$20,$89,$92,$21,$89,$1c,$89,$20,$a1
@ref379:
	.byte $21,$89,$a0,$20,$89,$92,$21,$89,$a0,$1b,$89,$92,$25,$95,$23,$89
@ref380:
	.byte $28,$89,$a0,$23,$81,$92,$28,$85,$24,$85,$28,$81,$a0,$1f,$89,$92
	.byte $28,$a1
@ref381:
	.byte $24,$89,$a0,$23,$89,$92,$24,$89,$a0,$1f,$89,$92,$28,$95,$23,$89
@ref382:
	.byte $20,$89,$a0,$20,$89,$92,$21,$89,$1c,$89,$19,$a1
@ref383:
	.byte $21,$89,$a0,$1b,$89,$92,$21,$89,$a0,$1d,$89,$92,$19,$89,$1c,$89
	.byte $1e,$89
@ref384:
	.byte $21,$89,$a0,$20,$89,$92,$21,$89,$1e,$89,$25,$a1
@ref385:
	.byte $1e,$89,$00,$89,$1e,$89,$a0,$20,$89,$92,$25,$95,$23,$89
@ref386:
	.byte $21,$89,$a0,$20,$89,$92,$21,$89,$19,$89,$25,$a1
@ref387:
	.byte $1e,$89,$00,$89,$1e,$89,$a0,$20,$89,$92,$19,$89,$1c,$89,$1e,$89
@ref388:
	.byte $21,$89,$a0,$20,$81,$92,$23,$8d,$21,$8d,$23,$8d,$21,$91
@ref389:
	.byte $1e,$89,$00,$89,$1e,$89,$a0,$20,$89,$92,$21,$95,$1c,$89
@ref390:
	.byte $1e,$89,$a0,$20,$89,$92,$1e,$89,$00,$89,$25,$a1
	.byte $ff,$0a
	.word @ref375
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref331
	.byte $ff,$0a
	.word @ref332
	.byte $ff,$0a
	.word @ref332
	.byte $ff,$0a
	.word @ref151
	.byte $ff,$0a
	.word @ref151
	.byte $ff,$0a
	.word @ref332
	.byte $ff,$0a
	.word @ref332
	.byte $ff,$0a
	.word @ref151
	.byte $ff,$0a
	.word @ref151
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
	.byte $ff,$0a
	.word @ref321
@ref418:
	.byte $1e,$d1
@ref419:
	.byte $00,$d1
	.byte $fd
	.word @song1ch2loop

; jazz
@song1ch3:
@song1ch3loop:
@ref420:
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$7c,$96,$01,$89,$7a,$94,$09,$89
	.byte $98,$05,$89,$94,$09,$89,$98,$05,$89
@ref421:
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$7c,$96,$01,$89,$7a,$94,$09,$89
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$98,$05,$89
	.byte $ff,$0e
	.word @ref420
@ref423:
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$7c,$96,$01,$89,$7a,$94,$09,$89
	.byte $7c,$96,$01,$89,$94,$09,$89,$98,$05,$83,$78,$05,$83
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$0e
	.word @ref421
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$10
	.word @ref423
	.byte $ff,$0e
	.word @ref420
@ref429:
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$7c,$96,$01,$89,$7a,$94,$09,$89
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$7c,$96,$10,$89
@ref430:
	.byte $7c,$01,$89,$7a,$94,$09,$89,$7c,$96,$01,$89,$7a,$94,$09,$89,$98
	.byte $05,$89,$94,$09,$89,$98,$05,$89
@ref431:
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$7c,$96,$01,$83,$7a,$98,$05,$83
	.byte $94,$09,$89,$7c,$96,$01,$89,$7a,$94,$09,$89,$98,$05,$83,$78,$05
	.byte $83
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$0e
	.word @ref421
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$10
	.word @ref423
	.byte $ff,$0e
	.word @ref420
@ref437:
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$7c,$96,$01,$89,$7a,$94,$09,$89
	.byte $7c,$96,$01,$89,$94,$09,$89,$98,$05,$89
	.byte $ff,$0e
	.word @ref420
@ref439:
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$98,$05,$89,$94,$09,$89,$98,$05
	.byte $89,$7c,$96,$01,$83,$02,$89,$78,$98,$05,$83
@ref440:
	.byte $7c,$96,$01,$89,$7a,$94,$09,$83,$7a,$09,$83,$7c,$96,$01,$89,$7a
	.byte $94,$09,$89,$98,$05,$89,$94,$09,$89,$98,$05,$89
@ref441:
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$7c,$96,$01,$89,$7a,$94,$09,$89
	.byte $7c,$96,$01,$83,$78,$98,$05,$83,$7a,$94,$09,$89,$98,$05,$89
	.byte $ff,$0e
	.word @ref420
@ref443:
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$7c,$96,$01,$89,$7a,$94,$09,$89
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$98,$05,$83,$78,$05,$83
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$0e
	.word @ref421
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$10
	.word @ref423
@ref448:
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$7c,$96,$01,$89,$7a,$94,$09,$89
	.byte $98,$05,$89,$94,$09,$89,$7c,$96,$02,$89
@ref449:
	.byte $01,$89,$7a,$94,$09,$89,$7c,$96,$01,$89,$7a,$94,$09,$89,$7c,$96
	.byte $01,$89,$7a,$94,$09,$89,$98,$05,$89
	.byte $ff,$0e
	.word @ref448
@ref451:
	.byte $01,$89,$7a,$94,$09,$89,$7c,$96,$01,$89,$7a,$94,$09,$89,$7c,$96
	.byte $01,$89,$7a,$94,$09,$89,$98,$05,$83,$78,$05,$83
	.byte $ff,$0e
	.word @ref448
	.byte $ff,$0e
	.word @ref449
	.byte $ff,$0e
	.word @ref448
	.byte $ff,$10
	.word @ref451
	.byte $ff,$0e
	.word @ref448
	.byte $ff,$0e
	.word @ref449
	.byte $ff,$0e
	.word @ref448
	.byte $ff,$10
	.word @ref451
	.byte $ff,$0e
	.word @ref448
	.byte $ff,$0e
	.word @ref449
	.byte $ff,$0e
	.word @ref448
	.byte $ff,$10
	.word @ref451
	.byte $ff,$0e
	.word @ref448
	.byte $ff,$0e
	.word @ref449
	.byte $ff,$0e
	.word @ref448
	.byte $ff,$10
	.word @ref451
	.byte $ff,$0e
	.word @ref448
	.byte $ff,$0e
	.word @ref449
	.byte $ff,$0e
	.word @ref448
	.byte $ff,$10
	.word @ref451
	.byte $ff,$0e
	.word @ref448
	.byte $ff,$0e
	.word @ref449
@ref474:
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$7c,$96,$01,$89,$7a,$94,$09,$89
	.byte $79,$98,$05,$89,$94,$09,$89,$7b,$96,$02,$89
@ref475:
	.byte $01,$89,$79,$94,$09,$89,$7b,$96,$01,$89,$79,$94,$09,$89,$7a,$96
	.byte $01,$89,$78,$94,$09,$89,$98,$05,$83,$75,$05,$83
@ref476:
	.byte $79,$96,$01,$89,$77,$94,$09,$89,$79,$96,$01,$89,$77,$94,$09,$89
	.byte $79,$96,$01,$89,$77,$94,$09,$89,$79,$96,$10,$89
@ref477:
	.byte $01,$89,$77,$94,$09,$89,$79,$96,$01,$89,$77,$94,$09,$89,$79,$96
	.byte $01,$89,$77,$94,$09,$89,$79,$96,$10,$89
	.byte $ff,$0e
	.word @ref477
@ref479:
	.byte $01,$89,$77,$94,$09,$89,$79,$96,$01,$89,$77,$94,$09,$89,$79,$96
	.byte $01,$89,$77,$94,$09,$89,$79,$96,$01,$83,$77,$10,$83
@ref480:
	.byte $78,$01,$89,$76,$94,$09,$89,$78,$96,$01,$89,$76,$94,$09,$89,$78
	.byte $96,$01,$89,$76,$94,$09,$89,$78,$96,$10,$89
@ref481:
	.byte $01,$89,$76,$94,$09,$89,$78,$96,$01,$89,$76,$94,$09,$89,$78,$96
	.byte $01,$89,$76,$94,$09,$89,$78,$96,$10,$89
@ref482:
	.byte $79,$01,$89,$77,$94,$09,$89,$79,$96,$01,$89,$77,$94,$09,$89,$7a
	.byte $96,$01,$89,$78,$94,$09,$89,$7a,$96,$10,$89
@ref483:
	.byte $7b,$01,$89,$79,$94,$09,$89,$98,$05,$89,$78,$92,$07,$06,$05,$04
	.byte $03,$02,$7c,$96,$01,$83,$02,$83,$7c,$01,$83,$7d,$98,$05,$83,$04
	.byte $83,$03,$83
	.byte $ff,$10
	.word @ref440
@ref485:
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$7c,$96,$01,$89,$7a,$94,$09,$83
	.byte $7a,$09,$83,$7c,$96,$01,$83,$78,$98,$05,$83,$7a,$94,$09,$89,$98
	.byte $05,$89
	.byte $ff,$10
	.word @ref440
@ref487:
	.byte $7c,$96,$01,$89,$7a,$94,$09,$89,$7c,$96,$01,$89,$7a,$94,$09,$83
	.byte $7a,$09,$83,$7c,$96,$01,$89,$7a,$94,$09,$89,$98,$05,$83,$78,$05
	.byte $83
	.byte $ff,$10
	.word @ref440
	.byte $ff,$12
	.word @ref485
	.byte $ff,$10
	.word @ref440
	.byte $ff,$12
	.word @ref487
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$0e
	.word @ref421
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$10
	.word @ref423
	.byte $ff,$10
	.word @ref440
	.byte $ff,$10
	.word @ref441
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$10
	.word @ref443
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$0e
	.word @ref421
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$10
	.word @ref423
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$0e
	.word @ref421
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$10
	.word @ref423
	.byte $ff,$10
	.word @ref440
	.byte $ff,$10
	.word @ref441
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$10
	.word @ref443
	.byte $ff,$10
	.word @ref440
	.byte $ff,$10
	.word @ref441
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$10
	.word @ref443
	.byte $ff,$0e
	.word @ref420
	.byte $ff,$0e
	.word @ref420
@ref518:
	.byte $7a,$92,$09,$08,$07,$06,$05,$04,$03,$02,$01,$79,$10,$89,$78,$8b
	.byte $77,$8b,$76,$8b,$75,$8b,$74,$85
@ref519:
	.byte $00,$d1
	.byte $fd
	.word @song1ch3loop

; jazz
@song1ch4:
@song1ch4loop:
@ref520:
	.byte $d3
@ref521:
	.byte $d3
@ref522:
	.byte $d3
@ref523:
	.byte $d3
@ref524:
	.byte $d3
@ref525:
	.byte $d3
@ref526:
	.byte $d3
@ref527:
	.byte $d3
@ref528:
	.byte $d3
@ref529:
	.byte $d3
@ref530:
	.byte $d3
@ref531:
	.byte $d3
@ref532:
	.byte $d3
@ref533:
	.byte $d3
@ref534:
	.byte $d3
@ref535:
	.byte $d3
@ref536:
	.byte $d3
@ref537:
	.byte $d3
@ref538:
	.byte $d3
@ref539:
	.byte $d3
@ref540:
	.byte $d3
@ref541:
	.byte $d3
@ref542:
	.byte $d3
@ref543:
	.byte $d3
@ref544:
	.byte $d3
@ref545:
	.byte $d3
@ref546:
	.byte $d3
@ref547:
	.byte $d3
@ref548:
	.byte $d3
@ref549:
	.byte $d3
@ref550:
	.byte $d3
@ref551:
	.byte $d3
@ref552:
	.byte $d3
@ref553:
	.byte $d3
@ref554:
	.byte $d3
@ref555:
	.byte $d3
@ref556:
	.byte $d3
@ref557:
	.byte $d3
@ref558:
	.byte $d3
@ref559:
	.byte $d3
@ref560:
	.byte $d3
@ref561:
	.byte $d3
@ref562:
	.byte $d3
@ref563:
	.byte $d3
@ref564:
	.byte $d3
@ref565:
	.byte $d3
@ref566:
	.byte $d3
@ref567:
	.byte $d3
@ref568:
	.byte $d3
@ref569:
	.byte $d3
@ref570:
	.byte $d3
@ref571:
	.byte $d3
@ref572:
	.byte $d3
@ref573:
	.byte $d3
@ref574:
	.byte $d3
@ref575:
	.byte $d3
@ref576:
	.byte $d3
@ref577:
	.byte $d3
@ref578:
	.byte $d3
@ref579:
	.byte $d3
@ref580:
	.byte $d3
@ref581:
	.byte $d3
@ref582:
	.byte $d3
@ref583:
	.byte $d3
@ref584:
	.byte $d3
@ref585:
	.byte $d3
@ref586:
	.byte $d3
@ref587:
	.byte $d3
@ref588:
	.byte $d3
@ref589:
	.byte $d3
@ref590:
	.byte $d3
@ref591:
	.byte $d3
@ref592:
	.byte $d3
@ref593:
	.byte $d3
@ref594:
	.byte $d3
@ref595:
	.byte $d3
@ref596:
	.byte $d3
@ref597:
	.byte $d3
@ref598:
	.byte $d3
@ref599:
	.byte $d3
@ref600:
	.byte $d3
@ref601:
	.byte $d3
@ref602:
	.byte $d3
@ref603:
	.byte $d3
@ref604:
	.byte $d3
@ref605:
	.byte $d3
@ref606:
	.byte $d3
@ref607:
	.byte $d3
@ref608:
	.byte $d3
@ref609:
	.byte $d3
@ref610:
	.byte $d3
@ref611:
	.byte $d3
@ref612:
	.byte $d3
@ref613:
	.byte $d3
@ref614:
	.byte $d3
@ref615:
	.byte $d3
@ref616:
	.byte $d3
@ref617:
	.byte $d3
@ref618:
	.byte $d3
@ref619:
	.byte $d3
	.byte $fd
	.word @song1ch4loop