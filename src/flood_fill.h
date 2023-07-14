//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_FLOOD_FILL_H__
#define __JEZNES_FLOOD_FILL_H__

#include "lib/nesdoug.h"

enum {
  MOVE_DIRECTION_RIGHT,
  MOVE_DIRECTION_DOWN,
  MOVE_DIRECTION_LEFT,
  MOVE_DIRECTION_UP,
  MOVE_DIRECTION_DEFAULT = MOVE_DIRECTION_RIGHT
};

const unsigned char reverse_direction_table[] = {
    MOVE_DIRECTION_LEFT,
    MOVE_DIRECTION_UP,
    MOVE_DIRECTION_RIGHT,
    MOVE_DIRECTION_DOWN,
};

const unsigned char turn_right_table[] = {
    MOVE_DIRECTION_DOWN,
    MOVE_DIRECTION_LEFT,
    MOVE_DIRECTION_UP,
    MOVE_DIRECTION_RIGHT,
};

const unsigned char turn_left_table[] = {
    MOVE_DIRECTION_UP,
    MOVE_DIRECTION_RIGHT,
    MOVE_DIRECTION_DOWN,
    MOVE_DIRECTION_LEFT,
};

// The current position is a byte-index into the playfield_tiles array and a number [0-3] indicating the offset into the byte data belonging to the current tile.
// The byte-index is stored in the low-byte and the in-byte offset is stored in the high-byte.
#define get_current_position() (temp_int_1)
#define set_current_position(a) (temp_int_1 = (a))

// Gets the playfield_tiles byte index of the position |i|.
#define get_playfield_byte_index_from_position(i) ((unsigned char)(i))

// Gets the playfield_tiles byte data in-byte index of the position |i|. [0,3]
#define get_playfield_in_byte_index_from_position(i) ((i)>>8)

// Gets the playfield_tiles byte index of the current position.
#define get_current_playfield_byte_index() (low_byte(get_current_position()))
#define set_current_playfield_byte_index(a) (low_byte(get_current_position()) = (a))
#define inc_current_playfield_byte_index() (++low_byte(get_current_position()))
#define dec_current_playfield_byte_index() (--low_byte(get_current_position()))

// Gets the playfield_tiles byte data in-byte index of the current position. [0,3]
#define get_current_playfield_in_byte_index() (high_byte(get_current_position()))
#define set_current_playfield_in_byte_index(a) (high_byte(get_current_position()) = (a))
#define inc_current_playfield_in_byte_index() (++high_byte(get_current_position()))
#define dec_current_playfield_in_byte_index() (--high_byte(get_current_position()))

// Returns true when tile at position |i| is uncleared and unmarked.
#define inside(i) (is_playfield_tile_type_uncleared_unmarked_from_byte_index(get_playfield_byte_index_from_position((i)), get_playfield_in_byte_index_from_position((i))))

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

#define reverse_direction() \
  (set_cur_dir(reverse_direction_table[get_cur_dir()]))
#define turn_right() (set_cur_dir(turn_right_table[get_cur_dir()]))
#define turn_left() (set_cur_dir(turn_left_table[get_cur_dir()]))
#define move_forward() (set_current_position(get_front()))

#define mark_current_position() (set_playfield_tile_type_uncleared_marked_from_byte_index(get_current_playfield_byte_index(), get_current_playfield_in_byte_index()))

#define playfield_index_current_move_up() (make_word(get_current_playfield_byte_index()-8, get_current_playfield_in_byte_index()))
#define playfield_index_current_move_down() (make_word(get_current_playfield_byte_index()+8, get_current_playfield_in_byte_index()))
#define playfield_index_current_move_left() (get_current_playfield_in_byte_index() == 0 ? \
    make_word(get_current_playfield_byte_index()-1, 3) : \
    make_word(get_current_playfield_byte_index(), get_current_playfield_in_byte_index() - 1))
#define playfield_index_current_move_right() (get_current_playfield_in_byte_index() == 3 ? \
    make_word(get_current_playfield_byte_index()+1, 0) : \
    make_word(get_current_playfield_byte_index(), get_current_playfield_in_byte_index() + 1))

