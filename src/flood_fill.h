//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_FLOOD_FILL_H__
#define __JEZNES_FLOOD_FILL_H__

#define playfield_index_move_up(i) ((i)-32)
#define playfield_index_move_down(i) ((i) + 32)
#define playfield_index_move_left(i) ((i)-1)
#define playfield_index_move_right(i) ((i) + 1)

#define inside(i) \
  ((playfield[(i)] & (PLAYFIELD_WALL | PLAYFIELD_BITMASK_MARK)) == 0)

enum {
  MOVE_DIRECTION_RIGHT,
  MOVE_DIRECTION_DOWN,
  MOVE_DIRECTION_LEFT,
  MOVE_DIRECTION_UP,
  MOVE_DIRECTION_DEFAULT = MOVE_DIRECTION_RIGHT
};

#define get_current_position() (temp_int_1)
#define set_current_position(a) (temp_int_1 = (a))
#define get_mark() (temp_int_2)
#define set_mark(a) (temp_int_2 = (a))
#define get_mark2() (temp_int_3)
#define set_mark2(a) (temp_int_3 = (a))
#define get_cur_dir() (temp_byte_1)
#define set_cur_dir(a) (temp_byte_1 = (a))
#define get_mark_dir() (temp_byte_4)
#define set_mark_dir(a) (temp_byte_4 = (a))
#define get_mark2_dir() (temp_byte_5)
#define set_mark2_dir(a) (temp_byte_5 = (a))
#define get_backtrack() (temp_byte_2)
#define set_backtrack(a) (temp_byte_2 = (a))
#define get_findloop() (temp_byte_3)
#define set_findloop(a) (temp_byte_3 = (a))
#define get_mark_null() (temp_byte_7)
#define set_mark_null(a) (temp_byte_7 = (a))
#define get_mark2_null() (temp_byte_8)
#define set_mark2_null(a) (temp_byte_8 = (a))

#define get_temp_forward_iterator() (temp_int_2)
#define set_temp_forward_iterator(a) (temp_int_2 = (a))

#define set_adjacent_marked_tile_count(a) (temp_byte_6 = (a))
#define get_adjacent_marked_tile_count() (temp_byte_6)
#define inc_adjacent_marked_tile_count() (++temp_byte_6)

#define reverse_direction() (set_cur_dir((get_cur_dir() + 2) % 4))
#define turn_right() (set_cur_dir((get_cur_dir() + 1) % 4))
#define turn_left() (set_cur_dir((get_cur_dir() + 3) % 4))
#define move_forward() (set_current_position(get_front()))

// Uses a constant-memory usage implementation of the painters algorithm to
// walk the playfield starting at the playfield tile where |ball_index| is
// currently located. Each reachable playfield tile is marked until we run
// out of unmarked playfield tiles to walk to.
// When this function returns, the region in which |ball_index| is bound will
// be made up entirely of marked playfield tiles.
void __fastcall__ compute_playfield_mark_bit_one_ball(unsigned char ball_index);

#endif  // __JEZNES_FLOOD_FILL_H__
