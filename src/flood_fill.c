//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#include "flood_fill.h"

#include "bss.h"
#include "constants/game.h"
#include "data.h"
#include "flags/playfield.h"
#include "lib/neslib.h"
#include "types.h"
#include "zeropage.h"

void compute_playfield_mark_bit_one_region(void) {
  // If the playfield tile at |get_current_position()| is marked, the region
  // containing the tile has already been marked. There's no point in remarking
  // the region.
  if (!is_current_inside()) {
    return;
  }

  // Set cur-dir to default direction
  set_cur_dir(MOVE_DIRECTION_DEFAULT);
  // Clear mark and mark2 (set values to null)
  set_mark1_null();
  set_mark2_null();
  // Set backtrack and findloop to false
  unset_backtrack();
  unset_findloop();

  // Move forward until the playfield tile in front is marked or not uncleared
  // (ie: it is not inside).
  // Note: This function does not do bounds checking, we assume the playfield
  //       has a border of wall tiles on all sides.
  while (is_front_inside()) {
    move_forward();
  }

  goto PAINTER_ALGORITHM_START;

  while (1) {
    move_forward();

    if (is_right_inside()) {
      if (get_backtrack() && !get_findloop() &&
          (is_front_inside() || is_left_inside())) {
        set_findloop();
      }
      turn_right();

    PAINTER_ALGORITHM_PAINT:
      move_forward();
    }

  PAINTER_ALGORITHM_START:
    // Count number of non-diagonally adjacent marked playfield tiles.
    set_adjacent_marked_tile_count(0);
    if (!is_above_tile_inside()) {
      inc_adjacent_marked_tile_count();
    }
    if (!is_below_tile_inside()) {
      inc_adjacent_marked_tile_count();
    }
    if (!is_left_tile_inside()) {
      inc_adjacent_marked_tile_count();
    }
    if (!is_right_tile_inside()) {
      inc_adjacent_marked_tile_count();
    }

    if (get_adjacent_marked_tile_count() != 4) {
      do {
        turn_right();
      } while (is_front_inside());
      do {
        turn_left();
      } while (!is_front_inside());
    }

    switch (get_adjacent_marked_tile_count()) {
      case 1:
        if (get_backtrack()) {
          set_findloop();
        } else if (get_findloop()) {
          if (get_mark1_null()) {
            unset_mark1_null();
          }
        } else if (is_front_left_inside() && is_back_left_inside()) {
          set_mark1_null();
          paint_current_position();
          goto PAINTER_ALGORITHM_PAINT;
        }
        break;
      case 2:
        if (!is_back_inside()) {
          if (is_front_left_inside()) {
            set_mark1_null();
            paint_current_position();
            goto PAINTER_ALGORITHM_PAINT;
          }
        } else if (get_mark1_null()) {
          set_mark1(get_current_position());
          unset_mark1_null();
          set_mark1_dir(get_cur_dir());
          set_mark2_null();
          unset_findloop();
          unset_backtrack();
        } else {
          if (get_mark2_null()) {
            if (get_current_position() == get_mark1()) {
              if (get_cur_dir() == get_mark1_dir()) {
                set_mark1_null();
                reverse_direction();
                paint_current_position();
                goto PAINTER_ALGORITHM_PAINT;
              } else {
                set_backtrack();
                unset_findloop();
                set_cur_dir(get_mark1_dir());
              }
            } else if (get_findloop()) {
              set_mark2(get_current_position());
              unset_mark2_null();
              set_mark2_dir(get_cur_dir());
            }
          } else {
            if (get_current_position() == get_mark1()) {
              set_current_position(get_mark2());
              set_cur_dir(get_mark2_dir());
              set_mark1_null();
              set_mark2_null();
              unset_backtrack();
              reverse_direction();
              paint_current_position();
              goto PAINTER_ALGORITHM_PAINT;
            } else if (get_current_position() == get_mark2()) {
              set_mark1(get_current_position());
              unset_mark1_null();
              set_cur_dir(get_mark2_dir());
              set_mark1_dir(get_mark2_dir());
              set_mark2_null();
            }
          }
        }
        break;
      case 3:
        set_mark1_null();
        paint_current_position();
        goto PAINTER_ALGORITHM_PAINT;
        break;
      case 4:
        paint_current_position();
        return;
    }
  }
}
