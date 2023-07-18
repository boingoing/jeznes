//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_FLOOD_FILL_H__
#define __JEZNES_FLOOD_FILL_H__

#include "debug.h"
#include "zeropage.h"

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
#define set_cur_dir_macro(a) (temp_byte_1 = (a))
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

#define reverse_direction_macro() (set_cur_dir(reverse_direction_table[get_cur_dir()]))
#define turn_right_macro() (set_cur_dir(turn_right_table[get_cur_dir()]))
#define turn_left_macro() (set_cur_dir(turn_left_table[get_cur_dir()]))

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

#define move_forward_macro() \
  switch(get_cur_dir()) { \
  case MOVE_DIRECTION_RIGHT:                   \
       move_to_right_tile(); \
       break; \
   case MOVE_DIRECTION_DOWN:                   \
       move_to_below_tile(); \
       break; \
   case MOVE_DIRECTION_LEFT:                   \
       move_to_left_tile(); \
       break;  \
    case MOVE_DIRECTION_UP:  \
       move_to_above_tile(); \
       break; \
  }

// Returns true when tile at position |byte_index, in_byte_index| is uncleared and unmarked.
#define is_inside_macro(byte_index, in_byte_index) (is_playfield_tile_type_uncleared_unmarked_from_byte_index((byte_index), (in_byte_index)))
#define is_current_inside_with_byte_index_and_in_byte_index_offset_macro(byte_index_offset, in_byte_index_offset) (is_inside(get_current_playfield_byte_index()+(byte_index_offset), get_current_playfield_in_byte_index()+(in_byte_index_offset)))
#define is_current_inside_with_byte_index_offset_macro(byte_index_offset) (is_inside(get_current_playfield_byte_index()+(byte_index_offset), get_current_playfield_in_byte_index()))
#define is_current_inside_with_in_byte_index_offset_macro(in_byte_index_offset) (is_inside(get_current_playfield_byte_index(), get_current_playfield_in_byte_index()+(in_byte_index_offset)))
#define is_current_inside_macro() (is_inside(get_current_playfield_byte_index(), get_current_playfield_in_byte_index()))

unsigned char is_right_tile_inside_table[] = {
    0b001,
    0b010,
    0b011,
    0b100,
};
unsigned char is_left_tile_inside_table[] = {
    0b011,
    0b000,
    0b001,
    0b010,
};

#define is_above_tile_inside_macro() (is_current_inside_with_byte_index_offset(-8))
#define is_below_tile_inside_macro() (is_current_inside_with_byte_index_offset(8))
#define is_right_tile_inside_macro() (is_inside(get_current_playfield_byte_index()+((set_temp_byte_10(is_right_tile_inside_table[get_current_playfield_in_byte_index()])) >> 2), get_temp_byte_10() & 0b11))
#define is_left_tile_inside_macro() (is_inside(get_current_playfield_in_byte_index() == 0 ? (get_current_playfield_byte_index()-1) : get_current_playfield_byte_index(), is_left_tile_inside_table[get_current_playfield_in_byte_index()]))

#define is_left_tile_inside_macro3() (is_inside(get_current_playfield_byte_index()-((set_temp_byte_10(is_left_tile_inside_table[get_current_playfield_in_byte_index()])) >> 2), get_temp_byte_10() & 0b11))

#define is_right_tile_inside_macro2() (get_current_playfield_in_byte_index() == 3 ? \
    is_inside(get_current_playfield_byte_index()+1, 0) : \
    is_current_inside_with_in_byte_index_offset(1))
#define is_left_tile_inside_macro2() (get_current_playfield_in_byte_index() == 0 ? \
    is_inside(get_current_playfield_byte_index()-1, 3) : \
    is_current_inside_with_in_byte_index_offset(-1))

#define is_above_right_tile_inside_macro() (get_current_playfield_in_byte_index() == 3 ? \
    is_inside(get_current_playfield_byte_index()-7, 0) : \
    is_current_inside_with_byte_index_and_in_byte_index_offset(-8, 1))
#define is_above_left_tile_inside_macro() (get_current_playfield_in_byte_index() == 0 ? \
    is_inside(get_current_playfield_byte_index()-9, 3) : \
    is_current_inside_with_byte_index_and_in_byte_index_offset(-8, -1))
