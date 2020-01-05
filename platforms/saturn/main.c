#define DEBUG

#include <jo/jo.h>
#include "../../include/platformhacks.h"
#include "../../include/input.h"
#include "../../include/textures.h"

/**
 * If drive door opens return to boot menu
 */
void checkDriveDoor(void)
{
    CdcStat status;
    CDC_GetCurStat(&status);

    if (status.status == CDC_ST_OPEN || 
        status.status == CDC_ST_NODISC)
    {
        jo_goto_boot_menu();
    }
}

/**
 * Main game loop
 */
void mainLoop(void)
{

}

/**
 * Handle game pad input on port 0
 */
void gamePadInput(void)
{
    int mouseSpeed = 1,
        position_x = input_mouse_x,
        position_y = input_mouse_y;

    // Use right trigger to speed up the mouse movement
    if (jo_is_pad1_key_pressed(JO_KEY_R))
    {
        mouseSpeed = 2;
    }

    // Move mouse using D-Pad
    if (jo_is_pad1_key_pressed(JO_KEY_UP))
    {
        position_y -= mouseSpeed;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_DOWN))
    {
        position_y += mouseSpeed;
    }
    
    if (jo_is_pad1_key_pressed(JO_KEY_LEFT))
    {
        position_x -= mouseSpeed;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_RIGHT))
    {
        position_x += mouseSpeed;
    }

    // Dont allow mouse to go outside of bounds
    input_mouse_x = JO_MAX(JO_MIN(position_x, JO_TV_WIDTH - 4), 0);
    input_mouse_y = JO_MAX(JO_MIN(position_y, JO_TV_HEIGHT - 4), 0);

    // Bind A,B, C buttons to keys
    if (jo_is_pad1_key_down(JO_KEY_A))
    {
        push_key_input(KI_SELECT);
    }
    
    if (jo_is_pad1_key_down(JO_KEY_B) || jo_is_pad1_key_down(JO_KEY_C))
    {
        push_key_input(KI_CANCEL);
    }
    
    if (jo_is_pad1_key_down(JO_KEY_START))
    {
        push_key_input(KI_ESCAPE);
    }
}

/**
 * Handle mouse input from port 0
 */
void mouseInput(void)
{
    input_mouse_x = jo_get_mouse_pos_x(0);
    input_mouse_y = jo_get_mouse_pos_y(0);

    if (jo_is_pad1_key_down(JO_MOUSE_LEFT_BUTTON))
    {
        push_key_input(KI_SELECT);
    }
    else if (jo_is_pad1_key_down(JO_MOUSE_RIGHT_BUTTON))
    {
        push_key_input(KI_CANCEL);
    }
    else if (jo_is_pad1_key_down(JO_MOUSE_START_BUTTON))
    {
        push_key_input(KI_ESCAPE);
    }
}

/**
 * Input process loop
 */
void inputLoop(void)
{
    bool inputIsValid = false;

    // Devices we really should support:
    // Sega saturn controller (standard)
    // Sega Saturn controller (3D)
    // Sega saturn mouse
    // push any current inputs to the active_inputs buffer (see input.h)
    // Mouse position goes to mouse_x and mouse_y
    if (jo_is_pad1_available())
    {
        jo_gamepad_type type = jo_get_input_type(0);

        if (type == JoRegularGamepad)
        {
            gamePadInput();
            inputIsValid = true;
        }
        else if (type == JoRegularMouse ||
                 type == JoShuttleMouse)
        {
            mouseInput();
            inputIsValid = true;
        }
    }

    if (inputIsValid)
    {
        process_inputs();
    }
    else
    {
        // set up a indicator to pause game and tell player to plug in gamepad or other supported device.
    }
}

/**
 * Render loop
 */
void renderLoop(void)
{
    // Draw mouse cursor at Z=100 and where X,Y have origin at top left corner of the screen
    jo_sprite_draw3D2(tex_cursor_idle, input_mouse_x - CURSOR_SIZE_HALF, input_mouse_y - CURSOR_SIZE_HALF, 100);
}

/**
 * Load sprites
 */
