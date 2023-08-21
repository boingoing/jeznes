//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_DEBUG_H__
#define __JEZNES_DEBUG_H__

// These macros enable various debugging features and should probably be turned
// off before release.
#define DEBUG 1

// DEBUG macro gates all the debug features.
#if DEBUG

// At the end of each game loop, render a horizontal gray line across the
// screen. The vertical position of this line indicates how much CPU was used
// by the game loop logic.
// ie: A line drawn lower down on the screen consumed more CPU.
#define DRAW_GRAY_LINE 0

// Draw a highlight tile on the playfield at the nearest tile location to each
// ball sprite.
#define DRAW_BALL_NEAREST_TILE_HIGHLIGHT 0

// Enable the use of cheat code input sequences.
#define ENABLE_CHEATS 0

// Put perf-sensitive code blocks into functions instead of macros.
// Use to enable profiling critical code blocks by isolating them.
#define PUT_CRITICAL_CODE_IN_FUNCTIONS 1

#endif

#endif  // __JEZNES_DEBUG_H__
