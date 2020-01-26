#define DEBUG

#include "../../include/platformhacks.h"
#include "../../include/input.h"
#include "../../include/time.h"
#include "../../include/windowManager.h"
#include "textures.h"
#include "WM_render.h"

/**
 * If this variable is false, game will pause and ask for supported device to be plugged in
 */
bool inputIsValid = true;

/**
 * Count frames, 60 = 1 second
 */
#define SECOND_IN_FRAMES 60
char frames = 0;

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
    if (!inputIsValid)
    {
        // Pause game
        return;
    }

    // TODO: call game logic
    // ...

    // All loops are synced to 60fps by JO-Engine
    if (++frames >= SECOND_IN_FRAMES)
    {
        frames = 0;
        time_UpdateSeconds(time_current);
    }
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
    input_mouse_x = JO_MAX(JO_MIN(position_x, JO_TV_WIDTH - 3), 0);
    input_mouse_y = JO_MAX(JO_MIN(position_y, JO_TV_HEIGHT - 3), 0);

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
    bool isValid = false;

    // Devices we really should support:
    // Sega saturn controller (standard)
    // Sega Saturn controller (3D)
    // Sega saturn mouse
    // push any current inputs to the active_inputs buffer (see input.h)
    // Mouse position goes to mouse_x and mouse_y
    if (jo_is_pad1_available())
    {
        jo_gamepad_type type = jo_get_input_type(0);

        // 3D gamepad has id of 0x16
        if (type == JoRegularGamepad)
        {
            gamePadInput();
            isValid = true;
        }
        else if (type == JoRegularMouse ||
                 type == JoShuttleMouse)
        {
            mouseInput();
            isValid = true;
        }
    }

    if (isValid)
    {
        process_inputs();
        WM_ProcessInput();
    }

    inputIsValid = isValid;
}

/**
 * Draw invalid input message on screen
 */
void showInvalidInputMessage(void)
{
    jo_clear_screen();
    jo_gamepad_type type = jo_get_input_type(0);

    switch (type)
    {
        case JoUnsupportedGamepad:
            jo_printf_with_color(7,18, JO_COLOR_INDEX_Red,"Controller not supported!");
            break;
        
        case JoNotConnectedGamepad:
            jo_printf_with_color(8,18, JO_COLOR_INDEX_Red,"No Controller detected!");
            break;

        default:
            break;
    }

    jo_sprite_draw3D(tex_input_error, 0, 0, 500);
}

/**
 * Render loop
 */
void renderLoop(void)
{
    // Show invalid input message
    if (!inputIsValid)
    {
        showInvalidInputMessage();
        return;
    }
    
    jo_printf(2,2,"%02d:%02d:%02d", time_current->Hours, time_current->Minutes, time_current->Seconds);

    // Draw mouse cursor at Z=100 and where X,Y have origin at top left corner of the screen
    jo_sprite_draw3D2(tex_cursor_idle, input_mouse_x - CURSOR_SIZE_HALF, input_mouse_y - CURSOR_SIZE_HALF, 100);
    
    // Render controls
    WM_Render();
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
    tex_input_error = jo_sprite_add_tga("UI", "GP_ERR.TGA", JO_COLOR_Black);
    tex_cnt_window = jo_sprite_add_tga("UI", "WND_BG.TGA", JO_COLOR_Black);
    tex_ui_font = jo_font_load("UI", "FN_UI.TGA", JO_COLOR_Green, 8, 8, 2, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"?=%&',.()*+-/");
    tex_ui_fontButton = jo_font_load("UI", "FN_UIB.TGA", JO_COLOR_Green, 8, 8, 2, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"?=%&',.()*+-/");

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
    time_current = p_alloc(sizeof(GameTime));
    time_Reset(time_current);
    WM_Initialize();

    // Setup SGL
    slCurWindow(winNear); 
    slWindow(0, 0, JO_TV_WIDTH-1, JO_TV_HEIGHT-1, 1024, JO_TV_WIDTH_2, JO_TV_HEIGHT_2);
    slScrWindow0(0, 0, JO_TV_WIDTH-1, JO_TV_HEIGHT-1);
    slScrWindowModeNbg1(win0_IN);
    slScrWindowModeNbg0(win1_IN);
    slPerspective(70);
    slZdspLevel(6);

    // Create test window with label and button
    WM_Control *window = WM_CreateWidget(50,50,200,75,WM_Type_Window,NULL,NULL);
    WM_CreateWidget(10,10,180,55,WM_Type_Label,window,NULL)->Text = "THIS IS A LABEL.";
    WM_CreateWidget(135,60,60,9,WM_Type_Button,window,NULL)->Text = "BUTTON";

    // Load stuff
    loadSprites();    

    // Run game logic on main CPU
    jo_core_add_callback(checkDriveDoor);
    jo_core_add_callback(inputLoop);
    jo_core_add_callback(mainLoop);

    // Render stuff on slave CPU
    jo_core_add_slave_callback(renderLoop);
    
    jo_core_run();
}
