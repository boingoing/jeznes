//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_FLAGS_PLAYFIELD_H__
#define __JEZNES_FLAGS_PLAYFIELD_H__

#include "base.h"
#include "lib/nesdoug.h"

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

#define PLAYFIELD_BIT_LINE_INDEX 0
#define PLAYFIELD_BITMASK_LINE_INDEX (1 << PLAYFIELD_BIT_LINE_INDEX)
#define PLAYFIELD_BIT_LINE_DIRECTION 1
#define PLAYFIELD_BITMASK_LINE_DIRECTION (1 << PLAYFIELD_BIT_LINE_DIRECTION)

#define PLAYFIELD_TILE_LINE_FLAG_TABLE(name, base_value) PLAYFIELD_TILE_TABLE(playfield_tile_line_##name, base_value)

PLAYFIELD_TILE_LINE_FLAG_TABLE(index, PLAYFIELD_BITMASK_LINE_INDEX)
PLAYFIELD_TILE_LINE_FLAG_TABLE(direction, PLAYFIELD_BITMASK_LINE_DIRECTION)

#define get_playfield_tile_byte_index(playfield_tile_index) ((unsigned char)((playfield_tile_index) >> 2))
#define get_playfield_tile_in_byte_index(playfield_tile_index) (((unsigned char)(playfield_tile_index)) & 0b11)

#define get_playfield_tile_position(playfield_tile_index) (make_word(get_playfield_tile_byte_index(get_current_position()), get_playfield_tile_in_byte_index(get_current_position())))

// Playfield tile accessor macros

#define get_playfield_tile_raw_value_from_byte_index(playfield_tile_byte_index, playfield_tiles_array) (playfield_tiles_array[(playfield_tile_byte_index)])
#define get_playfield_tile_raw_value(playfield_tile_index, playfield_tiles_array) (get_playfield_tile_raw_value_from_byte_index(get_playfield_tile_byte_index((playfield_tile_index)), (playfield_tiles_array)))

#define get_playfield_tile_masked_value_from_raw_byte_and_bitmask(raw_byte, bitmask) ((raw_byte) & (bitmask))
#define get_playfield_tile_masked_value_from_raw_byte(raw_byte, playfield_tile_in_byte_index) (get_playfield_tile_masked_value_from_raw_byte_and_bitmask((raw_byte), playfield_bitmask_tile_table[(playfield_tile_in_byte_index)]))
#define get_playfield_tile_masked_value_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index, playfield_tiles_array) (get_playfield_tile_masked_value_from_raw_byte(get_playfield_tile_raw_value_from_byte_index((playfield_tile_byte_index), (playfield_tiles_array)), (playfield_tile_in_byte_index)))
#define get_playfield_tile_masked_value(playfield_tile_index, playfield_tiles_array) (get_playfield_tile_masked_value_from_byte_index(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index)), (playfield_tiles_array)))

#define get_playfield_tile_value_from_masked_byte(masked_byte, playfield_tile_in_byte_index) ((masked_byte) >> ((playfield_tile_in_byte_index) << 1))
#define get_playfield_tile_value_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index, playfield_tiles_array) (get_playfield_tile_value_from_masked_byte(get_playfield_tile_masked_value_from_byte_index((playfield_tile_byte_index), (playfield_tile_in_byte_index), (playfield_tiles_array)), (playfield_tile_in_byte_index)))
#define get_playfield_tile_value(playfield_tile_index, playfield_tiles_array) (get_playfield_tile_value_from_byte_index(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index)), (playfield_tiles_array)))

#define is_playfield_tile_mask_set_from_raw_byte(raw_byte, playfield_tile_in_byte_index, bitmask) (get_playfield_tile_masked_value_from_raw_byte((raw_byte), (playfield_tile_in_byte_index)) == (bitmask))
#define is_playfield_tile_mask_set_from_raw_byte_table(raw_byte, playfield_tile_in_byte_index, bitmask_table) (is_playfield_tile_mask_set_from_raw_byte((raw_byte), (playfield_tile_in_byte_index), (bitmask_table)[(playfield_tile_in_byte_index)]))
#define is_playfield_tile_mask_set_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index, playfield_tiles_array, bitmask) (is_playfield_tile_mask_set_from_raw_byte(get_playfield_tile_raw_value_from_byte_index((playfield_tile_byte_index), (playfield_tiles_array)), (playfield_tile_in_byte_index), (bitmask)))
#define is_playfield_tile_mask_set_from_byte_index_table(playfield_tile_byte_index, playfield_tile_in_byte_index, playfield_tiles_array, bitmask_table) (is_playfield_tile_mask_set_from_byte_index((playfield_tile_byte_index), (playfield_tile_in_byte_index), (playfield_tiles_array), (bitmask_table)[(playfield_tile_in_byte_index)]))
#define is_playfield_tile_mask_set(playfield_tile_index, playfield_tiles_array, bitmask) (is_playfield_tile_mask_set_from_byte_index(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index)), (playfield_tiles_array), (bitmask)))
#define is_playfield_tile_mask_set_table(playfield_tile_index, playfield_tiles_array, bitmask_table) (is_playfield_tile_mask_set_from_byte_index_table(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index)), (playfield_tiles_array), (bitmask_table)))

