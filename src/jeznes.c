//------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for
// full license information.
//------------------------------------------------------------------------------

#include "jeznes.h"

#include "bss.h"
#include "constants/game.h"
#include "constants/sprites.h"
#include "constants/tiles.h"
#include "data.h"
#include "debug.h"
#include "flags/line.h"
#include "flags/player.h"
#include "flags/playfield.h"
#include "flood_fill.h"
#include "lib/nesdoug.h"
#include "lib/neslib.h"
#include "scoring.h"
#include "types.h"
#include "zeropage.h"

// Include the other C files here as a workaround for multiply-defined symbols
// ending up in each compilation unit as a result of the need to define common
// memory locations in their sections (ie: zeropage).
// TODO(boingoing): Fix this somehow. Define the memory in asm and the named
// locations in a header? Will this affect performance?
#include "flood_fill.c"
#include "scoring.c"

int main(void) {
#if ENABLE_CHEATS
  init_cheat_flags();
#endif  // ENABLE_CHEATS

  // Turn off the screen.
  ppu_off();

  // Sprites are in the 0th CHR bank.
  bank_spr(0);
  // Tiles are in the 1st CHR bank.
  bank_bg(1);

  // Start the game with the title screen loaded.
  init_title();

  // Set the vram buffer pointer.
  set_vram_buffer();
  clear_vram_buffer();

  // Turn on the screen.
  ppu_on_all();

  while (1) {
    ppu_wait_nmi();

    // Read the controllers.
    read_controllers();

    // Do at the beginning of each frame.
    clear_vram_buffer();

    if (game_state == GAME_STATE_TITLE) {
      // Clear all sprites from the sprite buffer.
      oam_clear();

      // Handle player pressing select.
      title_change_mode();
      // Handle player pressing start.
      if (!title_press_start()) {
        // Draw the cursor on the title screen if the player didn't press start.
        // Because title_press_start() writes to the screen, we would be drawing
        // the cursor sprite over the playfield.
        draw_title_cursor();

        // Move the ball positions in the playfield part of the title screen.
        move_and_draw_balls();
      }
    } else if (game_state == GAME_STATE_PLAYING) {
      // Clear all sprites from the sprite buffer.
      oam_clear();

      for (temp_byte_1 = 0; temp_byte_1 < get_player_count(); ++temp_byte_1) {
        // Respond to player gamepad.
        flip_player_orientation(temp_byte_1);
        start_line(temp_byte_1);
        if (pause_press_start(temp_byte_1) == TRUE) {
          continue;
        }

        // Stash a pointer to the player so we don't need to access it via the array everywhere.
        set_temp_ptr(&players[temp_byte_1]);

        // Move the player position in the playfield.
        move_player(temp_byte_1);

        // Draw the player, the playfield tile highlight, and the in-progress
        // line sprites.
        draw_player();
        draw_tile_highlight();
        draw_line(temp_byte_1);

        // Update the line for this player if there's one in progress.
        update_line(temp_byte_1);
      }

      // Move the ball positions in the playfield.
      move_and_draw_balls();

      // Check to see if any balls have collided with any lines.
#if ENABLE_CHEATS
      if (enable_ball_line_collisions != FALSE)
#endif  // ENABLE_CHEATS
      {
        check_ball_line_collisions();
      }
    } else if (game_state == GAME_STATE_PAUSED) {
      // Clear all sprites from the sprite buffer.
      oam_clear();

      // Handle any player pressing start to unpause.
      for (temp_byte_1 = 0; temp_byte_1 < get_player_count(); temp_byte_1++) {
        if (pause_press_start(temp_byte_1) == TRUE) {
          continue;
        }
      }

      // Draw the pause message on the screen while we're paused.
      draw_pause_sprites();
    } else if (game_state == GAME_STATE_LEVEL_UP) {
      // Clear all sprites from the sprite buffer.
      oam_clear();

      // Wait for player to press start.
      if (!press_start_level_up()) {
        // Move the ball positions in the playfield part of the level up screen.
        move_and_draw_balls();

        draw_cursor_level_up();
      }
    } else if (game_state == GAME_STATE_GAME_OVER) {
      // Clear all sprites from the sprite buffer.
      oam_clear();

      // Handle player pressing select.
      game_over_change_mode();
      // Handle player pressing start.
      if (!game_over_press_start()) {
        // Draw the cursor sprite wherever it should be if the player didn't
        // press start.
        draw_game_over_cursor();

        // Move the ball positions in the playfield part of the game over
        // screen.
        move_and_draw_balls();
      }
    } else if (game_state == GAME_STATE_UPDATING_PLAYFIELD) {
      // Restart the update of cleared playfield tiles.
      if (update_cleared_playfield_tiles() == TRUE) {
        // We might have cleared tiles, let's update the HUD.
        game_state = GAME_STATE_REQUEST_HUD_UPDATE;

        // We finished updating the playfield tiles, let's remove the mark bits.
        reset_playfield_mark_bit();
      }
    } else if (game_state == GAME_STATE_REQUEST_HUD_UPDATE) {
      // Update the level, lives remaining, percentages, etc.
      update_hud();

      // Then reset the game state to playing.
      game_state = GAME_STATE_PLAYING;

      // The cleared tile percentage is updated via update_hud().
      // Because that's an expensive operation, let's not redo it anywhere.
      // If we detect the target percentage has been reached, switch to the
      // level up state instead.
      if (cleared_tile_percentage > TARGET_CLEARED_TILE_PERCENTAGE) {
        change_to_level_up();
      }
    }
#if ENABLE_CHEATS
    else if (game_state == GAME_STATE_LEVEL_DOWN) {
      // Clear all sprites from the sprite buffer.
      oam_clear();

      // Just perform a level up.
      do_level_down();
    }

    handle_cheat_buttons();
#endif  // ENABLE_CHEATS

#if DRAW_GRAY_LINE
    // For debugging, render a line indicating how much CPU is used.
    gray_line();
#endif
  }

  return 0;
}

