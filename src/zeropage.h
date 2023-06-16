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
unsigned char unused_zp_bytes[5];

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

// Below temps may be used anywhere and should be prefered over function-local
// storage or passing arguments to function calls.
unsigned char temp_byte_1;
unsigned char temp_byte_2;
unsigned char temp_byte_3;
unsigned char temp_byte_4;
unsigned char temp_byte_5;
unsigned char temp_byte_6;
unsigned char temp_byte_7;
unsigned char temp_byte_8;

int temp_int_1;
int temp_int_2;
int temp_int_3;

void* temp_ptr_1;

#endif  // __JEZNES_ZEROPAGE_H__