#define set_playfield_tile_value_in_raw_byte(raw_byte, playfield_tile_in_byte_index, value_bitmask) ((raw_byte) = (raw_byte & ~(playfield_bitmask_tile_table[(playfield_tile_in_byte_index)])) | (value_bitmask))
#define set_playfield_tile_value_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index, playfield_tiles_array, value_bitmask) (set_playfield_tile_value_in_raw_byte(get_playfield_tile_raw_value_from_byte_index((playfield_tile_byte_index), (playfield_tiles_array)), (playfield_tile_in_byte_index), (value_bitmask)))
#define set_playfield_tile_value_from_byte_index_table(playfield_tile_byte_index, playfield_tile_in_byte_index, playfield_tiles_array, value_bitmask_table) (set_playfield_tile_value_from_byte_index((playfield_tile_byte_index), (playfield_tile_in_byte_index), (playfield_tiles_array), (value_bitmask_table)[(playfield_tile_in_byte_index)]))
#define set_playfield_tile_value(playfield_tile_index, playfield_tiles_array, value_bitmask) (set_playfield_tile_value_from_byte_index(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index)), (playfield_tiles_array), (value_bitmask)))
#define set_playfield_tile_value_table(playfield_tile_index, playfield_tiles_array, value_bitmask_table) (set_playfield_tile_value_from_byte_index_table(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index)), (playfield_tiles_array), (value_bitmask_table)))

// Playfield tile type accessor macros

#define is_playfield_tile_type_uncleared_unmarked_from_byte(byte, playfield_tile_in_byte_index) (is_playfield_tile_mask_set_from_raw_byte_table((byte), (playfield_tile_in_byte_index), playfield_tile_type_uncleared_unmarked_table))
#define is_playfield_tile_type_uncleared_unmarked_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index) (is_playfield_tile_mask_set_from_byte_index_table((playfield_tile_byte_index), (playfield_tile_in_byte_index), playfield_tiles, playfield_tile_type_uncleared_unmarked_table))
#define is_playfield_tile_type_uncleared_unmarked(playfield_tile_index) (is_playfield_tile_mask_set_table((playfield_tile_index), playfield_tiles, playfield_tile_type_uncleared_unmarked_table))

#define is_playfield_tile_type_uncleared_marked_from_byte(byte, playfield_tile_in_byte_index) (is_playfield_tile_mask_set_from_raw_byte_table((byte), (playfield_tile_in_byte_index), playfield_tile_type_uncleared_marked_table))
#define is_playfield_tile_type_uncleared_marked_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index) (is_playfield_tile_mask_set_from_byte_index_table((playfield_tile_byte_index), (playfield_tile_in_byte_index), playfield_tiles, playfield_tile_type_uncleared_marked_table))
#define is_playfield_tile_type_uncleared_marked(playfield_tile_index) (is_playfield_tile_mask_set_table((playfield_tile_index), playfield_tiles, playfield_tile_type_uncleared_marked_table))

#define is_playfield_tile_type_wall_from_byte(byte, playfield_tile_in_byte_index) (is_playfield_tile_mask_set_from_raw_byte_table((byte), (playfield_tile_in_byte_index), playfield_tile_type_wall_table))
#define is_playfield_tile_type_wall_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index) (is_playfield_tile_mask_set_from_byte_index_table((playfield_tile_byte_index), (playfield_tile_in_byte_index), playfield_tiles, playfield_tile_type_wall_table))
#define is_playfield_tile_type_wall(playfield_tile_index) (is_playfield_tile_mask_set_table((playfield_tile_index), playfield_tiles, playfield_tile_type_wall_table))

