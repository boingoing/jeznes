//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_JEZNES_H__
#define __JEZNES_JEZNES_H__

#include "debug.h"

#define rand2() (rand8() % 2)

int main(void);

// Initializes the locations and directions of the active balls on the valid
// ball region of the playfield pattern defined by |current_playfield_pattern|.
void init_balls(void);

void init_title(void);
void title_change_mode(void);

// Handle player start button presses for the title screen.
// Returns TRUE if the player did press start.
unsigned char title_press_start(void);
void draw_title_cursor(void);

// Handle player start button presses for the pause screen.
// Returns TRUE if the player did press start.
unsigned char pause_press_start(unsigned char player_index);

// Draw the words PAUSE on the screen.
void draw_pause_sprites(void);

void init_game(void);
void reset_playfield(void);

// Loads the playfield pattern defined by |current_playfield_pattern| into the
// in-memory playfield.
void load_playfield(void);

void change_to_game_over(void);
void game_over_change_mode(void);

// Draw the score, level, etc stats on the game over screen.
void game_over_update_hud(void);

// Handle player start button presses for the game over screen.
// Returns TRUE if the player did press start.
unsigned char game_over_press_start(void);
void draw_game_over_cursor(void);

#if ENABLE_CHEATS
// Set initial state for cheat flags.
void init_cheat_flags(void);

// Handle player cheat button presses.
void handle_cheat_buttons(void);
#endif  // ENABLE_CHEATS

// Perform a level up:
// Increment the current level
// Add one to the current lives counter
// Add one to the current number of balls
// Reset the playfield
// Change the game state to in-game
void do_level_up(void);

void do_level_down(void);

void read_controllers(void);

void move_player(unsigned char player_index);

// Moves one ball which needs to have been stored already in |temp_ptr_1|.
void move_ball();

// Moves each ball into the next position and then draws it.
void move_and_draw_balls(void);

void check_ball_line_collisions(void);

void start_line(unsigned char player_index);
void flip_player_orientation(unsigned char player_index);

// Update the player nearest tile values for the player stored in |temp_ptr_1|.
// Should be called when the player sprite moves, changes orientation, etc.
void update_nearest_tile(void);

// Update the metadata for the in-progress line at |line_index|. Returns TRUE if
// the line completed.
unsigned char update_line(unsigned char line_index);

// Draws player stored in |temp_ptr_1|.
void draw_player(void);

// Draws the highlight tile under player stored in |temp_ptr_1|.
void draw_tile_highlight(void);

// Draws an in-progress line stored in |temp_ptr_1|.
void draw_line(void);

void handle_gameplay_logic_player_and_line(void);

void update_hud(void);

unsigned char update_cleared_playfield_tiles(void);
void line_completed(void);

void set_playfield_tile(unsigned int tile_index,
                        unsigned char playfield_tile_type,
                        unsigned char playfield_bg_tile);

void write_two_digit_number_to_bg(unsigned char num, unsigned char tile_x,
                                  unsigned char tile_y);

void write_score_to_bg(unsigned int score, unsigned char tile_x, unsigned char tile_y);

// Load the level up summary screen.
void change_to_level_up(void);

// Update the score display values for the level up summary screen.
void update_hud_level_up(void);

// Handle player pressing start on the level up summary screen.
// Returns TRUE if the player did press start.
unsigned char press_start_level_up(void);

// Draw the cursor on the level up summary screen.
// Note: There are no options on this screen, just CONTINUE.
void draw_cursor_level_up(void);

#endif  // __JEZNES_JEZNES_H__
