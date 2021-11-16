#define MAX_BALLS 10

#define PLAYFIELD_LEFT_WALL 0xe
#define PLAYFIELD_RIGHT_WALL 0xea
#define PLAYFIELD_TOP_WALL 0x15
#define PLAYFIELD_BOTTOM_WALL 0xb1

#define PLAYER_SPEED 0x3
#define BALL_SPEED 0x1

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

#pragma bss-name(push, "BSS")

struct ObjectBase {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
};

struct Player {
    unsigned char x;
    unsigned char y;
};

struct Player players[2];

struct Ball {
    unsigned char x;
    unsigned char y;
    signed char x_velocity;
    signed char y_velocity;
};

struct Ball balls[MAX_BALLS];

unsigned char playfield[30 * 22];

#include "graphics.h"

void init_game(void);
void move_player(void);
void move_balls(void);
void draw_player(void);
void draw_balls(void);
