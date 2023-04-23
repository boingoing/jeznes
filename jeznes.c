#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "sprites.h"
#include "playfield.h"
#include "jeznes.h"

void main(void) {
    // Turn off the screen.
    ppu_off();

    // Sprites are in the 0th CHR bank.
    bank_spr(0);
    // Tiles are in the 1st CHR bank.
    bank_bg(1);

    init_title();

    set_vram_buffer();
    clear_vram_buffer();

    // Turn on the screen.
    ppu_on_all();

    while (1) {
        ppu_wait_nmi();

        // Read the controllers.
        read_controllers();

        // Do at the beginning of each frame.
        clear_vram_buffer();

        if (game_state == GAME_STATE_TITLE) {
            // Clear all sprites from the sprite buffer.
            oam_clear();

            // Handle player pressing start.
            start_game();
        } else if (game_state == GAME_STATE_PLAYING) {
            // Clear all sprites from the sprite buffer.
            oam_clear();

            for (temp_byte_1 = 0; temp_byte_1 < get_player_count(); temp_byte_1++) {
                // Respond to player gamepad.
                flip_player_orientation(temp_byte_1);
                start_line(temp_byte_1);

                // Move the player position in the playfield.
                move_player(temp_byte_1);

                // Draw the player, the playfield tile highlight, and the in-progress line sprites.
                draw_player(temp_byte_1);
                draw_tile_highlight(temp_byte_1);
                draw_line(temp_byte_1);

                // Update the line for this player if there's one in progress.
                update_line(temp_byte_1);
            }

            // Move the ball positions in the playfield.
            move_balls();

            // Check to see if any balls have collided with any lines.
            check_ball_line_collisions();

            // Draw the ball sprites.
            draw_balls();
        } else if (game_state == GAME_STATE_LEVEL_UP) {
            // Clear all sprites from the sprite buffer.
            oam_clear();

            // Just perform a level up.
            do_level_up();
        } else if (game_state == GAME_STATE_GAME_OVER) {
            // Clear all sprites from the sprite buffer.
            oam_clear();

            // Handle player pressing select.
            game_over_change_mode();
            // Handle player pressing start.
            game_over_press_start();

            // Draw the cursor sprite wherever it should be.
            draw_game_over_cursor();
        } else if (game_state == GAME_STATE_UPDATING_PLAYFIELD) {
            // Restart the update of cleared playfield tiles.
            if (update_cleared_playfield_tiles() == TRUE) {
                // We might have cleared tiles, let's update the HUD.
                game_state = GAME_STATE_REQUEST_HUD_UPDATE;

                // We finished updating the playfield tiles, let's remove the mark bits.
                reset_playfield_mark_bit();
            }
        } else if (game_state == GAME_STATE_REQUEST_HUD_UPDATE) {
            // Update the level, lives remaining, percentages, etc.
            update_hud();

            // Then reset the game state to playing.
            game_state = GAME_STATE_PLAYING;

            // The cleared tile percentage is updated via update_hud().
            // Because that's an expensive operation, let's not redo it anywhere.
            // If we detect the target percentage has been reached, switch to the
            // level up state instead.
            if (cleared_tile_percentage > TARGET_CLEARED_TILE_PERCENTAGE) {
                game_state = GAME_STATE_LEVEL_UP;
            }
        }

#if DEBUG
#if DRAW_GRAY_LINE
        // For debugging, render a line indicating how much CPU is used.
        gray_line();
#endif
#endif
    }
}

void init_title(void) {
    // Load title screen graphics.
    pal_bg(title_bg_palette);
    pal_spr(title_sprite_palette);
    vram_adr(NAMETABLE_A);
    vram_unrle(title_screen);

    // Starting or returning to the title screen.
    game_state = GAME_STATE_TITLE;
}

void start_game(void) {
    if (pads[0] & PAD_START) {
        if (get_player_is_pause_pressed(0)) {
            return;
        }

        set_player_is_pause_pressed(0);

        // Fade to black
        pal_fade_to(4, 0);
        // Screen off
        ppu_off();

        init_game();

        // Screen on
        ppu_on_all();
        // Back to normal brightness
        pal_bright(4);
    } else {
        unset_player_is_pause_pressed(0);
    }
}