void init_balls(void) {
  for (temp_byte_1 = 0; temp_byte_1 < get_ball_count(); ++temp_byte_1) {
    balls[temp_byte_1].x =
        rand8() % playfield_pattern_valid_ball_width_in_pixels
                      [get_playfield_pattern()] +
        playfield_pattern_valid_ball_start_pixel_x[get_playfield_pattern()];
    balls[temp_byte_1].y =
        rand8() % playfield_pattern_valid_ball_height_in_pixels
                      [get_playfield_pattern()] +
        playfield_pattern_valid_ball_start_pixel_y[get_playfield_pattern()];
    if (rand2()) {
      balls[temp_byte_1].x_velocity = BALL_SPEED_POSITIVE;
    } else {
      balls[temp_byte_1].x_velocity = BALL_SPEED_NEGATIVE;
    }
    if (rand2()) {
      balls[temp_byte_1].y_velocity = BALL_SPEED_POSITIVE;
    } else {
      balls[temp_byte_1].y_velocity = BALL_SPEED_NEGATIVE;
    }
  }
}

void init_title(void) {
  // Make sure we only have a fixed count of balls bouncing around on the title
  // screen.
  set_ball_count(TITLE_SCREEN_BALL_COUNT);

  // By default set to 1-player so we can read inputs for the title screen.
  set_player_count(1);

  // Set the playfield pattern to use.
  set_playfield_pattern(PLAYFIELD_PATTERN_TITLE_SCREEN);

  // Draw bouncing balls in the playfield section of the title screen.
  init_balls();

  // Load title screen graphics.
  pal_bg(title_bg_palette);
  pal_spr(title_sprite_palette);
  vram_adr(NAMETABLE_A);
  vram_unrle(title_screen);

  // Load the playfield pattern which we show on the title screen.
  load_playfield();

  // By default, select the "1 Player" option.
  set_title_mode(TITLE_1_PLAYER);

  // Starting or returning to the title screen.
  game_state = GAME_STATE_TITLE;
}

unsigned char title_press_start(void) {
  if (pads_new[0] & PAD_START) {
    if (get_title_mode() == TITLE_1_PLAYER) {
      set_player_count(1);
    } else {
      // get_title_mode() == TITLE_2_PLAYER
      set_player_count(2);
    }

    // Fade to black
    pal_fade_to(4, 0);
    // Screen off
    ppu_off();

    init_game();

    // Screen on
    ppu_on_all();
    // Back to normal brightness
    pal_bright(4);

    return TRUE;
  }

  return FALSE;
}

#if ENABLE_CHEATS
void init_cheat_flags(void) {
  enable_ball_line_collisions = TRUE;
  enable_losing_lives = TRUE;
}

void handle_cheat_buttons(void) {
  // Cheat buttons are prefixed with the select button.
  if (pads[0] & PAD_SELECT) {
    // Once a cheat is entered, player needs to stop pressing select.
    if (get_player_is_cheat_pressed(0)) {
      return;
    }

    if ((pads[0] &
         (PAD_LEFT | PAD_B | PAD_A | PAD_UP | PAD_DOWN | PAD_START)) != 0) {
      // Remember a cheat keypress is being held with the select button.
      set_player_is_cheat_pressed(0);
    }

    if (pads[0] & PAD_LEFT) {
      // SELECT + LEFT => Re-initialize the ball locations and directions.
      init_balls();
    } else if (pads[0] & PAD_B) {
      // SELECT + B => Enable / Disable ball-line collisions.
      enable_ball_line_collisions ^= TRUE;
    } else if (pads[0] & PAD_A) {
      // SELECT + A => Enable / Disable losing a life on ball-line collision.
      enable_losing_lives ^= TRUE;
    } else if (pads[0] & PAD_UP) {
      // SELECT + UP => Perform level-up
      do_level_up();
    } else if (pads[0] & PAD_DOWN) {
      // SELECT + DOWN => Perform level-down
      game_state = GAME_STATE_LEVEL_DOWN;
    } else if (pads[0] & PAD_START) {
      // SELECT + START => Reset to title
      // Screen off
      ppu_off();
      init_title();
      // Screen on
      ppu_on_all();
    }
  } else {
    // Select is not pressed, reset cheat flag.
    unset_player_is_cheat_pressed(0);
  }
}
#endif  // ENABLE_CHEATS

void title_change_mode(void) {
  if (pads_new[0] & PAD_SELECT) {
    // Toggle game over mode between retry and quit.
    set_title_mode(get_title_mode() ^ 1);
  }
}

void init_game(void) {
  // Seed the random number generator - it's based on frame count.
  seed_rng();

  // Starting game state.
  game_state = GAME_STATE_PLAYING;
  current_level = 1;
  lives_count = STARTING_LIVES_COUNT;
  score = 0;
  set_ball_count(current_level + 1);

  // Set the current playfield pattern.
  set_playfield_pattern(FIRST_PLAYFIELD_PATTERN);

  // Player initial positions.
  for (temp_byte_1 = 0; temp_byte_1 < get_player_count(); ++temp_byte_1) {
    players[temp_byte_1].x = playfield_pattern_player_default_x[temp_byte_1];
    players[temp_byte_1].y = playfield_pattern_player_default_y[temp_byte_1];
    set_temp_ptr(&players[temp_byte_1]);
    update_nearest_tile();

    set_player_orientation_flag(temp_byte_1, ORIENTATION_HORIZ);
  }

  // Always loads |get_playfield_pattern()|
  reset_playfield();
}

void load_playfield(void) {
  memcpy(&playfield, playfield_patterns[get_playfield_pattern()],
         PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT);
}

void read_controllers(void) {
  pads[0] = pad_poll(0);
  pads_new[0] = get_pad_new(0);

  if (get_player_count() == 2) {
    pads[1] = pad_poll(1);
    pads_new[1] = get_pad_new(1);
  }
}

void reset_playfield() {
  // Reset per-level state.
  cleared_tile_count = 0;
  cleared_tile_percentage = 0;

  // Make sure no lines will be rendered.
  unset_line_is_started_flag(0);
  unset_line_is_started_flag(1);

  // Ball random initial positions and directions.
  init_balls();

  // Load playfield graphics.
  // Note: Screen must be off!
  pal_bg(playfield_bg_palette);
  pal_spr(playfield_sprite_palette);
  vram_adr(NAMETABLE_A);
  vram_unrle(playfield_screen);

  // Load playfield pattern.
  load_playfield();

  // Display the level, lives remaining, percentages, etc.
  update_hud();
}

