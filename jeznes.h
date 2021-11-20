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

#define TILE_INDEX_PLAYFIELD_UNCLEARED 0x0
#define TILE_INDEX_PLAYFIELD_CLEARED 0x3
#define TILE_INDEX_PLAYFIELD_LINE_HORIZ 0x1
#define TILE_INDEX_PLAYFIELD_LINE_VERT 0x2

#define TILE_INDEX_BALL_BASE 0x30
#define TILE_INDEX_TILE_HIGHLIGHT 0x18

enum {
    ORIENTATION_HORIZ,
    ORIENTATION_VERT
};

#pragma bss-name(push, "ZEROPAGE")

unsigned char pad1;
unsigned char pad1_new;

enum {
    GAME_STATE_TITLE,
    GAME_STATE_PLAYING
};
unsigned char game_state;

unsigned char current_level;
unsigned char current_player;

unsigned char temp_byte_1;
unsigned char temp_byte_2;
unsigned char temp_byte_3;
unsigned char temp_byte_4;
unsigned char temp_byte_5;

signed char temp_signed_byte_1;

int temp_int_1;

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
    unsigned char x;
    unsigned char y;
    signed char x_velocity;
    signed char y_velocity;
};

struct Ball balls[MAX_BALLS];

struct Line {
    // Origin playfield tile for the line
    unsigned char origin_x;
    unsigned char origin_y;

    // Horiz or Vert orientation
    unsigned char orientation;

    // Current tile for line in both directions
    unsigned char current_neg;
    unsigned char current_pos;

    // Completion of the current block [0-7]
    unsigned char current_block_completion;
};

struct Line lines[MAX_PLAYERS];

#pragma bss-name(push, "BSS")

struct ObjectBase {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
};

enum {
    PLAYFIELD_UNCLEARED,
    PLAYFIELD_WALL,
    PLAYFIELD_LINE
};
unsigned char playfield[PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT];

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
