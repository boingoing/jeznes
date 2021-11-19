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

            flip_player_orientation();
            start_line();

            move_player();
            move_balls();

            // clear all sprites from sprite buffer
            oam_clear();
            draw_player();
            draw_balls();
            draw_tile_highlight();

            // For debugging, render a line indicating how much CPU is used.
            gray_line();
        }
    }
}

void init_game(void) {
    // Load graphics
    pal_bg(bg_palette);
    pal_spr(sprite_palette);
    vram_adr(NAMETABLE_A);
    vram_unrle(playfield_screen);

    seed_rng();

    // Starting game state
    game_state = GAME_STATE_PLAYING;
    current_level = 1;

    // Player position
    players[0].x = 0x86;
    players[0].y = 0x66;
    players[0].orientation = PLAYER_ORIENTATION_HORIZ;
    players[0].rotate_pressed = 0;
    players[0].place_pressed = 0;
    update_nearest_tile();

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
    if (pad1 & PAD_LEFT) {
        temp_byte_1 = players[0].x;
        temp_byte_1 -= PLAYER_SPEED;
        if (players[0].orientation & PLAYER_ORIENTATION_VERT) {
            temp_byte_2 = PLAYFIELD_LEFT_WALL;
        } else {
            temp_byte_2 = PLAYFIELD_LEFT_WALL + 8;
        }
        if (temp_byte_1 <= temp_byte_2) {
            players[0].x = temp_byte_2;
        } else {
            players[0].x = temp_byte_1;
        }
        update_nearest_tile();
    } else if (pad1 & PAD_RIGHT) {
        temp_byte_1 = players[0].x;
        temp_byte_1 += PLAYER_SPEED;
        if (temp_byte_1 >= PLAYFIELD_RIGHT_WALL) {
            players[0].x = PLAYFIELD_RIGHT_WALL;
        } else {
            players[0].x = temp_byte_1;
        }
        update_nearest_tile();
    }

    if (pad1 & PAD_DOWN) {
        temp_byte_1 = players[0].y;
        temp_byte_1 += PLAYER_SPEED;
        if (temp_byte_1 >= PLAYFIELD_BOTTOM_WALL) {
            players[0].y = PLAYFIELD_BOTTOM_WALL;
        } else {
            players[0].y = temp_byte_1;
        }
        update_nearest_tile();
    } else if (pad1 & PAD_UP) {
        temp_byte_1 = players[0].y;
        temp_byte_1 -= PLAYER_SPEED;
        if (players[0].orientation & PLAYER_ORIENTATION_VERT) {
            temp_byte_2 = PLAYFIELD_TOP_WALL + 8;
        } else {
            temp_byte_2 = PLAYFIELD_TOP_WALL;
        }
        if (temp_byte_1 <= temp_byte_2) {
            players[0].y = temp_byte_2;
        } else {
            players[0].y = temp_byte_1;
        }
        update_nearest_tile();
    }
}

