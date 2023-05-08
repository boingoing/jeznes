//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_SCREENS_TITLE_H__
#define __JEZNES_SCREENS_TITLE_H__

// Cursor sprite location for the title screen.
#define TITLE_CURSOR_1_PLAYER_X 0x58
#define TITLE_CURSOR_1_PLAYER_Y 0xa8
#define TITLE_CURSOR_2_PLAYERS_X 0x58
#define TITLE_CURSOR_2_PLAYERS_Y 0xb8

// How many balls should bounce around inside the playfield section of the title screen.
#define TITLE_SCREEN_BALL_COUNT 4

const char title_bg_palette[] = {
    0x0f,0x30,0x16,0x28,0x0f,0x00,0x27,0x31,0x0f,0x11,0x21,0x31,0x0f,0x1b,0x29,0x39
};

const char title_sprite_palette[] = {
    0x0f,0x30,0x16,0x28,0x0f,0x0f,0x27,0x37,0x0f,0x13,0x23,0x33,0x0f,0x14,0x24,0x34
};

// Title screen RLE graphics data.
const unsigned char title_screen[] = {
    0x01,0x70,0x01,0x42,0x41,0x43,0x01,0x17,0x42,0x70,0x01,0x05,0x40,0x70,0x01,0x17,
    0x50,0x70,0x01,0x05,0x40,0x70,0x70,0x45,0x43,0x44,0x46,0x41,0x43,0x46,0x45,0x43,
    0x3b,0x70,0x41,0x42,0x66,0x41,0x43,0x46,0x3c,0x43,0x46,0x70,0x70,0x50,0x70,0x01,
    0x05,0x40,0x70,0x01,0x03,0x40,0x70,0x54,0x49,0x4a,0x70,0x4c,0x4b,0x70,0x40,0x50,
    0x50,0x54,0x49,0x4a,0x3d,0x69,0x70,0x01,0x02,0x50,0x70,0x01,0x05,0x40,0x70,0x70,
    0x66,0x70,0x40,0x70,0x64,0x59,0x5a,0x4c,0x5b,0x6b,0x70,0x40,0x50,0x50,0x64,0x59,
    0x5a,0x7b,0x6a,0x69,0x70,0x70,0x50,0x70,0x01,0x05,0x40,0x70,0x70,0x57,0x53,0x58,
    0x70,0x51,0x53,0x76,0x5c,0x6c,0x76,0x70,0x55,0x57,0x52,0x51,0x53,0x76,0x75,0x7c,
    0x5e,0x70,0x70,0x50,0x70,0x01,0x05,0x40,0x70,0x01,0x17,0x50,0x70,0x01,0x05,0x61,
    0x53,0x01,0x17,0x62,0x70,0x01,0x05,0x40,0x00,0x01,0x17,0x50,0x70,0x01,0x05,0x40,
    0x00,0x01,0x17,0x50,0x70,0x01,0x05,0x40,0x00,0x01,0x17,0x50,0x70,0x01,0x05,0x40,
    0x00,0x01,0x17,0x50,0x70,0x01,0x05,0x40,0x00,0x01,0x17,0x50,0x70,0x01,0x05,0x40,
    0x00,0x01,0x17,0x50,0x70,0x01,0x05,0x40,0x00,0x01,0x17,0x50,0x70,0x01,0x05,0x40,
    0x00,0x01,0x17,0x50,0x70,0x01,0x05,0x51,0x53,0x01,0x17,0x52,0x70,0x01,0x4f,0x1f,
    0x70,0x13,0x0f,0x04,0x1c,0x08,0x15,0x70,0x01,0x37,0x20,0x70,0x13,0x0f,0x04,0x1c,
    0x08,0x15,0x16,0x70,0x01,0x71,0x29,0x70,0x20,0x1e,0x20,0x21,0x70,0x05,0x12,0x0c,
    0x11,0x0a,0x12,0x0c,0x11,0x0a,0x70,0x01,0x47,0xc0,0xf0,0x01,0x05,0x30,0xcc,0xbb,
    0xaa,0x01,0x03,0xee,0x33,0xcc,0x5f,0x01,0x05,0x33,0xcc,0x55,0x01,0x05,0x33,0xcc,
    0xf5,0x01,0x05,0x33,0x00,0x01,0x16,0x00,0x01,0x00
};

#endif  // __JEZNES_SCREENS_TITLE_H__