void init_game(void) {
    static unsigned char player_default_x[2] = {0x56, 0x96};
    static unsigned char player_default_y[2] = {0x46, 0x86};

    // Seed the random number generator - it's based on frame count.
    seed_rng();

    // Starting game state.
    game_state = GAME_STATE_PLAYING;
    current_level = 1;
    lives_count = current_level + 1;

    // Player initial positions.
    for (temp_byte_1 = 0; temp_byte_1 < get_player_count(); ++temp_byte_1) {
        players[temp_byte_1].x = player_default_x[temp_byte_1];
        players[temp_byte_1].y = player_default_y[temp_byte_1];
        update_nearest_tile(temp_byte_1);

        set_player_orientation_flag(temp_byte_1, ORIENTATION_HORIZ);
        unset_player_is_place_pressed_flag(temp_byte_1);
        unset_player_is_rotate_pressed(temp_byte_1);
    }

    // Always loads |current_level|
    reset_playfield();
}

void load_playfield(unsigned char playfield_index) {
    memcpy(&playfield, playfield_patterns[playfield_index], PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT);
}

void read_controllers(void) {
    for (temp_byte_1 = 0; temp_byte_1 < MAX_PLAYERS; ++temp_byte_1) {
        pads[temp_byte_1] = pad_poll(temp_byte_1);
        pads_new[temp_byte_1] = get_pad_new(temp_byte_1);
    }
}

void reset_playfield() {
    // Reset per-level state.
    cleared_tile_count = 0;
    cleared_tile_percentage = 0;

    // Make sure no lines will be rendered.
    unset_line_is_started_flag(0);
    unset_line_is_started_flag(1);

    // Ball random initial positions and directions.
    for (temp_byte_1 = 0; temp_byte_1 < get_ball_count(); ++temp_byte_1) {
        balls[temp_byte_1].x = rand8() % (0xff - 0x30) + 0x18;
        balls[temp_byte_1].y = rand8() % (0xff - 0x70) + 0x20;
        if (rand8() > 0x7f) {
            balls[temp_byte_1].x_velocity = BALL_SPEED;
        } else {
            balls[temp_byte_1].x_velocity = -BALL_SPEED;
        }
        if (rand8() > 0x7f) {
            balls[temp_byte_1].y_velocity = BALL_SPEED;
        } else {
            balls[temp_byte_1].y_velocity = -BALL_SPEED;
        }
    }

    // Load playfield graphics.
    // Note: Screen must be off!
    pal_bg(playfield_bg_palette);
    pal_spr(playfield_sprite_palette);
    vram_adr(NAMETABLE_A);
    vram_unrle(playfield_screen);

    // Load playfield pattern.
    load_playfield(0);

    // Display the level, lives remaining, percentages, etc.
    update_hud();
}

void do_level_up(void) {
    // Actually level up the player.
    current_level++;
    lives_count++;

    // Turn off the screen.
    ppu_off();

    // Redraw the playfield and reset the balls while adding one more.
    reset_playfield();

    // Turn on the screen.
    ppu_on_all();

    // Reset state to playing the game.
    game_state = GAME_STATE_PLAYING;
}

void change_to_game_over(void) {
    // Fade to black
    pal_fade_to(4, 0);
    // Screen off
    ppu_off();

    // Load game over screen graphics.
    vram_adr(NAMETABLE_A);
    vram_unrle(game_over_screen);

    // Screen on
    ppu_on_all();
    // Back to normal brightness
    pal_bright(4);

    set_game_over_mode(GAME_OVER_RETRY);
    game_state = GAME_STATE_GAME_OVER;
}

void game_over_change_mode(void) {
    if (pads[0] & PAD_SELECT) {
        // Ignore the mode change if player holds the button.
        if (get_player_is_select_pressed(0)) {
            return;
        }

        // Track player is holding the button.
        set_player_is_select_pressed(0);

        // Toggle game over mode between retry and quit.
        set_game_over_mode(get_game_over_mode() ^ 1);
    } else {
        // No longer holding down the button.
        unset_player_is_select_pressed(0);
    }
}

void game_over_press_start(void) {
    if (pads[0] & PAD_START) {
        if (get_player_is_pause_pressed(0)) {
            return;
        }

        set_player_is_pause_pressed(0);

        if (get_game_over_mode() == GAME_OVER_RETRY) {
            // Reset our lives count back to the default.
            lives_count = current_level + 1;

            // Screen off
            ppu_off();

            reset_playfield();

            // Screen on
            ppu_on_all();

            game_state = GAME_STATE_PLAYING;
        } else {
            // get_game_over_mode() == GAME_OVER_QUIT

            // Fade to black
            pal_fade_to(4, 0);
            // Screen off
            ppu_off();

            // Back to the title screen
            init_title();

            // Screen on
            ppu_on_all();
            // Back to normal brightness
            pal_bright(4);
        }
    } else {
        unset_player_is_pause_pressed(0);
    }
}

#define get_tile_alphanumeric_number(v) (TILE_INDEX_ALPHANUMERIC_ZERO + (v))

