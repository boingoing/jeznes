//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_SCREENS_PLAYFIELD_H__
#define __JEZNES_SCREENS_PLAYFIELD_H__

#include "../constants/tiles.h"

// Playfield dimensions.
#define PLAYFIELD_WIDTH 32
#define PLAYFIELD_HEIGHT 22

// Playfield tile offsets
#define PLAYFIELD_FIRST_TILE_X 1
#define PLAYFIELD_FIRST_TILE_Y 2
#define PLAYFIELD_FIRST_TILE_INDEX 32

// Playfield bounds in pixel-coords
#define PLAYFIELD_LEFT_WALL 0xe
#define PLAYFIELD_RIGHT_WALL 0xea
#define PLAYFIELD_TOP_WALL 0xd
#define PLAYFIELD_BOTTOM_WALL 0xa9

// HUD value locations in tile-coords
#define HUD_LEVEL_DISPLAY_TILE_X 8
#define HUD_LEVEL_DISPLAY_TILE_Y 24
#define HUD_LIVES_DISPLAY_TILE_X 8
#define HUD_LIVES_DISPLAY_TILE_Y 25
#define HUD_CLEAR_DISPLAY_TILE_X 18
#define HUD_CLEAR_DISPLAY_TILE_Y 24
#define HUD_TARGET_DISPLAY_TILE_X 18
#define HUD_TARGET_DISPLAY_TILE_Y 25

// Calculate the playfield tile position in (x,y) of the playfield tile |i|.
#define playfield_index_x(i) ((i) % 32)
#define playfield_index_y(i) ((i) >> 5)

// Calculate the bg tile position in pixel coords of the playfield tile |i|.
#define playfield_index_pixel_coord_x(i) \
  (playfield_index_x((i) + PLAYFIELD_FIRST_TILE_INDEX) << 3)
#define playfield_index_pixel_coord_y(i) \
  (playfield_index_y((i) + PLAYFIELD_FIRST_TILE_INDEX) << 3)

// Get the bg tile graphic index for lines.
// Indicate horizontal or vertical via |orientation| which should be
// ORIENTATION_HORIZ or ORIENTATION_VERT.
#define get_playfield_bg_tile_line(orientation) \
  (TILE_INDEX_PLAYFIELD_LINE_HORIZ + (orientation))

const char playfield_bg_palette[] = {0x0f, 0x30, 0x16, 0x28, 0x0f, 0x00,
                                     0x27, 0x31, 0x0f, 0x06, 0x16, 0x26,
                                     0x0f, 0x09, 0x19, 0x29};

const char playfield_sprite_palette[] = {0x0f, 0x30, 0x16, 0x28, 0x0f, 0x0f,
                                         0x27, 0x37, 0x0f, 0x13, 0x23, 0x33,
                                         0x0f, 0x14, 0x24, 0x34};

// Playfield screen RLE graphics data.
const unsigned char playfield_screen[] = {
    0x01, 0x70, 0x01, 0x20, 0x41, 0x43, 0x01, 0x1b, 0x42, 0x70, 0x70, 0x40,
    0x00, 0x01, 0x1b, 0x50, 0x70, 0x70, 0x40, 0x00, 0x01, 0x1b, 0x50, 0x70,
    0x70, 0x40, 0x00, 0x01, 0x1b, 0x50, 0x70, 0x70, 0x40, 0x00, 0x01, 0x1b,
    0x50, 0x70, 0x70, 0x40, 0x00, 0x01, 0x1b, 0x50, 0x70, 0x70, 0x40, 0x00,
    0x01, 0x1b, 0x50, 0x70, 0x70, 0x40, 0x00, 0x01, 0x1b, 0x50, 0x70, 0x70,
    0x40, 0x00, 0x01, 0x1b, 0x50, 0x70, 0x70, 0x40, 0x00, 0x01, 0x1b, 0x50,
    0x70, 0x70, 0x40, 0x00, 0x01, 0x1b, 0x50, 0x70, 0x70, 0x40, 0x00, 0x01,
    0x1b, 0x50, 0x70, 0x70, 0x40, 0x00, 0x01, 0x1b, 0x50, 0x70, 0x70, 0x40,
    0x00, 0x01, 0x1b, 0x50, 0x70, 0x70, 0x40, 0x00, 0x01, 0x1b, 0x50, 0x70,
    0x70, 0x40, 0x00, 0x01, 0x1b, 0x50, 0x70, 0x70, 0x40, 0x00, 0x01, 0x1b,
    0x50, 0x70, 0x70, 0x40, 0x00, 0x01, 0x1b, 0x50, 0x70, 0x70, 0x40, 0x00,
    0x01, 0x1b, 0x50, 0x70, 0x70, 0x40, 0x00, 0x01, 0x1b, 0x50, 0x70, 0x70,
    0x40, 0x00, 0x01, 0x1b, 0x50, 0x70, 0x70, 0x61, 0x53, 0x01, 0x1b, 0x62,
    0x70, 0x70, 0x40, 0x60, 0x01, 0x1b, 0x50, 0x70, 0x70, 0x40, 0x0f, 0x08,
    0x19, 0x08, 0x0f, 0x60, 0x1e, 0x1e, 0x60, 0x06, 0x0f, 0x08, 0x04, 0x15,
    0x36, 0x60, 0x1e, 0x1e, 0x60, 0x16, 0x06, 0x12, 0x15, 0x08, 0x60, 0x01,
    0x03, 0x50, 0x70, 0x70, 0x40, 0x0f, 0x0c, 0x19, 0x08, 0x16, 0x60, 0x1e,
    0x1e, 0x60, 0x17, 0x04, 0x15, 0x0a, 0x08, 0x17, 0x60, 0x1e, 0x1e, 0x60,
    0x60, 0x1e, 0x01, 0x07, 0x50, 0x70, 0x70, 0x40, 0x60, 0x01, 0x1b, 0x50,
    0x70, 0x70, 0x51, 0x53, 0x01, 0x1b, 0x52, 0x70, 0x01, 0x40, 0x6a, 0x5a,
    0x01, 0x05, 0x9a, 0x66, 0x55, 0x01, 0x05, 0x99, 0x66, 0x55, 0x01, 0x05,
    0x99, 0x66, 0x55, 0x01, 0x05, 0x99, 0x66, 0x55, 0x01, 0x05, 0x99, 0xa6,
    0xa5, 0x01, 0x05, 0xa9, 0xae, 0xaf, 0x01, 0x05, 0xab, 0x0a, 0x01, 0x06,
    0x0a, 0x01, 0x00};

#endif  // __JEZNES_SCREENS_PLAYFIELD_H__
