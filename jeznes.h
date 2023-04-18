// playfield tile offsets
#define PLAYFIELD_FIRST_TILE_X 1
#define PLAYFIELD_FIRST_TILE_Y 2
#define PLAYFIELD_FIRST_TILE_INDEX 32
#define PLAYFIELD_WIDTH 32
#define PLAYFIELD_HEIGHT 22

// playfield bounds in pixel-coords
#define PLAYFIELD_LEFT_WALL 0xe
#define PLAYFIELD_RIGHT_WALL 0xea
#define PLAYFIELD_TOP_WALL 0xd
#define PLAYFIELD_BOTTOM_WALL 0xa9

#define MAX_PLAYERS 2
#define PLAYER_SPEED 0x2

#define MAX_BALLS 20
#define BALL_SPEED 0x1
#define BALL_WIDTH 8
#define BALL_HEIGHT 8

#define SPRITE_INDEX_PLAYFIELD_LINE_HORIZ_BASE 0x80
#define SPRITE_INDEX_PLAYFIELD_LINE_VERT_BASE 0x90

#define TILE_INDEX_PLAYFIELD_UNCLEARED 0x0
#define TILE_INDEX_PLAYFIELD_CLEARED 0x3
#define TILE_INDEX_PLAYFIELD_LINE_HORIZ 0x1
#define TILE_INDEX_PLAYFIELD_LINE_VERT 0x2

#define TILE_INDEX_BALL_BASE 0x30
#define TILE_INDEX_TILE_HIGHLIGHT 0x18

#define TILE_INDEX_ALPHANUMERIC_ZERO 0x1e

#define HUD_LEVEL_DISPLAY_TILE_X 8
#define HUD_LEVEL_DISPLAY_TILE_Y 24
#define HUD_LIVES_DISPLAY_TILE_X 8
#define HUD_LIVES_DISPLAY_TILE_Y 25
#define HUD_CLEAR_DISPLAY_TILE_X 18
#define HUD_CLEAR_DISPLAY_TILE_Y 24
#define HUD_TARGET_DISPLAY_TILE_X 18
#define HUD_TARGET_DISPLAY_TILE_Y 25

// These macros enable various debugging features and should probably be turned off before release
#define DEBUG 1
#define DRAW_BALL_NEAREST_TILE_HIGHLIGHT 1

#define make_word(lo,hi) ((lo)|(hi << 8))

#define get_ball_count() (current_level)
#define get_player_count() (1)

#define get_playfield_index() (temp_int_3)
#define set_playfield_index(a) (temp_int_3 = (a))
#define inc_playfield_index() (++temp_int_3)

// Calculate the playfield tile position in (x,y) of the playfield tile |i|.
#define playfield_index_x(i) ((i)%32)
#define playfield_index_y(i) ((i)>>5)

// Calculate the bg tile position in pixel coords of the playfield tile |i|.
#define playfield_index_pixel_coord_x(i) (playfield_index_x((i)+PLAYFIELD_FIRST_TILE_INDEX)<<3)
#define playfield_index_pixel_coord_y(i) (playfield_index_y((i)+PLAYFIELD_FIRST_TILE_INDEX)<<3)

enum {
    ORIENTATION_HORIZ,
    ORIENTATION_VERT
};

#pragma bss-name(push, "ZEROPAGE")

unsigned char pads[MAX_PLAYERS];
unsigned char pads_new[MAX_PLAYERS];

enum {
    GAME_STATE_TITLE,
    GAME_STATE_PLAYING,
    GAME_STATE_UPDATING_PLAYFIELD
};

unsigned char game_state;
unsigned char current_level;
unsigned char lives_count;
int cleared_tile_count;

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

#define get_flag(flags_byte, bitmask) (((flags_byte) & (bitmask)) != 0)
#define set_flag(flags_byte, bitmask) ((flags_byte) |= (bitmask))
#define unset_flag(flags_byte, bitmask) ((flags_byte) &= ~(bitmask))

#define PLAYER_BIT_ORIENTATION 0
#define PLAYER_BITMASK_ORIENTATION (1 << PLAYER_BIT_ORIENTATION)
#define PLAYER_BIT_IS_ROTATE_PRESSED 1
#define PLAYER_BITMASK_IS_ROTATE_PRESSED (1 << PLAYER_BIT_IS_ROTATE_PRESSED)
#define PLAYER_BIT_IS_PLACE_PRESSED 2
#define PLAYER_BITMASK_IS_PLACE_PRESSED (1 << PLAYER_BIT_IS_PLACE_PRESSED)

#define get_player_flag(player_index, bitmask) get_flag(players[(player_index)].flags, (bitmask))
#define set_player_flag(player_index, bitmask) set_flag(players[(player_index)].flags, (bitmask))
#define unset_player_flag(player_index, bitmask) unset_flag(players[(player_index)].flags, (bitmask))

