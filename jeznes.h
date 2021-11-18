#define MAX_BALLS 20

// playfield tile offsets
#define PLAYFIELD_FIRST_TILE_X 1
#define PLAYFIELD_FIRST_TILE_Y 2
#define PLAYFIELD_FIRST_TILE_INDEX 64
#define PLAYFIELD_WIDTH 32
#define PLAYFIELD_HEIGHT 22

// playfield bounds in pixel-coords
#define PLAYFIELD_LEFT_WALL 0xe
#define PLAYFIELD_RIGHT_WALL 0xea
#define PLAYFIELD_TOP_WALL 0x15
#define PLAYFIELD_BOTTOM_WALL 0xb1

#define PLAYER_SPEED 0x3
#define BALL_SPEED 0x1
#define BALL_WIDTH 8
#define BALL_HEIGHT 8

#pragma bss-name(push, "ZEROPAGE")

unsigned char pad1;
unsigned char pad1_new;

enum {GAME_STATE_TITLE, GAME_STATE_PLAYING};
unsigned char game_state;

unsigned char current_level;

unsigned char temp_byte_1;
unsigned char temp_byte_2;
unsigned char temp_byte_3;
unsigned char temp_byte_4;
unsigned char temp_byte_5;

signed char temp_signed_byte_1;

int temp_int_1;

enum {PLAYER_ORIENTATION_HORIZ, PLAYER_ORIENTATION_VERT};
struct Player {
    unsigned char x;
    unsigned char y;
    unsigned char orientation;
    unsigned char rotate_pressed;
};

struct Player players[2];

struct Ball {
    unsigned char x;
    unsigned char y;
    signed char x_velocity;
    signed char y_velocity;
};

struct Ball balls[MAX_BALLS];

#pragma bss-name(push, "BSS")

struct ObjectBase {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
};

enum {PLAYFIELD_UNCLEARED, PLAYFIELD_WALL, PLAYFIELD_CLEARED, PLAYFIELD_LINE};
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
