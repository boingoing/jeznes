//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#ifndef __JEZNES_FLAGS_BASE_H__
#define __JEZNES_FLAGS_BASE_H__

#define get_flag(flags_byte, bitmask) (((flags_byte) & (bitmask)) != 0)
#define set_flag(flags_byte, bitmask) ((flags_byte) |= (bitmask))
#define unset_flag(flags_byte, bitmask) ((flags_byte) &= ~(bitmask))

#endif  // __JEZNES_FLAGS_BASE_H__
