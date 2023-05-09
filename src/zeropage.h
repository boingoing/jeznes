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
unsigned char unused_zp_bytes[8];

unsigned char pads[MAX_PLAYERS];
unsigned char pads_new[MAX_PLAYERS];

unsigned char game_state;
unsigned char current_level;
unsigned char current_ball_count;
unsigned char current_playfield_pattern;
unsigned char player_count;
unsigned char lives_count;
unsigned char cleared_tile_percentage;
unsigned int cleared_tile_count;

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
int temp_int_4;

void* temp_ptr_1;

struct Player players[MAX_PLAYERS];
struct Ball balls[MAX_BALLS];
struct Line lines[MAX_PLAYERS];

#endif  // __JEZNES_ZEROPAGE_H__