void do_level_down(void) {
  // Actually level down the player.
  current_level--;

  // Update the count of balls.
  set_ball_count(current_level + 1);

  // Set the current playfield pattern.
  // TODO(boingoing): If the current_level affects the playfield.
  set_playfield_pattern(FIRST_PLAYFIELD_PATTERN);

  // Turn off the screen.
  ppu_off();

  // Redraw the playfield and reset the balls while adding one more.
  reset_playfield();

  // Turn on the screen.
  ppu_on_all();

  // Reset state to playing the game.
  game_state = GAME_STATE_PLAYING;
}

void do_level_up(void) {
  // Actually level up the player.
  current_level++;
  lives_count++;

  // Update the ball count.
  set_ball_count(current_level + 1);

  // Set the current playfield pattern.
  // TODO(boingoing): If the current_level affects the playfield.
  set_playfield_pattern(FIRST_PLAYFIELD_PATTERN);

  // Fade to black
  pal_fade_to(4, 0);
  // Screen off
  ppu_off();

  // Redraw the playfield and reset the balls while adding one more.
  reset_playfield();

  // Screen on
  ppu_on_all();
  // Back to normal brightness
  pal_bright(4);

  // Reset state to playing the game.
  game_state = GAME_STATE_PLAYING;
}

void change_to_level_up(void) {
  // Reset the count of balls so it isn't variable.
  set_ball_count(LEVEL_UP_SCREEN_BALL_COUNT);

  // Set the playfield pattern to use.
  set_playfield_pattern(PLAYFIELD_PATTERN_LEVEL_UP_SCREEN);

  // Draw bouncing balls in the playfield section of the level-up screen.
  init_balls();

  // Fade to black
  pal_fade_to(4, 0);
  // Screen off
  ppu_off();

  // Load level-up screen graphics.
  vram_adr(NAMETABLE_A);
  vram_unrle(level_up_screen);

  // Screen on
  ppu_on_all();
  // Back to normal brightness
  pal_bright(4);

  // Load the playfield pattern which we show on the level-up screen.
  load_playfield();

  // Compute bonus gained from current level.
  // Do this now so the correct new score shows up on the level up summary.
  add_score_for_level_up();

  // Write the score and bonus values to the level-up screen.
  update_hud_level_up();

  // Transition state to level-up.
  game_state = GAME_STATE_LEVEL_UP;
}

void update_hud_level_up(void) {
  write_two_digit_number_to_bg(current_level, LEVEL_UP_LEVEL_DISPLAY_TILE_X,
                               LEVEL_UP_LEVEL_DISPLAY_TILE_Y);
  write_score_to_bg(get_lives_bonus(lives_count), LEVEL_UP_LIFE_BONUS_DISPLAY_TILE_X,
                    LEVEL_UP_LIFE_BONUS_DISPLAY_TILE_Y);
  write_score_to_bg(get_cleared_bonus(cleared_tile_percentage), LEVEL_UP_CLEAR_BONUS_DISPLAY_TILE_X,
                    LEVEL_UP_CLEAR_BONUS_DISPLAY_TILE_Y);
  write_score_to_bg(score, LEVEL_UP_SCORE_DISPLAY_TILE_X,
                    LEVEL_UP_SCORE_DISPLAY_TILE_Y);
}

unsigned char press_start_level_up(void) {
  if (pads_new[0] & PAD_START) {
    // Load the next level playfield.
    do_level_up();

    return TRUE;
  }

  return FALSE;
}

void draw_cursor_level_up(void) {
  oam_spr(LEVEL_UP_CURSOR_CONTINUE_X, LEVEL_UP_CURSOR_CONTINUE_Y, SPRITE_INDEX_CURSOR, 0);
}

void game_over_update_hud(void) {
  write_two_digit_number_to_bg(current_level, GAME_OVER_LEVEL_DISPLAY_TILE_X,
                               GAME_OVER_LEVEL_DISPLAY_TILE_Y);
  write_score_to_bg(score, GAME_OVER_SCORE_DISPLAY_TILE_X,
                    GAME_OVER_SCORE_DISPLAY_TILE_Y);
}

void change_to_game_over(void) {
  // Reset the count of balls so it isn't variable.
  set_ball_count(GAME_OVER_SCREEN_BALL_COUNT);

  // Set the playfield pattern to use.
  set_playfield_pattern(PLAYFIELD_PATTERN_GAME_OVER_SCREEN);

  // Draw bouncing balls in the playfield section of the game over screen.
  init_balls();

  // Fade to black
  pal_fade_to(4, 0);
  // Screen off
  ppu_off();

  // Load game over screen graphics.
  vram_adr(NAMETABLE_A);
  vram_unrle(game_over_screen);

  // Screen on
  ppu_on_all();
  // Back to normal brightness
  pal_bright(4);

  // Load the playfield pattern which we show on the game over screen.
  load_playfield();

  // Write the score values to the game over screen.
  game_over_update_hud();

  // Default to the Retry option.
  set_game_over_mode(GAME_OVER_RETRY);

  // Transition state to game over.
  game_state = GAME_STATE_GAME_OVER;
}

void game_over_change_mode(void) {
  if (pads_new[0] & PAD_SELECT) {
    // Toggle game over mode between retry and quit.
    set_game_over_mode(get_game_over_mode() ^ 1);
  }
}

unsigned char game_over_press_start(void) {
  if (pads_new[0] & PAD_START) {
    if (get_game_over_mode() == GAME_OVER_RETRY) {
      // Reset our lives count back to the default.
      lives_count = STARTING_LIVES_COUNT;

      // Reset the score to zero.
      // TODO(boingoing): Should we leave this alone?
      score = 0;

      // Reset the count of balls for the playfield.
      set_ball_count(current_level + 1);

      // Set the current playfield pattern.
      // TODO(boingoing): Load a different pattern if the current_level should
      // affect this.
      set_playfield_pattern(FIRST_PLAYFIELD_PATTERN);

      // Screen off
      ppu_off();

      reset_playfield();

      // Screen on
      ppu_on_all();

      game_state = GAME_STATE_PLAYING;
    } else {
      // get_game_over_mode() == GAME_OVER_QUIT

      // Fade to black
      pal_fade_to(4, 0);
      // Screen off
      ppu_off();

      // Back to the title screen
      init_title();

      // Screen on
      ppu_on_all();
      // Back to normal brightness
      pal_bright(4);
    }

    return TRUE;
  }

  return FALSE;
}

