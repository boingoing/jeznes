//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_FLAGS_PLAYFIELD_H__
#define __JEZNES_FLAGS_PLAYFIELD_H__

#include "base.h"

#define PLAYFIELD_BITMASK_ALL 0b11

#define PLAYFIELD_TILE_TABLE(name, base_value) \
const unsigned char name##_table[] = { \
  base_value << 0, \
  base_value << 2, \
  base_value << 4, \
  base_value << 6, \
};

#define PLAYFIELD_TILE_TYPE_TABLE(name, base_value) PLAYFIELD_TILE_TABLE(playfield_tile_type_##name, base_value)

PLAYFIELD_TILE_TABLE(playfield_bitmask_tile, PLAYFIELD_BITMASK_ALL)

#define PLAYFIELD_TILE_TYPE_UNCLEARED_UNMARKED 0b00
#define PLAYFIELD_TILE_TYPE_UNCLEARED_MARKED 0b01
#define PLAYFIELD_TILE_TYPE_LINE 0b10
#define PLAYFIELD_TILE_TYPE_WALL 0b11

PLAYFIELD_TILE_TYPE_TABLE(uncleared_unmarked, PLAYFIELD_TILE_TYPE_UNCLEARED_UNMARKED)
PLAYFIELD_TILE_TYPE_TABLE(uncleared_marked, PLAYFIELD_TILE_TYPE_UNCLEARED_MARKED)
PLAYFIELD_TILE_TYPE_TABLE(line, PLAYFIELD_TILE_TYPE_LINE)
PLAYFIELD_TILE_TYPE_TABLE(wall, PLAYFIELD_TILE_TYPE_WALL)

#define get_playfield_tile_byte_index(playfield_tile_index) ((unsigned char)((playfield_tile_index) >> 2))
#define get_playfield_tile_in_byte_index(playfield_tile_index) (((unsigned char)(playfield_tile_index)) & 0b11)

#define get_playfield_tile_raw_value_from_byte_index(playfield_tile_byte_index, playfield_tiles_array) (playfield_tiles_array[(playfield_tile_byte_index)])
#define get_playfield_tile_raw_value(playfield_tile_index, playfield_tiles_array) (get_playfield_tile_raw_value_from_byte_index(get_playfield_tile_byte_index((playfield_tile_index)), (playfield_tiles_array)))

#define get_playfield_tile_masked_value_from_byte_and_bitmask(byte, bitmask) ((byte) & (bitmask))
#define get_playfield_tile_masked_value_from_byte(byte, playfield_tile_in_byte_index) (get_playfield_tile_masked_value_from_byte_and_bitmask((byte), playfield_bitmask_tile_table[(playfield_tile_in_byte_index)]))
#define get_playfield_tile_masked_value_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index, playfield_tiles_array) (get_playfield_tile_masked_value_from_byte(get_playfield_tile_raw_value_from_byte_index((playfield_tile_byte_index), (playfield_tiles_array)), (playfield_tile_in_byte_index)))
#define get_playfield_tile_masked_value(playfield_tile_index, playfield_tiles_array) (get_playfield_tile_masked_value_from_byte_index(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index)), (playfield_tiles_array)))

#define get_playfield_tile_value_from_masked_byte(masked_byte, playfield_tile_in_byte_index) ((masked_byte) >> ((playfield_tile_in_byte_index) << 1))
#define get_playfield_tile_value_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index, playfield_tiles_array) (get_playfield_tile_value_from_masked_byte(get_playfield_tile_masked_value_from_byte_index((playfield_tile_byte_index), (playfield_tile_in_byte_index), (playfield_tiles_array)), (playfield_tile_in_byte_index)))


#define get_playfield_tile_masked_value_table(byte, playfield_tile_in_byte_index, in_byte_table) (get_playfield_tile_masked_value_from_byte((byte), (in_byte_table[(playfield_tile_in_byte_index)])))

