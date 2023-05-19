//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_CONSTANTS_SPRITES_H__
#define __JEZNES_CONSTANTS_SPRITES_H__

#define SPRITE_INDEX_PLAYFIELD_LINE_HORIZ_BASE 0x80
#define SPRITE_INDEX_PLAYFIELD_LINE_VERT_BASE 0x90

#define SPRITE_INDEX_TILE_HIGHLIGHT 0x18

// Sprites for the ball are laid-out sequentially starting at this sprite index.
#define SPRITE_INDEX_BALL_BASE 0x31

// Count of sprite animation frames for the ball.
#define SPRITE_FRAME_COUNT_BALL 16

#define SPRITE_INDEX_CURSOR 0x17

// Sprites for the PAUSE! chars are laid-out sequentially starting at this
// sprite index.
#define SPRITE_INDEX_PAUSE_BASE 0x23

#endif  // __JEZNES_CONSTANTS_SPRITES_H__