unsigned char pause_press_start(unsigned char player_index) {
  // Any player can pause / unpause right now?
  // TODO(boingoing): Track which player initiated the pause and only let that
  // player unpause.
  if (pads_new[player_index] & PAD_START) {
    if (game_state == GAME_STATE_PLAYING) {
      // Fade the screen a bit
      pal_fade_to(4, 4);

      game_state = GAME_STATE_PAUSED;
    } else {
      // game_state == GAME_STATE_PAUSED
      // Back to normal brightness
      pal_bright(4);

      game_state = GAME_STATE_PLAYING;
    }

    return TRUE;
  }

  return FALSE;
}

void write_two_digit_number_to_bg(unsigned char num, unsigned char tile_x,
                                  unsigned char tile_y) {
  one_vram_buffer(get_tile_alphanumeric_number(num / 10),
                  NTADR_A(tile_x, tile_y));
  one_vram_buffer(get_tile_alphanumeric_number(num % 10),
                  NTADR_A(tile_x + 1, tile_y));
}

void write_score_to_bg(int score, unsigned char tile_x, unsigned char tile_y) {
  temp_byte_1 = score / 10000;
  one_vram_buffer(get_tile_alphanumeric_number(temp_byte_1),
                  NTADR_A(tile_x, tile_y));

  score %= 10000;
  temp_byte_1 = score / 1000;
  one_vram_buffer(get_tile_alphanumeric_number(temp_byte_1),
                  NTADR_A(tile_x + 1, tile_y));

  score %= 1000;
  temp_byte_1 = score / 100;
  one_vram_buffer(get_tile_alphanumeric_number(temp_byte_1),
                  NTADR_A(tile_x + 2, tile_y));

  score %= 100;
  temp_byte_1 = score / 10;
  one_vram_buffer(get_tile_alphanumeric_number(temp_byte_1),
                  NTADR_A(tile_x + 3, tile_y));
  one_vram_buffer(get_tile_alphanumeric_number(score % 10),
                  NTADR_A(tile_x + 4, tile_y));
}

void update_hud(void) {
  write_two_digit_number_to_bg(current_level, HUD_LEVEL_DISPLAY_TILE_X,
                               HUD_LEVEL_DISPLAY_TILE_Y);
  write_two_digit_number_to_bg(lives_count, HUD_LIVES_DISPLAY_TILE_X,
                               HUD_LIVES_DISPLAY_TILE_Y);
  write_two_digit_number_to_bg(TARGET_CLEARED_TILE_PERCENTAGE,
                               HUD_TARGET_DISPLAY_TILE_X,
                               HUD_TARGET_DISPLAY_TILE_Y);

  cleared_tile_percentage =
      (cleared_tile_count * 100) /
      playfield_pattern_uncleared_tile_counts[FIRST_PLAYFIELD_PATTERN];
  write_two_digit_number_to_bg(cleared_tile_percentage,
                               HUD_CLEAR_DISPLAY_TILE_X,
                               HUD_CLEAR_DISPLAY_TILE_Y);

  write_score_to_bg(score, HUD_SCORE_DISPLAY_TILE_X, HUD_SCORE_DISPLAY_TILE_Y);
}

void move_player(unsigned char player_index) {
  temp_byte_2 = pads[player_index];

  // Quit early if no d-pad buttons are pressed.
  if ((temp_byte_2 & (PAD_LEFT | PAD_RIGHT | PAD_UP | PAD_DOWN)) == 0) {
    return;
  }

  // Move player left or right (both cannot be pressed at the same time).
  if (temp_byte_2 & PAD_RIGHT) {
    set_pixel_coord_x(get_temp_ptr(struct Player)->x + PLAYER_SPEED);
    get_temp_ptr(struct Player)->x = MIN(get_pixel_coord_x(), PLAYFIELD_RIGHT_WALL);
  } else if (temp_byte_2 & PAD_LEFT) {
    set_pixel_coord_x(get_temp_ptr(struct Player)->x - PLAYER_SPEED);
    if (get_player_orientation_flag_from_byte(get_temp_ptr(struct Player)->flags) == ORIENTATION_HORIZ) {
      temp_byte_3 = PLAYFIELD_LEFT_WALL + 8;
    } else {
      temp_byte_3 = PLAYFIELD_LEFT_WALL;
    }
    get_temp_ptr(struct Player)->x = MAX(get_pixel_coord_x(), temp_byte_3);
  }

  // Now move the player up or down (both cannot be pressed at the same time).
  if (temp_byte_2 & PAD_DOWN) {
    set_pixel_coord_y(get_temp_ptr(struct Player)->y + PLAYER_SPEED);
    get_temp_ptr(struct Player)->y = MIN(get_pixel_coord_y(), PLAYFIELD_BOTTOM_WALL);
  } else if (temp_byte_2 & PAD_UP) {
    set_pixel_coord_y(get_temp_ptr(struct Player)->y - PLAYER_SPEED);
    if (get_player_orientation_flag_from_byte(get_temp_ptr(struct Player)->flags) == ORIENTATION_HORIZ) {
      temp_byte_3 = PLAYFIELD_TOP_WALL;
    } else {
      temp_byte_3 = PLAYFIELD_TOP_WALL + 8;
    }
    get_temp_ptr(struct Player)->y = MAX(get_pixel_coord_y(), temp_byte_3);
  }

  update_nearest_tile();
}

void move_and_draw_balls(void) {
  temp_byte_5 = get_frame_count();

  // Determine which sprite frame we should use for the balls - they are all drawn with the same animation frame.
  // get_frame_count() returns [0x0,0xff].
  // SPRITE_FRAME_COUNT_BALL should be a factor of 256.
  temp_byte_5 = ((temp_byte_5 >> 2) % SPRITE_FRAME_COUNT_BALL) + SPRITE_INDEX_BALL_BASE;

  for (temp_byte_1 = 0; temp_byte_1 < get_ball_count(); ++temp_byte_1) {
    set_temp_ptr(&balls[temp_byte_1]);

    // Move ball position.
    move_ball();

    // Draw ball sprite.
    oam_spr(get_temp_ptr(struct Ball)->x, get_temp_ptr(struct Ball)->y,
            temp_byte_5, 0);

#if DRAW_BALL_NEAREST_TILE_HIGHLIGHT
    temp_int_1 = get_temp_ptr(struct Ball)->nearest_playfield_tile;
    oam_spr(playfield_index_pixel_coord_x(temp_int_1),
            playfield_index_pixel_coord_y(temp_int_1) - 1,
            SPRITE_INDEX_TILE_HIGHLIGHT, 1);
#endif
  }
}

