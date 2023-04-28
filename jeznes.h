#define MAX_PLAYERS 2
#define PLAYER_SPEED 0x2

#define MAX_BALLS 20
#define BALL_SPEED 0x1
#define BALL_WIDTH 8
#define BALL_HEIGHT 8

#define TARGET_CLEARED_TILE_PERCENTAGE 75

// Playfield tile offsets
#define PLAYFIELD_FIRST_TILE_X 1
#define PLAYFIELD_FIRST_TILE_Y 2
#define PLAYFIELD_FIRST_TILE_INDEX 32
#define PLAYFIELD_WIDTH 32
#define PLAYFIELD_HEIGHT 22

// Playfield bounds in pixel-coords
#define PLAYFIELD_LEFT_WALL 0xe
#define PLAYFIELD_RIGHT_WALL 0xea
#define PLAYFIELD_TOP_WALL 0xd
#define PLAYFIELD_BOTTOM_WALL 0xa9

#define SPRITE_INDEX_PLAYFIELD_LINE_HORIZ_BASE 0x80
#define SPRITE_INDEX_PLAYFIELD_LINE_VERT_BASE 0x90

#define SPRITE_INDEX_CURSOR 0x17

#define TILE_INDEX_PLAYFIELD_UNCLEARED 0x0
#define TILE_INDEX_PLAYFIELD_CLEARED 0x3
#define TILE_INDEX_PLAYFIELD_LINE_HORIZ 0x1
#define TILE_INDEX_PLAYFIELD_LINE_VERT 0x2

#define TILE_INDEX_BALL_BASE 0x30
#define TILE_INDEX_TILE_HIGHLIGHT 0x18
#define TILE_INDEX_ALPHANUMERIC_ZERO 0x1e

// HUD value locations in tile-coords
#define HUD_LEVEL_DISPLAY_TILE_X 8
#define HUD_LEVEL_DISPLAY_TILE_Y 24
#define HUD_LIVES_DISPLAY_TILE_X 8
#define HUD_LIVES_DISPLAY_TILE_Y 25
#define HUD_CLEAR_DISPLAY_TILE_X 18
#define HUD_CLEAR_DISPLAY_TILE_Y 24
#define HUD_TARGET_DISPLAY_TILE_X 18
#define HUD_TARGET_DISPLAY_TILE_Y 25

// Cursor sprite location for the title screen
#define TITLE_CURSOR_1_PLAYER_X 0x58
#define TITLE_CURSOR_1_PLAYER_Y 0xa8
#define TITLE_CURSOR_2_PLAYERS_X 0x58
#define TITLE_CURSOR_2_PLAYERS_Y 0xb8

// Cursor sprite location for the game over screen
#define GAME_OVER_CURSOR_RETRY_X 0x70
#define GAME_OVER_CURSOR_RETRY_Y 0xc0
#define GAME_OVER_CURSOR_QUIT_X 0x70
#define GAME_OVER_CURSOR_QUIT_Y 0xd0

// These macros enable various debugging features and should probably be turned off before release
#define DEBUG 0

// DEBUG macro gates all the debug features
#if DEBUG
#define DRAW_GRAY_LINE 1
#define DRAW_BALL_NEAREST_TILE_HIGHLIGHT 1
#define ENABLE_CHEATS 1
#endif

enum {
    GAME_STATE_TITLE,
    GAME_STATE_PLAYING,
    GAME_STATE_LEVEL_UP,
    GAME_STATE_LEVEL_DOWN,
    GAME_STATE_GAME_OVER,
    GAME_STATE_UPDATING_PLAYFIELD,
    GAME_STATE_REQUEST_HUD_UPDATE
};

enum {
    ORIENTATION_HORIZ,
    ORIENTATION_VERT
};

enum {
    LINE_DIRECTION_POSITIVE,
    LINE_DIRECTION_NEGATIVE
};

enum {
    PLAYFIELD_UNCLEARED,
    PLAYFIELD_WALL,
    PLAYFIELD_LINE
};

