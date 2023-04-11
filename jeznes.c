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
                // Update the playfield in-memory structure
                temp_int_1 = lines[0].current_neg;
                playfield[temp_int_1] = PLAYFIELD_LINE & (temp_byte_4 << PLAYFIELD_BIT_LINE_ORIENTATION) & (0 << PLAYFIELD_BIT_LINE_INDEX);
                temp_byte_1 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) % 32) << 3;
                temp_byte_2 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) >> 5) << 3;
                // Set the bg tile
                temp_byte_3 = TILE_INDEX_PLAYFIELD_LINE_HORIZ + temp_byte_4;
                one_vram_buffer(temp_byte_3, get_ppu_addr(0, temp_byte_1, temp_byte_2));

                lines[0].current_neg -= temp_byte_5;
                temp_int_1 = lines[0].current_neg;

                // If the current block is not an uncleared tile, that means we hit the end for that line.
                // Walk back over the blocks until we reach origin and update them to cleared.
                if (playfield[temp_int_1] != PLAYFIELD_UNCLEARED) {
                    while (1) {
                        temp_int_1 += temp_byte_5;
                        if (temp_int_1 == lines[0].origin) {
                            break;
                        }
                        // Update the playfield in-memory structure
                        playfield[temp_int_1] = PLAYFIELD_WALL;

                        temp_byte_1 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) % 32) << 3;
                        temp_byte_2 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) >> 5) << 3;
                        // Set the bg tile
                        one_vram_buffer(TILE_INDEX_PLAYFIELD_CLEARED, get_ppu_addr(0, temp_byte_1, temp_byte_2));
                    }
                    lines[0].is_neg_complete = TRUE;

                    line_completed();
                }
            } else {
                // When both directions are complete the line is done
                if (lines[0].is_pos_complete == TRUE) {
                    temp_int_1 = lines[0].origin;

                    // Update the playfield in-memory structure
                    playfield[temp_int_1] = PLAYFIELD_WALL;

                    temp_byte_1 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) % 32) << 3;
                    temp_byte_2 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) >> 5) << 3;
                    // Set the bg tile
                    one_vram_buffer(TILE_INDEX_PLAYFIELD_CLEARED, get_ppu_addr(0, temp_byte_1, temp_byte_2));

                    lines[0].is_started = FALSE;
                }
            }

            // Now do the positive direction.
            if (lines[0].is_pos_complete == FALSE) {
                // Before moving the current line head, update the metadata for the tile we're moving from
                // Update the playfield in-memory structure
                temp_int_1 = lines[0].current_pos;
                playfield[temp_int_1] = PLAYFIELD_LINE & (temp_byte_4 << PLAYFIELD_BIT_LINE_ORIENTATION) & (0 << PLAYFIELD_BIT_LINE_INDEX);
                temp_byte_1 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) % 32) << 3;
                temp_byte_2 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) >> 5) << 3;
                // Set the bg tile
                temp_byte_3 = TILE_INDEX_PLAYFIELD_LINE_HORIZ + temp_byte_4;
                one_vram_buffer(temp_byte_3, get_ppu_addr(0, temp_byte_1, temp_byte_2));

                lines[0].current_pos += temp_byte_5;
                temp_int_1 = lines[0].current_pos;
                if (playfield[temp_int_1] != PLAYFIELD_UNCLEARED) {
                    while (1) {
                        temp_int_1 -= temp_byte_5;
                        if (temp_int_1 == lines[0].origin) {
                            break;
                        }
                        // Update the playfield in-memory structure
                        playfield[temp_int_1] = PLAYFIELD_WALL;
                        temp_byte_1 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) % 32) << 3;
                        temp_byte_2 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) >> 5) << 3;
                        // Set the bg tile
                        one_vram_buffer(TILE_INDEX_PLAYFIELD_CLEARED, get_ppu_addr(0, temp_byte_1, temp_byte_2));
                    }
                    lines[0].is_pos_complete = TRUE;

                    line_completed();
                }
            } else {
                // When both directions are complete the line is done
                if (lines[0].is_neg_complete == TRUE) {
                    temp_int_1 = lines[0].origin;

                    // Update the playfield in-memory structure
                    playfield[temp_int_1] = PLAYFIELD_WALL;

                    temp_byte_1 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) % 32) << 3;
                    temp_byte_2 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) >> 5) << 3;
                    // Set the bg tile
                    one_vram_buffer(TILE_INDEX_PLAYFIELD_CLEARED, get_ppu_addr(0, temp_byte_1, temp_byte_2));

                    lines[0].is_started = FALSE;
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
    temp_byte_1 = 0;
    compute_playfield_mark_bit_one_ball();
    update_cleared_playfield_tiles();
}

void update_cleared_playfield_tiles(void) {
    // Look over all tiles in the playfield and for each uncleared, unmarked tile change it to cleared
    for (temp_int_1 = 0; temp_int_1 < PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT; ++temp_int_1) {
        // Skip tiles which are not uncleared (this includes marked tiles)
        if (playfield[temp_int_1] != PLAYFIELD_UNCLEARED) {
            continue;
        }

        // Change tile to cleared
        playfield[temp_int_1] = PLAYFIELD_WALL;
        temp_byte_1 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) % 32) << 3;
        temp_byte_2 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) >> 5) << 3;
        // Set the bg tile
        one_vram_buffer(TILE_INDEX_PLAYFIELD_CLEARED, get_ppu_addr(0, temp_byte_1, temp_byte_2));
    }
}