void write_two_digit_number_to_bg(unsigned char num, unsigned char tile_x, unsigned char tile_y) {
    one_vram_buffer(get_tile_alphanumeric_number(num / 10), NTADR_A(tile_x, tile_y));
    one_vram_buffer(get_tile_alphanumeric_number(num % 10), NTADR_A(tile_x+1, tile_y));
}

void update_hud(void) {
    write_two_digit_number_to_bg(current_level, HUD_LEVEL_DISPLAY_TILE_X, HUD_LEVEL_DISPLAY_TILE_Y);
    write_two_digit_number_to_bg(lives_count, HUD_LIVES_DISPLAY_TILE_X, HUD_LIVES_DISPLAY_TILE_Y);
    write_two_digit_number_to_bg(TARGET_CLEARED_TILE_PERCENTAGE, HUD_TARGET_DISPLAY_TILE_X, HUD_TARGET_DISPLAY_TILE_Y);

    cleared_tile_percentage = (cleared_tile_count * 100) / playfield_pattern_uncleared_tile_counts[0];
    write_two_digit_number_to_bg(cleared_tile_percentage, HUD_CLEAR_DISPLAY_TILE_X, HUD_CLEAR_DISPLAY_TILE_Y);
}

#define get_pixel_coord_x() (temp_byte_4)
#define set_pixel_coord_x(a) (temp_byte_4 = (a))
#define get_pixel_coord_y() (temp_byte_4)
#define set_pixel_coord_y(a) (temp_byte_4 = (a))

void move_player(unsigned char player_index) {
    if (pads[player_index] & PAD_LEFT) {
        set_pixel_coord_x(players[player_index].x - PLAYER_SPEED);
        if (get_player_orientation_flag(player_index) == ORIENTATION_VERT) {
            temp_byte_2 = PLAYFIELD_LEFT_WALL;
        } else {
            temp_byte_2 = PLAYFIELD_LEFT_WALL + 8;
        }
        if (get_pixel_coord_x() <= temp_byte_2) {
            players[player_index].x = temp_byte_2;
        } else {
            players[player_index].x = get_pixel_coord_x();
        }
        update_nearest_tile(player_index);
    } else if (pads[player_index] & PAD_RIGHT) {
        set_pixel_coord_x(players[player_index].x + PLAYER_SPEED);
        if (get_pixel_coord_x() >= PLAYFIELD_RIGHT_WALL) {
            players[player_index].x = PLAYFIELD_RIGHT_WALL;
        } else {
            players[player_index].x = get_pixel_coord_x();
        }
        update_nearest_tile(player_index);
    }

    if (pads[player_index] & PAD_DOWN) {
        set_pixel_coord_y(players[player_index].y + PLAYER_SPEED);
        if (get_pixel_coord_y() >= PLAYFIELD_BOTTOM_WALL) {
            players[player_index].y = PLAYFIELD_BOTTOM_WALL;
        } else {
            players[player_index].y = get_pixel_coord_y();
        }
        update_nearest_tile(player_index);
    } else if (pads[player_index] & PAD_UP) {
        set_pixel_coord_y(players[player_index].y - PLAYER_SPEED);
        if (get_player_orientation_flag(player_index) == ORIENTATION_VERT) {
            temp_byte_2 = PLAYFIELD_TOP_WALL + 8;
        } else {
            temp_byte_2 = PLAYFIELD_TOP_WALL;
        }
        if (get_pixel_coord_y() <= temp_byte_2) {
            players[player_index].y = temp_byte_2;
        } else {
            players[player_index].y = get_pixel_coord_y();
        }
        update_nearest_tile(player_index);
    }
}

void move_balls(void) {
    for (temp_byte_1 = 0; temp_byte_1 < get_ball_count(); temp_byte_1++) {
        move_ball(temp_byte_1);
    }
}

void move_ball(unsigned char ball_index) {
    temp_signed_byte_1 = balls[ball_index].x_velocity;
    temp_byte_2 = balls[ball_index].x;
    temp_byte_2 += temp_signed_byte_1;

    // y tile coord
    temp_byte_5 = balls[ball_index].y >> 3;
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
    // Bounce off a wall tile
    if (playfield[temp_int_1] == PLAYFIELD_WALL) {
        balls[ball_index].x_velocity *= -1;
        temp_byte_2 = temp_byte_4;
    }
    balls[ball_index].x = temp_byte_2;

    temp_signed_byte_1 = balls[ball_index].y_velocity;
    temp_byte_3 = balls[ball_index].y;
    temp_byte_3 += temp_signed_byte_1;

    // x tile coord
    temp_byte_5 = balls[ball_index].x >> 3;
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
    // Bounce off a wall tile
    if (playfield[temp_int_1] == PLAYFIELD_WALL) {
        balls[ball_index].y_velocity *= -1;
        temp_byte_3 = temp_byte_4;
    }
    balls[ball_index].y = temp_byte_3;

    // Update nearest playfield tile
    temp_byte_2 = (temp_byte_2 + 4) >> 3;
    temp_byte_3 = (temp_byte_3 + 4) >> 3;
    balls[ball_index].nearest_playfield_tile = temp_byte_2 + (temp_byte_3 << 5) - PLAYFIELD_FIRST_TILE_INDEX;
}