enum {
    GAME_OVER_RETRY,
    GAME_OVER_QUIT
};

enum {
    TITLE_1_PLAYER,
    TITLE_2_PLAYERS
};

struct Player {
    // Player metasprite location in pixel-coords
    unsigned char x;
    unsigned char y;

    // Pixel-coords of nearest bg tile under the player
    unsigned char nearest_tile_x;
    unsigned char nearest_tile_y;

    // Playfield tile index for nearest tile
    int nearest_playfield_tile;

    // Hold bit-flags used to track state of this player.
    unsigned char flags;
};

struct Ball {
    // Ball sprite location in pixel-coords
    unsigned char x;
    unsigned char y;

    signed char x_velocity;
    signed char y_velocity;

    // Playfield tile index for nearest tile
    int nearest_playfield_tile;
};

struct Line {
    // Origin playfield tile index for the line.
    // This is the tile on which the player pressed the start line button.
    // For the game, the origin tile is always part of the line segment which
    // spreads in the negative direction and the tile to the right
    // (for ORIENTATION_HORIZ lines) or the tile below it (for ORIENTATION_VERT
    // lines) is where the line segment which spreads in the positive direction
    // begins.
    int origin;

    // How many steps away from the origin playfield tiles have we taken?
    unsigned char tile_step_count;

    // Completion of the current block [0-7]
    unsigned char current_block_completion;

    // Hold bit-flags used to track state of this line.
    unsigned char flags;
};

// Used by sprite collision detection code.
struct ObjectBase {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
};

#pragma bss-name(push, "ZEROPAGE")

// Placeholder to track how many bytes are unused in the zeropage.
unsigned char unused_zp_bytes[11];

unsigned char pads[MAX_PLAYERS];
unsigned char pads_new[MAX_PLAYERS];

unsigned char game_state;
unsigned char current_level;
unsigned char current_playfield_pattern;
unsigned char player_count;
unsigned char lives_count;
unsigned char cleared_tile_percentage;
unsigned int cleared_tile_count;

unsigned char temp_byte_1;
unsigned char temp_byte_2;
unsigned char temp_byte_3;
unsigned char temp_byte_4;
unsigned char temp_byte_5;
unsigned char temp_byte_6;

signed char temp_signed_byte_1;
signed char temp_signed_byte_2;

int temp_int_1;
int temp_int_2;
int temp_int_3;
int temp_int_4;

struct Player players[MAX_PLAYERS];
struct Ball balls[MAX_BALLS];
struct Line lines[MAX_PLAYERS];

#define make_word(lo,hi) ((lo)|(hi << 8))

#define get_ball_count() (current_level+1)
#define get_player_count() (player_count)

#define get_playfield_pattern() (current_playfield_pattern)
#define set_playfield_pattern(a) (current_playfield_pattern = (a))

#define get_playfield_index() (temp_int_3)
#define set_playfield_index(a) (temp_int_3 = (a))
#define inc_playfield_index() (++temp_int_3)

#define get_game_over_mode() (temp_byte_6)
#define set_game_over_mode(a) (temp_byte_6 = (a))

#define get_title_mode() (temp_byte_6)
#define set_title_mode(a) (temp_byte_6 = (a))

// Calculate the playfield tile position in (x,y) of the playfield tile |i|.
#define playfield_index_x(i) ((i)%32)
#define playfield_index_y(i) ((i)>>5)

// Calculate the bg tile position in pixel coords of the playfield tile |i|.
#define playfield_index_pixel_coord_x(i) (playfield_index_x((i)+PLAYFIELD_FIRST_TILE_INDEX)<<3)
#define playfield_index_pixel_coord_y(i) (playfield_index_y((i)+PLAYFIELD_FIRST_TILE_INDEX)<<3)

#define get_flag(flags_byte, bitmask) (((flags_byte) & (bitmask)) != 0)
#define set_flag(flags_byte, bitmask) ((flags_byte) |= (bitmask))
#define unset_flag(flags_byte, bitmask) ((flags_byte) &= ~(bitmask))

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

