#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "sprites.h"
#include "playfield.h"
#include "jeznes.h"

void main(void) {
    // screen off
    ppu_off();

    // sprites are in the 0th CHR bank.
    bank_spr(0);
    // tiles are in the 1st CHR bank.
    bank_bg(1);

    init_game();

    set_vram_buffer();
    clear_vram_buffer();

    // turn on screen
    ppu_on_all();

    while (1) {
        ppu_wait_nmi();

        // read the first controller
        pad1 = pad_poll(0);
        pad1_new = get_pad_new(0);

        // do at the beginning of each frame
        clear_vram_buffer();

        if (game_state == GAME_STATE_PLAYING) {
            update_line();

            flip_player_orientation();
            start_line();

            move_player();
            move_balls();

            // clear all sprites from sprite buffer
            oam_clear();
            draw_player();
            draw_balls();
            draw_tile_highlight();
            draw_line();
        } else if (game_state == GAME_STATE_UPDATING_PLAYFIELD) {
            // Restart the update of cleared playfield tiles.
            update_cleared_playfield_tiles();

            // If we finished updating the playfield tiles, let's remove the mark bits.
            if (game_state == GAME_STATE_PLAYING) {
                reset_playfield_mark_bit();
            }
        }

#if DEBUG
        // For debugging, render a line indicating how much CPU is used.
        gray_line();
#endif
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
    players[0].orientation = ORIENTATION_HORIZ;
    players[0].rotate_pressed = FALSE;
    players[0].place_pressed = FALSE;
    update_nearest_tile();

    lines[0].is_started = FALSE;

    // Ball positions
    for (temp_byte_1 = 0; temp_byte_1 < current_level; ++temp_byte_1) {
        balls[temp_byte_1].x = rand8() % (0xff - 0x30) + 0x18;
        balls[temp_byte_1].y = rand8() % (0xff - 0x70) + 0x20;
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
    memcpy(&playfield, playfield_patterns[0], PLAYFIELD_WIDTH*PLAYFIELD_HEIGHT);
}

void move_player(void) {
    if (pad1 & PAD_LEFT) {
        temp_byte_1 = players[0].x;
        temp_byte_1 -= PLAYER_SPEED;
        if (players[0].orientation & ORIENTATION_VERT) {
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
        if (players[0].orientation & ORIENTATION_VERT) {
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
        if (playfield[temp_int_1] == PLAYFIELD_WALL) {
            balls[temp_byte_1].x_velocity *= -1;
            temp_byte_2 = temp_byte_4;
        }
        balls[temp_byte_1].x = temp_byte_2;

        temp_signed_byte_1 = balls[temp_byte_1].y_velocity;
        temp_byte_3 = balls[temp_byte_1].y;
        temp_byte_3 += temp_signed_byte_1;

        // x tile coord
        temp_byte_5 = balls[temp_byte_1].x >> 3;
        if (temp_signed_byte_1 > 0) {
            // Moving down
            temp_byte_4 = (temp_byte_3 + 7) >> 3;
            temp_int_1 = temp_byte_5 + 32 * temp_byte_4 - PLAYFIELD_FIRST_TILE_INDEX;
            temp_byte_4 = (temp_byte_4 << 3) - 8;
        } else {
            // Moving up
            temp_byte_4 = temp_byte_3 >> 3;
            temp_int_1 = temp_byte_5 + 32 * temp_byte_4 - PLAYFIELD_FIRST_TILE_INDEX;
            temp_byte_4 = (temp_byte_4 << 3) + 8;
        }
        if (playfield[temp_int_1] == PLAYFIELD_WALL) {
            balls[temp_byte_1].y_velocity *= -1;
            temp_byte_3 = temp_byte_4;
        }
        balls[temp_byte_1].y = temp_byte_3;

        // Update nearest playfield tile
        temp_byte_2 = (temp_byte_2 + 4) >> 3;
        temp_byte_3 = (temp_byte_3 + 4) >> 3;
        balls[temp_byte_1].nearest_playfield_tile = temp_byte_2 + (temp_byte_3 << 5) - PLAYFIELD_FIRST_TILE_INDEX;
    }
}

void draw_player(void) {
    temp_byte_1 = get_frame_count();
    temp_byte_1 = temp_byte_1 >> 3 & 1;
    if (players[0].orientation & ORIENTATION_VERT) {
        temp_byte_2 = temp_byte_1;
    } else {
        temp_byte_2 = 2 + temp_byte_1;
    }
    oam_meta_spr(players[0].x, players[0].y, player_metasprite_list[temp_byte_2]);
}

void draw_balls(void) {
    temp_byte_2 = get_frame_count();
    temp_byte_2 = (temp_byte_2 >> 2) % 18 + TILE_INDEX_BALL_BASE;
    for (temp_byte_1 = 0; temp_byte_1 < current_level; ++temp_byte_1) {
        oam_spr(balls[temp_byte_1].x, balls[temp_byte_1].y, temp_byte_2, 0);

#if DRAW_BALL_NEAREST_TILE_HIGHLIGHT
        temp_int_1 = balls[temp_byte_1].nearest_playfield_tile;
        temp_byte_3 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) % 32) << 3;
        temp_byte_4 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) >> 5) << 3;
        oam_spr(temp_byte_3, temp_byte_4 - 1, TILE_INDEX_TILE_HIGHLIGHT, 1);
#endif
    }
}

