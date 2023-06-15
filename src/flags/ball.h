//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_FLAGS_BALL_H__
#define __JEZNES_FLAGS_BALL_H__

#include "base.h"

#define BALL_BIT_DIRECTION_X 0
#define BALL_BITMASK_DIRECTION_X (1 << BALL_BIT_DIRECTION_X)
#define BALL_BIT_DIRECTION_Y 1
#define BALL_BITMASK_DIRECTION_Y (1 << BALL_BIT_DIRECTION_Y)

#define get_ball_flag(ball_index, bitmask) \
  get_flag(balls[(ball_index)].flags, (bitmask))
#define set_ball_flag(ball_index, bitmask) \
  set_flag(balls[(ball_index)].flags, (bitmask))
#define unset_ball_flag(ball_index, bitmask) \
  unset_flag(balls[(ball_index)].flags, (bitmask))

#define get_ball_x_direction(ball_index) \
  get_ball_flag((ball_index), BALL_BITMASK_DIRECTION_X)
#define set_ball_x_direction(ball_index) \
  set_ball_flag((ball_index), BALL_BITMASK_DIRECTION_X)
#define unset_ball_x_direction(ball_index) \
  unset_ball_flag((ball_index), BALL_BITMASK_DIRECTION_X)

#define get_ball_y_direction(ball_index) \
  get_ball_flag((ball_index), BALL_BITMASK_DIRECTION_Y)
#define set_ball_y_direction(ball_index) \
  set_ball_flag((ball_index), BALL_BITMASK_DIRECTION_Y)
#define unset_ball_y_direction(ball_index) \
  unset_ball_flag((ball_index), BALL_BITMASK_DIRECTION_Y)

// Returns either BALL_DIRECTION_POSITIVE or BALL_DIRECTION_NEGATIVE
#define get_ball_x_direction_flag_from_byte(flags_byte) \
  ((flags_byte)&BALL_BITMASK_DIRECTION_X)

// Sets the ball x-direction |direction| which must be either BALL_DIRECTION_POSITIVE or BALL_DIRECTION_NEGATIVE
#define set_ball_x_direction_flag_in_byte(flags_byte, direction) \
  ((flags_byte) = (flags_byte) & (~BALL_BIT_DIRECTION_X | (direction)))

// Returns either BALL_DIRECTION_POSITIVE or BALL_DIRECTION_NEGATIVE
#define get_ball_y_direction_flag_from_byte(flags_byte) \
  (((flags_byte)&BALL_BITMASK_DIRECTION_Y)>>BALL_BIT_DIRECTION_Y)

// Sets the ball y-direction |direction| which must be either BALL_DIRECTION_POSITIVE or BALL_DIRECTION_NEGATIVE
#define set_ball_y_direction_flag_in_byte(flags_byte, direction) \
  ((flags_byte) = (flags_byte) & (~BALL_BIT_DIRECTION_Y | (direction<<BALL_BIT_DIRECTION_Y)))

#define flip_ball_x_direction_flag_in_byte(flags_byte) \
  ((flags_byte) ^= 1)

#define flip_ball_y_direction_flag_in_byte(flags_byte) \
  ((flags_byte) ^= (1<<BALL_BIT_DIRECTION_Y))

#endif  // __JEZNES_FLAGS_BALL_H__
