//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_FLAGS_LINE_H__
#define __JEZNES_FLAGS_LINE_H__

#include "base.h"

#define LINE_BIT_ORIENTATION 0
#define LINE_BITMASK_ORIENTATION (1 << LINE_BIT_ORIENTATION)
#define LINE_BIT_IS_STARTED 1
#define LINE_BITMASK_IS_STARTED (1 << LINE_BIT_IS_STARTED)
#define LINE_BIT_IS_POS_COMPLETE 2
#define LINE_BITMASK_IS_POS_COMPLETE (1 << LINE_BIT_IS_POS_COMPLETE)
#define LINE_BIT_IS_NEG_COMPLETE 3
#define LINE_BITMASK_IS_NEG_COMPLETE (1 << LINE_BIT_IS_NEG_COMPLETE)
#define LINE_BIT_IS_FIRST_STEP 4
#define LINE_BITMASK_IS_FIRST_STEP (1 << LINE_BIT_IS_FIRST_STEP)

#define get_line_flag(line_index, bitmask) \
  get_flag(lines[(line_index)].flags, (bitmask))
#define set_line_flag(line_index, bitmask) \
  set_flag(lines[(line_index)].flags, (bitmask))
#define unset_line_flag(line_index, bitmask) \
  unset_flag(lines[(line_index)].flags, (bitmask))

#define get_line_is_first_step_flag_from_byte(flags_byte) \
  get_flag((flags_byte), LINE_BITMASK_IS_FIRST_STEP)
#define set_line_is_first_step_flag_in_byte(flags_byte) \
  set_flag((flags_byte), LINE_BITMASK_IS_FIRST_STEP)
#define unset_line_is_first_step_flag_in_byte(flags_byte) \
  unset_flag((flags_byte), LINE_BITMASK_IS_FIRST_STEP)

#define get_line_is_first_step_flag(line_index) \
  get_line_flag((line_index), LINE_BITMASK_IS_FIRST_STEP)
#define set_line_is_first_step_flag(line_index) \
  set_line_flag((line_index), LINE_BITMASK_IS_FIRST_STEP)
#define unset_line_is_first_step_flag(line_index) \
  unset_line_flag((line_index), LINE_BITMASK_IS_FIRST_STEP)

#define get_line_is_started_flag_from_byte(flags_byte) \
  get_flag((flags_byte), LINE_BITMASK_IS_STARTED)
#define set_line_is_started_flag_in_byte(flags_byte) \
  set_flag((flags_byte), LINE_BITMASK_IS_STARTED)
#define unset_line_is_started_flag_in_byte(flags_byte) \
  unset_flag((flags_byte), LINE_BITMASK_IS_STARTED)

#define get_line_is_started_flag(line_index) \
  get_line_flag((line_index), LINE_BITMASK_IS_STARTED)
#define set_line_is_started_flag(line_index) \
  set_line_flag((line_index), LINE_BITMASK_IS_STARTED)
#define unset_line_is_started_flag(line_index) \
  unset_line_flag((line_index), LINE_BITMASK_IS_STARTED)

#define get_line_is_positive_complete_flag_from_byte(flags_byte) \
  get_flag((flags_byte), LINE_BITMASK_IS_POS_COMPLETE)
#define set_line_is_positive_complete_flag_in_byte(flags_byte) \
  set_flag((flags_byte), LINE_BITMASK_IS_POS_COMPLETE)
#define unset_line_is_positive_complete_flag_in_byte(flags_byte) \
  unset_flag((flags_byte), LINE_BITMASK_IS_POS_COMPLETE)

#define get_line_is_positive_complete_flag(line_index) \
  get_line_flag((line_index), LINE_BITMASK_IS_POS_COMPLETE)
#define set_line_is_positive_complete_flag(line_index) \
  set_line_flag((line_index), LINE_BITMASK_IS_POS_COMPLETE)
#define unset_line_is_positive_complete_flag(line_index) \
  unset_line_flag((line_index), LINE_BITMASK_IS_POS_COMPLETE)

#define get_line_is_negative_complete_flag_from_byte(flags_byte) \
  get_flag((flags_byte), LINE_BITMASK_IS_NEG_COMPLETE)
#define set_line_is_negative_complete_flag_in_byte(flags_byte) \
  set_flag((flags_byte), LINE_BITMASK_IS_NEG_COMPLETE)
#define unset_line_is_negative_complete_flag_in_byte(flags_byte) \
  unset_flag((flags_byte), LINE_BITMASK_IS_NEG_COMPLETE)

#define get_line_is_negative_complete_flag(line_index) \
  get_line_flag((line_index), LINE_BITMASK_IS_NEG_COMPLETE)
#define set_line_is_negative_complete_flag(line_index) \
  set_line_flag((line_index), LINE_BITMASK_IS_NEG_COMPLETE)
#define unset_line_is_negative_complete_flag(line_index) \
  unset_line_flag((line_index), LINE_BITMASK_IS_NEG_COMPLETE)

// Returns either ORIENTATION_HORIZ or ORIENTATION_VERT
#define get_line_orientation_flag_from_byte(flags_byte) \
  ((flags_byte)&LINE_BITMASK_ORIENTATION)

// Returns either ORIENTATION_HORIZ or ORIENTATION_VERT
#define get_line_orientation_flag(line_index) \
  (get_line_orientation_flag_from_byte(lines[(line_index)].flags))

// Sets the orientation for |line_index| line to |orientation| which must be
// either ORIENTATION_HORIZ or ORIENTATION_VERT
#define set_line_orientation_flag(line_index, orientation) \
  (lines[(line_index)].flags =                             \
       lines[(line_index)].flags & ~LINE_BITMASK_ORIENTATION | (orientation))

// Get the sprite index for a single line tile.
// Indicate horizontal or vertical via |orientation| which must be
// ORIENTATION_HORIZ or ORIENTATION_VERT. Indicate the line segment front tile
// completion via |completion| which should be [0,7].
#define get_line_sprite_index(orientation, completion) \
  (SPRITE_INDEX_PLAYFIELD_LINE_HORIZ_BASE + (completion) + 0x10 * (orientation))

// Table of sprite flags indexed by orientation flag.
const unsigned char negative_direction_line_sprite_flags_table[] = {
    OAM_FLIP_H,
    OAM_FLIP_V,
};

// Negative-direction line segment front tile sprite needs to be flipped
// (OAM_FLIP_H 0x40). If the line is vertical, we need to do vertical flip
// instead (OAM_FLIP_V 0x80).
#define get_negative_direction_line_sprite_flags(orientation) \
  (negative_direction_line_sprite_flags_table[(orientation)])

#endif  // __JEZNES_FLAGS_LINE_H__