void draw_tile_highlight(void) {
    if (playfield[players[0].nearest_playfield_tile] == PLAYFIELD_UNCLEARED) {
        oam_spr(players[0].nearest_tile_x, players[0].nearest_tile_y - 1, TILE_INDEX_TILE_HIGHLIGHT, 1);
    }
}

void draw_line(void) {
    if (lines[0].is_started == TRUE) {
        if (lines[0].orientation & ORIENTATION_VERT) {
            temp_byte_3 = SPRITE_INDEX_PLAYFIELD_LINE_VERT_BASE + lines[0].current_block_completion;
            temp_byte_4 = OAM_FLIP_V;
        } else {
            temp_byte_3 = SPRITE_INDEX_PLAYFIELD_LINE_HORIZ_BASE + lines[0].current_block_completion;
            temp_byte_4 = OAM_FLIP_H;
        }

        if (lines[0].is_neg_complete == FALSE) {
            temp_int_1 = lines[0].current_neg;
            temp_byte_1 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) % 32) << 3;
            temp_byte_2 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) >> 5) << 3;
            oam_spr(temp_byte_1, temp_byte_2 - 1, temp_byte_3, 1 | temp_byte_4);
        }

        if (lines[0].is_pos_complete == FALSE) {
            temp_int_1 = lines[0].current_pos;
            temp_byte_1 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) % 32) << 3;
            temp_byte_2 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) >> 5) << 3;
            oam_spr(temp_byte_1, temp_byte_2 - 1, temp_byte_3, 1);
        }
    }
}

