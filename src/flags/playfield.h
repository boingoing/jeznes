//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_FLAGS_PLAYFIELD_H__
#define __JEZNES_FLAGS_PLAYFIELD_H__

#include "base.h"

#define PLAYFIELD_BIT_LINE_ORIENTATION 7
#define PLAYFIELD_BITMASK_LINE_ORIENTATION (1 << PLAYFIELD_BIT_LINE_ORIENTATION)
#define PLAYFIELD_BIT_LINE_INDEX 6
#define PLAYFIELD_BITMASK_LINE_INDEX (1 << PLAYFIELD_BIT_LINE_INDEX)
#define PLAYFIELD_BIT_LINE_DIRECTION 5
#define PLAYFIELD_BITMASK_LINE_DIRECTION (1 << PLAYFIELD_BIT_LINE_DIRECTION)
#define PLAYFIELD_BIT_MARK 4
#define PLAYFIELD_BITMASK_MARK (1 << PLAYFIELD_BIT_MARK)

// Bitmask for playfield byte which separates only the flag bits.
#define PLAYFIELD_BITMASK_ALL                                          \
  (PLAYFIELD_BITMASK_LINE_ORIENTATION | PLAYFIELD_BITMASK_LINE_INDEX | \
   PLAYFIELD_BITMASK_LINE_DIRECTION | PLAYFIELD_BITMASK_MARK)

// Bitmask for playfield byte which removes the flag bits.
#define PLAYFIELD_BITMASK_NONE (~PLAYFIELD_BITMASK_ALL)

#define get_playfield_flag(playfield_index, bitmask) \
  get_flag(playfield[(playfield_index)], (bitmask))
#define set_playfield_flag(playfield_index, bitmask) \
  set_flag(playfield[(playfield_index)], (bitmask))
#define unset_playfield_flag(playfield_index, bitmask) \
  unset_flag(playfield[(playfield_index)], (bitmask))

#define get_playfield_is_marked_flag(playfield_index) \
  get_playfield_flag((playfield_index), PLAYFIELD_BITMASK_MARK)
#define set_playfield_is_marked_flag(playfield_index) \
  set_playfield_flag((playfield_index), PLAYFIELD_BITMASK_MARK)
#define unset_playfield_is_marked_flag(playfield_index) \
  unset_playfield_flag((playfield_index), PLAYFIELD_BITMASK_MARK)

// Returns the line direction for a playfield byte which will be either
// LINE_DIRECTION_POSITIVE or LINE_DIRECTION_NEGATIVE
#define get_playfield_line_direction_flag_from_byte(flags_byte) \
  (((flags_byte)&PLAYFIELD_BITMASK_LINE_DIRECTION) >>           \
   PLAYFIELD_BIT_LINE_DIRECTION)

// Returns the line direction for a playfield tile at |playfield_index| which
// will be either LINE_DIRECTION_POSITIVE or LINE_DIRECTION_NEGATIVE
#define get_playfield_line_direction_flag(playfield_index) \
  get_playfield_line_direction_flag_from_byte((playfield_index))

// Returns the line orientation for a playfield byte which will be either
// ORIENTATION_HORIZ or ORIENTATION_VERT
#define get_playfield_line_orientation_flag_from_byte(flags_byte) \
  (((flags_byte)&PLAYFIELD_BITMASK_LINE_ORIENTATION) >>           \
   PLAYFIELD_BIT_LINE_ORIENTATION)

// Returns the line orientation for a playfield tile at |playfield_index| which
// will be either ORIENTATION_HORIZ or ORIENTATION_VERT
#define get_playfield_line_orientation_flag(playfield_index) \
  (get_playfield_line_orientation_flag_from_byte(playfield[(playfield_index)]))

// Sets the line orientation for |playfield_index| to |orientation| which must
// be either ORIENTATION_HORIZ or ORIENTATION_VERT
#define set_playfield_line_orientation_flag(playfield_index, orientation)   \
  (playfield[(playfield_index)] =                                           \
       playfield[(playfield_index)] & ~PLAYFIELD_BITMASK_LINE_ORIENTATION | \
       (orientation << PLAYFIELD_BIT_LINE_ORIENTATION))

// Returns the line index from a playfield byte which will be either 0 or 1
#define get_playfield_line_index_flag_from_byte(flags_byte) \
  (((flags_byte)&PLAYFIELD_BITMASK_LINE_INDEX) >> PLAYFIELD_BIT_LINE_INDEX)

// Returns the line index for a playfield tile at |playfield_index| which will
// be either 0 or 1
#define get_playfield_line_index_flag(playfield_index) \
  (get_playfield_line_index_flag_from_byte(playfield[(playfield_index)]))

// Sets the line index for |playfield_index| to |index| which must be either 0
// or 1
#define set_playfield_line_index_flag(playfield_index, index)         \
  (playfield[(playfield_index)] =                                     \
       playfield[(playfield_index)] & ~PLAYFIELD_BITMASK_LINE_INDEX | \
       (index << PLAYFIELD_BIT_LINE_INDEX))

// Returns the playfield tile type from a playfield byte |flags_byte| by
// removing the bit-flags from the playfield byte
#define get_playfield_tile_type_from_byte(flags_byte) \
  ((flags_byte)&PLAYFIELD_BITMASK_NONE)

// Returns the playfield tile type for |playfield_index| by removing the
// bit-flags from the playfield byte
#define get_playfield_tile_type(playfield_index) \
  get_playfield_tile_type_from_byte(playfield[(playfield_index)])

// Get the playfield tile type for lines.
// Indicate horizontal or vertical via |orientation| which should be
// ORIENTATION_HORIZ or ORIENTATION_VERT. Indicate the line index via
// |line_index| which must be 0 or 1. Indicate the line direction via
// |line_direction| which should be LINE_DIRECTION_POSITIVE or
// LINE_DIRECTION_NEGATIVE.
#define get_playfield_tile_type_line(orientation, line_index, line_direction) \
  (PLAYFIELD_LINE | ((orientation) << PLAYFIELD_BIT_LINE_ORIENTATION) |       \
   ((line_index) << PLAYFIELD_BIT_LINE_INDEX) |                               \
   ((line_direction) << PLAYFIELD_BIT_LINE_DIRECTION))

#endif  // __JEZNES_FLAGS_PLAYFIELD_H__