#define is_below_right_tile_inside_macro() (get_current_playfield_in_byte_index() == 3 ? \
    is_inside(get_current_playfield_byte_index()+9, 0) : \
    is_current_inside_with_byte_index_and_in_byte_index_offset(8, 1))
#define is_below_left_tile_inside_macro() (get_current_playfield_in_byte_index() == 0 ? \
    is_inside(get_current_playfield_byte_index()+7, 3) : \
    is_current_inside_with_byte_index_and_in_byte_index_offset(8, -1))

#define get_temp_byte_10() (*((unsigned char*)&temp_ptr_1))
#define set_temp_byte_10(a) (get_temp_byte_10() = a)
#define get_temp_byte_11() (*(((unsigned char*)&temp_ptr_1)+1))
#define set_temp_byte_11(a) (get_temp_byte_11() = a)

#define is_current_offset_inside_2(byte_index_table, in_byte_index_table) \
    set_temp_byte_11(in_byte_index_table[get_cur_dir()]); \
    set_temp_byte_10(get_current_playfield_in_byte_index()+get_temp_byte_11()); \
    return ((get_temp_byte_10() > 3) ? \
    (get_temp_byte_10() == 4) ? \
    is_inside(get_current_playfield_byte_index()+byte_index_table[get_cur_dir()]+1, 0) : \
    is_inside(get_current_playfield_byte_index()+byte_index_table[get_cur_dir()]-1, 3) : \
    is_inside(get_current_playfield_byte_index()+byte_index_table[get_cur_dir()], get_temp_byte_10()));

const signed char is_front_inside_byte_index_offset_table[] = {
    0, // MOVE_DIRECTION_RIGHT
    8, // MOVE_DIRECTION_DOWN
    0, // MOVE_DIRECTION_LEFT
    -8, // MOVE_DIRECTION_UP
};
const signed char is_front_inside_in_byte_index_offset_table[] = {
    1, // MOVE_DIRECTION_RIGHT
    0, // MOVE_DIRECTION_DOWN
    -1, // MOVE_DIRECTION_LEFT
    0, // MOVE_DIRECTION_UP
};

#define is_front_inside_macro2() is_current_offset_inside_2(is_front_inside_byte_index_offset_table, is_front_inside_in_byte_index_offset_table)
#define is_front_inside_macro()                                          \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                    \
       ? is_right_tile_inside() \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                   \
       ? is_left_tile_inside()  \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                   \
       ? is_below_tile_inside()  \
       : is_above_tile_inside())
#define is_back_inside_macro()                                          \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                    \
       ? is_left_tile_inside()  \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                   \
       ? is_right_tile_inside() \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                   \
       ? is_above_tile_inside()    \
       : is_below_tile_inside())
#define is_right_inside_macro()                                        \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                   \
       ? is_below_tile_inside() \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                  \
       ? is_above_tile_inside()   \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                  \
       ? is_left_tile_inside() \
       : is_right_tile_inside())
#define is_left_inside_macro()                                          \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                    \
       ? is_above_tile_inside()    \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                   \
       ? is_below_tile_inside()  \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                   \
       ? is_right_tile_inside() \
       : is_left_tile_inside())
#define is_front_left_inside_macro()                                         \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                         \
       ? is_above_right_tile_inside()   \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                        \
       ? is_below_left_tile_inside()  \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                        \
       ? is_below_right_tile_inside() \
       : is_above_left_tile_inside())
#define is_back_left_inside_macro()                                          \
  (get_cur_dir() == MOVE_DIRECTION_RIGHT                         \
       ? is_above_left_tile_inside()    \
   : get_cur_dir() == MOVE_DIRECTION_LEFT                        \
       ? is_below_right_tile_inside() \
   : get_cur_dir() == MOVE_DIRECTION_DOWN                        \
       ? is_above_right_tile_inside()   \
       : is_below_left_tile_inside())

