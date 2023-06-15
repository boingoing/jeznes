//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_DATA_H__
#define __JEZNES_DATA_H__

#define make_word(lo, hi) ((lo) | ((hi) << 8))

// Define a lot of helpers for accessing or modifying named data.

#define get_ball_count() (current_ball_count)
#define set_ball_count(v) (current_ball_count = (v))

#define get_player_count() (player_count)
#define set_player_count(v) (player_count = (v))

#define get_playfield_pattern() (current_playfield_pattern)
#define set_playfield_pattern(a) (current_playfield_pattern = (a))

// Define accessors and modifiers for data meant to be used as local temps.
// Note: Make sure that functions in the same stack don't reuse local temps
//       by abstracting the usage into these macros.

#define set_temp_ptr(p) (temp_ptr_1 = (p))
#define get_temp_ptr(type) ((type*)temp_ptr_1)

#define get_playfield_index() (temp_int_3)
#define set_playfield_index(a) (temp_int_3 = (a))
#define inc_playfield_index() (++temp_int_3)

#define get_game_over_mode() (temp_byte_6)
#define set_game_over_mode(a) (temp_byte_6 = (a))

#define get_title_mode() (temp_byte_6)
#define set_title_mode(a) (temp_byte_6 = (a))

#define get_pixel_coord_x() (temp_byte_4)
#define set_pixel_coord_x(a) (temp_byte_4 = (a))
#define get_pixel_coord_y() (temp_byte_4)
#define set_pixel_coord_y(a) (temp_byte_4 = (a))

#define get_line_orientation() (temp_byte_4)
#define set_line_orientation(a) (temp_byte_4 = (a))
#define get_current_playfield_index() (temp_int_1)
#define set_current_playfield_index(a) (temp_int_1 = (a))
#define get_was_line_segment_completed() (temp_byte_3)
#define set_was_line_segment_completed(a) (temp_byte_3 = (a))

#define get_tile_index_delta() (temp_byte_5)
#define set_tile_index_delta(a) (temp_byte_5 = (a))
#define get_negative_line_segment_origin() (temp_int_2)
#define set_negative_line_segment_origin(a) (temp_int_2 = (a))
#define get_positive_line_segment_origin() (temp_int_2)
#define set_positive_line_segment_origin(a) (temp_int_2 = (a))

#define get_flags_byte() (temp_byte_8)
#define set_flags_byte(a) (temp_byte_8 = (a))

#define get_x_direction() (temp_byte_7)
#define set_x_direction(a) (temp_byte_7 = (a))
#define get_y_direction() (temp_byte_7)
#define set_y_direction(a) (temp_byte_7 = (a))

#define get_x_candidate_pixel_coord() (temp_byte_2)
#define set_x_candidate_pixel_coord(a) (temp_byte_2 = (a))
#define inc_x_candidate_pixel_coord() (++temp_byte_2)
#define dec_x_candidate_pixel_coord() (--temp_byte_2)

#define get_y_candidate_pixel_coord() (temp_byte_3)
#define set_y_candidate_pixel_coord(a) (temp_byte_3 = (a))
#define inc_y_candidate_pixel_coord() (++temp_byte_3)
#define dec_y_candidate_pixel_coord() (--temp_byte_3)

#define get_x_compare_pixel_coord() (temp_byte_4)
#define set_x_compare_pixel_coord(a) (temp_byte_4 = (a))
#define get_y_compare_pixel_coord() (temp_byte_4)
#define set_y_compare_pixel_coord(a) (temp_byte_4 = (a))

#define get_playfield_tile_value() (temp_byte_4)
#define set_playfield_tile_value(a) (temp_byte_4 = (a))

#endif  // __JEZNES_DATA_H__