#define playfield_index_current_move_right_up() (get_current_playfield_in_byte_index() == 3 ? \
    make_word(get_current_playfield_byte_index()-7, 0) : \
    make_word(get_current_playfield_byte_index()-8, get_current_playfield_in_byte_index() + 1))
#define playfield_index_current_move_left_down() (get_current_playfield_in_byte_index() == 0 ? \
    make_word(get_current_playfield_byte_index()+7, 3) : \
    make_word(get_current_playfield_byte_index()+8, get_current_playfield_in_byte_index() - 1))
#define playfield_index_current_move_left_up() (get_current_playfield_in_byte_index() == 0 ? \
    make_word(get_current_playfield_byte_index()-9, 3) : \
    make_word(get_current_playfield_byte_index()-8, get_current_playfield_in_byte_index() - 1))
#define playfield_index_current_move_right_down() (get_current_playfield_in_byte_index() == 3 ? \
    make_word(get_current_playfield_byte_index()+9, 0) : \
    make_word(get_current_playfield_byte_index()+8, get_current_playfield_in_byte_index() + 1))

#define playfield_index_move_up(i) (make_word(get_playfield_byte_index_from_position(i)-8, get_playfield_in_byte_index_from_position(i)))
#define playfield_index_move_down(i) (make_word(get_playfield_byte_index_from_position(i)+8, get_playfield_in_byte_index_from_position(i)))
#define playfield_index_move_left(i) (get_playfield_in_byte_index_from_position(i) == 0 ? \
    make_word(get_playfield_byte_index_from_position(i)-1, 3) : \
    make_word(get_playfield_byte_index_from_position(i), get_playfield_in_byte_index_from_position(i)-1))
#define playfield_index_move_right(i) (get_playfield_in_byte_index_from_position(i) == 3 ? \
    make_word(get_playfield_byte_index_from_position(i)+1, 0) : \
    make_word(get_playfield_byte_index_from_position(i), get_playfield_in_byte_index_from_position(i)+1))

#define get_front()                                         \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                    \
       ? playfield_index_current_move_right() \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                   \
       ? playfield_index_current_move_left()  \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                   \
       ? playfield_index_current_move_down()  \
       : playfield_index_current_move_up())

#define get_back()                                          \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                    \
       ? playfield_index_current_move_left()  \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                   \
       ? playfield_index_current_move_right() \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                   \
       ? playfield_index_current_move_up()    \
       : playfield_index_current_move_down())

#define get_right()                                        \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                   \
       ? playfield_index_current_move_down() \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                  \
       ? playfield_index_current_move_up()   \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                  \
       ? playfield_index_current_move_left() \
       : playfield_index_current_move_right())

#define get_left()                                          \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                    \
       ? playfield_index_current_move_up()    \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                   \
       ? playfield_index_current_move_down()  \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                   \
       ? playfield_index_current_move_right() \
       : playfield_index_current_move_left())

#define get_front_left()                                         \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                         \
       ? playfield_index_current_move_right_up()   \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                        \
       ? playfield_index_current_move_left_down()  \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                        \
       ? playfield_index_current_move_right_down() \
       : playfield_index_current_move_left_up())

#define get_back_left()                                          \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                         \
       ? playfield_index_current_move_left_up()    \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                        \
       ? playfield_index_current_move_right_down() \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                        \
       ? playfield_index_current_move_right_up()   \
       : playfield_index_current_move_left_down())

// Uses a constant-memory usage implementation of the painters algorithm to walk
// the playfield starting at the playfield tile returned via
// |get_current_position()|. Be sure to set the current position via
// |set_current_position()| before calling this function. Each reachable
// playfield tile is marked until we run out of unmarked playfield tiles to walk
// to. When this function returns, the region in which |get_current_position()|
// is located will be made up entirely of marked playfield tiles.
void compute_playfield_mark_bit_one_region(void);

#endif  // __JEZNES_FLOOD_FILL_H__
