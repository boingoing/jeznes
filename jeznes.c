#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "sprites.h"
#include "playfield.h"
#include "jeznes.h"

void main(void) {
    // screen off
    ppu_off();

    // sprites are in the 0th CHR bank.
    // tiles are in the 1st CHR bank.
    bank_spr(0);
    bank_bg(1);

    init_game();

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

            start_line();

            move_player();
            move_balls();

            // clear all sprites from sprite buffer
            oam_clear();
            draw_player();
            draw_balls();

            // For debugging, render a line indicating how much CPU is used.
            gray_line();
        }
    }
}

void init_game(void) {
    // Load graphics
    pal_bg(palette);
    pal_spr(palette);
    vram_adr(NAMETABLE_A);
    vram_unrle(playfield_screen);

    seed_rng();

    // Starting game state
    game_state = GAME_STATE_PLAYING;
    current_level = 1;

    // Player position
    players[0].x = 0x86;
    players[0].y = 0x66;

    // Ball positions
    for (temp_byte_1 = 0; temp_byte_1 < current_level; ++temp_byte_1) {
        balls[temp_byte_1].x = rand8();
        balls[temp_byte_1].y = rand8();
        balls[temp_byte_1].x_velocity = BALL_SPEED;
        balls[temp_byte_1].y_velocity = BALL_SPEED;
    }

    for (temp_byte_1 = current_level; temp_byte_1 < MAX_BALLS; ++temp_byte_1) {
        balls[temp_byte_1].y = 0xff;
    }

    // Load playfield pattern
    load_playfield();
}

void load_playfield(void) {
    memcpy(&playfield, &playfield_pattern_1, PLAYFIELD_WIDTH*PLAYFIELD_HEIGHT);
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

void move_balls(void) {
    for (temp_byte_1 = 0; temp_byte_1 < current_level; ++temp_byte_1) {
        temp_byte_2 = balls[temp_byte_1].x;
        temp_byte_2 += balls[temp_byte_1].x_velocity;
        if (temp_byte_2 >= PLAYFIELD_RIGHT_WALL) {
            balls[temp_byte_1].x = PLAYFIELD_RIGHT_WALL;
            balls[temp_byte_1].x_velocity *= -1;
        } else if (temp_byte_2 <= PLAYFIELD_LEFT_WALL) {
            balls[temp_byte_1].x = PLAYFIELD_LEFT_WALL;
            balls[temp_byte_1].x_velocity *= -1;
        } else {
            balls[temp_byte_1].x = temp_byte_2;
        }

        temp_byte_2 = balls[temp_byte_1].y;
        temp_byte_2 += balls[temp_byte_1].y_velocity;
        if (temp_byte_2 >= PLAYFIELD_BOTTOM_WALL) {
            balls[temp_byte_1].y = PLAYFIELD_BOTTOM_WALL;
            balls[temp_byte_1].y_velocity *= -1;
        } else if (temp_byte_2 <= PLAYFIELD_TOP_WALL) {
            balls[temp_byte_1].y = PLAYFIELD_TOP_WALL;
            balls[temp_byte_1].y_velocity *= -1;
        } else {
            balls[temp_byte_1].y = temp_byte_2;
        }
    }
}

void draw_player(void) {
    oam_spr(players[0].x, players[0].y, 0x14, 0);
}

void draw_balls(void) {
    for (temp_byte_1 = 0; temp_byte_1 < current_level; ++temp_byte_1) {
        oam_spr(balls[temp_byte_1].x, balls[temp_byte_1].y, 0x4, 0);
    }
}

void start_line(void) {
    if (pad1 & PAD_A) {
        temp_byte_1 = players[0].x - PLAYFIELD_LEFT_WALL + 0x3;
        temp_byte_1 = temp_byte_1 >> 3;
        temp_byte_2 = players[0].y - PLAYFIELD_TOP_WALL + 0x3;
        temp_byte_2 = temp_byte_2 >> 3;

        temp_int_1 = get_ppu_addr(0, players[0].x, players[0].y);
        one_vram_buffer(0xff, temp_int_1);
    }
}