#define get_playfield_tile_value_from_byte_no_shift(byte, playfield_tile_in_byte_index) (get_playfield_tile_masked_value_table((byte), (playfield_tile_in_byte_index), (playfield_bitmask_tile_table)))
#define get_playfield_tile_value_from_byte(byte, playfield_tile_in_byte_index) (get_playfield_tile_value_from_byte_no_shift((byte), (playfield_tile_in_byte_index)) >> ((playfield_tile_in_byte_index)<<1))
#define get_playfield_tile_value_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index, playfield_tiles_array) (get_playfield_tile_value_from_byte(playfield_tiles_array[(playfield_tile_byte_index)], (playfield_tile_in_byte_index)))
#define get_playfield_tile_value(playfield_tile_index, playfield_tiles_array) (get_playfield_tile_value_from_byte_index(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index)), playfield_tiles_array))

#define is_mask_set_in_playfield_tile_value_from_byte(byte, playfield_tile_in_byte_index, bitmask) (get_playfield_tile_value_from_byte_no_shift((byte), (playfield_tile_in_byte_index)) == (bitmask))
#define is_mask_set_in_playfield_tile_value_from_byte_table(byte, playfield_tile_in_byte_index, bitmask_table) (get_playfield_tile_value_from_byte_no_shift((byte), (playfield_tile_in_byte_index)) == tile_type_table[(playfield_tile_in_byte_index)])


#define get_playfield_tile_type_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index) (get_playfield_tile_value_from_byte_index((playfield_tile_byte_index), (playfield_tile_in_byte_index), playfield_tiles))
#define get_playfield_tile_type(playfield_tile_index) (get_playfield_tile_value((playfield_tile_index), playfield_tiles))

#define set_playfield_tile_type_no_shift(playfield_tile_byte_index, playfield_tile_in_byte_index, byte) (playfield_tiles[(playfield_tile_byte_index)] = (playfield_tiles[(playfield_tile_byte_index)] & ~(playfield_bitmask_tile_table[(playfield_tile_in_byte_index)])) | (byte))
#define set_playfield_tile_type_with_byte_and_in_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index, tile_type) (set_playfield_tile_type_no_shift((playfield_tile_byte_index), (playfield_tile_in_byte_index), ((tile_type) << ((playfield_tile_in_byte_index) << 1))))
#define set_playfield_tile_type(playfield_tile_index, tile_type) (set_playfield_tile_type_with_byte_and_in_byte_index(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index)), (tile_type)))

#define set_playfield_tile_type_byte_index_table(playfield_tile_byte_index, playfield_tile_in_byte_index, tile_type_table) (set_playfield_tile_type_no_shift((playfield_tile_byte_index), (playfield_tile_in_byte_index), tile_type_table[(playfield_tile_in_byte_index)]))

#define set_playfield_tile_type_byte_index_uncleared_unmarked(playfield_tile_byte_index, playfield_tile_in_byte_index) (playfield_tiles[(playfield_tile_byte_index)] &= ~(playfield_bitmask_tile_table[(playfield_tile_in_byte_index)]))
#define set_playfield_tile_type_byte_index_wall(playfield_tile_byte_index, playfield_tile_in_byte_index) (playfield_tiles[(playfield_tile_byte_index)] |= playfield_bitmask_tile_table[(playfield_tile_in_byte_index)])
#define set_playfield_tile_type_byte_index_uncleared_marked(playfield_tile_byte_index, playfield_tile_in_byte_index) (set_playfield_tile_type_byte_index_table((playfield_tile_byte_index), (playfield_tile_in_byte_index), playfield_tile_type_uncleared_marked_table))
#define set_playfield_tile_type_byte_index_line(playfield_tile_byte_index, playfield_tile_in_byte_index) (set_playfield_tile_type_byte_index_table((playfield_tile_byte_index), (playfield_tile_in_byte_index), playfield_tile_type_line_table))

