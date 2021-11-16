#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "sprites.h"
#include "jeznes.h"

void main(void) {
    // screen off
    ppu_off();

    // temp.?
    game_state = GAME_STATE_PLAYING;
    players[0].x = 0x66;
    players[0].y = 0x66;
    pal_bg(palette);
    pal_spr(palette);
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
        while (game_state == GAME_STATE_PLAYING) {
            ppu_wait_nmi();

            // read the first controller
            pad1 = pad_poll(0);
            pad1_new = get_pad_new(0);

            // do at the beginning of each frame
            clear_vram_buffer();

            move_player();

            draw_player();

            // For debugging, render a line indicating how much CPU is used.
            gray_line();
        }
    }
}

void move_player(void) {
    temp_byte_1 = players[0].x;
    if (pad1 & PAD_LEFT) {
        temp_byte_1 -= PLAYER_SPEED;
        if (temp_byte_1 <= PLAYFIELD_LEFT_WALL) {
            players[0].x = PLAYFIELD_LEFT_WALL;
        } else {
            players[0].x = temp_byte_1;
        }
    } else if (pad1 & PAD_RIGHT) {
        temp_byte_1 += PLAYER_SPEED;
        if (temp_byte_1 >= PLAYFIELD_RIGHT_WALL) {
            players[0].x = PLAYFIELD_RIGHT_WALL;
        } else {
            players[0].x = temp_byte_1;
        }
    }

    temp_byte_1 = players[0].y;
    if (pad1 & PAD_DOWN) {
        temp_byte_1 += PLAYER_SPEED;
        if (temp_byte_1 >= PLAYFIELD_BOTTOM_WALL) {
            players[0].y = PLAYFIELD_BOTTOM_WALL;
        } else {
            players[0].y = temp_byte_1;
        }
    } else if (pad1 & PAD_UP) {
        temp_byte_1 -= PLAYER_SPEED;
        if (temp_byte_1 <= PLAYFIELD_TOP_WALL) {
            players[0].y = PLAYFIELD_TOP_WALL;
        } else {
            players[0].y = temp_byte_1;
        }
    }
}

void draw_player(void) {
    // clear all sprites from sprite buffer
    oam_clear();

    oam_spr(players[0].x, players[0].y, 0x14, 0);
}
