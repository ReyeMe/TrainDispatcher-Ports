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
    if (jo_is_pad1_key_pressed(JO_KEY_UP) && input_mouse_y > 0)
    {
        input_mouse_y--;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_DOWN) && input_mouse_y < JO_TV_HEIGHT - 3)
    {
        input_mouse_y++;
    }
    
    if (jo_is_pad1_key_pressed(JO_KEY_LEFT) && input_mouse_x > 0)
    {
        input_mouse_x--;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_RIGHT) && input_mouse_x < JO_TV_WIDTH - 3)
    {
        input_mouse_x++;
    }

    if (jo_is_pad1_key_down(JO_KEY_A))
    {
        push_key_input(KI_SELECT);
    }
    
    if (jo_is_pad1_key_down(JO_KEY_B))
    {
        
    }
    
    if (jo_is_pad1_key_down(JO_KEY_START))
    {

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
        
    }
    else if (jo_is_pad1_key_down(JO_MOUSE_START_BUTTON))
    {
        
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
    // Sega saturn keyboard (for debugging only, not suported by joEngine, use SGL. Extra buttons are handy)
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
    jo_sprite_draw3D2(tex_cursor_default, input_mouse_x - CURSOR_SIZE_HALF, input_mouse_y - CURSOR_SIZE_HALF, 100);

}

/**
 * Load sprites
 */
void loadSprites(void)
{
    tex_cursor_default = jo_sprite_add_tga("UI", "CUR_DF.TGA", JO_COLOR_Black);
}

/**
 * If code here is being run, the game has died horribly, and some form of crash report needs to be shown.
 * @param panic_reason Reason of the crash.
 * @param function_name Origin of the crash.
 */
void saturn_panic_handler(char* panic_reason, const char* function_name) {
    #ifdef JO_DEBUG
    __jo_core_error(panic_reason, function_name);
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