void reset_playfield_mark_bit(void) {
    for (temp_int_1 = 0; temp_int_1 < PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT; ++temp_int_1) {
        playfield[temp_int_1] &= ~PLAYFIELD_BITMASK_MARK;
    }
}

// temp_byte_1 is the ball index
void compute_playfield_mark_bit_one_ball(void) {
    temp_int_1 = balls[temp_byte_1].nearest_playfield_tile;

    // Simple flood fill scan-and-fill implementation.
    // Mark all the uncleared playfield tiles inside the region enclosing this ball.

    // if not Inside(x, y) then return
    temp_byte_2 = playfield[temp_int_1];
    if ((temp_byte_2 & PLAYFIELD_BITMASK_MARK) == PLAYFIELD_BITMASK_MARK) {
        return;
    }

    // let s = new empty queue or stack
    stack_init();

    // Add (x, x, y, 1) to s
    // (x,y)
    stack_temp = temp_int_1;
    stack_push();
    // (x,1)
    stack_temp = (temp_int_1 % 32) | (0x1 << 8);
    stack_push();

    // Add (x, x, y - 1, -1) to s
    // (x,y-1)
    stack_temp = temp_int_1 - 32;
    stack_push();
    // (x,-1)
    stack_temp = (temp_int_1 % 32) | (0xff << 8);
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
        temp_byte_3 = stack_temp % 32;

        // if Inside(x, y):
        if (playfield[temp_int_1] == PLAYFIELD_UNCLEARED) {
            // while Inside(x - 1, y):
            while (1) {
                // (x-1,y)
                stack_temp = temp_int_1 - 1;
                if (playfield[stack_temp] != PLAYFIELD_UNCLEARED) {
                    break;
                }
                // Set(x - 1, y)
                playfield[stack_temp] |= PLAYFIELD_BITMASK_MARK;
                // x = x - 1
                temp_int_1 = stack_temp;

#if DEBUG
                temp_byte_1 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) % 32) << 3;
                temp_byte_4 = ((temp_int_1 + PLAYFIELD_FIRST_TILE_INDEX) >> 5) << 3;
                // Set the bg tile
                one_vram_buffer(TILE_INDEX_PLAYFIELD_CLEARED, get_ppu_addr(0, temp_byte_1, temp_byte_4));
#endif
            }
        }

        // if x < x1:
        if ((temp_int_1 % 32) < temp_byte_3) {
            // Add (x, x1-1, y-dy, -dy) to s
            // (x,y-dy)
            stack_temp = temp_int_1 - temp_signed_byte_1 * 32;
            stack_push();
            // (x1-1,-dy)
            stack_temp = (temp_byte_3 - 1) | ((-1 * temp_signed_byte_1) << 8);
            stack_push();
        }

        // while x1 < x2:
        while (1) {
            if (temp_byte_3 >= temp_byte_2) {
                break;
            }
            // while Inside(x1, y):
            while (1) {
                if (playfield[temp_int_2] != PLAYFIELD_UNCLEARED) {
                    break;
                }
                // Set(x1, y)
                playfield[temp_int_2] |= PLAYFIELD_BITMASK_MARK;

#if DEBUG
                temp_byte_1 = ((temp_int_2 + PLAYFIELD_FIRST_TILE_INDEX) % 32) << 3;
                temp_byte_4 = ((temp_int_2 + PLAYFIELD_FIRST_TILE_INDEX) >> 5) << 3;
                // Set the bg tile
                one_vram_buffer(TILE_INDEX_PLAYFIELD_CLEARED, get_ppu_addr(0, temp_byte_1, temp_byte_4));
#endif

                // x1 = x1 + 1
                ++temp_int_2;
            }

            // Update x1
            temp_byte_3 = temp_int_2 % 32;

#if DEBUG
            if (stack_top > 0x40) {
                return;
            }
#endif

            // Add (x, x1 - 1, y+dy, dy) to s
            // (x,y+dy)
            stack_temp = temp_int_1 + temp_signed_byte_1 * 32;
            stack_push();
            // (x1-1,dy)
            stack_temp = (temp_byte_3 - 1) | (temp_signed_byte_1 << 8);
            stack_push();

            // if x1 - 1 > x2:
            if ((temp_byte_3 - 1) > temp_byte_2) {
                // Add (x2 + 1, x1 - 1, y-dy, -dy)
                // (x2+1,y-dy)
                stack_temp = (temp_byte_2 + 1) + (((temp_int_2 >> 5) - temp_signed_byte_1) << 5);
                stack_push();
                // (x1-1,-dy)
                stack_temp = (temp_byte_3 - 1) | ((-1 * temp_signed_byte_1) << 8);
                stack_push();
            }

            // while x1 < x2 and not Inside(x1, y):
            while (1) {
                if (temp_byte_3 >= temp_byte_2 || playfield[temp_int_2] == PLAYFIELD_UNCLEARED) {
                    break;
                }
                // x1 = x1 + 1
                ++temp_int_2;
                temp_byte_3 = temp_int_2 % 32;
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
