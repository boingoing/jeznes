//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_DEBUG_H__
#define __JEZNES_DEBUG_H__

// These macros enable various debugging features and should probably be turned off before release
#define DEBUG 0

// DEBUG macro gates all the debug features
#if DEBUG
#define DRAW_GRAY_LINE 1
#define DRAW_BALL_NEAREST_TILE_HIGHLIGHT 1
#define ENABLE_CHEATS 1
#endif

#endif  // __JEZNES_DEBUG_H__