void move_ball() {
  // Consider moving right or left first.
  set_x_velocity(get_temp_ptr(struct Ball)->x_velocity);
  set_x_candidate_pixel_coord(get_temp_ptr(struct Ball)->x + get_x_velocity());
  set_x_compare_pixel_coord(get_x_candidate_pixel_coord());
  // Moving right
  if (get_x_velocity() == BALL_SPEED_POSITIVE) {
    // Balls are 8 pixels wide, compare to the right-edge.
    set_x_compare_pixel_coord(get_x_compare_pixel_coord() + BALL_WIDTH);
  }
  // Find x-direction candidate playfield tile index.
  temp_int_1 = playfield_tile_from_pixel_coords(get_x_compare_pixel_coord(),
                                                get_temp_ptr(struct Ball)->y);
  // Bounce off a left or right wall tile.
  if (playfield[temp_int_1] == PLAYFIELD_WALL) {
    // Reverse x-direction.
    set_x_velocity(get_x_velocity() * -1);
    // Move the ball such that it's in the non-wall tile opposite the candidate.
    set_x_candidate_pixel_coord(get_temp_ptr(struct Ball)->x +
                                get_x_velocity());
    // Update the ball velocity.
    get_temp_ptr(struct Ball)->x_velocity = get_x_velocity();
  }
  get_temp_ptr(struct Ball)->x = get_x_candidate_pixel_coord();

  // Consider moving up or down next (we already moved right/left).
  set_y_velocity(get_temp_ptr(struct Ball)->y_velocity);
  set_y_candidate_pixel_coord(get_temp_ptr(struct Ball)->y + get_y_velocity());
  set_y_compare_pixel_coord(get_y_candidate_pixel_coord());
  // Moving down
  if (get_y_velocity() == BALL_SPEED_POSITIVE) {
    // Balls are 8 pixels tall, compare to the bottom edge.
    set_y_compare_pixel_coord(get_y_compare_pixel_coord() + BALL_HEIGHT);
  }
  // Find y-direction candidate playfield tile index.
  temp_int_2 = playfield_tile_from_pixel_coords(get_x_candidate_pixel_coord(),
                                                get_y_compare_pixel_coord());
  // Bounce off a top or bottom wall tile.
  if (playfield[temp_int_2] == PLAYFIELD_WALL) {
    // Reverse y-direction.
    set_y_velocity(get_y_velocity() * -1);
    // Move the ball such that it's in the non-wall tile opposite the candidate.
    set_y_candidate_pixel_coord(get_temp_ptr(struct Ball)->y +
                                get_y_velocity());
    // Update the ball velocity.
    get_temp_ptr(struct Ball)->y_velocity = get_y_velocity();
  }
  get_temp_ptr(struct Ball)->y = get_y_candidate_pixel_coord();

  // Update nearest playfield tile - center of the ball.
  temp_byte_2 = get_x_candidate_pixel_coord() + 4;
  temp_byte_3 = get_y_candidate_pixel_coord() + 4;
  get_temp_ptr(struct Ball)->nearest_playfield_tile =
      playfield_tile_from_pixel_coords(temp_byte_2, temp_byte_3);
}

void draw_player(void) {
  temp_byte_2 = get_frame_count();

  // Default to horizontal sprite.
  temp_byte_2 = temp_byte_2 >> 3 & 1;

  // Add 2 to get the vertical sprite.
  if (get_player_orientation_flag_from_byte(get_temp_ptr(struct Player)->flags) != ORIENTATION_HORIZ) {
    temp_byte_2 += 2;
  }

  oam_meta_spr(get_temp_ptr(struct Player)->x, get_temp_ptr(struct Player)->y,
               player_metasprite_list[temp_byte_2]);
}

void draw_tile_highlight(void) {
  if (playfield[get_temp_ptr(struct Player)->nearest_playfield_tile] ==
      PLAYFIELD_UNCLEARED) {
    oam_spr(get_temp_ptr(struct Player)->nearest_tile_x,
            get_temp_ptr(struct Player)->nearest_tile_y - 1, SPRITE_INDEX_TILE_HIGHLIGHT,
            1);
  }
}

void draw_title_cursor(void) {
  if (get_title_mode() == TITLE_1_PLAYER) {
    oam_spr(TITLE_CURSOR_1_PLAYER_X, TITLE_CURSOR_1_PLAYER_Y,
            SPRITE_INDEX_CURSOR, 0);
  } else {
    oam_spr(TITLE_CURSOR_2_PLAYERS_X, TITLE_CURSOR_2_PLAYERS_Y,
            SPRITE_INDEX_CURSOR, 0);
  }
}

void draw_game_over_cursor(void) {
  if (get_game_over_mode() == GAME_OVER_RETRY) {
    oam_spr(GAME_OVER_CURSOR_RETRY_X, GAME_OVER_CURSOR_RETRY_Y,
            SPRITE_INDEX_CURSOR, 0);
  } else {
    oam_spr(GAME_OVER_CURSOR_QUIT_X, GAME_OVER_CURSOR_QUIT_Y,
            SPRITE_INDEX_CURSOR, 0);
  }
}

void draw_pause_sprites(void) {
  for (temp_byte_2 = 0; temp_byte_2 < 6; ++temp_byte_2) {
    temp_byte_3 = temp_byte_2 << 3;
    oam_spr(PAUSE_LETTER_BASE_X + temp_byte_3, PAUSE_LETTER_BASE_Y,
            SPRITE_INDEX_PAUSE_BASE + temp_byte_2, 0);
  }
}

// The tile index delta is the number we add to a playfield tile index to move
// to the next playfield tile along the line segment.
// |orientation| is the line orientation and must be one of ORIENTATION_HORIZ
// or ORIENTATION_VERT.
#define compute_tile_index_delta(orientation) ((orientation)*31 + 1)