void loadSprites(void)
{
    // Load cursors
    tex_cursor_idle = jo_sprite_add_tga("UI", "CUR_IDLE.TGA", JO_COLOR_Black);
    tex_cursor_hand = jo_sprite_add_tga("UI", "CUR_HAND.TGA", JO_COLOR_Black);
    tex_cursor_wait = jo_sprite_add_tga("UI", "CUR_WAIT.TGA", JO_COLOR_Black);

    // Load UI stuff

    // Load tiles
    tex_tile_depo_left = jo_sprite_add_tga("TILES", "DL.TGA", JO_COLOR_Black);
    tex_tile_depo_up = jo_sprite_add_tga("TILES", "DU.TGA", JO_COLOR_Black);
    tex_tile_depo_right = jo_sprite_add_tga("TILES", "DR.TGA", JO_COLOR_Black);
    tex_tile_depo_down = jo_sprite_add_tga("TILES", "DD.TGA", JO_COLOR_Black);

    tex_tile_signal_enabled_left = jo_sprite_add_tga("TILES", "SGLE.TGA", JO_COLOR_Black);
    tex_tile_signal_enabled_up = jo_sprite_add_tga("TILES", "SGUE.TGA", JO_COLOR_Black);
    tex_tile_signal_enabled_right = jo_sprite_add_tga("TILES", "SGRE.TGA", JO_COLOR_Black);
    tex_tile_signal_enabled_down = jo_sprite_add_tga("TILES", "SGDE.TGA", JO_COLOR_Black);
    
    tex_tile_signal_disabled_left = jo_sprite_add_tga("TILES", "SGLD.TGA", JO_COLOR_Black);
    tex_tile_signal_disabled_up = jo_sprite_add_tga("TILES", "SGUD.TGA", JO_COLOR_Black);
    tex_tile_signal_disabled_right = jo_sprite_add_tga("TILES", "SGRD.TGA", JO_COLOR_Black);
    tex_tile_signal_disabled_down = jo_sprite_add_tga("TILES", "SGDD.TGA", JO_COLOR_Black);

    tex_tile_turn_left = jo_sprite_add_tga("TILES", "TL.TGA", JO_COLOR_Black);
    tex_tile_turn_up = jo_sprite_add_tga("TILES", "TU.TGA", JO_COLOR_Black);
    tex_tile_turn_right = jo_sprite_add_tga("TILES", "TR.TGA", JO_COLOR_Black);
    tex_tile_turn_down = jo_sprite_add_tga("TILES", "TD.TGA", JO_COLOR_Black);
    tex_tile_turn_left_mirror = jo_sprite_add_tga("TILES", "TLM.TGA", JO_COLOR_Black);
    tex_tile_turn_up_mirror = jo_sprite_add_tga("TILES", "TUM.TGA", JO_COLOR_Black);
    tex_tile_turn_right_mirror = jo_sprite_add_tga("TILES", "TRM.TGA", JO_COLOR_Black);
    tex_tile_turn_down_mirror = jo_sprite_add_tga("TILES", "TDM.TGA", JO_COLOR_Black);

    tex_tile_straight_horizontal = jo_sprite_add_tga("TILES", "SH.TGA", JO_COLOR_Black);
    tex_tile_straight_vertical = jo_sprite_add_tga("TILES", "SV.TGA", JO_COLOR_Black);

    tex_tile_straight_diagonal_left = jo_sprite_add_tga("TILES", "SDL.TGA", JO_COLOR_Black);
    tex_tile_straight_diagonal_right = jo_sprite_add_tga("TILES", "SDR.TGA", JO_COLOR_Black);
}

/**
 * If code here is being run, the game has died horribly, and some form of crash report needs to be shown.
 * @param panic_reason Reason of the crash.
 * @param function_name Origin of the crash.
 */
void saturn_panic_handler(char* panic_reason, const char* function_name) {
    #ifdef JO_DEBUG
    __jo_core_error(panic_reason, function_name);
    jo_clear_background(JO_COLOR_Black);
    #endif
}

/**
 * Saturn application entry point
 */
void jo_main(void)
{
    // platform setup code
    p_alloc = jo_malloc; // Set up platform allocator
    p_free = jo_free;
    p_panic = saturn_panic_handler;

    // Initialize engine core
    jo_core_init(JO_COLOR_Black);
    loadSprites();    

    // Run game logic on main CPU
    jo_core_add_callback(checkDriveDoor);
    jo_core_add_callback(inputLoop);
    jo_core_add_callback(mainLoop);

    // Render stuff on slave CPU
    jo_core_add_slave_callback(renderLoop);
    
    jo_core_run();
}