void draw_balls(void) {
    temp_byte_2 = get_frame_count();
    temp_byte_2 = (temp_byte_2 >> 2) % 18 + TILE_INDEX_BALL_BASE;
    for (temp_byte_1 = 0; temp_byte_1 < get_ball_count(); ++temp_byte_1) {
        oam_spr(balls[temp_byte_1].x, balls[temp_byte_1].y, temp_byte_2, 0);

#if DRAW_BALL_NEAREST_TILE_HIGHLIGHT
        temp_int_1 = balls[temp_byte_1].nearest_playfield_tile;
        oam_spr(playfield_index_pixel_coord_x(temp_int_1), playfield_index_pixel_coord_y(temp_int_1) - 1, TILE_INDEX_TILE_HIGHLIGHT, 1);
#endif
    }
}

#define get_player_sprite_frame() (get_frame_count() >> 3 & 1)

void draw_player(unsigned char player_index) {
    temp_byte_2 = get_player_sprite_frame();
    if (get_player_orientation_flag(player_index) == ORIENTATION_HORIZ) {
        temp_byte_2 += 2;
    }
    oam_meta_spr(players[player_index].x, players[player_index].y, player_metasprite_list[temp_byte_2]);
}

void draw_tile_highlight(unsigned char player_index) {
    if (playfield[players[player_index].nearest_playfield_tile] == PLAYFIELD_UNCLEARED) {
        oam_spr(players[player_index].nearest_tile_x, players[player_index].nearest_tile_y - 1, TILE_INDEX_TILE_HIGHLIGHT, 1);
    }
}

void draw_game_over_cursor(void) {
    if (get_game_over_mode() == GAME_OVER_RETRY) {
        oam_spr(GAME_OVER_CURSOR_RETRY_X, GAME_OVER_CURSOR_RETRY_Y, TILE_INDEX_TILE_HIGHLIGHT, 1);
    } else {
        oam_spr(GAME_OVER_CURSOR_QUIT_X, GAME_OVER_CURSOR_QUIT_Y, TILE_INDEX_TILE_HIGHLIGHT, 1);
    }
}

#define get_line_orientation() (temp_byte_4)
#define set_line_orientation(a) (temp_byte_4 = (a))
#define get_current_playfield_index() (temp_int_1)
#define set_current_playfield_index(a) (temp_int_1 = (a))
#define get_was_line_segment_completed() (temp_byte_3)
#define set_was_line_segment_completed(a) (temp_byte_3 = (a))

#define get_tile_index_delta() (temp_byte_5)
#define set_tile_index_delta(a) (temp_byte_5 = (a))
#define get_negative_line_segment_origin() (temp_int_2)
#define set_negative_line_segment_origin(a) (temp_int_2 = (a))
#define get_positive_line_segment_origin() (temp_int_2)
#define set_positive_line_segment_origin(a) (temp_int_2 = (a))

// The tile index delta is the number we add to a playfield tile index to move
// to the next playfield tile along the line segment.
// |orientation| is the line orientation and must be one of ORIENTATION_HORIZ
// or ORIENTATION_VERT.
#define compute_tile_index_delta(orientation) ((orientation) * 31 + 1)