void update_line(unsigned char line_index) {
  // Do nothing if the line is not started.
  if (!get_line_is_started_flag(line_index)) {
    return;
  }

  set_was_line_segment_completed(FALSE);

  // The playfield tile at the front of each line segment is the only one
  // we need to worry about here.
  // The tile is 8 pixels wide and we draw one line of pixels at a time.
  // When we've reached completion of the tile, move the front of both
  // line segments forward in their directions.
  if (lines[line_index].current_block_completion == 8) {
    set_line_orientation(get_line_orientation_flag(line_index));
    set_tile_index_delta(compute_tile_index_delta(get_line_orientation()));
    set_negative_line_segment_origin(lines[line_index].origin);

    // Try and move the front of the negative-direction line segment forward
    // and complete the line segment if it's reached a wall.
    if (!get_line_is_negative_complete_flag(line_index)) {
      // Before moving the line segment forward, update the metadata for the
      // tile we're moving from.
      set_current_playfield_index(get_negative_line_segment_origin() -
                                  lines[line_index].tile_step_count *
                                      get_tile_index_delta());

      // While it was the front of the line segment, current playfield tile was
      // being drawn as a sprite. Now that it's complete, update the playfield
      // and bg tile. Note: The origin tile already has the playfield flags (and
      // bg tile) set. We can ignore that one (when tile_step_count == 0).
      if (lines[line_index].tile_step_count != 0) {
        set_playfield_tile(
            get_current_playfield_index(),
            get_playfield_tile_type_line(get_line_orientation(), line_index,
                                         LINE_DIRECTION_NEGATIVE),
            get_playfield_bg_tile_line(get_line_orientation()));
      }

      // Now move the front of the line segment forward by one tile.
      set_current_playfield_index(get_current_playfield_index() -
                                  get_tile_index_delta());

      // If the next tile is not an uncleared tile, that means we hit the end
      // for the line segment. Walk back over the tiles until we reach line
      // segment origin and update them to cleared.
      if (get_playfield_tile_type(get_current_playfield_index()) ==
          PLAYFIELD_WALL) {
        while (1) {
          // Walk back towards origin by one tile.
          set_current_playfield_index(get_current_playfield_index() +
                                      get_tile_index_delta());
          // Update the tile to cleared.
          cleared_tile_count++;
          set_playfield_tile(get_current_playfield_index(), PLAYFIELD_WALL,
                             TILE_INDEX_PLAYFIELD_CLEARED);
          add_score_for_cleared_tiles(1);

          // Stop when we reach the origin.
          if (get_current_playfield_index() ==
              get_negative_line_segment_origin()) {
            break;
          }
        }

        // Finished the negative-direction line segment.
        set_line_is_negative_complete_flag(line_index);

        // When line segments in both directions are complete, the line is done.
        if (get_line_is_positive_complete_flag(line_index)) {
          unset_line_is_started_flag(line_index);
        }

        set_was_line_segment_completed(TRUE);
      }
    }

    // Now do the positive direction.
    if (!get_line_is_positive_complete_flag(line_index)) {
      // Calculate positive-direction line segment origin based
      // on negative-direction line segment origin.
      set_positive_line_segment_origin(get_negative_line_segment_origin() +
                                       get_tile_index_delta());

      // Before moving the line segment forward, update the metadata for the
      // tile we're moving from.
      set_current_playfield_index(get_positive_line_segment_origin() +
                                  lines[line_index].tile_step_count *
                                      get_tile_index_delta());

      // While it was the front of the line segment, current playfield tile was
      // being drawn as a sprite. Now that it's complete, update the playfield
      // and bg tile. Note: The origin tile already has the playfield flags (and
      // bg tile) set. We can ignore that one (when tile_step_count == 0).
      if (lines[line_index].tile_step_count != 0) {
        set_playfield_tile(
            get_current_playfield_index(),
            get_playfield_tile_type_line(get_line_orientation(), line_index,
                                         LINE_DIRECTION_POSITIVE),
            get_playfield_bg_tile_line(get_line_orientation()));
      }

      // Now move the front of the line segment forward by one tile.
      set_current_playfield_index(get_current_playfield_index() +
                                  get_tile_index_delta());

      // If the next tile is not an uncleared tile, that means we hit the end
      // for the line segment. Walk back over the tiles until we reach line
      // segment origin and update them to cleared.
      if (get_playfield_tile_type(get_current_playfield_index()) ==
          PLAYFIELD_WALL) {
        while (1) {
          // Walk back towards origin by one tile.
          set_current_playfield_index(get_current_playfield_index() -
                                      get_tile_index_delta());
          // Update the tile to cleared.
          cleared_tile_count++;
          set_playfield_tile(get_current_playfield_index(), PLAYFIELD_WALL,
                             TILE_INDEX_PLAYFIELD_CLEARED);
          add_score_for_cleared_tiles(1);

          // Stop when we reach the origin.
          if (get_current_playfield_index() ==
              get_positive_line_segment_origin()) {
            break;
          }
        }

        // Finished the positive-direction line segment.
        set_line_is_positive_complete_flag(line_index);

        // When line segments in both directions are complete, the line is done.
        if (get_line_is_negative_complete_flag(line_index)) {
          unset_line_is_started_flag(line_index);
        }

        set_was_line_segment_completed(TRUE);
      }
    }

    lines[line_index].current_block_completion = 0;
    lines[line_index].tile_step_count++;
  } else {
    if (get_frame_count() % 2 == 0) {
      ++lines[line_index].current_block_completion;
    }
  }

  if (get_was_line_segment_completed()) {
    line_completed();
  }
}