void update_line(void) {
    if (lines[0].is_started == TRUE) {
        // There are 8 pixels in the block and we draw them one by one.
        // When we've reached completion of the 8 pixels, move the current head of the lines forward in either direction.
        if (lines[0].current_block_completion == 8) {
            temp_byte_4 = lines[0].orientation;
            if (temp_byte_4 & ORIENTATION_VERT) {
                temp_byte_5 = 32;
            } else {
                temp_byte_5 = 1;
            }

            if (lines[0].is_neg_complete == FALSE) {
                // Before moving the current line head, update the metadata for the tile we're moving from
                temp_int_1 = lines[0].current_neg;
                temp_byte_1 = PLAYFIELD_LINE & (temp_byte_4 << PLAYFIELD_BIT_LINE_ORIENTATION) & (0 << PLAYFIELD_BIT_LINE_INDEX);
                temp_byte_3 = TILE_INDEX_PLAYFIELD_LINE_HORIZ + temp_byte_4;
                set_playfield_tile(temp_int_1, temp_byte_1, temp_byte_3);

                temp_int_1 -= temp_byte_5;
                lines[0].current_neg = temp_int_1;

                // If the current block is not an uncleared tile, that means we hit the end for that line.
                // Walk back over the blocks until we reach origin and update them to cleared.
                if (playfield[temp_int_1] != PLAYFIELD_UNCLEARED) {
                    while (1) {
                        temp_int_1 += temp_byte_5;
                        if (temp_int_1 == lines[0].origin) {
                            break;
                        }
                        set_playfield_tile(temp_int_1, PLAYFIELD_WALL, TILE_INDEX_PLAYFIELD_CLEARED);
                    }
                    lines[0].is_neg_complete = TRUE;

                    // When both directions are complete the line is done
                    if (lines[0].is_pos_complete == TRUE) {
                        temp_int_1 = lines[0].origin;
                        set_playfield_tile(temp_int_1, PLAYFIELD_WALL, TILE_INDEX_PLAYFIELD_CLEARED);

                        lines[0].is_started = FALSE;
                    }

                    line_completed();
                }
            }

            // Now do the positive direction.
            if (lines[0].is_pos_complete == FALSE) {
                // Before moving the current line head, update the metadata for the tile we're moving from
                temp_int_1 = lines[0].current_pos;
                temp_byte_1 = PLAYFIELD_LINE & (temp_byte_4 << PLAYFIELD_BIT_LINE_ORIENTATION) & (0 << PLAYFIELD_BIT_LINE_INDEX);
                temp_byte_3 = TILE_INDEX_PLAYFIELD_LINE_HORIZ + temp_byte_4;
                set_playfield_tile(temp_int_1, temp_byte_1, temp_byte_3);

                temp_int_1 += temp_byte_5;
                lines[0].current_pos = temp_int_1;

                if (playfield[temp_int_1] != PLAYFIELD_UNCLEARED) {
                    while (1) {
                        temp_int_1 -= temp_byte_5;
                        if (temp_int_1 == lines[0].origin) {
                            break;
                        }
                        set_playfield_tile(temp_int_1, PLAYFIELD_WALL, TILE_INDEX_PLAYFIELD_CLEARED);
                    }
                    lines[0].is_pos_complete = TRUE;

                    // When both directions are complete the line is done
                    if (lines[0].is_neg_complete == TRUE) {
                        temp_int_1 = lines[0].origin;
                        set_playfield_tile(temp_int_1, PLAYFIELD_WALL, TILE_INDEX_PLAYFIELD_CLEARED);

                        lines[0].is_started = FALSE;
                    }

                    line_completed();
                }
            }

            lines[0].current_block_completion = 0;
        } else {
            if (get_frame_count() % 3 == 0) {
                ++lines[0].current_block_completion;
            }
        }
    }
}

void start_line(void) {
    if (pad1 & PAD_A) {
        if (players[0].place_pressed == FALSE) {
            players[0].place_pressed = TRUE;

            // Do nothing if a line is already active
            if (lines[0].is_started == TRUE) {
                return;
            }

            temp_int_1 = players[0].nearest_playfield_tile;
            // We only want to start a line if the origin tile is not already cleared
            if (playfield[temp_int_1] != PLAYFIELD_UNCLEARED) {
                return;
            }

            // Update the playfield in-memory structure
            temp_byte_5 = players[0].orientation;
            playfield[temp_int_1] = PLAYFIELD_LINE & (temp_byte_5 << PLAYFIELD_BIT_LINE_ORIENTATION) & (0 << PLAYFIELD_BIT_LINE_INDEX);

            // Set the bg tile
            one_vram_buffer(TILE_INDEX_PLAYFIELD_LINE_HORIZ + temp_byte_5, get_ppu_addr(0, players[0].nearest_tile_x, players[0].nearest_tile_y));

            // Update the line data
            lines[0].current_block_completion = 8;
            lines[0].orientation = temp_byte_5;
            lines[0].is_started = TRUE;
            lines[0].origin = temp_int_1;
            lines[0].current_neg = temp_int_1;
            lines[0].current_pos = temp_int_1;
            lines[0].is_neg_complete = FALSE;
            lines[0].is_pos_complete = FALSE;
        }
    } else {
        players[0].place_pressed = FALSE;
    }
}