void update_line(unsigned char line_index) {
    // Do nothing if the line is not started.
    if (!get_line_is_started_flag(line_index)) {
        return;
    }

    set_was_line_segment_completed(FALSE);

    // The playfield tile at the front of each line segment is the only one
    // we need to worry about here.
    // The tile is 8 pixels wide and we draw one line of pixels at a time.
    // When we've reached completion of the tile, move the front of both
    // line segments forward in their directions.
    if (lines[line_index].current_block_completion == 8) {
        set_line_orientation(get_line_orientation_flag(line_index));
        set_tile_index_delta(compute_tile_index_delta(get_line_orientation()));
        set_negative_line_segment_origin(lines[line_index].origin);

        // Try and move the front of the negative-direction line segment forward
        // and complete the line segment if it's reached a wall.
        if (!get_line_is_negative_complete_flag(line_index)) {
            // Before moving the line segment forward, update the metadata for the tile we're moving from.
            set_current_playfield_index(get_negative_line_segment_origin() - lines[line_index].tile_step_count * get_tile_index_delta());

            // While it was the front of the line segment, current playfield tile was being
            // drawn as a sprite. Now that it's complete, update the playfield and bg tile.
            // Note: The origin tile already has the playfield flags (and bg tile) set. We
            // can ignore that one (when tile_step_count == 0).
            if (lines[line_index].tile_step_count != 0) {
                set_playfield_tile(get_current_playfield_index(), get_playfield_tile_type_line(get_line_orientation(), line_index, LINE_DIRECTION_NEGATIVE), get_playfield_bg_tile_line(get_line_orientation()));
            }

            // Now move the front of the line segment forward by one tile.
            set_current_playfield_index(get_current_playfield_index() - get_tile_index_delta());

            // If the next tile is not an uncleared tile, that means we hit the end for the line segment.
            // Walk back over the tiles until we reach line segment origin and update them to cleared.
            if (get_playfield_tile_type(get_current_playfield_index()) == PLAYFIELD_WALL) {
                while (1) {
                    // Walk back towards origin by one tile.
                    set_current_playfield_index(get_current_playfield_index() + get_tile_index_delta());
                    // Update the tile to cleared.
                    cleared_tile_count++;
                    set_playfield_tile(get_current_playfield_index(), PLAYFIELD_WALL, TILE_INDEX_PLAYFIELD_CLEARED);
                    // Stop when we reach the origin.
                    if (get_current_playfield_index() == get_negative_line_segment_origin()) {
                        break;
                    }
                }

                // Finished the negative-direction line segment.
                set_line_is_negative_complete_flag(line_index);

                // When line segments in both directions are complete, the line is done.
                if (get_line_is_positive_complete_flag(line_index)) {
                    unset_line_is_started_flag(line_index);
                }

                set_was_line_segment_completed(TRUE);
            }
        }

        // Now do the positive direction.
        if (!get_line_is_positive_complete_flag(line_index)) {
            // Calculate positive-direction line segment origin based
            // on negative-direction line segment origin.
            set_positive_line_segment_origin(get_negative_line_segment_origin() + get_tile_index_delta());

            // Before moving the line segment forward, update the metadata for the tile we're moving from.
            set_current_playfield_index(get_positive_line_segment_origin() + lines[line_index].tile_step_count * get_tile_index_delta());

            // While it was the front of the line segment, current playfield tile was being
            // drawn as a sprite. Now that it's complete, update the playfield and bg tile.
            // Note: The origin tile already has the playfield flags (and bg tile) set. We
            // can ignore that one (when tile_step_count == 0).
            if (lines[line_index].tile_step_count != 0) {
                set_playfield_tile(get_current_playfield_index(), get_playfield_tile_type_line(get_line_orientation(), line_index, LINE_DIRECTION_POSITIVE), get_playfield_bg_tile_line(get_line_orientation()));
            }

            // Now move the front of the line segment forward by one tile.
            set_current_playfield_index(get_current_playfield_index() + get_tile_index_delta());

            // If the next tile is not an uncleared tile, that means we hit the end for the line segment.
            // Walk back over the tiles until we reach line segment origin and update them to cleared.
            if (get_playfield_tile_type(get_current_playfield_index()) == PLAYFIELD_WALL) {
                while (1) {
                    // Walk back towards origin by one tile.
                    set_current_playfield_index(get_current_playfield_index() - get_tile_index_delta());
                    // Update the tile to cleared.
                    cleared_tile_count++;
                    set_playfield_tile(get_current_playfield_index(), PLAYFIELD_WALL, TILE_INDEX_PLAYFIELD_CLEARED);
                    // Stop when we reach the origin.
                    if (get_current_playfield_index() == get_positive_line_segment_origin()) {
                        break;
                    }
                }

                // Finished the positive-direction line segment.
                set_line_is_positive_complete_flag(line_index);

                // When line segments in both directions are complete, the line is done.
                if (get_line_is_negative_complete_flag(line_index)) {
                    unset_line_is_started_flag(line_index);
                }

                set_was_line_segment_completed(TRUE);
            }
        }

        lines[line_index].current_block_completion = 0;
        lines[line_index].tile_step_count++;
    } else {
        if (get_frame_count() % 3 == 0) {
            ++lines[line_index].current_block_completion;
        }
    }

    if (get_was_line_segment_completed()) {
        line_completed();
    }
}