#define is_playfield_tile_type_from_byte(byte, playfield_tile_in_byte_index, tile_type_table) (get_playfield_tile_value_from_byte_no_shift((byte), (playfield_tile_in_byte_index)) == tile_type_table[(playfield_tile_in_byte_index)])
#define is_playfield_tile_type_with_byte_and_in_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index, tile_type_table) (is_playfield_tile_type_from_byte(playfield_tiles[(playfield_tile_byte_index)], (playfield_tile_in_byte_index), (tile_type_table)))
#define is_playfield_tile_type(playfield_tile_index, tile_type_table) (is_playfield_tile_type_with_byte_and_in_byte_index(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index)), (tile_type_table)))

#define is_playfield_tile_uncleared_unmarked_from_byte(byte, playfield_tile_in_byte_index) (is_playfield_tile_type_from_byte((byte), (playfield_tile_in_byte_index), playfield_tile_type_uncleared_unmarked_table))
#define is_playfield_tile_byte_index_uncleared_unmarked(playfield_tile_byte_index, playfield_tile_in_byte_index) (is_playfield_tile_type_with_byte_and_in_byte_index((playfield_tile_byte_index), (playfield_tile_in_byte_index), playfield_tile_type_uncleared_unmarked_table))
#define is_playfield_tile_index_uncleared_unmarked(playfield_tile_index) (is_playfield_tile_type((playfield_tile_index), playfield_tile_type_uncleared_unmarked_table))

#define is_playfield_tile_uncleared_marked_from_byte(byte, playfield_tile_in_byte_index) (is_playfield_tile_type_from_byte((byte), (playfield_tile_in_byte_index), playfield_tile_type_uncleared_marked_table))
#define is_playfield_tile_byte_index_uncleared_marked(playfield_tile_byte_index, playfield_tile_in_byte_index) (is_playfield_tile_type_with_byte_and_in_byte_index((playfield_tile_byte_index), (playfield_tile_in_byte_index), playfield_tile_type_uncleared_marked_table))
#define is_playfield_tile_index_uncleared_marked(playfield_tile_index) (is_playfield_tile_type((playfield_tile_index), playfield_tile_type_uncleared_marked_table))

#define is_playfield_tile_wall_from_byte(byte, playfield_tile_in_byte_index) (is_playfield_tile_type_from_byte((byte), (playfield_tile_in_byte_index), playfield_tile_type_wall_table))
#define is_playfield_tile_byte_index_wall(playfield_tile_byte_index, playfield_tile_in_byte_index) (is_playfield_tile_type_with_byte_and_in_byte_index((playfield_tile_byte_index), (playfield_tile_in_byte_index), playfield_tile_type_wall_table))
#define is_playfield_tile_index_wall(playfield_tile_index) (is_playfield_tile_type((playfield_tile_index), playfield_tile_type_wall_table))

#define is_playfield_tile_line_from_byte(byte, playfield_tile_in_byte_index) (is_playfield_tile_type_from_byte((byte), (playfield_tile_in_byte_index), playfield_tile_type_line_table))
#define is_playfield_tile_byte_index_line(playfield_tile_byte_index, playfield_tile_in_byte_index) (is_playfield_tile_type_with_byte_and_in_byte_index((playfield_tile_byte_index), (playfield_tile_in_byte_index), playfield_tile_type_line_table))
#define is_playfield_tile_index_line(playfield_tile_index) (is_playfield_tile_type((playfield_tile_index), playfield_tile_type_line_table))

#define PLAYFIELD_BIT_LINE_INDEX 0
#define PLAYFIELD_BITMASK_LINE_INDEX (1 << PLAYFIELD_BIT_LINE_INDEX)
#define PLAYFIELD_BIT_LINE_DIRECTION 1
#define PLAYFIELD_BITMASK_LINE_DIRECTION (1 << PLAYFIELD_BIT_LINE_DIRECTION)

#define PLAYFIELD_TILE_LINE_FLAG_TABLE(name, base_value) PLAYFIELD_TILE_TABLE(playfield_tile_line_##name, base_value)

