//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_SCORING_H__
#define __JEZNES_SCORING_H__

#define SCORE_PER_CLEARED_TILE 1
#define SCORE_PER_CLEARED_LINE 5
#define SCORE_BONUS_PER_CLEARED_PERCENTAGE 10
#define SCORE_BONUS_PER_REMAINING_LIFE 20

#define get_cleared_bonus(pct) (SCORE_BONUS_PER_CLEARED_PERCENTAGE * (pct))
#define get_lives_bonus(c) (SCORE_BONUS_PER_REMAINING_LIFE * (c))

// Adds to the score for |tile_count| number of cleared tiles.
void add_score_for_cleared_tiles(unsigned char tile_count);

// Adds to the score for a line segment clear.
void add_score_for_cleared_line(void);

// Adds bonus points to the score calculated at level completion.
void add_score_for_level_up(void);

#endif  // __JEZNES_SCORING_H__
