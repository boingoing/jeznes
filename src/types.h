//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_TYPES_H__
#define __JEZNES_TYPES_H__

enum {
  GAME_STATE_TITLE,
  GAME_STATE_PLAYING,
  GAME_STATE_LEVEL_UP,
  GAME_STATE_LEVEL_DOWN,
  GAME_STATE_GAME_OVER,
  GAME_STATE_UPDATING_PLAYFIELD,
  GAME_STATE_REQUEST_HUD_UPDATE,
  GAME_STATE_PAUSED
};

enum { ORIENTATION_HORIZ, ORIENTATION_VERT };

enum { LINE_DIRECTION_NEGATIVE, LINE_DIRECTION_POSITIVE };

enum { PLAYFIELD_UNCLEARED, PLAYFIELD_WALL, PLAYFIELD_LINE };

enum { GAME_OVER_RETRY, GAME_OVER_QUIT };

enum { TITLE_1_PLAYER, TITLE_2_PLAYERS };

struct Player {
  // Player metasprite location in pixel-coords
  unsigned char x;
  unsigned char y;

  // Pixel-coords of nearest bg tile under the player
  unsigned char nearest_tile_x;
  unsigned char nearest_tile_y;

  // Playfield tile index for nearest tile
  int nearest_playfield_tile;

  // Hold bit-flags used to track state of this player.
  unsigned char flags;
};

struct Ball {
  // Ball sprite location in pixel-coords
  unsigned char x;
  unsigned char y;

  signed char x_velocity;
  signed char y_velocity;

  // Playfield tile index for nearest tile
  int nearest_playfield_tile;
};

struct Line {
  // Origin playfield tile index for the line.
  // This is the tile on which the player pressed the start line button.
  // For the game, the origin tile is always part of the line segment which
  // spreads in the negative direction and the tile to the right
  // (for ORIENTATION_HORIZ lines) or the tile below it (for ORIENTATION_VERT
  // lines) is where the line segment which spreads in the positive direction
  // begins.
  int origin;

  // How many steps away from the origin playfield tiles have we taken?
  unsigned char tile_step_count;

  // Completion of the current block [0-7]
  unsigned char current_block_completion;

  // Hold bit-flags used to track state of this line.
  unsigned char flags;
};

// Used by sprite collision detection code.
struct ObjectBase {
  unsigned char x;
  unsigned char y;
  unsigned char width;
  unsigned char height;
};

#endif  // __JEZNES_TYPES_H__
