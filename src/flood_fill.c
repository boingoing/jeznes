//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#include "flood_fill.h"

#include "lib/neslib.h"
#include "zeropage.h"
#include "bss.h"
#include "types.h"
#include "data.h"
#include "constants/game.h"
#include "flags/playfield.h"

int get_front() {
    switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
        return playfield_index_move_right(get_cur());
    case MOVE_DIRECTION_LEFT:
        return playfield_index_move_left(get_cur());
    case MOVE_DIRECTION_DOWN:
        return playfield_index_move_down(get_cur());
    case MOVE_DIRECTION_UP:
        return playfield_index_move_up(get_cur());
    }
}

int get_back() {
    switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
        return playfield_index_move_left(get_cur());
    case MOVE_DIRECTION_LEFT:
        return playfield_index_move_right(get_cur());
    case MOVE_DIRECTION_DOWN:
        return playfield_index_move_up(get_cur());
    case MOVE_DIRECTION_UP:
        return playfield_index_move_down(get_cur());
    }
}

int get_right() {
    switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
        return playfield_index_move_down(get_cur());
    case MOVE_DIRECTION_LEFT:
        return playfield_index_move_up(get_cur());
    case MOVE_DIRECTION_DOWN:
        return playfield_index_move_left(get_cur());
    case MOVE_DIRECTION_UP:
        return playfield_index_move_right(get_cur());
    }
}

int get_left() {
    switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
        return playfield_index_move_up(get_cur());
    case MOVE_DIRECTION_LEFT:
        return playfield_index_move_down(get_cur());
    case MOVE_DIRECTION_DOWN:
        return playfield_index_move_right(get_cur());
    case MOVE_DIRECTION_UP:
        return playfield_index_move_left(get_cur());
    }
}

int get_front_left() {
    switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
        return playfield_index_move_right(playfield_index_move_up(get_cur()));
    case MOVE_DIRECTION_LEFT:
        return playfield_index_move_left(playfield_index_move_down(get_cur()));
    case MOVE_DIRECTION_DOWN:
        return playfield_index_move_down(playfield_index_move_right(get_cur()));
    case MOVE_DIRECTION_UP:
        return playfield_index_move_up(playfield_index_move_left(get_cur()));
    }
}

int get_back_left() {
    switch (get_cur_dir()) {
    case MOVE_DIRECTION_RIGHT:
        return playfield_index_move_left(playfield_index_move_up(get_cur()));
    case MOVE_DIRECTION_LEFT:
        return playfield_index_move_right(playfield_index_move_down(get_cur()));
    case MOVE_DIRECTION_DOWN:
        return playfield_index_move_up(playfield_index_move_right(get_cur()));
    case MOVE_DIRECTION_UP:
        return playfield_index_move_down(playfield_index_move_left(get_cur()));
    }
}

void compute_playfield_mark_bit_one_ball(unsigned char ball_index) {
    // Set cur to starting playfield tile
    set_cur(balls[ball_index].nearest_playfield_tile);

    // If the playfield tile where |ball_index| is located has already been marked,
    // another ball is in the same region of the playfield as |ball_index|. There's
    // no point in remarking the region.
    if (!inside(get_cur())) {
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
    temp_int_4 = get_front();
    while (inside(temp_int_4)) {
        set_cur(temp_int_4);
        temp_int_4 = get_front();
    }

    goto PAINTER_ALGORITHM_START;

    while(1) {
        // Move forward
        set_cur(get_front());

        if (inside(get_right())) {
            if (get_backtrack() == TRUE && get_findloop() == FALSE && (inside(get_front()) || inside(get_left()))) {
                set_findloop(TRUE);
            }
            // Turn right
            set_cur_dir((get_cur_dir() + 1) % 4);

PAINTER_ALGORITHM_PAINT:
            // Move forward
            set_cur(get_front());
        }

PAINTER_ALGORITHM_START:
        // Count number of non-diagonally adjacent marked playfield tiles.
        temp_byte_6 = 0;
        if (!inside(playfield_index_move_up(get_cur()))) {
            temp_byte_6++;
        }
        if (!inside(playfield_index_move_down(get_cur()))) {
            temp_byte_6++;
        }
        if (!inside(playfield_index_move_left(get_cur()))) {
            temp_byte_6++;
        }
        if (!inside(playfield_index_move_right(get_cur()))) {
            temp_byte_6++;
        }

        if (temp_byte_6 != 4) {
            do {
                // Turn right
                set_cur_dir((get_cur_dir() + 1) % 4);
            } while (inside(get_front()));
            do {
                // Turn left
                set_cur_dir((get_cur_dir() + 3) % 4);
            } while (!inside(get_front()));
        }

        switch (temp_byte_6) {
        case 1:
            if (get_backtrack() == TRUE) {
                set_findloop(TRUE);
            } else if (get_findloop() == TRUE) {
                if (get_mark_null() == TRUE) {
                    set_mark_null(FALSE);
                }
            } else if (inside(get_front_left()) && inside(get_back_left())) {
                set_mark_null(TRUE);
                set_playfield_is_marked_flag(get_cur());
                goto PAINTER_ALGORITHM_PAINT;
            }
            break;
        case 2:
            if (!inside(get_back())) {
                if (inside(get_front_left())) {
                    set_mark_null(TRUE);
                    set_playfield_is_marked_flag(get_cur());
                    goto PAINTER_ALGORITHM_PAINT;
                }
            } else if (get_mark_null() == TRUE) {
                set_mark(get_cur());
                set_mark_null(FALSE);
                set_mark_dir(get_cur_dir());
                set_mark2_null(TRUE);
                set_findloop(FALSE);
                set_backtrack(FALSE);
            } else {
                if (get_mark2_null() == TRUE) {
                    if (get_cur() == get_mark()) {
                        if (get_cur_dir() == get_mark_dir()) {
                            set_mark_null(TRUE);
                            // Turn around
                            set_cur_dir((get_cur_dir() + 2) % 4);
                            set_playfield_is_marked_flag(get_cur());
                            goto PAINTER_ALGORITHM_PAINT;
                        } else {
                            set_backtrack(TRUE);
                            set_findloop(FALSE);
                            set_cur_dir(get_mark_dir());
                        }
                    } else if (get_findloop() == TRUE) {
                        set_mark2(get_cur());
                        set_mark2_null(FALSE);
                        set_mark2_dir(get_cur_dir());
                    }
                } else {
                    if (get_cur() == get_mark()) {
                        set_cur(get_mark2());
                        set_cur_dir(get_mark2_dir());
                        set_mark_null(TRUE);
                        set_mark2_null(TRUE);
                        set_backtrack(FALSE);
                        // Turn around
                        set_cur_dir((get_cur_dir() + 2) % 4);
                        set_playfield_is_marked_flag(get_cur());
                        goto PAINTER_ALGORITHM_PAINT;
                    } else if (get_cur() == get_mark2()) {
                        set_mark(get_cur());
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
            set_playfield_is_marked_flag(get_cur());
            goto PAINTER_ALGORITHM_PAINT;
            break;
        case 4:
            set_playfield_is_marked_flag(get_cur());
            return;
        }
    }
}