void start_line(unsigned char player_index) {
    if (pads[player_index] & PAD_A) {
        // Do nothing if the player is holding the button and we already handled the press.
        if (get_player_is_place_pressed_flag(player_index)) {
            return;
        }

        // Keep track that user is pressing this button.
        set_player_is_place_pressed_flag(player_index);

        // Do nothing if a line is already started for |player_index|.
        if (get_line_is_started_flag(player_index)) {
            return;
        }

        // Origin for the line is whatever tile we're tracking as "nearest" to the player metasprite.
        // This is technically the origin tile for the negative-direction line segment.
        // The origin tile for the positive-direction line segment is origin + 1 (for horiz line) or
        // origin + 32 (for vert line) but we only keep track of one origin in the line itself.
        set_negative_line_segment_origin(players[player_index].nearest_playfield_tile);

        // We only want to start a line if the origin tile is not already cleared.
        if (get_playfield_tile_type(get_negative_line_segment_origin()) == PLAYFIELD_WALL) {
            return;
        }

        // Orientation of the line itself matches the current orientation of the player.
        set_line_orientation(get_player_orientation_flag(player_index));

        // Update the playfield origin tile.
        set_playfield_tile(get_negative_line_segment_origin(), get_playfield_tile_type_line(get_line_orientation(), player_index, LINE_DIRECTION_NEGATIVE), get_playfield_bg_tile_line(get_line_orientation()));

        // Update the line data for the negative-direction line segment.
        lines[player_index].origin = get_negative_line_segment_origin();
        unset_line_is_negative_complete_flag(player_index);

        // Now check to see if we can start a positive-direction line segment.
        set_tile_index_delta(compute_tile_index_delta(get_line_orientation()));
        set_positive_line_segment_origin(get_negative_line_segment_origin() + get_tile_index_delta());

        // We can only start the positive-direction line segment if it would have origin on an
        // uncleared playfield tile.
        if (get_playfield_tile_type(get_positive_line_segment_origin()) != PLAYFIELD_WALL) {
            // Update the positive-direction line segment origin playfield tile.
            set_playfield_tile(get_positive_line_segment_origin(), get_playfield_tile_type_line(get_line_orientation(), player_index, LINE_DIRECTION_POSITIVE), get_playfield_bg_tile_line(get_line_orientation()));
            unset_line_is_positive_complete_flag(player_index);
        }

        // Current line segment front tile is the origin tile.
        lines[player_index].tile_step_count = 0;
        // The origin tiles start at complete.
        lines[player_index].current_block_completion = 8;

        set_line_orientation_flag(player_index, get_line_orientation());
        set_line_is_started_flag(player_index);
    } else {
        unset_player_is_place_pressed_flag(player_index);
    }
}

void draw_line(unsigned char line_index) {
    if (get_line_is_started_flag(line_index)) {
        set_line_orientation(get_line_orientation_flag(line_index));
        set_tile_index_delta(compute_tile_index_delta(get_line_orientation()));
        set_negative_line_segment_origin(lines[line_index].origin);

        if (!get_line_is_negative_complete_flag(line_index)) {
            set_current_playfield_index(get_negative_line_segment_origin() - lines[line_index].tile_step_count * get_tile_index_delta());

            // Negative-direction line segment front tile sprite needs to be flipped (OAM_FLIP_H 0x40).
            // If the line is vertical, we need to do vertical flip instead (OAM_FLIP_V 0x80).
            temp_byte_2 = OAM_FLIP_H + OAM_FLIP_H * get_line_orientation();
            oam_spr(playfield_index_pixel_coord_x(get_current_playfield_index()), playfield_index_pixel_coord_y(get_current_playfield_index()) - 1, get_line_sprite_index(get_line_orientation(), lines[line_index].current_block_completion), 1 | temp_byte_2);
        }

        if (!get_line_is_positive_complete_flag(line_index)) {
            set_positive_line_segment_origin(get_negative_line_segment_origin() + get_tile_index_delta());
            set_current_playfield_index(get_positive_line_segment_origin() + lines[line_index].tile_step_count * get_tile_index_delta());
            oam_spr(playfield_index_pixel_coord_x(get_current_playfield_index()), playfield_index_pixel_coord_y(get_current_playfield_index()) - 1, get_line_sprite_index(get_line_orientation(), lines[line_index].current_block_completion), 1);
        }
    }
}