void start_line(unsigned char player_index) {
  if (pads_new[player_index] & PAD_A) {
    // Do nothing if a line is already started for |player_index|.
    if (get_line_is_started_flag(player_index)) {
      return;
    }

    // Origin for the line is whatever tile we're tracking as "nearest" to the
    // player metasprite. This is technically the origin tile for the
    // negative-direction line segment. The origin tile for the
    // positive-direction line segment is origin + 1 (for horiz line) or origin
    // + 32 (for vert line) but we only keep track of one origin in the line
    // itself.
    set_negative_line_segment_origin(
        players[player_index].nearest_playfield_tile);

    // We only want to start a line if the origin tile is not already cleared.
    if (get_playfield_tile_type(get_negative_line_segment_origin()) ==
        PLAYFIELD_WALL) {
      return;
    }

    // Orientation of the line itself matches the current orientation of the
    // player.
    set_line_orientation(get_player_orientation_flag(player_index));

    // Update the playfield origin tile.
    set_playfield_tile(
        get_negative_line_segment_origin(),
        get_playfield_tile_type_line(get_line_orientation(), player_index,
                                     LINE_DIRECTION_NEGATIVE),
        get_playfield_bg_tile_line_origin(get_line_orientation(), FALSE));

    // Update the line data for the negative-direction line segment.
    lines[player_index].origin = get_negative_line_segment_origin();
    unset_line_is_negative_complete_flag(player_index);

    // Now check to see if we can start a positive-direction line segment.
    set_tile_index_delta(compute_tile_index_delta(get_line_orientation()));
    set_positive_line_segment_origin(get_negative_line_segment_origin() +
                                     get_tile_index_delta());

    // We can only start the positive-direction line segment if it would have
    // origin on an uncleared playfield tile.
    if (get_playfield_tile_type(get_positive_line_segment_origin()) !=
        PLAYFIELD_WALL) {
      // Update the positive-direction line segment origin playfield tile.
      set_playfield_tile(
          get_positive_line_segment_origin(),
          get_playfield_tile_type_line(get_line_orientation(), player_index,
                                       LINE_DIRECTION_POSITIVE),
          get_playfield_bg_tile_line_origin(get_line_orientation(), TRUE));
      unset_line_is_positive_complete_flag(player_index);
    }

    // Current line segment front tile is the origin tile.
    lines[player_index].tile_step_count = 0;
    // The origin tiles start at complete.
    lines[player_index].current_block_completion = 8;

    set_line_orientation_flag(player_index, get_line_orientation());
    set_line_is_started_flag(player_index);
  }
}

void draw_line(unsigned char line_index) {
  if (get_line_is_started_flag(line_index)) {
    set_line_orientation(get_line_orientation_flag(line_index));
    set_tile_index_delta(compute_tile_index_delta(get_line_orientation()));
    set_negative_line_segment_origin(lines[line_index].origin);

    if (!get_line_is_negative_complete_flag(line_index)) {
      set_current_playfield_index(get_negative_line_segment_origin() -
                                  lines[line_index].tile_step_count *
                                      get_tile_index_delta());

      // Negative-direction line segment front tile sprite needs to be flipped
      // (OAM_FLIP_H 0x40). If the line is vertical, we need to do vertical flip
      // instead (OAM_FLIP_V 0x80).
      temp_byte_2 = OAM_FLIP_H + OAM_FLIP_H * get_line_orientation();
      oam_spr(playfield_index_pixel_coord_x(get_current_playfield_index()),
              playfield_index_pixel_coord_y(get_current_playfield_index()) - 1,
              get_line_sprite_index(get_line_orientation(),
                                    lines[line_index].current_block_completion),
              1 | temp_byte_2);
    }

    if (!get_line_is_positive_complete_flag(line_index)) {
      set_positive_line_segment_origin(get_negative_line_segment_origin() +
                                       get_tile_index_delta());
      set_current_playfield_index(get_positive_line_segment_origin() +
                                  lines[line_index].tile_step_count *
                                      get_tile_index_delta());
      oam_spr(playfield_index_pixel_coord_x(get_current_playfield_index()),
              playfield_index_pixel_coord_y(get_current_playfield_index()) - 1,
              get_line_sprite_index(get_line_orientation(),
                                    lines[line_index].current_block_completion),
              1);
    }
  }
}

void check_ball_line_collisions(void) {
  // There can be no collision when there are no started lines.
  if (!get_line_is_started_flag(0) && !get_line_is_started_flag(1)) {
    return;
  }

  for (temp_byte_1 = 0; temp_byte_1 < get_ball_count(); ++temp_byte_1) {
    set_current_playfield_index(balls[temp_byte_1].nearest_playfield_tile);
    temp_byte_2 = playfield[get_current_playfield_index()];

    if (get_playfield_tile_type_from_byte(temp_byte_2) != PLAYFIELD_LINE) {
      // No collision.
      continue;
    }

    set_line_orientation(
        get_playfield_line_orientation_flag_from_byte(temp_byte_2));
    set_tile_index_delta(compute_tile_index_delta(get_line_orientation()));
    temp_byte_3 = get_playfield_line_index_flag_from_byte(temp_byte_2);
    set_negative_line_segment_origin(lines[temp_byte_3].origin);

    if (get_playfield_line_direction_flag_from_byte(temp_byte_2) ==
        LINE_DIRECTION_NEGATIVE) {
      // Playfield tile index of the line segment front tile.
      // This is drawn as a sprite and we haven't updated the playfield
      // metadata to include the line flags for this tile so we don't need
      // to do anything to the playfield for this tile index.
      set_current_playfield_index(get_negative_line_segment_origin() -
                                  lines[temp_byte_3].tile_step_count *
                                      get_tile_index_delta());

      // Walk back across the line segment (to origin) and reset the playfield
      // tiles to uncleared.
      while (1) {
        // Walk back towards origin by one tile.
        set_current_playfield_index(get_current_playfield_index() +
                                    get_tile_index_delta());
        // Reset the tile to uncleared.
        set_playfield_tile(get_current_playfield_index(), PLAYFIELD_UNCLEARED,
                           TILE_INDEX_PLAYFIELD_UNCLEARED);
        // Stop when we reach the origin.
        if (get_current_playfield_index() ==
            get_negative_line_segment_origin()) {
          break;
        }
      }

      // Turn off the negative-direction line segment - the collision stopped
      // it.
      set_line_is_negative_complete_flag(temp_byte_3);

      // If the other direction is also complete, reset the is_started flag of
      // the line.
      if (get_line_is_positive_complete_flag(temp_byte_3)) {
        unset_line_is_started_flag(temp_byte_3);
      }
    } else {
      set_positive_line_segment_origin(get_negative_line_segment_origin() +
                                       get_tile_index_delta());

      // Playfield tile index of the line segment front tile.
      // This is drawn as a sprite and we haven't updated the playfield
      // metadata to include the line flags for this tile so we don't need
      // to do anything to the playfield for this tile index.
      set_current_playfield_index(get_positive_line_segment_origin() +
                                  lines[temp_byte_3].tile_step_count *
                                      get_tile_index_delta());

      // Walk back across the line segment (to origin) and reset the playfield
      // tiles to uncleared.
      while (1) {
        // Walk back towards origin by one tile.
        set_current_playfield_index(get_current_playfield_index() -
                                    get_tile_index_delta());
        // Reset the tile to uncleared.
        set_playfield_tile(get_current_playfield_index(), PLAYFIELD_UNCLEARED,
                           TILE_INDEX_PLAYFIELD_UNCLEARED);
        // Stop when we reach the origin.
        if (get_current_playfield_index() ==
            get_positive_line_segment_origin()) {
          break;
        }
      }

      // Turn off the positive-direction line segment - the collision stopped
      // it.
      set_line_is_positive_complete_flag(temp_byte_3);

      // If the other direction is also complete, reset the is_started flag of
      // the line.
      if (get_line_is_negative_complete_flag(temp_byte_3)) {
        unset_line_is_started_flag(temp_byte_3);
      }
    }

#if ENABLE_CHEATS
    if (enable_losing_lives == TRUE)
#endif  // ENABLE_CHEATS
    {
      lives_count--;
    }

    if (lives_count == 0) {
      // We ran out of lives, move to game over state.
      change_to_game_over();
    } else {
      // We changed the lives count, let's redraw the HUD.
      game_state = GAME_STATE_REQUEST_HUD_UPDATE;
    }
  }
}