void flip_player_orientation(void) {
    if (pad1 & PAD_B) {
        if (players[0].rotate_pressed == FALSE) {
            players[0].rotate_pressed = TRUE;
            players[0].orientation = players[0].orientation ^ 1;

            if (players[0].orientation == ORIENTATION_HORIZ) {
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
        players[0].rotate_pressed = FALSE;
    }
}

// Don't modify temp_byte_1, temp_byte_2
void update_nearest_tile(void) {
    if (players[0].orientation & ORIENTATION_VERT) {
        temp_byte_3 = players[0].x + 4;
        temp_byte_4 = players[0].y;
    } else {
        temp_byte_3 = players[0].x;
        temp_byte_4 = players[0].y + 4;
    }
    temp_byte_3 = temp_byte_3 >> 3;
    temp_byte_4 = temp_byte_4 >> 3;

    players[0].nearest_tile_x = temp_byte_3 << 3;
    players[0].nearest_tile_y = temp_byte_4 << 3;
    players[0].nearest_playfield_tile = temp_byte_3 + (temp_byte_4 << 5) - PLAYFIELD_FIRST_TILE_INDEX;
}

void line_completed(void) {
    reset_playfield_mark_bit();
    compute_playfield_mark_bit_one_ball(0);

    // Reset |playfield_index|, set the game state to updating the playfield, which will cause us to call
    // update_cleared_playfield_tiles() from the beginning next frame.
    // If we need to call it again after that, we will call it in restartable mode.
    playfield_index = 0;
    game_state = GAME_STATE_UPDATING_PLAYFIELD;
}

// input:
// temp_int_1 has the playfield tile index
// playfield_tile_type has the new playfield tile type (ie: PLAYFIELD_WALL)
// playfield_bg_tile has the new bg tile graphic index (ie: TILE_INDEX_PLAYFIELD_CLEARED)
//
// scratch:
// temp_byte_1
// temp_byte_2
void set_playfield_tile(unsigned int tile_index, unsigned char playfield_tile_type, unsigned char playfield_bg_tile) {
    // Update the playfield in-memory structure.
    playfield[tile_index] = playfield_tile_type;
    // Calculate the bg tile index in pixel coords of the playfield tile
    temp_byte_1 = ((tile_index + PLAYFIELD_FIRST_TILE_INDEX) % 32) << 3;
    temp_byte_2 = ((tile_index + PLAYFIELD_FIRST_TILE_INDEX) >> 5) << 3;
    // Set the bg tile graphic
    one_vram_buffer(playfield_bg_tile, get_ppu_addr(0, temp_byte_1, temp_byte_2));
}

// Update uncleared, unmarked playfield tiles to be cleared.
// Note: This function can potentially queue more vram updates than are allowed during the next v-blank.
//       For that reason, it is restartable.
//       The current playfield_index needs to be reset to zero once at the beginning of the operation.
//       Otherwise, calling this function will continue from where it left off last time.
//       It returns TRUE when all vram updates are queued and FALSE if there are additonal vram updates pending.
//
// scratch:
// temp_int_1
// temp_byte_3
// temp_byte_4
void update_cleared_playfield_tiles(void) {
    temp_byte_3 = 0;
    // Look over all tiles in the playfield and for each uncleared, unmarked tile change it to cleared
    for (; playfield_index < PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT; ++playfield_index) {
        // Skip tiles which are not uncleared (this includes marked tiles)
        if (playfield[playfield_index] != PLAYFIELD_UNCLEARED) {
            continue;
        }

        temp_byte_3++;
        set_playfield_tile(playfield_index, PLAYFIELD_WALL, TILE_INDEX_PLAYFIELD_CLEARED);

        // We can only queue about 40 tile updates per v-blank.
        if (temp_byte_3 >= 40) {
            return;
        }
    }

    // Reset the game state to playing.
    game_state = GAME_STATE_PLAYING;
}

// Reset the mark bit in all playfield tiles.
//
// scratch:
// temp_int_1
void reset_playfield_mark_bit(void) {
    for (temp_int_1 = 0; temp_int_1 < PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT; ++temp_int_1) {
        playfield[temp_int_1] &= ~PLAYFIELD_BITMASK_MARK;
    }
}

#define make_word(lo,hi) ((lo)|(hi << 8))
#define playfield_index_x(i) ((i)%32)
#define playfield_index_y(i) ((i)>>5)

#define playfield_index_move_up(i) ((i) - 32)
#define playfield_index_move_down(i) ((i) + 32)
#define playfield_index_move_left(i) ((i) - 1)
#define playfield_index_move_right(i) ((i) + 1)

//#define inside(i) ((playfield[(i)] & (PLAYFIELD_BITMASK_NONE | PLAYFIELD_BITMASK_MARK)) == PLAYFIELD_UNCLEARED)
#define set(i) (playfield[(i)] |= PLAYFIELD_BITMASK_MARK)
#define is_set(i) ((playfield[(i)] & PLAYFIELD_BITMASK_MARK) != 0)

// Returns true if the playfield tile |index| is not a wall and not marked.
unsigned char __fastcall__ inside(int index) {
    return (playfield[index] & PLAYFIELD_BITMASK_MARK) == 0 && (playfield[index] & PLAYFIELD_BITMASK_NONE) != PLAYFIELD_WALL;
}

enum {
    MOVE_DIRECTION_RIGHT,
    MOVE_DIRECTION_DOWN,
    MOVE_DIRECTION_LEFT,
    MOVE_DIRECTION_UP,
    MOVE_DIRECTION_DEFAULT = MOVE_DIRECTION_RIGHT
};

#define get_cur() (temp_int_1)
#define set_cur(a) (temp_int_1 = (a))
#define get_mark() (temp_int_2)
#define set_mark(a) (temp_int_2 = (a))
#define get_mark2() (temp_int_3)
#define set_mark2(a) (temp_int_3 = (a))
#define get_cur_dir() (temp_byte_1)
#define set_cur_dir(a) (temp_byte_1 = (a))
#define get_mark_dir() (temp_byte_4)
#define set_mark_dir(a) (temp_byte_4 = (a))
#define get_mark2_dir() (temp_byte_5)
#define set_mark2_dir(a) (temp_byte_5 = (a))
#define get_backtrack() (temp_byte_2)
#define set_backtrack(a) (temp_byte_2 = (a))
#define get_findloop() (temp_byte_3)
#define set_findloop(a) (temp_byte_3 = (a))
#define get_mark_null() (temp_signed_byte_1)
#define set_mark_null(a) (temp_signed_byte_1 = (a))
#define get_mark2_null() (temp_signed_byte_2)
#define set_mark2_null(a) (temp_signed_byte_2 = (a))

int get_front() {
    switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
        return playfield_index_move_right(get_cur());
    case MOVE_DIRECTION_LEFT:
        return playfield_index_move_left(get_cur());
    case MOVE_DIRECTION_DOWN:
        return playfield_index_move_down(get_cur());
    case MOVE_DIRECTION_UP:
        return playfield_index_move_up(get_cur());
    }
}

int get_back() {
    switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
        return playfield_index_move_left(get_cur());
    case MOVE_DIRECTION_LEFT:
        return playfield_index_move_right(get_cur());
    case MOVE_DIRECTION_DOWN:
        return playfield_index_move_up(get_cur());
    case MOVE_DIRECTION_UP:
        return playfield_index_move_down(get_cur());
    }
}

