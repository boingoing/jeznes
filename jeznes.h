#define MAX_BALLS 20

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

// These macros enable various debugging features and should probably be turned off before release
#define DRAW_BALL_NEAREST_TILE_HIGHLIGHT 1
#define DEBUG 1

enum {
    ORIENTATION_HORIZ,
    ORIENTATION_VERT
};

#pragma bss-name(push, "ZEROPAGE")

unsigned char pad1;
unsigned char pad1_new;

enum {
    GAME_STATE_TITLE,
    GAME_STATE_PLAYING,
    GAME_STATE_UPDATING_PLAYFIELD
};
unsigned char game_state;

unsigned char current_level;

unsigned char temp_byte_1;
unsigned char temp_byte_2;
unsigned char temp_byte_3;
unsigned char temp_byte_4;
unsigned char temp_byte_5;

signed char temp_signed_byte_1;
signed char temp_signed_byte_2;

int temp_int_1;
int temp_int_2;

int playfield_index;

struct Player {
    // Player metasprite location in pixel-coords
    unsigned char x;
    unsigned char y;

    // Pixel-coords of nearest bg tile under the player
    unsigned char nearest_tile_x;
    unsigned char nearest_tile_y;

    // Playfield tile index for nearest tile
    int nearest_playfield_tile;

    // Horiz or Vert orientation
    unsigned char orientation;

    // Rotate button pressed (B)
    unsigned char rotate_pressed;
    // Place button pressed (A)
    unsigned char place_pressed;
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

struct Line {
    // Origin playfield tile index for the line
    int origin;

    // Horiz or Vert orientation
    unsigned char orientation;

    // Current playfield tile index for line in both directions
    int current_neg;
    int current_pos;

    // Completion of the current block [0-7]
    unsigned char current_block_completion;

    // Is the line active
    unsigned char is_started;
    unsigned char is_neg_complete;
    unsigned char is_pos_complete;
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
void load_playfield(void);
void move_player(void);
void move_balls(void);
void draw_player(void);
void draw_balls(void);
void start_line(void);
void draw_tile_highlight(void);
void flip_player_orientation(void);
void update_nearest_tile(void);
void update_line(void);
void draw_line(void);

void stack_init(void);
void stack_empty(void);
void stack_push(void);
void stack_pop(void);

void reset_playfield_mark_bit(void);
void __fastcall__ compute_playfield_mark_bit_one_ball(unsigned char ball_index);
void update_cleared_playfield_tiles(void);
void line_completed(void);

void __fastcall__ set_playfield_tile(unsigned int tile_index, unsigned char playfield_tile_type, unsigned char playfield_bg_tile);
