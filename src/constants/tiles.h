//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_CONSTANTS_TILES_H__
#define __JEZNES_CONSTANTS_TILES_H__

#define MAX_TILE_UPDATES_PER_FRAME 40

#define TILE_INDEX_PLAYFIELD_UNCLEARED 0x0
#define TILE_INDEX_PLAYFIELD_CLEARED 0x3
#define TILE_INDEX_PLAYFIELD_LINE_HORIZ 0x1
#define TILE_INDEX_PLAYFIELD_LINE_VERT 0x2

#define TILE_INDEX_BALL_BASE 0x30
#define TILE_INDEX_TILE_HIGHLIGHT 0x18
#define TILE_INDEX_ALPHANUMERIC_ZERO 0x1e

#define get_tile_alphanumeric_number(v) (TILE_INDEX_ALPHANUMERIC_ZERO + (v))

#endif  // __JEZNES_CONSTANTS_TILES_H__
