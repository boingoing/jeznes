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

unsigned int get_front() {
  switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
      return playfield_index_move_right(get_current_position());
    case MOVE_DIRECTION_LEFT:
      return playfield_index_move_left(get_current_position());
    case MOVE_DIRECTION_DOWN:
      return playfield_index_move_down(get_current_position());
    case MOVE_DIRECTION_UP:
      return playfield_index_move_up(get_current_position());
  }
}

unsigned int get_back() {
  switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
      return playfield_index_move_left(get_current_position());
    case MOVE_DIRECTION_LEFT:
      return playfield_index_move_right(get_current_position());
    case MOVE_DIRECTION_DOWN:
      return playfield_index_move_up(get_current_position());
    case MOVE_DIRECTION_UP:
      return playfield_index_move_down(get_current_position());
  }
}

unsigned int get_right() {
  switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
      return playfield_index_move_down(get_current_position());
    case MOVE_DIRECTION_LEFT:
      return playfield_index_move_up(get_current_position());
    case MOVE_DIRECTION_DOWN:
      return playfield_index_move_left(get_current_position());
    case MOVE_DIRECTION_UP:
      return playfield_index_move_right(get_current_position());
  }
}

unsigned int get_left() {
  switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
      return playfield_index_move_up(get_current_position());
    case MOVE_DIRECTION_LEFT:
      return playfield_index_move_down(get_current_position());
    case MOVE_DIRECTION_DOWN:
      return playfield_index_move_right(get_current_position());
    case MOVE_DIRECTION_UP:
      return playfield_index_move_left(get_current_position());
  }
}

unsigned int get_front_left() {
  switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
      return playfield_index_move_right(playfield_index_move_up(get_current_position()));
    case MOVE_DIRECTION_LEFT:
      return playfield_index_move_left(playfield_index_move_down(get_current_position()));
    case MOVE_DIRECTION_DOWN:
      return playfield_index_move_down(playfield_index_move_right(get_current_position()));
    case MOVE_DIRECTION_UP:
      return playfield_index_move_up(playfield_index_move_left(get_current_position()));
  }
}

unsigned int get_back_left() {
  switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
      return playfield_index_move_left(playfield_index_move_up(get_current_position()));
    case MOVE_DIRECTION_LEFT:
      return playfield_index_move_right(playfield_index_move_down(get_current_position()));
    case MOVE_DIRECTION_DOWN:
      return playfield_index_move_up(playfield_index_move_right(get_current_position()));
    case MOVE_DIRECTION_UP:
      return playfield_index_move_down(playfield_index_move_left(get_current_position()));
  }
}

void compute_playfield_mark_bit_one_ball(unsigned char ball_index) {
  // Set cur to starting playfield tile
  set_current_position(balls[ball_index].nearest_playfield_tile);

  // If the playfield tile where |ball_index| is located has already been
  // marked, another ball is in the same region of the playfield as
  // |ball_index|. There's no point in remarking the region.
  if (!inside(get_current_position())) {
    return;
  }

  // Set cur-dir to default direction
  set_cur_dir(MOVE_DIRECTION_DEFAULT);
  // Clear mark and mark2 (set values to null)
  set_mark_null(TRUE);
  set_mark2_null(TRUE);
  // Set backtrack and findloop to false
  set_backtrack(FALSE);
  set_findloop(FALSE);

  // Move forward until the playfield tile in front is marked or not uncleared
  // (ie: it is not inside).
  // Note: This function does not do bounds checking, we assume the playfield
  //       has a border of wall tiles on all sides.
  set_temp_forward_iterator(get_front());
  while (inside(get_temp_forward_iterator())) {
    set_current_position(get_temp_forward_iterator());
    set_temp_forward_iterator(get_front());
  }

  goto PAINTER_ALGORITHM_START;

  while (1) {
    move_forward();

    if (inside(get_right())) {
      if (get_backtrack() == TRUE && get_findloop() == FALSE &&
          (inside(get_front()) || inside(get_left()))) {
        set_findloop(TRUE);
      }
      turn_right();

    PAINTER_ALGORITHM_PAINT:
      move_forward();
    }

  PAINTER_ALGORITHM_START:
    // Count number of non-diagonally adjacent marked playfield tiles.
    set_adjacent_marked_tile_count(0);
    if (!inside(playfield_index_move_up(get_current_position()))) {
      inc_adjacent_marked_tile_count();
    }
    if (!inside(playfield_index_move_down(get_current_position()))) {
      inc_adjacent_marked_tile_count();
    }
    if (!inside(playfield_index_move_left(get_current_position()))) {
      inc_adjacent_marked_tile_count();
    }
    if (!inside(playfield_index_move_right(get_current_position()))) {
      inc_adjacent_marked_tile_count();
    }

    if (get_adjacent_marked_tile_count() != 4) {
      do {
        turn_right();
      } while (inside(get_front()));
      do {
        turn_left();
      } while (!inside(get_front()));
    }

    switch (get_adjacent_marked_tile_count()) {
      case 1:
        if (get_backtrack() == TRUE) {
          set_findloop(TRUE);
        } else if (get_findloop() == TRUE) {
          if (get_mark_null() == TRUE) {
            set_mark_null(FALSE);
          }
        } else if (inside(get_front_left()) && inside(get_back_left())) {
          set_mark_null(TRUE);
          set_playfield_is_marked_flag(get_current_position());
          goto PAINTER_ALGORITHM_PAINT;
        }
        break;
      case 2:
        if (!inside(get_back())) {
          if (inside(get_front_left())) {
            set_mark_null(TRUE);
            set_playfield_is_marked_flag(get_current_position());
            goto PAINTER_ALGORITHM_PAINT;
          }
        } else if (get_mark_null() == TRUE) {
          set_mark(get_current_position());
          set_mark_null(FALSE);
          set_mark_dir(get_cur_dir());
          set_mark2_null(TRUE);
          set_findloop(FALSE);
          set_backtrack(FALSE);
        } else {
          if (get_mark2_null() == TRUE) {
            if (get_current_position() == get_mark()) {
              if (get_cur_dir() == get_mark_dir()) {
                set_mark_null(TRUE);
                reverse_direction();
                set_playfield_is_marked_flag(get_current_position());
                goto PAINTER_ALGORITHM_PAINT;
              } else {
                set_backtrack(TRUE);
                set_findloop(FALSE);
                set_cur_dir(get_mark_dir());
              }
            } else if (get_findloop() == TRUE) {
              set_mark2(get_current_position());
              set_mark2_null(FALSE);
              set_mark2_dir(get_cur_dir());
            }
          } else {
            if (get_current_position() == get_mark()) {
              set_current_position(get_mark2());
              set_cur_dir(get_mark2_dir());
              set_mark_null(TRUE);
              set_mark2_null(TRUE);
              set_backtrack(FALSE);
              reverse_direction();
              set_playfield_is_marked_flag(get_current_position());
              goto PAINTER_ALGORITHM_PAINT;
            } else if (get_current_position() == get_mark2()) {
              set_mark(get_current_position());
              set_mark_null(FALSE);
              set_cur_dir(get_mark2_dir());
              set_mark_dir(get_mark2_dir());
              set_mark2_null(TRUE);
            }
          }
        }
        break;
      case 3:
        set_mark_null(TRUE);
        set_playfield_is_marked_flag(get_current_position());
        goto PAINTER_ALGORITHM_PAINT;
        break;
      case 4:
        set_playfield_is_marked_flag(get_current_position());
        return;
    }
  }
}