void check_ball_line_collisions(void) {
    for (temp_byte_1 = 0; temp_byte_1 < get_ball_count(); ++temp_byte_1) {
        set_current_playfield_index(balls[temp_byte_1].nearest_playfield_tile);
        temp_byte_2 = playfield[get_current_playfield_index()];

        if (get_playfield_tile_type_from_byte(temp_byte_2) != PLAYFIELD_LINE) {
            // No collision.
            continue;
        }

        set_line_orientation(get_playfield_line_orientation_flag_from_byte(temp_byte_2));
        set_tile_index_delta(compute_tile_index_delta(get_line_orientation()));
        temp_byte_3 = get_playfield_line_index_flag_from_byte(temp_byte_2);
        set_negative_line_segment_origin(lines[temp_byte_3].origin);

        if (get_playfield_line_direction_flag_from_byte(temp_byte_2) == LINE_DIRECTION_NEGATIVE) {
            // Playfield tile index of the line segment front tile.
            // This is drawn as a sprite and we haven't updated the playfield
            // metadata to include the line flags for this tile so we don't need
            // to do anything to the playfield for this tile index.
            set_current_playfield_index(get_negative_line_segment_origin() - lines[temp_byte_3].tile_step_count * get_tile_index_delta());

            // Walk back across the line segment (to origin) and reset the playfield tiles to uncleared.
            while (1) {
                // Walk back towards origin by one tile.
                set_current_playfield_index(get_current_playfield_index() + get_tile_index_delta());
                // Reset the tile to uncleared.
                set_playfield_tile(get_current_playfield_index(), PLAYFIELD_UNCLEARED, TILE_INDEX_PLAYFIELD_UNCLEARED);
                // Stop when we reach the origin.
                if (get_current_playfield_index() == get_negative_line_segment_origin()) {
                    break;
                }
            }

            // Turn off the negative-direction line segment - the collision stopped it.
            set_line_is_negative_complete_flag(temp_byte_3);

            // If the other direction is also complete, reset the is_started flag of the line.
            if (get_line_is_positive_complete_flag(temp_byte_3)) {
                unset_line_is_started_flag(temp_byte_3);
            }
        } else {
            set_positive_line_segment_origin(get_negative_line_segment_origin() + get_tile_index_delta());

            // Playfield tile index of the line segment front tile.
            // This is drawn as a sprite and we haven't updated the playfield
            // metadata to include the line flags for this tile so we don't need
            // to do anything to the playfield for this tile index.
            set_current_playfield_index(get_positive_line_segment_origin() + lines[temp_byte_3].tile_step_count * get_tile_index_delta());

            // Walk back across the line segment (to origin) and reset the playfield tiles to uncleared.
            while (1) {
                // Walk back towards origin by one tile.
                set_current_playfield_index(get_current_playfield_index() - get_tile_index_delta());
                // Reset the tile to uncleared.
                set_playfield_tile(get_current_playfield_index(), PLAYFIELD_UNCLEARED, TILE_INDEX_PLAYFIELD_UNCLEARED);
                // Stop when we reach the origin.
                if (get_current_playfield_index() == get_positive_line_segment_origin()) {
                    break;
                }
            }

            // Turn off the positive-direction line segment - the collision stopped it.
            set_line_is_positive_complete_flag(temp_byte_3);

            // If the other direction is also complete, reset the is_started flag of the line.
            if (get_line_is_negative_complete_flag(temp_byte_3)) {
                unset_line_is_started_flag(temp_byte_3);
            }
        }

        lives_count--;

        if (lives_count == 0) {
            // We ran out of lives, move to game over state.
            change_to_game_over();
        } else {
            // We changed the lives count, let's redraw the HUD.
            game_state = GAME_STATE_REQUEST_HUD_UPDATE;
        }
    }
}

void flip_player_orientation(unsigned char player_index) {
    if (pads[player_index] & PAD_B) {
        if (get_player_is_rotate_pressed(player_index)) {
            return;
        }

        set_player_is_rotate_pressed(player_index);
        set_player_orientation_flag(player_index, get_player_orientation_flag(player_index) ^ 1);

        if (get_player_orientation_flag(player_index) == ORIENTATION_HORIZ) {
            temp_byte_2 = PLAYFIELD_LEFT_WALL + 8;
            if (players[player_index].x <= temp_byte_2) {
                players[player_index].x = temp_byte_2;
            }
        } else {
            temp_byte_2 = PLAYFIELD_TOP_WALL + 8;
            if (players[player_index].y <= temp_byte_2) {
                players[player_index].y = temp_byte_2;
            }
        }

        update_nearest_tile(player_index);
    } else {
        unset_player_is_rotate_pressed(player_index);
    }
}

// Don't modify temp_byte_1, temp_byte_2
void update_nearest_tile(unsigned char player_index) {
    if (get_player_orientation_flag(player_index) == ORIENTATION_VERT) {
        temp_byte_3 = players[player_index].x + 4;
        temp_byte_4 = players[player_index].y;
    } else {
        temp_byte_3 = players[player_index].x;
        temp_byte_4 = players[player_index].y + 4;
    }
    temp_byte_3 = temp_byte_3 >> 3;
    temp_byte_4 = temp_byte_4 >> 3;

    players[player_index].nearest_tile_x = temp_byte_3 << 3;
    players[player_index].nearest_tile_y = temp_byte_4 << 3;
    players[player_index].nearest_playfield_tile = temp_byte_3 + (temp_byte_4 << 5) - PLAYFIELD_FIRST_TILE_INDEX;
}

