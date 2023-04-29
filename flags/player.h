//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_FLAGS_PLAYER_H__
#define __JEZNES_FLAGS_PLAYER_H__

#include "base.h"

#define PLAYER_BIT_ORIENTATION 0
#define PLAYER_BITMASK_ORIENTATION (1 << PLAYER_BIT_ORIENTATION)
#define PLAYER_BIT_IS_ROTATE_PRESSED 1
#define PLAYER_BITMASK_IS_ROTATE_PRESSED (1 << PLAYER_BIT_IS_ROTATE_PRESSED)
#define PLAYER_BIT_IS_PLACE_PRESSED 2
#define PLAYER_BITMASK_IS_PLACE_PRESSED (1 << PLAYER_BIT_IS_PLACE_PRESSED)
#define PLAYER_BIT_IS_PAUSE_PRESSED 3
#define PLAYER_BITMASK_IS_PAUSE_PRESSED (1 << PLAYER_BIT_IS_PAUSE_PRESSED)
#define PLAYER_BIT_IS_SELECT_PRESSED 4
#define PLAYER_BITMASK_IS_SELECT_PRESSED (1 << PLAYER_BIT_IS_SELECT_PRESSED)
#define PLAYER_BIT_IS_CHEAT_PRESSED 5
#define PLAYER_BITMASK_IS_CHEAT_PRESSED (1 << PLAYER_BIT_IS_CHEAT_PRESSED)

#define get_player_flag(player_index, bitmask) get_flag(players[(player_index)].flags, (bitmask))
#define set_player_flag(player_index, bitmask) set_flag(players[(player_index)].flags, (bitmask))
#define unset_player_flag(player_index, bitmask) unset_flag(players[(player_index)].flags, (bitmask))

#define get_player_is_rotate_pressed(player_index) get_player_flag((player_index), PLAYER_BITMASK_IS_ROTATE_PRESSED)
#define set_player_is_rotate_pressed(player_index) set_player_flag((player_index), PLAYER_BITMASK_IS_ROTATE_PRESSED)
#define unset_player_is_rotate_pressed(player_index) unset_player_flag((player_index), PLAYER_BITMASK_IS_ROTATE_PRESSED)

#define get_player_is_place_pressed_flag(player_index) get_player_flag((player_index), PLAYER_BITMASK_IS_PLACE_PRESSED)
#define set_player_is_place_pressed_flag(player_index) set_player_flag((player_index), PLAYER_BITMASK_IS_PLACE_PRESSED)
#define unset_player_is_place_pressed_flag(player_index) unset_player_flag((player_index), PLAYER_BITMASK_IS_PLACE_PRESSED)

#define get_player_is_pause_pressed(player_index) get_player_flag((player_index), PLAYER_BITMASK_IS_PAUSE_PRESSED)
#define set_player_is_pause_pressed(player_index) set_player_flag((player_index), PLAYER_BITMASK_IS_PAUSE_PRESSED)
#define unset_player_is_pause_pressed(player_index) unset_player_flag((player_index), PLAYER_BITMASK_IS_PAUSE_PRESSED)

#define get_player_is_select_pressed(player_index) get_player_flag((player_index), PLAYER_BITMASK_IS_SELECT_PRESSED)
#define set_player_is_select_pressed(player_index) set_player_flag((player_index), PLAYER_BITMASK_IS_SELECT_PRESSED)
#define unset_player_is_select_pressed(player_index) unset_player_flag((player_index), PLAYER_BITMASK_IS_SELECT_PRESSED)

#define get_player_is_cheat_pressed(player_index) get_player_flag((player_index), PLAYER_BITMASK_IS_CHEAT_PRESSED)
#define set_player_is_cheat_pressed(player_index) set_player_flag((player_index), PLAYER_BITMASK_IS_CHEAT_PRESSED)
#define unset_player_is_cheat_pressed(player_index) unset_player_flag((player_index), PLAYER_BITMASK_IS_CHEAT_PRESSED)

// Returns either ORIENTATION_HORIZ or ORIENTATION_VERT
#define get_player_orientation_flag(player_index) (players[(player_index)].flags & PLAYER_BITMASK_ORIENTATION)

// Sets the orientation for |player_index| player to |orientation| which must be either ORIENTATION_HORIZ or ORIENTATION_VERT
#define set_player_orientation_flag(player_index, orientation) (players[(player_index)].flags = players[(player_index)].flags & ~LINE_BITMASK_ORIENTATION | (orientation))

#endif  // __JEZNES_FLAGS_PLAYER_H__
