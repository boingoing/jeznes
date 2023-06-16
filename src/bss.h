//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_BSS_H__
#define __JEZNES_BSS_H__

// Clang-format tries to change the name "bss-name"
// clang-format off
// Define the contents of the BSS section.
#pragma bss-name(push, "BSS")
// clang-format on

#include "playfield.h"
#include "screens/game_over.h"
#include "screens/level_up.h"
#include "screens/pause.h"
#include "screens/playfield.h"
#include "screens/title.h"
#include "sprites.h"

// Current level when the game is in playing state.
unsigned char current_level;

// Count of balls displayed on the playfield section of whichever screen is
// loaded.
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

// How many playfield tiles have been cleared. This is used to compute the
// percentage |cleared_tile_percentage|.
unsigned int cleared_tile_count;

// Current score counter.
unsigned int score;

unsigned char playfield[PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT];

#if ENABLE_CHEATS
unsigned char enable_ball_line_collisions;
unsigned char enable_losing_lives;
#endif  // ENABLE_CHEATS

#endif  // __JEZNES_BSS_H__
