#ifndef __TEXTURES__
#define __TEXTURES__

/**
 * Cursor textures
 */
#define CURSOR_SIZE_HALF 16
int tex_cursor_idle;
int tex_cursor_hand;
int tex_cursor_wait;

/**
 * UI textures
 */
jo_font *tex_ui_font;
jo_font *tex_ui_fontButton;
int tex_input_error;
int tex_cnt_window;

/**
 * Rail tile sprite indexes
 */
#define TILE_STRAIGHT_ROTATION 2
int tex_tile_straight_horizontal;
int tex_tile_straight_vertical;
int tex_tile_straight_diagonal_left;
int tex_tile_straight_diagonal_right;

#define TILE_TURN_ROTATION 4
int tex_tile_turn_left;
int tex_tile_turn_up;
int tex_tile_turn_right;
int tex_tile_turn_down;
int tex_tile_turn_left_mirror;
int tex_tile_turn_up_mirror;
int tex_tile_turn_right_mirror;
int tex_tile_turn_down_mirror;

/**
 * Special rail tile sprite indexes
 */
#define TILE_SPECIAL_ROTATION 4
int tex_tile_depo_left;
int tex_tile_depo_up;
int tex_tile_depo_right;
int tex_tile_depo_down;

int tex_tile_signal_enabled_left;
int tex_tile_signal_enabled_up;
int tex_tile_signal_enabled_right;
int tex_tile_signal_enabled_down;

int tex_tile_signal_disabled_left;
int tex_tile_signal_disabled_up;
int tex_tile_signal_disabled_right;
int tex_tile_signal_disabled_down;

#endif