#define is_playfield_tile_type_line_from_byte(byte, playfield_tile_in_byte_index) (is_playfield_tile_mask_set_from_raw_byte_table((byte), (playfield_tile_in_byte_index), playfield_tile_type_line_table))
#define is_playfield_tile_type_line_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index) (is_playfield_tile_mask_set_from_byte_index_table((playfield_tile_byte_index), (playfield_tile_in_byte_index), playfield_tiles, playfield_tile_type_line_table))
#define is_playfield_tile_type_line(playfield_tile_index) (is_playfield_tile_mask_set_table((playfield_tile_index), playfield_tiles, playfield_tile_type_line_table))

#define set_playfield_tile_type_uncleared_unmarked_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index) (playfield_tiles[(playfield_tile_byte_index)] &= ~(playfield_bitmask_tile_table[(playfield_tile_in_byte_index)]))
#define set_playfield_tile_type_wall_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index) (playfield_tiles[(playfield_tile_byte_index)] |= playfield_bitmask_tile_table[(playfield_tile_in_byte_index)])
#define set_playfield_tile_type_uncleared_marked_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index) (set_playfield_tile_value_from_byte_index_table((playfield_tile_byte_index), (playfield_tile_in_byte_index), playfield_tiles, playfield_tile_type_uncleared_marked_table))
#define set_playfield_tile_type_line_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index) (set_playfield_tile_value_from_byte_index_table((playfield_tile_byte_index), (playfield_tile_in_byte_index), playfield_tiles, playfield_tile_type_line_table))

#define set_playfield_tile_type_uncleared_unmarked(playfield_tile_index) (set_playfield_tile_type_uncleared_unmarked_from_byte_index(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index))))
#define set_playfield_tile_type_wall(playfield_tile_index) (set_playfield_tile_type_wall_from_byte_index(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index))))
#define set_playfield_tile_type_uncleared_marked(playfield_tile_index) (set_playfield_tile_type_uncleared_marked_from_byte_index(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index))))
#define set_playfield_tile_type_line(playfield_tile_index) (set_playfield_tile_type_line_from_byte_index(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index))))

// Playfield tile line flags accessor macros

#define is_playfield_tile_line_index_from_byte(byte, playfield_tile_in_byte_index) (is_playfield_tile_mask_set_from_raw_byte_table((byte), (playfield_tile_in_byte_index), playfield_tile_line_index_table))
#define is_playfield_tile_line_index_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index) (is_playfield_tile_mask_set_from_byte_index_table((playfield_tile_byte_index), (playfield_tile_in_byte_index), playfield_line_flags, playfield_tile_line_index_table))
#define is_playfield_tile_line_index(playfield_tile_index) (is_playfield_tile_mask_set_table((playfield_tile_index), playfield_line_flags, playfield_tile_line_index_table))

#define is_playfield_tile_line_direction_from_byte(byte, playfield_tile_in_byte_index) (is_playfield_tile_mask_set_from_raw_byte_table((byte), (playfield_tile_in_byte_index), playfield_tile_line_direction_table))
#define is_playfield_tile_line_direction_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index) (is_playfield_tile_mask_set_from_byte_index_table((playfield_tile_byte_index), (playfield_tile_in_byte_index), playfield_line_flags, playfield_tile_line_direction_table))
#define is_playfield_tile_line_direction(playfield_tile_index) (is_playfield_tile_mask_set_table((playfield_tile_index), playfield_line_flags, playfield_tile_line_direction_table))

#define set_playfield_tile_line_flags_in_byte(byte, playfield_tile_in_byte_index, line_index, line_direction) (set_playfield_tile_value_in_raw_byte((byte), (playfield_tile_in_byte_index), ((line_index) ? playfield_tile_line_index_table[(playfield_tile_in_byte_index)] : 0x0) | ((line_direction) ? playfield_tile_line_direction_table[(playfield_tile_in_byte_index)] : 0x0)))
#define set_playfield_tile_line_flags_from_byte_index(playfield_tile_byte_index, playfield_tile_in_byte_index, line_index, line_direction) (set_playfield_tile_line_flags_in_byte(playfield_line_flags[(playfield_tile_byte_index)], (playfield_tile_in_byte_index), (line_index), (line_direction)))
#define set_playfield_tile_line_flags(playfield_tile_index, line_index, line_direction) (set_playfield_tile_line_flags_from_byte_index(get_playfield_tile_byte_index((playfield_tile_index)), get_playfield_tile_in_byte_index((playfield_tile_index)), (line_index), (line_direction)))

#endif  // __JEZNES_FLAGS_PLAYFIELD_H__
