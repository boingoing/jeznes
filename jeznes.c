#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "sprites.h"
#include "jeznes.h"

void main (void) {
    // screen off
    ppu_off();

    // temp.?
    pal_bg(palette);
    vram_adr(NAMETABLE_A);
    vram_unrle(playfield_screen);

    // sprites are in the 0th CHR bank.
    // tiles are in the 1st CHR bank.
    bank_spr(0);
    bank_bg(1);

    set_vram_buffer();
    clear_vram_buffer();

    // turn on screen
    ppu_on_all();

    while (1) {
        
    }
}