int get_right() {
    switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
        return playfield_index_move_down(get_cur());
    case MOVE_DIRECTION_LEFT:
        return playfield_index_move_up(get_cur());
    case MOVE_DIRECTION_DOWN:
        return playfield_index_move_left(get_cur());
    case MOVE_DIRECTION_UP:
        return playfield_index_move_right(get_cur());
    }
}

int get_left() {
    switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
        return playfield_index_move_up(get_cur());
    case MOVE_DIRECTION_LEFT:
        return playfield_index_move_down(get_cur());
    case MOVE_DIRECTION_DOWN:
        return playfield_index_move_right(get_cur());
    case MOVE_DIRECTION_UP:
        return playfield_index_move_left(get_cur());
    }
}

int get_front_left() {
    switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
        return playfield_index_move_right(playfield_index_move_up(get_cur()));
    case MOVE_DIRECTION_LEFT:
        return playfield_index_move_left(playfield_index_move_down(get_cur()));
    case MOVE_DIRECTION_DOWN:
        return playfield_index_move_down(playfield_index_move_right(get_cur()));
    case MOVE_DIRECTION_UP:
        return playfield_index_move_up(playfield_index_move_left(get_cur()));
    }
}

int get_back_left() {
    switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
        return playfield_index_move_left(playfield_index_move_up(get_cur()));
    case MOVE_DIRECTION_LEFT:
        return playfield_index_move_right(playfield_index_move_down(get_cur()));
    case MOVE_DIRECTION_DOWN:
        return playfield_index_move_up(playfield_index_move_right(get_cur()));
    case MOVE_DIRECTION_UP:
        return playfield_index_move_down(playfield_index_move_left(get_cur()));
    }
}

