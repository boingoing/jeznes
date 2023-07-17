//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_FLOOD_FILL_H__
#define __JEZNES_FLOOD_FILL_H__

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

#define set_current_position_from_tile_index(tile_index) \
    set_current_playfield_byte_index(get_playfield_tile_byte_index(tile_index)); \
    set_current_playfield_in_byte_index(get_playfield_tile_in_byte_index(tile_index));

// The current position is a byte-index into the playfield_tiles array and a number [0-3] indicating the offset into the byte data belonging to the current tile.
// The byte-index is stored in the low-byte and the in-byte offset is stored in the high-byte.
#define get_current_position() (*((unsigned int*)&temp_int_3_byte_1))
#define set_current_position(a) ((*((unsigned int*)&temp_int_3_byte_1)) = (a))

// Gets the playfield_tiles byte index of the current position.
#define get_current_playfield_byte_index() (temp_int_3_byte_1)
#define set_current_playfield_byte_index(a) (temp_int_3_byte_1 = (a))
#define inc_current_playfield_byte_index() (++temp_int_3_byte_1)
#define dec_current_playfield_byte_index() (--temp_int_3_byte_1)

// Gets the playfield_tiles byte data in-byte index of the current position. [0,3]
#define get_current_playfield_in_byte_index() (temp_int_3_byte_2)
#define set_current_playfield_in_byte_index(a) (temp_int_3_byte_2 = (a))
#define inc_current_playfield_in_byte_index() (++temp_int_3_byte_2)
#define dec_current_playfield_in_byte_index() (--temp_int_3_byte_2)

#define get_mark() (temp_int_2)
#define set_mark(a) (temp_int_2 = (a))
#define get_mark2() (temp_int_1)
#define set_mark2(a) (temp_int_1 = (a))
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

#define set_adjacent_marked_tile_count(a) (temp_byte_6 = (a))
#define get_adjacent_marked_tile_count() (temp_byte_6)
#define inc_adjacent_marked_tile_count() (++temp_byte_6)

#define paint_current_position() (set_playfield_tile_type_uncleared_marked_from_byte_index(get_current_playfield_byte_index(), get_current_playfield_in_byte_index()))

#define reverse_direction() \
  (set_cur_dir(reverse_direction_table[get_cur_dir()]))
#define turn_right() (set_cur_dir(turn_right_table[get_cur_dir()]))
#define turn_left() (set_cur_dir(turn_left_table[get_cur_dir()]))

#define move_to_above_tile() (get_current_playfield_byte_index() -= 8)
#define move_to_below_tile() (get_current_playfield_byte_index() += 8)
#define move_to_right_tile() \
    if (get_current_playfield_in_byte_index() == 3) { \
    inc_current_playfield_byte_index(); \
    set_current_playfield_in_byte_index(0); \
    } else { \
    inc_current_playfield_in_byte_index(); \
    }
#define move_to_left_tile() \
    if (get_current_playfield_in_byte_index() == 0) { \
    dec_current_playfield_byte_index(); \
    set_current_playfield_in_byte_index(3); \
     } else { \
    dec_current_playfield_in_byte_index(); \
    }

#define move_forward() \
  switch(get_cur_dir()) { \
  case MOVE_DIRECTION_RIGHT:                   \
       move_to_right_tile(); \
       break; \
   case MOVE_DIRECTION_LEFT:                   \
       move_to_left_tile(); \
       break;  \
   case MOVE_DIRECTION_DOWN:                   \
       move_to_below_tile(); \
       break; \
    case MOVE_DIRECTION_UP:  \
       move_to_above_tile(); \
       break; \
  }

// Returns true when tile at position |byte_index, in_byte_index| is uncleared and unmarked.
#define is_inside(byte_index, in_byte_index) (is_playfield_tile_type_uncleared_unmarked_from_byte_index((byte_index), (in_byte_index)))
#define is_current_offset_inside(byte_index_offset, in_byte_index_offset) (is_inside(get_current_playfield_byte_index()+(byte_index_offset), get_current_playfield_in_byte_index()+(in_byte_index_offset)))
#define is_current_inside() (is_inside(get_current_playfield_byte_index(), get_current_playfield_in_byte_index()))

#define is_above_tile_inside() (is_current_offset_inside(-8, 0))
#define is_below_tile_inside() (is_current_offset_inside(8, 0))
#define is_right_tile_inside() (get_current_playfield_in_byte_index() == 3 ? \
    is_inside(get_current_playfield_byte_index()+1, 0) : \
    is_current_offset_inside(0, 1))
#define is_left_tile_inside() (get_current_playfield_in_byte_index() == 0 ? \
    is_inside(get_current_playfield_byte_index()-1, 3) : \
    is_current_offset_inside(0, -1))

#define is_above_right_tile_inside() (get_current_playfield_in_byte_index() == 3 ? \
    is_inside(get_current_playfield_byte_index()-7, 0) : \
    is_current_offset_inside(-8, 1))
#define is_above_left_tile_inside() (get_current_playfield_in_byte_index() == 0 ? \
    is_inside(get_current_playfield_byte_index()-9, 3) : \
    is_current_offset_inside(-8, -1))
#define is_below_right_tile_inside() (get_current_playfield_in_byte_index() == 3 ? \
    is_inside(get_current_playfield_byte_index()+9, 0) : \
    is_current_offset_inside(8, 1))
#define is_below_left_tile_inside() (get_current_playfield_in_byte_index() == 0 ? \
    is_inside(get_current_playfield_byte_index()+7, 3) : \
    is_current_offset_inside(8, -1))

#define is_front_inside()                                          \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                    \
       ? is_right_tile_inside() \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                   \
       ? is_left_tile_inside()  \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                   \
       ? is_below_tile_inside()  \
       : is_above_tile_inside())
#define is_back_inside()                                          \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                    \
       ? is_left_tile_inside()  \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                   \
       ? is_right_tile_inside() \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                   \
       ? is_above_tile_inside()    \
       : is_below_tile_inside())
#define is_right_inside()                                        \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                   \
       ? is_below_tile_inside() \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                  \
       ? is_above_tile_inside()   \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                  \
       ? is_left_tile_inside() \
       : is_right_tile_inside())
#define is_left_inside()                                          \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                    \
       ? is_above_tile_inside()    \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                   \
       ? is_below_tile_inside()  \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                   \
       ? is_right_tile_inside() \
       : is_left_tile_inside())
#define is_front_left_inside()                                         \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                         \
       ? is_above_right_tile_inside()   \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                        \
       ? is_below_left_tile_inside()  \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                        \
       ? is_below_right_tile_inside() \
       : is_above_left_tile_inside())
#define is_back_left_inside()                                          \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                         \
       ? is_above_left_tile_inside()    \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                        \
       ? is_below_right_tile_inside() \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                        \
       ? is_above_right_tile_inside()   \
       : is_below_left_tile_inside())

// Uses a constant-memory usage implementation of the painters algorithm to walk
// the playfield starting at the playfield tile returned via
// |get_current_position()|. Be sure to set the current position via
// |set_current_position()| before calling this function. Each reachable
// playfield tile is marked until we run out of unmarked playfield tiles to walk
// to. When this function returns, the region in which |get_current_position()|
// is located will be made up entirely of marked playfield tiles.
void compute_playfield_mark_bit_one_region(void);

#endif  // __JEZNES_FLOOD_FILL_H__