#define LINE_BIT_ORIENTATION 0
#define LINE_BITMASK_ORIENTATION (1 << LINE_BIT_ORIENTATION)
#define LINE_BIT_IS_STARTED 1
#define LINE_BITMASK_IS_STARTED (1 << LINE_BIT_IS_STARTED)
#define LINE_BIT_IS_POS_COMPLETE 2
#define LINE_BITMASK_IS_POS_COMPLETE (1 << LINE_BIT_IS_POS_COMPLETE)
#define LINE_BIT_IS_NEG_COMPLETE 3
#define LINE_BITMASK_IS_NEG_COMPLETE (1 << LINE_BIT_IS_NEG_COMPLETE)

#define get_line_flag(line_index, bitmask) get_flag(lines[(line_index)].flags, (bitmask))
#define set_line_flag(line_index, bitmask) set_flag(lines[(line_index)].flags, (bitmask))
#define unset_line_flag(line_index, bitmask) unset_flag(lines[(line_index)].flags, (bitmask))

#define get_line_is_started_flag(line_index) get_line_flag((line_index), LINE_BITMASK_IS_STARTED)
#define set_line_is_started_flag(line_index) set_line_flag((line_index), LINE_BITMASK_IS_STARTED)
#define unset_line_is_started_flag(line_index) unset_line_flag((line_index), LINE_BITMASK_IS_STARTED)

#define get_line_is_positive_complete_flag(line_index) get_line_flag((line_index), LINE_BITMASK_IS_POS_COMPLETE)
#define set_line_is_positive_complete_flag(line_index) set_line_flag((line_index), LINE_BITMASK_IS_POS_COMPLETE)
#define unset_line_is_positive_complete_flag(line_index) unset_line_flag((line_index), LINE_BITMASK_IS_POS_COMPLETE)

#define get_line_is_negative_complete_flag(line_index) get_line_flag((line_index), LINE_BITMASK_IS_NEG_COMPLETE)
#define set_line_is_negative_complete_flag(line_index) set_line_flag((line_index), LINE_BITMASK_IS_NEG_COMPLETE)
#define unset_line_is_negative_complete_flag(line_index) unset_line_flag((line_index), LINE_BITMASK_IS_NEG_COMPLETE)

// Returns either ORIENTATION_HORIZ or ORIENTATION_VERT
#define get_line_orientation_flag(line_index) (lines[(line_index)].flags & LINE_BITMASK_ORIENTATION)

// Sets the orientation for |line_index| line to |orientation| which must be either ORIENTATION_HORIZ or ORIENTATION_VERT
#define set_line_orientation_flag(line_index, orientation) (lines[(line_index)].flags = lines[(line_index)].flags & ~LINE_BITMASK_ORIENTATION | (orientation))

// Get the sprite index for a single line tile.
// Indicate horizontal or vertical via |orientation| which must be ORIENTATION_HORIZ or ORIENTATION_VERT.
// Indicate the line segment front tile completion via |completion| which should be [0,7].
#define get_line_sprite_index(orientation, completion) (SPRITE_INDEX_PLAYFIELD_LINE_HORIZ_BASE + (completion) + 0x10 * (orientation))

#define PLAYFIELD_BIT_LINE_ORIENTATION 7
#define PLAYFIELD_BITMASK_LINE_ORIENTATION (1 << PLAYFIELD_BIT_LINE_ORIENTATION)
#define PLAYFIELD_BIT_LINE_INDEX 6
#define PLAYFIELD_BITMASK_LINE_INDEX (1 << PLAYFIELD_BIT_LINE_INDEX)
#define PLAYFIELD_BIT_LINE_DIRECTION 5
#define PLAYFIELD_BITMASK_LINE_DIRECTION (1 << PLAYFIELD_BIT_LINE_DIRECTION)
#define PLAYFIELD_BIT_MARK 4
#define PLAYFIELD_BITMASK_MARK (1 << PLAYFIELD_BIT_MARK)