#define get_player_is_rotate_pressed(player_index) get_player_flag((player_index), PLAYER_BITMASK_IS_ROTATE_PRESSED)
#define set_player_is_rotate_pressed(player_index) set_player_flag((player_index), PLAYER_BITMASK_IS_ROTATE_PRESSED)
#define unset_player_is_rotate_pressed(player_index) unset_player_flag((player_index), PLAYER_BITMASK_IS_ROTATE_PRESSED)

#define get_player_is_place_pressed(player_index) get_player_flag((player_index), PLAYER_BITMASK_IS_PLACE_PRESSED)
#define set_player_is_place_pressed(player_index) set_player_flag((player_index), PLAYER_BITMASK_IS_PLACE_PRESSED)
#define unset_player_is_place_pressed(player_index) unset_player_flag((player_index), PLAYER_BITMASK_IS_PLACE_PRESSED)

// Returns either ORIENTATION_HORIZ or ORIENTATION_VERT
#define get_player_orientation_flag(player_index) (players[(player_index)].flags & PLAYER_BITMASK_ORIENTATION)

// Sets the orientation for |player_index| player to |orientation| which must be either ORIENTATION_HORIZ or ORIENTATION_VERT
#define set_player_orientation_flag(player_index, orientation) (players[(player_index)].flags = players[(player_index)].flags & ~LINE_BITMASK_ORIENTATION | (orientation))

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

struct Player players[MAX_PLAYERS];

struct Ball {
    // Ball sprite location in pixel-coords
    unsigned char x;
    unsigned char y;

    signed char x_velocity;
    signed char y_velocity;

    // Playfield tile index for nearest tile
    int nearest_playfield_tile;
};

struct Ball balls[MAX_BALLS];

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

struct Line {
    // Origin playfield tile index for the line
    int origin;

    // Current playfield tile index for line in both directions
    int current_neg;
    int current_pos;

    // Completion of the current block [0-7]
    unsigned char current_block_completion;

    // Hold bit-flags used to track state of this line.
    unsigned char flags;
};

struct Line lines[MAX_PLAYERS];

#pragma bss-name(push, "BSS")

struct ObjectBase {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
};

#define PLAYFIELD_BIT_LINE_ORIENTATION 7
#define PLAYFIELD_BITMASK_LINE_ORIENTATION (1 << PLAYFIELD_BIT_LINE_ORIENTATION)
#define PLAYFIELD_BIT_LINE_INDEX 6
#define PLAYFIELD_BITMASK_LINE_INDEX (1 << PLAYFIELD_BIT_LINE_INDEX)
#define PLAYFIELD_BIT_MARK 5
#define PLAYFIELD_BITMASK_MARK (1 << PLAYFIELD_BIT_MARK)
// Bitmask for playfield byte which separates only the flag bits.
#define PLAYFIELD_BITMASK_ALL (PLAYFIELD_BITMASK_LINE_ORIENTATION | PLAYFIELD_BITMASK_LINE_INDEX | PLAYFIELD_BITMASK_MARK)
// Bitmask for playfield byte which removes the flag bits.
#define PLAYFIELD_BITMASK_NONE (~PLAYFIELD_BITMASK_ALL)

enum {
    PLAYFIELD_UNCLEARED,
    PLAYFIELD_WALL,
    PLAYFIELD_LINE
};
unsigned char playfield[PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT];

#define STACK_MAX_HEIGHT 0x90

unsigned int stack[STACK_MAX_HEIGHT];
unsigned char stack_top;
unsigned int stack_temp;

#include "graphics.h"

void init_game(void);
void __fastcall__ load_playfield(unsigned char playfield_index);

void read_controllers(void);

void __fastcall__ move_player(unsigned char player_index);
void __fastcall__ move_ball(unsigned char ball_index);

void move_balls(void);
void draw_balls(void);

void __fastcall__ start_line(unsigned char player_index);
void __fastcall__ flip_player_orientation(unsigned char player_index);

void __fastcall__ update_nearest_tile(unsigned char player_index);
void __fastcall__ update_line(unsigned char line_index);

void __fastcall__ draw_player(unsigned char player_index);
void __fastcall__ draw_tile_highlight(unsigned char player_index);
void __fastcall__ draw_line(unsigned char line_index);

void draw_hud(void);

void stack_init(void);
void stack_empty(void);
void stack_push(void);
void stack_pop(void);

void reset_playfield_mark_bit(void);
void __fastcall__ compute_playfield_mark_bit_one_ball(unsigned char ball_index);
unsigned char __fastcall__ update_cleared_playfield_tiles(void);
void line_completed(void);

void __fastcall__ set_playfield_tile(unsigned int tile_index, unsigned char playfield_tile_type, unsigned char playfield_bg_tile);