void compute_playfield_mark_bit_one_ball(unsigned char ball_index) {
    // Set cur to starting playfield tile
    set_cur(balls[ball_index].nearest_playfield_tile);

    // If the playfield tile where |ball_index| is located has already been marked,
    // another ball is in the same region of the playfield as |ball_index|. There's
    // no point in remarking the region.
    if (!inside(get_cur())) {
        return;
    }

    // Set cur-dir to default direction
    set_cur_dir(MOVE_DIRECTION_DEFAULT);
    // Clear mark and mark2 (set values to null)
    set_mark_null(TRUE);
    set_mark2_null(TRUE);
    // Set backtrack and findloop to false
    set_backtrack(FALSE);
    set_findloop(FALSE);

    // Move forward until the playfield tile in front is marked or not uncleared
    // (ie: it is not inside).
    // Note: This function does not do bounds checking, we assume the playfield
    //       has a border of wall tiles on all sides.
    temp_int_4 = get_front();
    while (inside(temp_int_4)) {
        set_cur(temp_int_4);
        temp_int_4 = get_front();
    }

    goto PAINTER_ALGORITHM_START;

    while(1) {
        // Move forward
        set_cur(get_front());

        if (inside(get_right())) {
            if (get_backtrack() == TRUE && get_findloop() == FALSE && (inside(get_front()) || inside(get_left()))) {
                set_findloop(TRUE);
            }
            // Turn right
            set_cur_dir((get_cur_dir() + 1) % 4);

PAINTER_ALGORITHM_PAINT:
            // Move forward
            set_cur(get_front());
        }

PAINTER_ALGORITHM_START:
        // Count number of non-diagonally adjacent marked playfield tiles.
        temp_byte_6 = 0;
        if (!inside(playfield_index_move_up(get_cur()))) {
            temp_byte_6++;
        }
        if (!inside(playfield_index_move_down(get_cur()))) {
            temp_byte_6++;
        }
        if (!inside(playfield_index_move_left(get_cur()))) {
            temp_byte_6++;
        }
        if (!inside(playfield_index_move_right(get_cur()))) {
            temp_byte_6++;
        }

        if (temp_byte_6 != 4) {
            do {
                // Turn right
                set_cur_dir((get_cur_dir() + 1) % 4);
            } while (inside(get_front()));
            do {
                // Turn left
                set_cur_dir((get_cur_dir() + 3) % 4);
            } while (!inside(get_front()));
        }

        switch (temp_byte_6) {
        case 1:
            if (get_backtrack() == TRUE) {
                set_findloop(TRUE);
            } else if (get_findloop() == TRUE) {
                if (get_mark_null() == TRUE) {
                    set_mark_null(FALSE);
                }
            } else if (inside(get_front_left()) && inside(get_back_left())) {
                set_mark_null(TRUE);
                set(get_cur());
                goto PAINTER_ALGORITHM_PAINT;
            }
            break;
        case 2:
            if (!inside(get_back())) {
                if (inside(get_front_left())) {
                    set_mark_null(TRUE);
                    set(get_cur());
                    goto PAINTER_ALGORITHM_PAINT;
                }
            } else if (get_mark_null() == TRUE) {
                set_mark(get_cur());
                set_mark_null(FALSE);
                set_mark_dir(get_cur_dir());
                set_mark2_null(TRUE);
                set_findloop(FALSE);
                set_backtrack(FALSE);
            } else {
                if (get_mark2_null() == TRUE) {
                    if (get_cur() == get_mark()) {
                        if (get_cur_dir() == get_mark_dir()) {
                            set_mark_null(TRUE);
                            // Turn around
                            set_cur_dir((get_cur_dir() + 2) % 4);
                            set(get_cur());
                            goto PAINTER_ALGORITHM_PAINT;
                        } else {
                            set_backtrack(TRUE);
                            set_findloop(FALSE);
                            set_cur_dir(get_mark_dir());
                        }
                    } else if (get_findloop() == TRUE) {
                        set_mark2(get_cur());
                        set_mark2_null(FALSE);
                        set_mark2_dir(get_cur_dir());
                    }
                } else {
                    if (get_cur() == get_mark()) {
                        set_cur(get_mark2());
                        set_cur_dir(get_mark2_dir());
                        set_mark_null(TRUE);
                        set_mark2_null(TRUE);
                        set_backtrack(FALSE);
                        // Turn around
                        set_cur_dir((get_cur_dir() + 2) % 4);
                        set(get_cur());
                        goto PAINTER_ALGORITHM_PAINT;
                    } else if (get_cur() == get_mark2()) {
                        set_mark(get_cur());
                        set_mark_null(FALSE);
                        set_cur_dir(get_mark2_dir());
                        set_mark_dir(get_mark2_dir());
                        set_mark2_null(TRUE);
                    }
                }
            }
            break;
        case 3:
            set_mark_null(TRUE);
            set(get_cur());
            goto PAINTER_ALGORITHM_PAINT;
            break;
        case 4:
            set(get_cur());
            return;
        }
    }
}

