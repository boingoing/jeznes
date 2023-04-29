//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_BSS_H__
#define __JEZNES_BSS_H__

// Define the contents of the BSS section.
#pragma bss-name(push, "BSS")

#include "screens/game_over.h"
#include "screens/playfield.h"
#include "screens/pause.h"
#include "screens/title.h"
#include "sprites.h"
#include "playfield.h"

unsigned char playfield[PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT];

#if ENABLE_CHEATS
unsigned char enable_ball_line_collisions;
unsigned char enable_losing_lives;
#endif  // ENABLE_CHEATS

#endif  // __JEZNES_BSS_H__