// Bitmask for playfield byte which separates only the flag bits.
#define PLAYFIELD_BITMASK_ALL (PLAYFIELD_BITMASK_LINE_ORIENTATION | PLAYFIELD_BITMASK_LINE_INDEX | PLAYFIELD_BITMASK_LINE_DIRECTION | PLAYFIELD_BITMASK_MARK)

// Bitmask for playfield byte which removes the flag bits.
#define PLAYFIELD_BITMASK_NONE (~PLAYFIELD_BITMASK_ALL)

#define get_playfield_flag(playfield_index, bitmask) get_flag(playfield[(playfield_index)], (bitmask))
#define set_playfield_flag(playfield_index, bitmask) set_flag(playfield[(playfield_index)], (bitmask))
#define unset_playfield_flag(playfield_index, bitmask) unset_flag(playfield[(playfield_index)], (bitmask))

#define get_playfield_is_marked_flag(playfield_index) get_playfield_flag((playfield_index), PLAYFIELD_BITMASK_MARK)
#define set_playfield_is_marked_flag(playfield_index) set_playfield_flag((playfield_index), PLAYFIELD_BITMASK_MARK)
#define unset_playfield_is_marked_flag(playfield_index) unset_playfield_flag((playfield_index), PLAYFIELD_BITMASK_MARK)

// Returns the line direction for a playfield byte which will be either LINE_DIRECTION_POSITIVE or LINE_DIRECTION_NEGATIVE
#define get_playfield_line_direction_flag_from_byte(flags_byte) (((flags_byte) & PLAYFIELD_BITMASK_LINE_DIRECTION) >> PLAYFIELD_BIT_LINE_DIRECTION)

// Returns the line direction for a playfield tile at |playfield_index| which will be either LINE_DIRECTION_POSITIVE or LINE_DIRECTION_NEGATIVE
#define get_playfield_line_direction_flag(playfield_index) get_playfield_line_direction_flag_from_byte((playfield_index))

// Returns the line orientation for a playfield byte which will be either ORIENTATION_HORIZ or ORIENTATION_VERT
#define get_playfield_line_orientation_flag_from_byte(flags_byte) (((flags_byte) & PLAYFIELD_BITMASK_LINE_ORIENTATION) >> PLAYFIELD_BIT_LINE_ORIENTATION)

// Returns the line orientation for a playfield tile at |playfield_index| which will be either ORIENTATION_HORIZ or ORIENTATION_VERT
#define get_playfield_line_orientation_flag(playfield_index) (get_playfield_line_orientation_flag_from_byte(playfield[(playfield_index)]))

// Sets the line orientation for |playfield_index| to |orientation| which must be either ORIENTATION_HORIZ or ORIENTATION_VERT
#define set_playfield_line_orientation_flag(playfield_index, orientation) (playfield[(playfield_index)] = playfield[(playfield_index)] & ~PLAYFIELD_BITMASK_LINE_ORIENTATION | (orientation << PLAYFIELD_BIT_LINE_ORIENTATION))

// Returns the line index from a playfield byte which will be either 0 or 1
#define get_playfield_line_index_flag_from_byte(flags_byte) (((flags_byte) & PLAYFIELD_BITMASK_LINE_INDEX) >> PLAYFIELD_BIT_LINE_INDEX)

// Returns the line index for a playfield tile at |playfield_index| which will be either 0 or 1
#define get_playfield_line_index_flag(playfield_index) (get_playfield_line_index_flag_from_byte(playfield[(playfield_index)]))

// Sets the line index for |playfield_index| to |index| which must be either 0 or 1
#define set_playfield_line_index_flag(playfield_index, index) (playfield[(playfield_index)] = playfield[(playfield_index)] & ~PLAYFIELD_BITMASK_LINE_INDEX | (index << PLAYFIELD_BIT_LINE_INDEX))

// Returns the playfield tile type from a playfield byte |flags_byte| by removing the bit-flags from the playfield byte
#define get_playfield_tile_type_from_byte(flags_byte) ((flags_byte) & PLAYFIELD_BITMASK_NONE)