// Simple flood fill scan-and-fill implementation.
// Mark all the uncleared playfield tiles inside the region enclosing |ball_index|.
// Starting at the playfield tile under the ball, walks to all adjacent playfield
// tiles which are uncleared and not marked. Each such tile is then marked and we
// will visit its adjacent tiles.
//
// scratch:
// temp_int_1
// temp_byte_2
void compute_playfield_mark_bit_one_ball2(unsigned char ball_index) {
    temp_int_1 = balls[ball_index].nearest_playfield_tile;

    // if not Inside(x, y) then return
    if (!inside(temp_int_1)) {
        return;
    }

    // let s = new empty stack
    stack_init();

    // Add (x, x, y, 1) to s
    // (x,y)
    stack_temp = temp_int_1;
    stack_push();
    // x
    temp_byte_1 = playfield_index_x(temp_int_1);
    // (x,1)
    stack_temp = make_word(temp_byte_1, 1);
    stack_push();

    // Add (x, x, y-1, -1) to s
    // (x,y-1)
    stack_temp = temp_int_1 - 32;
    stack_push();
    // (x,-1)
    stack_temp = make_word(temp_byte_1, -1);
    stack_push();

    // while s is not empty:
    while (1) {
        stack_empty();
        if (temp_byte_2 & TRUE) {
            break;
        }

        // Remove an (x1, x2, y, dy) from s
        stack_pop();
        // x2
        temp_byte_2 = low_byte(stack_temp);
        // dy
        temp_signed_byte_1 = high_byte(stack_temp);
        stack_pop();
        // (x1,y)
        temp_int_2 = stack_temp;
        // let x = x1
        temp_int_1 = stack_temp;
        // x1
        temp_byte_3 = playfield_index_x(stack_temp);

        // if Inside(x, y):
        if (inside(temp_int_1)) {
            // while Inside(x-1, y):
            while (1) {
                // (x-1,y)
                stack_temp = temp_int_1 - 1;
                if (!inside(stack_temp)) {
                    break;
                }
                // Set(x-1, y)
                set(stack_temp);
                // x = x-1
                temp_int_1 = stack_temp;
            }
        }

        // if x < x1:
        if (playfield_index_x(temp_int_1) < temp_byte_3) {
            // Add (x, x1-1, y-dy, -dy) to s
            // -dy
            temp_signed_byte_2 = temp_signed_byte_1 * -1;
            // (x,y-dy)
            stack_temp = temp_int_1 + temp_signed_byte_2 * 32;
            stack_push();
            // (x1-1,-dy)
            stack_temp = make_word(temp_byte_3 - 1, temp_signed_byte_2);
            stack_push();
        }

        // while x1 <= x2:
        while (1) {
            if (temp_byte_3 > temp_byte_2) {
                break;
            }
            // while Inside(x1, y):
            while (1) {
                if (!inside(temp_int_2)) {
                    break;
                }
                // Set(x1, y)
                set(temp_int_2);
                // x1 = x1 + 1
                ++temp_int_2;
                // Update x1
                temp_byte_3 = playfield_index_x(temp_int_2);

                // Add (x, x1-1, y+dy, dy) to s
                // x1-1
                temp_byte_4 = temp_byte_3 - 1;
                // (x,y+dy)
                stack_temp = temp_int_1 + temp_signed_byte_1 * 32;
                stack_push();
                // (x1-1,dy)
                stack_temp = make_word(temp_byte_4, temp_signed_byte_1);
                stack_push();

                // if x1-1 > x2:
                if (temp_byte_4 > temp_byte_2) {
                    // Add (x2+1, x1-1, y-dy, -dy)
                    // -dy
                    temp_signed_byte_2 = temp_signed_byte_1 * -1;
                    // (x2+1,y-dy)
                    stack_temp = (temp_byte_2 + 1) + ((playfield_index_y(temp_int_2) + temp_signed_byte_2) << 5);
                    stack_push();
                    // (x1-1,-dy)
                    stack_temp = make_word(temp_byte_4, temp_signed_byte_2);
                    stack_push();
                }
            }

            // x1 = x1 + 1
            ++temp_int_2;
            // Update x1
            temp_byte_3 = playfield_index_x(temp_int_2);

            // while x1 < x2 and not Inside(x1, y):
            while (1) {
                if (temp_byte_3 >= temp_byte_2 || inside(temp_int_2)) {
                    break;
                }
                // x1 = x1 + 1
                ++temp_int_2;
                // update x1
                temp_byte_3 = playfield_index_x(temp_int_2);
            }

            // x = x1
            temp_int_1 = temp_int_2;
        }
    }
}

// Empties the stack
void stack_init(void) {
    stack_top = 0;
}

// Pushes stack_temp onto the top of stack
void stack_push(void) {
    if (stack_top == STACK_MAX_HEIGHT) {
        // This should be a break or assert.
        return;
    }
    stack[stack_top] = stack_temp;
    ++stack_top;
}

// Pops top of stack and stores in stack_temp
void stack_pop(void) {
    --stack_top;
    stack_temp = stack[stack_top];
}

// Sets temp_byte_2 to TRUE when the stack is empty
void stack_empty(void) {
    if (stack_top == 0) {
        temp_byte_2 = TRUE;
    } else {
        temp_byte_2 = FALSE;
    }
}
