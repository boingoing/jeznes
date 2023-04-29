//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_SPRITES_H__
#define __JEZNES_SPRITES_H__

const unsigned char player_metasprite_0_data[] = {
    0,0,0x19,0|OAM_FLIP_V,
    0,-8,0x19,0,
    128
};

const unsigned char player_metasprite_1_data[] = {
    0,0,0x1a,0|OAM_FLIP_V,
    0,-8,0x1a,0,
    128
};

const unsigned char player_metasprite_2_data[] = {
    0,0,0x1b,0|OAM_FLIP_H,
    -8,0,0x1b,0,
    128
};

const unsigned char player_metasprite_3_data[] = {
    0,0,0x1c,0|OAM_FLIP_H,
    -8,0,0x1c,0,
    128
};

const unsigned char* const player_metasprite_list[] = {
    player_metasprite_0_data,
    player_metasprite_1_data,
    player_metasprite_2_data,
    player_metasprite_3_data
};

#endif  // __JEZNES_SPRITES_H__
