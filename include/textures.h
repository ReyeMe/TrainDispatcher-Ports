#ifndef __TEXTURES__
#define __TEXTURES__

/**
 * Sursor textures
 */
#define CURSOR_SIZE_HALF 16
static int tex_cursor_idle;
static int tex_cursor_hand;
static int tex_cursor_wait;

/**
 * UI textures
 */



/**
 * Rail tile sprite indexes
 */
#define TILE_STRAIGHT_ROTATION 2
static int tex_tile_straight_horizontal = 0;
static int tex_tile_straight_vertical = 0;
static int tex_tile_straight_diagonal_left = 0;
static int tex_tile_straight_diagonal_right = 0;

#define TILE_TURN_ROTATION 4
static int tex_tile_turn_left = 0;
static int tex_tile_turn_up = 0;
static int tex_tile_turn_right = 0;
static int tex_tile_turn_down = 0;
static int tex_tile_turn_left_mirror = 0;
static int tex_tile_turn_up_mirror = 0;
static int tex_tile_turn_right_mirror = 0;
static int tex_tile_turn_down_mirror = 0;

/**
 * Special rail tile sprite indexes
 */
#define TILE_SPECIAL_ROTATION 4
static int tex_tile_depo_left = 0;
static int tex_tile_depo_up = 0;
static int tex_tile_depo_right = 0;
static int tex_tile_depo_down = 0;

static int tex_tile_signal_enabled_left = 0;
static int tex_tile_signal_enabled_up = 0;
static int tex_tile_signal_enabled_right = 0;
static int tex_tile_signal_enabled_down = 0;

static int tex_tile_signal_disabled_left = 0;
static int tex_tile_signal_disabled_up = 0;
static int tex_tile_signal_disabled_right = 0;
static int tex_tile_signal_disabled_down = 0;

#endif