void flip_player_orientation(unsigned char player_index) {
  if (pads_new[player_index] & PAD_B) {
    set_player_orientation_flag(player_index,
                                get_player_orientation_flag(player_index) ^ 1);

    if (get_player_orientation_flag(player_index) == ORIENTATION_HORIZ) {
      temp_byte_2 = PLAYFIELD_LEFT_WALL + 8;
      if (players[player_index].x <= temp_byte_2) {
        players[player_index].x = temp_byte_2;
      }
    } else {
      temp_byte_2 = PLAYFIELD_TOP_WALL + 8;
      if (players[player_index].y <= temp_byte_2) {
        players[player_index].y = temp_byte_2;
      }
    }

    set_temp_ptr(&players[temp_byte_1]);
    update_nearest_tile();
  }
}

// Don't modify temp_byte_1
void update_nearest_tile(void) {
  temp_byte_2 = get_player_orientation_flag_from_byte(get_temp_ptr(struct Player)->flags);

  // Center of the player meta-sprite in pixel coords.
  if (temp_byte_2 == ORIENTATION_HORIZ) {
    temp_byte_3 = get_temp_ptr(struct Player)->x;
    temp_byte_4 = get_temp_ptr(struct Player)->y + 4;
  } else {
    temp_byte_3 = get_temp_ptr(struct Player)->x + 4;
    temp_byte_4 = get_temp_ptr(struct Player)->y;
  }

  get_temp_ptr(struct Player)->nearest_tile_x = (temp_byte_3 >> 3) << 3;
  get_temp_ptr(struct Player)->nearest_tile_y = (temp_byte_4 >> 3) << 3;
  get_temp_ptr(struct Player)->nearest_playfield_tile = playfield_tile_from_pixel_coords(temp_byte_3, temp_byte_4);
}

void line_completed(void) {
  unsigned char i;
  reset_playfield_mark_bit();

  for (i = 0; i < get_ball_count(); ++i) {
    compute_playfield_mark_bit_one_ball(i);
  }

  // Grant score for clearing a line segment.
  add_score_for_cleared_line();

  // Reset |playfield_index|, set the game state to updating the playfield,
  // which will cause us to call update_cleared_playfield_tiles() from the
  // beginning next frame. If we need to call it again after that, we will call
  // it in restartable mode.
  set_playfield_index(0);
  game_state = GAME_STATE_UPDATING_PLAYFIELD;
}

// Updates the playfield in-memory data structure and sets |tile_index| to
// |playfield_tile_type|. Sets the bg tile graphic for |tile_index| to
// |playfield_bg_tile|.
//
// |playfield_tile_type| has the new playfield tile type (ie: PLAYFIELD_WALL)
// |playfield_bg_tile| has the new bg tile graphic index (ie:
// TILE_INDEX_PLAYFIELD_CLEARED)
void set_playfield_tile(unsigned int tile_index,
                        unsigned char playfield_tile_type,
                        unsigned char playfield_bg_tile) {
  // Update the playfield in-memory structure.
  playfield[tile_index] = playfield_tile_type;
  // Set the bg tile graphic
  one_vram_buffer(playfield_bg_tile,
                  get_ppu_addr(0, playfield_index_pixel_coord_x(tile_index),
                               playfield_index_pixel_coord_y(tile_index)));
}

// Update uncleared, unmarked playfield tiles to be cleared. Returns TRUE when
// all uncleared tiles have been updated. Note: This function can potentially
// queue more vram updates than are allowed during the next v-blank.
//       For that reason, it is restartable.
//       The current playfield_index needs to be reset to zero once at the
//       beginning of the operation. Otherwise, calling this function will
//       continue from where it left off last time. It returns TRUE when all
//       vram updates are queued and FALSE if there are additonal vram updates
//       pending.
//
// scratch:
// temp_byte_3
unsigned char update_cleared_playfield_tiles(void) {
  temp_byte_3 = 0;
  // Look over all tiles in the playfield and for each uncleared, unmarked tile
  // change it to cleared
  for (; get_playfield_index() < PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT;
       inc_playfield_index()) {
    // Skip tiles which are not uncleared (this includes marked tiles)
    if (playfield[get_playfield_index()] != PLAYFIELD_UNCLEARED) {
      continue;
    }

    temp_byte_3++;
    cleared_tile_count++;
    set_playfield_tile(get_playfield_index(), PLAYFIELD_WALL,
                       TILE_INDEX_PLAYFIELD_CLEARED);

    // We can only queue about 40 tile updates per v-blank.
    if (temp_byte_3 >= MAX_TILE_UPDATES_PER_FRAME) {
      add_score_for_cleared_tiles(temp_byte_3);
      return FALSE;
    }
  }

  add_score_for_cleared_tiles(temp_byte_3);
  return TRUE;
}

// Reset the mark bit in all playfield tiles.
//
// scratch:
// temp_int_1
void reset_playfield_mark_bit(void) {
  for (temp_int_1 = 0; temp_int_1 < PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT;
       ++temp_int_1) {
    unset_playfield_is_marked_flag(temp_int_1);
  }
}