void move_balls(void) {
    for (temp_byte_1 = 0; temp_byte_1 < current_level; ++temp_byte_1) {
        temp_signed_byte_1 = balls[temp_byte_1].x_velocity;
        temp_byte_2 = balls[temp_byte_1].x;
        temp_byte_2 += temp_signed_byte_1;

        // y tile coord
        temp_byte_5 = balls[temp_byte_1].y >> 3;
        if (temp_signed_byte_1 > 0) {
            // Moving right
            temp_byte_4 = (temp_byte_2 + 7) >> 3;
            temp_int_1 = temp_byte_4 + 32 * temp_byte_5 - PLAYFIELD_FIRST_TILE_INDEX;
            temp_byte_4 = (temp_byte_4 << 3) - 8;
        } else {
            // Moving left
            temp_byte_4 = temp_byte_2 >> 3;
            temp_int_1 = temp_byte_4 + 32 * temp_byte_5 - PLAYFIELD_FIRST_TILE_INDEX;
            temp_byte_4 = (temp_byte_4 << 3) + 8;
        }
        temp_byte_3 = playfield[temp_int_1];
        if (temp_byte_3 == PLAYFIELD_WALL) {
            balls[temp_byte_1].x_velocity *= -1;
            temp_byte_2 = temp_byte_4;
        }
        balls[temp_byte_1].x = temp_byte_2;

        temp_signed_byte_1 = balls[temp_byte_1].y_velocity;
        temp_byte_2 = balls[temp_byte_1].y;
        temp_byte_2 += temp_signed_byte_1;

        // x tile coord
        temp_byte_5 = balls[temp_byte_1].x >> 3;
        if (temp_signed_byte_1 > 0) {
            // Moving down
            temp_byte_4 = (temp_byte_2 + 7) >> 3;
            temp_int_1 = temp_byte_5 + 32 * temp_byte_4 - PLAYFIELD_FIRST_TILE_INDEX;
            temp_byte_4 = (temp_byte_4 << 3) - 8;
        } else {
            // Moving up
            temp_byte_4 = temp_byte_2 >> 3;
            temp_int_1 = temp_byte_5 + 32 * temp_byte_4 - PLAYFIELD_FIRST_TILE_INDEX;
            temp_byte_4 = (temp_byte_4 << 3) + 8;
        }
        temp_byte_3 = playfield[temp_int_1];
        if (temp_byte_3 == PLAYFIELD_WALL) {
            balls[temp_byte_1].y_velocity *= -1;
            temp_byte_2 = temp_byte_4;
        }
        balls[temp_byte_1].y = temp_byte_2;
    }
}

void draw_player(void) {
    temp_byte_1 = get_frame_count();
    temp_byte_1 = temp_byte_1 >> 3 & 1;
    if (players[0].orientation & PLAYER_ORIENTATION_VERT) {
        temp_byte_2 = temp_byte_1;
    } else {
        temp_byte_2 = 2 + temp_byte_1;
    }
    oam_meta_spr(players[0].x, players[0].y, player_metasprite_list[temp_byte_2]);
}

void draw_balls(void) {
    temp_byte_2 = get_frame_count();
    temp_byte_2 = (temp_byte_2 >> 2) % 18 + 0x30;
    for (temp_byte_1 = 0; temp_byte_1 < current_level; ++temp_byte_1) {
        oam_spr(balls[temp_byte_1].x, balls[temp_byte_1].y, temp_byte_2, 0);
    }
}

void draw_tile_highlight(void) {
    oam_spr(players[0].nearest_tile_x, players[0].nearest_tile_y, 0x18, 0);
}

void start_line(void) {
    if (pad1 & PAD_A) {
        if (players[0].place_pressed == 0) {
            players[0].place_pressed = 1;

            temp_int_1 = get_ppu_addr(0, players[0].nearest_tile_x, players[0].nearest_tile_y);
            one_vram_buffer(TILE_INDEX_PLAYFIELD_CLEARED, temp_int_1);
        }
    } else {
        players[0].place_pressed = 0;
    }
}

void flip_player_orientation(void) {
    if (pad1 & PAD_B) {
        if (players[0].rotate_pressed == 0) {
            players[0].rotate_pressed = 1;
            players[0].orientation = players[0].orientation ^ 1;

            if (players[0].orientation == PLAYER_ORIENTATION_HORIZ) {
                temp_byte_2 = PLAYFIELD_LEFT_WALL + 8;
                if (players[0].x <= temp_byte_2) {
                    players[0].x = temp_byte_2;
                }
            } else {
                temp_byte_2 = PLAYFIELD_TOP_WALL + 8;
                if (players[0].y <= temp_byte_2) {
                    players[0].y = temp_byte_2;
                }
            }
        }
    } else {
        players[0].rotate_pressed = 0;
    }
}

void update_nearest_tile(void) {
    if (players[0].orientation & PLAYER_ORIENTATION_VERT) {
        temp_byte_3 = players[0].x + 4;
        temp_byte_4 = players[0].y;
    } else {
        temp_byte_3 = players[0].x;
        temp_byte_4 = players[0].y + 4;
    }
    players[0].nearest_tile_x = temp_byte_3 >> 3 << 3;
    players[0].nearest_tile_y = temp_byte_4 >> 3 << 3;
}
