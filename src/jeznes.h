//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_JEZNES_H__
#define __JEZNES_JEZNES_H__

#include "debug.h"

void main(void);

// Initializes the locations and directions of the active balls on the valid
// ball region of the playfield pattern defined by |current_playfield_pattern|.
void init_balls();

void init_title(void);
void title_change_mode(void);

// Handle player start button presses for the title screen.
// Returns TRUE if the player did press start.
unsigned char title_press_start(void);
void draw_title_cursor(void);

// Handle player start button presses for the pause screen.
// Returns TRUE if the player did press start.
unsigned char __fastcall__ pause_press_start(unsigned char player_index);

// Draw the words PAUSE on the screen.
void draw_pause_sprites(void);

void init_game(void);
void reset_playfield(void);

// Loads the playfield pattern defined by |current_playfield_pattern| into the
// in-memory playfield.
void load_playfield();

void change_to_game_over(void);
void game_over_change_mode(void);

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

void __fastcall__ move_player(struct Player* player, unsigned char player_index);
void __fastcall__ move_ball(unsigned char ball_index);

void move_balls(void);
void draw_balls(void);
void check_ball_line_collisions(void);

void __fastcall__ start_line(struct Player* player, unsigned char player_index);
void __fastcall__ flip_player_orientation(struct Player* player, unsigned char player_index);

void __fastcall__ update_nearest_tile(struct Player* player);
void __fastcall__ update_line(unsigned char line_index);

void __fastcall__ draw_player(struct Player* player);
void __fastcall__ draw_tile_highlight(struct Player* player);
void __fastcall__ draw_line(unsigned char line_index);

void update_hud(void);

void reset_playfield_mark_bit(void);
unsigned char __fastcall__ update_cleared_playfield_tiles(void);
void line_completed(void);

void __fastcall__ set_playfield_tile(unsigned int tile_index, unsigned char playfield_tile_type, unsigned char playfield_bg_tile);

#endif  // __JEZNES_JEZNES_H__
