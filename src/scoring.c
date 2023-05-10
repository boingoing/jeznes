//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#include "scoring.h"

#include "bss.h"
#include "constants/game.h"
#include "data.h"
#include "types.h"
#include "zeropage.h"

void add_score_for_cleared_tiles(unsigned char tile_count) {
  score += tile_count * SCORE_PER_CLEARED_TILE;
}

void add_score_for_level_up(void) {
  score += lives_count * SCORE_BONUS_PER_REMAINING_LIFE;
  score += cleared_tile_percentage * SCORE_BONUS_PER_CLEARED_PERCENTAGE;
}
