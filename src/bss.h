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

#define PLAYFIELD_BYTES ((PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) >> 2)

// The state of every tile in the playfield.
unsigned char playfield_tiles[PLAYFIELD_BYTES];

// The line-related flags for each playfield tile.
unsigned char playfield_line_flags[PLAYFIELD_BYTES];

#if ENABLE_CHEATS
unsigned char enable_ball_line_collisions;
unsigned char enable_losing_lives;
#endif  // ENABLE_CHEATS

#endif  // __JEZNES_BSS_H__