PLAYFIELD_TILE_LINE_FLAG_TABLE(index, PLAYFIELD_BITMASK_LINE_INDEX)
PLAYFIELD_TILE_LINE_FLAG_TABLE(direction, PLAYFIELD_BITMASK_LINE_DIRECTION)

#define get_playfield_tile_line_flag_from_byte(byte, playfield_tile_in_byte_index, in_byte_table) (get_playfield_tile_masked_value_table((byte), (playfield_tile_in_byte_index), in_byte_table) == in_byte_table[(playfield_tile_in_byte_index)])
#define get_playfield_tile_byte_index_line_flag(playfield_tile_byte_index, playfield_tile_in_byte_index, in_byte_table) (get_playfield_tile_line_flag_from_byte(playfield_line_flags[(playfield_tile_byte_index)], (playfield_tile_in_byte_index), in_byte_table))

#define get_playfield_tile_line_direction_flag_from_byte(byte, playfield_tile_in_byte_index) (get_playfield_tile_line_flag_from_byte((byte), (playfield_tile_in_byte_index), playfield_tile_line_direction_table))
#define get_playfield_tile_byte_index_line_direction_flag(playfield_tile_byte_index, playfield_tile_in_byte_index) (get_playfield_tile_line_direction_flag_from_byte(playfield_line_flags[(playfield_tile_byte_index)], (playfield_tile_in_byte_index)))
#define get_playfield_tile_index_line_direction_flag(playfield_tile_index) (get_playfield_tile_byte_index_line_direction_flag(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index))))

#define get_playfield_tile_line_index_flag_from_byte(byte, playfield_tile_in_byte_index) (get_playfield_tile_line_flag_from_byte((byte), (playfield_tile_in_byte_index), playfield_tile_line_index_table))
#define get_playfield_tile_byte_index_line_index_flag(playfield_tile_byte_index, playfield_tile_in_byte_index) (get_playfield_tile_line_index_flag_from_byte(playfield_line_flags[(playfield_tile_byte_index)], (playfield_tile_in_byte_index)))
#define get_playfield_tile_index_line_index_flag(playfield_tile_index) (get_playfield_tile_byte_index_line_index_flag(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index))))

#define get_playfield_tile_line_flags_from_byte(byte, playfield_tile_in_byte_index) (get_playfield_tile_value_from_byte((byte), (playfield_tile_in_byte_index)))
#define get_playfield_tile_byte_index_line_flags(playfield_tile_byte_index, playfield_tile_in_byte_index) (get_playfield_tile_line_flags_from_byte(playfield_line_flags[(playfield_tile_byte_index)], (playfield_tile_in_byte_index)))
#define get_playfield_tile_index_line_flags(playfield_tile_index) (get_playfield_tile_byte_index_line_flags(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index))))

#define set_playfield_tile_line_flags_in_byte(byte, playfield_tile_in_byte_index, line_index, line_direction) ((byte) = ((byte) & ~(playfield_bitmask_tile_table[(playfield_tile_in_byte_index)])) | (playfield_bitmask_tile_table[(playfield_tile_in_byte_index)] & ((((line_index) << PLAYFIELD_BIT_LINE_INDEX) | ((line_direction) << PLAYFIELD_BIT_LINE_DIRECTION)) << ((playfield_tile_in_byte_index) << 1))))
#define set_playfield_tile_byte_index_line_flags(playfield_tile_byte_index, playfield_tile_in_byte_index, line_index, line_direction) (set_playfield_tile_line_flags_in_byte(playfield_line_flags[(playfield_tile_byte_index)], (playfield_tile_in_byte_index), (line_index), (line_direction)))
#define set_playfield_tile_index_line_flags(playfield_tile_index, line_index, line_direction) (set_playfield_tile_byte_index_line_flags(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index)), (line_index), (line_direction)))

#endif  // __JEZNES_FLAGS_PLAYFIELD_H__
