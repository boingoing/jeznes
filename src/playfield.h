//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_PLAYFIELD_H__
#define __JEZNES_PLAYFIELD_H__

enum {
  PLAYFIELD_PATTERN_TITLE_SCREEN = 0,
  PLAYFIELD_PATTERN_GAME_OVER_SCREEN,
  PLAYFIELD_PATTERN_LEVEL_UP_SCREEN,
  PLAYFIELD_PATTERN_1,
  FIRST_PLAYFIELD_PATTERN = PLAYFIELD_PATTERN_1
};

// The playfield patterns and arrays here are intended to be human-readable-ish.
// The patterns are laid-out matching the full screen which they correspond to.
// The arrays need to all have the same number of entries.
// Disable clang-format for this file as it'll screw those up.

// clang-format off

// The title screen includes a playfield section which is this pattern.
const unsigned char title_screen_pattern[] = {
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
};

// The game over screen includes a playfield section which is this pattern.
const unsigned char game_over_screen_pattern[] = {
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
};

// The level up screen includes a playfield section which is this pattern.
const unsigned char level_up_screen_pattern[] = {
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
};

const unsigned char playfield_pattern_1[] = {
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11110000,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
};

const unsigned char * const playfield_patterns[] = {
  title_screen_pattern,
  game_over_screen_pattern,
  level_up_screen_pattern,
  playfield_pattern_1
};

// Maximum here should not exeed 655
const unsigned int playfield_pattern_uncleared_tile_counts[] = {
  0, // Cannot be cleared
  0, // Cannot be cleared
  0,
  560
};

const unsigned char playfield_pattern_valid_ball_start_pixel_x[] = {
  0x28,
  0x28,
  0x28,
  0x18
};

const unsigned char playfield_pattern_valid_ball_start_pixel_y[] = {
  0x58,
  0x68,
  0x68,
  0x20
};

const unsigned char playfield_pattern_valid_ball_width_in_pixels[] = {
  0xb0,
  0xb0,
  0xb0,
  0xd0
};

const unsigned char playfield_pattern_valid_ball_height_in_pixels[] = {
  0x28,
  0x28,
  0x28,
  0x90
};

// Default x-coord in pixel-space of the player sprites (1 and 2).
// TODO(boingoing): Per-playfield if needed.
const unsigned char playfield_pattern_player_default_x[2] = {
  0x56,
  0x96
};

// Default y-coord in pixel-space of the player sprites (1 and 2).
// TODO(boingoing): Per-playfield if needed.
const unsigned char playfield_pattern_player_default_y[2] = {
  0x46,
  0x86
};

// clang-format on

#endif  // __JEZNES_PLAYFIELD_H__