// Returns the playfield tile type for |playfield_index| by removing the bit-flags from the playfield byte
#define get_playfield_tile_type(playfield_index) get_playfield_tile_type_from_byte(playfield[(playfield_index)])

// Get the playfield tile type for lines.
// Indicate horizontal or vertical via |orientation| which should be ORIENTATION_HORIZ or ORIENTATION_VERT.
// Indicate the line index via |line_index| which must be 0 or 1.
// Indicate the line direction via |line_direction| which should be LINE_DIRECTION_POSITIVE or LINE_DIRECTION_NEGATIVE.
#define get_playfield_tile_type_line(orientation, line_index, line_direction) (PLAYFIELD_LINE | ((orientation) << PLAYFIELD_BIT_LINE_ORIENTATION) | ((line_index) << PLAYFIELD_BIT_LINE_INDEX) | ((line_direction) << PLAYFIELD_BIT_LINE_DIRECTION))

// Get the bg tile graphic index for lines.
// Indicate horizontal or vertical via |orientation| which should be ORIENTATION_HORIZ or ORIENTATION_VERT.
#define get_playfield_bg_tile_line(orientation) (TILE_INDEX_PLAYFIELD_LINE_HORIZ + (orientation))

#pragma bss-name(push, "BSS")

unsigned char playfield[PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT];

#define STACK_MAX_HEIGHT 0x90

unsigned int stack[STACK_MAX_HEIGHT];
unsigned char stack_top;
unsigned int stack_temp;

#if ENABLE_CHEATS
unsigned char enable_ball_line_collisions;
#endif  //  ENABLE_CHEATS

// Must be included under BSS section
#include "graphics.h"

// Initializes the locations and directions of the active balls on the valid
// ball region of the playfield pattern defined by |current_playfield_pattern|.
void init_balls();

void init_title(void);
void title_change_mode(void);

// Handle player start button presses for the title screen.
// Returns TRUE if the player did press start.
unsigned char title_press_start(void);
void draw_title_cursor(void);

void init_game(void);
void reset_playfield(void);

// Loads the playfield pattern defined by |current_playfield_pattern| into the
// in-memory playfield.
void load_playfield();

void change_to_game_over(void);
void game_over_change_mode(void);

// Handle player start button presses for the game over screen.
// Returns TRUE if the player did press start.
unsigned char game_over_press_start(void);
void draw_game_over_cursor(void);

#if ENABLE_CHEATS
// Set initial state for cheat flags.
void init_cheat_flags(void);

// Handle player cheat button presses.
void handle_cheat_buttons(void);
#endif  // ENABLE_CHEATS

// Perform a level up:
// Increment the current level
// Add one to the current lives counter
// Add one to the current number of balls
// Reset the playfield
// Change the game state to in-game
void do_level_up(void);

void do_level_down(void);

void read_controllers(void);

void __fastcall__ move_player(unsigned char player_index);
void __fastcall__ move_ball(unsigned char ball_index);

void move_balls(void);
void draw_balls(void);
void check_ball_line_collisions(void);

void __fastcall__ start_line(unsigned char player_index);
void __fastcall__ flip_player_orientation(unsigned char player_index);

void __fastcall__ update_nearest_tile(unsigned char player_index);
void __fastcall__ update_line(unsigned char line_index);

void __fastcall__ draw_player(unsigned char player_index);
void __fastcall__ draw_tile_highlight(unsigned char player_index);
void __fastcall__ draw_line(unsigned char line_index);

void update_hud(void);

void stack_init(void);
void stack_empty(void);
void stack_push(void);
void stack_pop(void);

void reset_playfield_mark_bit(void);
void __fastcall__ compute_playfield_mark_bit_one_ball(unsigned char ball_index);
unsigned char __fastcall__ update_cleared_playfield_tiles(void);
void line_completed(void);

void __fastcall__ set_playfield_tile(unsigned int tile_index, unsigned char playfield_tile_type, unsigned char playfield_bg_tile);
