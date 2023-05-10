//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_ZEROPAGE_H__
#define __JEZNES_ZEROPAGE_H__

#include "constants/game.h"
#include "types.h"

// Clang-format tries to change the name "bss-name"
// clang-format off
// Define the contents of the zeropage - address range [$00 - $ff].
#pragma bss-name(push, "ZEROPAGE")
// clang-format on

// Placeholder to track how many bytes are unused in the zeropage.
unsigned char unused_zp_bytes[10];

// Controller state storage.
unsigned char pads[MAX_PLAYERS];
unsigned char pads_new[MAX_PLAYERS];

// All data needed to represent state of each player.
struct Player players[MAX_PLAYERS];

// State of all balls which may be active on playfield sections.
struct Ball balls[MAX_BALLS];

// State of the lines which players may draw.
struct Line lines[MAX_PLAYERS];

// Which state the game is in (ie: playing, paused, title screen, etc).
unsigned char game_state;

// Current level when the game is in playing state.
unsigned char current_level;

// Count of balls displayed on the playfield section of whichever screen is loaded.
unsigned char current_ball_count;

// Which playfield pattern is currently being displayed.
unsigned char current_playfield_pattern;

// Number of players currently playing.
unsigned char player_count;

// Count of lives remaining.
unsigned char lives_count;

// Percentage of the playfield which has been cleared.
// Note: This is calculated in update_hud() only because it's expensive.
unsigned char cleared_tile_percentage;

// How many playfield tiles have been cleared. This is used to compute the percentage |cleared_tile_percentage|.
unsigned int cleared_tile_count;

// Below temps may be used anywhere and should be prefered over function-local storage or passing arguments to function calls.
unsigned char temp_byte_1;
unsigned char temp_byte_2;
unsigned char temp_byte_3;
unsigned char temp_byte_4;
unsigned char temp_byte_5;
unsigned char temp_byte_6;

signed char temp_signed_byte_1;
signed char temp_signed_byte_2;

int temp_int_1;
int temp_int_2;
int temp_int_3;

void* temp_ptr_1;

#endif  // __JEZNES_ZEROPAGE_H__
