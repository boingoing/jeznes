//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_CONSTANTS_TILES_H__
#define __JEZNES_CONSTANTS_TILES_H__

#define MAX_TILE_UPDATES_PER_FRAME 41

#define TILE_INDEX_PLAYFIELD_UNCLEARED 0x0
#define TILE_INDEX_PLAYFIELD_CLEARED 0x3

#define TILE_INDEX_PLAYFIELD_LINE_HORIZ_NEGATIVE 0xb0
#define TILE_INDEX_PLAYFIELD_LINE_HORIZ_POSITIVE 0xb1
#define TILE_INDEX_PLAYFIELD_LINE_VERT_NEGATIVE 0xb2
#define TILE_INDEX_PLAYFIELD_LINE_VERT_POSITIVE 0xb3
#define TILE_INDEX_PLAYFIELD_LINE_HORIZ_NEGATIVE_ORIGIN 0xa0
#define TILE_INDEX_PLAYFIELD_LINE_HORIZ_POSITIVE_ORIGIN 0xa1
#define TILE_INDEX_PLAYFIELD_LINE_VERT_NEGATIVE_ORIGIN 0xa2
#define TILE_INDEX_PLAYFIELD_LINE_VERT_POSITIVE_ORIGIN 0xa3

#define TILE_INDEX_ALPHANUMERIC_ZERO 0x1e

#define get_tile_alphanumeric_number(v) (TILE_INDEX_ALPHANUMERIC_ZERO + (v))

#endif  // __JEZNES_CONSTANTS_TILES_H__