void line_completed(void) {
    unsigned char i;
    reset_playfield_mark_bit();

    for (i = 0; i < get_ball_count(); ++i) {
        compute_playfield_mark_bit_one_ball(i);
    }

    // Reset |playfield_index|, set the game state to updating the playfield, which will cause us to call
    // update_cleared_playfield_tiles() from the beginning next frame.
    // If we need to call it again after that, we will call it in restartable mode.
    set_playfield_index(0);
    game_state = GAME_STATE_UPDATING_PLAYFIELD;
}

// Updates the playfield in-memory data structure and sets |tile_index| to |playfield_tile_type|.
// Sets the bg tile graphic for |tile_index| to |playfield_bg_tile|.
//
// |playfield_tile_type| has the new playfield tile type (ie: PLAYFIELD_WALL)
// |playfield_bg_tile| has the new bg tile graphic index (ie: TILE_INDEX_PLAYFIELD_CLEARED)
void set_playfield_tile(unsigned int tile_index, unsigned char playfield_tile_type, unsigned char playfield_bg_tile) {
    // Update the playfield in-memory structure.
    playfield[tile_index] = playfield_tile_type;
    // Set the bg tile graphic
    one_vram_buffer(playfield_bg_tile, get_ppu_addr(0, playfield_index_pixel_coord_x(tile_index), playfield_index_pixel_coord_y(tile_index)));
}

// Update uncleared, unmarked playfield tiles to be cleared. Returns TRUE when all uncleared tiles have
// been updated.
// Note: This function can potentially queue more vram updates than are allowed during the next v-blank.
//       For that reason, it is restartable.
//       The current playfield_index needs to be reset to zero once at the beginning of the operation.
//       Otherwise, calling this function will continue from where it left off last time.
//       It returns TRUE when all vram updates are queued and FALSE if there are additonal vram updates pending.
//
// scratch:
// temp_byte_3
unsigned char update_cleared_playfield_tiles(void) {
    temp_byte_3 = 0;
    // Look over all tiles in the playfield and for each uncleared, unmarked tile change it to cleared
    for (; get_playfield_index() < PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT; inc_playfield_index()) {
        // Skip tiles which are not uncleared (this includes marked tiles)
        if (playfield[get_playfield_index()] != PLAYFIELD_UNCLEARED) {
            continue;
        }

        temp_byte_3++;
        cleared_tile_count++;
        set_playfield_tile(get_playfield_index(), PLAYFIELD_WALL, TILE_INDEX_PLAYFIELD_CLEARED);

        // We can only queue about 40 tile updates per v-blank.
        if (temp_byte_3 >= 40) {
            return FALSE;
        }
    }

    return TRUE;
}

// Reset the mark bit in all playfield tiles.
//
// scratch:
// temp_int_1
void reset_playfield_mark_bit(void) {
    for (temp_int_1 = 0; temp_int_1 < PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT; ++temp_int_1) {
        unset_playfield_is_marked_flag(temp_int_1);
    }
}

#define playfield_index_move_up(i) ((i) - 32)
#define playfield_index_move_down(i) ((i) + 32)
#define playfield_index_move_left(i) ((i) - 1)
#define playfield_index_move_right(i) ((i) + 1)

#define inside(i) ((playfield[(i)] & (PLAYFIELD_WALL | PLAYFIELD_BITMASK_MARK)) == 0)

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

// Uses a constant-memory usage implementation of the painters algorithm to
// walk the playfield starting at the playfield tile where |ball_index| is
// currently located. Each reachable playfield tile is marked until we run
// out of unmarked playfield tiles to walk to.
// When this function returns, the region in which |ball_index| is bound will
// be made up entirely of marked playfield tiles.
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
                set_playfield_is_marked_flag(get_cur());
                goto PAINTER_ALGORITHM_PAINT;
            }
            break;
        case 2:
            if (!inside(get_back())) {
                if (inside(get_front_left())) {
                    set_mark_null(TRUE);
                    set_playfield_is_marked_flag(get_cur());
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
                            set_playfield_is_marked_flag(get_cur());
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
                        set_playfield_is_marked_flag(get_cur());
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
            set_playfield_is_marked_flag(get_cur());
            goto PAINTER_ALGORITHM_PAINT;
            break;
        case 4:
            set_playfield_is_marked_flag(get_cur());
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
                set_playfield_is_marked_flag(stack_temp);
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
                set_playfield_is_marked_flag(temp_int_2);
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