#if PUT_CRITICAL_CODE_IN_FUNCTIONS
void set_cur_dir(unsigned char a) {
    set_cur_dir_macro(a);
}
void reverse_direction(void) {
    reverse_direction_macro();
}
void turn_right(void) {
    turn_right_macro();
}
void turn_left(void) {
    turn_left_macro();
}
void move_forward(void) {
    move_forward_macro();
}
unsigned char is_inside(unsigned char byte_index, unsigned char in_byte_index) {
    return is_inside_macro(byte_index, in_byte_index);
}
unsigned char is_current_inside_with_byte_index_and_in_byte_index_offset(signed char byte_index_offset, signed char in_byte_index_offset) {
    return is_current_inside_with_byte_index_and_in_byte_index_offset_macro(byte_index_offset, in_byte_index_offset);
}
unsigned char is_current_inside_with_byte_index_offset(signed char byte_index_offset) {
    return is_current_inside_with_byte_index_offset_macro(byte_index_offset);
}
unsigned char is_current_inside_with_in_byte_index_offset(signed char in_byte_index_offset) {
    return is_current_inside_with_in_byte_index_offset_macro(in_byte_index_offset);
}
unsigned char is_current_inside(void) { 
    return is_current_inside_macro();
}
unsigned char is_above_tile_inside(void) {
    return is_above_tile_inside_macro();
}
unsigned char is_below_tile_inside(void) {
    return is_below_tile_inside_macro();
}
unsigned char is_right_tile_inside(void) {
    return is_right_tile_inside_macro();
}
unsigned char is_left_tile_inside(void) {
    return is_left_tile_inside_macro();
}
unsigned char is_above_right_tile_inside(void) {
    return is_above_right_tile_inside_macro();
}
unsigned char is_above_left_tile_inside(void) {
    return is_above_left_tile_inside_macro();
}
unsigned char is_below_right_tile_inside(void) {
    return is_below_right_tile_inside_macro();
}
unsigned char is_below_left_tile_inside(void) {
    return is_below_left_tile_inside_macro();
}
unsigned char is_front_inside(void) {
    return is_front_inside_macro();
}
unsigned char is_back_inside(void) {
    return is_back_inside_macro();
}
unsigned char is_right_inside(void) {
    return is_right_inside_macro();
}
unsigned char is_left_inside(void) {
    return is_left_inside_macro();
}
unsigned char is_front_left_inside(void) {
    return is_front_left_inside_macro();
}
unsigned char is_back_left_inside(void) {
    return is_back_left_inside_macro();
}
#else
#define set_cur_dir(a) set_cur_dir_macro(a)

#define reverse_direction() reverse_direction_macro()
#define turn_right() turn_right_macro()
#define turn_left() turn_left_macro()

#define move_forward() move_forward_macro()

#define is_inside(byte_index, in_byte_index) is_inside_macro(byte_index, in_byte_index)
#define is_current_inside_with_byte_index_and_in_byte_index_offset(byte_index_offset, in_byte_index_offset) is_current_inside_with_byte_index_and_in_byte_index_offset_macro(byte_index_offset, in_byte_index_offset)
#define is_current_inside_with_byte_index_offset(byte_index_offset) is_current_inside_with_byte_index_offset_macro(byte_index_offset)
#define is_current_inside_with_in_byte_index_offset(in_byte_index_offset) is_current_inside_with_in_byte_index_offset_macro(in_byte_index_offset)
#define is_current_inside() is_current_inside_macro()

#define is_above_tile_inside() is_above_tile_inside_macro()
#define is_below_tile_inside() is_below_tile_inside_macro()
#define is_right_tile_inside() is_right_tile_inside_macro()
#define is_left_tile_inside() is_left_tile_inside_macro()

#define is_above_right_tile_inside() is_above_right_tile_inside_macro()
#define is_above_left_tile_inside() is_above_left_tile_inside_macro()
#define is_below_right_tile_inside() is_below_right_tile_inside_macro()
#define is_below_left_tile_inside() is_below_left_tile_inside_macro()

#define is_front_inside() is_front_inside_macro()
#define is_back_inside() is_back_inside_macro()
#define is_right_inside() is_right_inside_macro()
#define is_left_inside() is_left_inside_macro()
#define is_front_left_inside() is_front_left_inside_macro()
#define is_back_left_inside() is_back_left_inside_macro()
#endif

// Uses a constant-memory usage implementation of the painters algorithm to walk
// the playfield starting at the playfield tile returned via
// |get_current_position()|. Be sure to set the current position via
// |set_current_position()| before calling this function. Each reachable
// playfield tile is marked until we run out of unmarked playfield tiles to walk
// to. When this function returns, the region in which |get_current_position()|
// is located will be made up entirely of marked playfield tiles.
void compute_playfield_mark_bit_one_region(void);

#endif  // __JEZNES_FLOOD_FILL_H__
