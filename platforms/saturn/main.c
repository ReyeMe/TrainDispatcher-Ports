#define DEBUG

#include "../../include/platformhacks.h"
#include "../../include/input.h"
#include "../../include/time.h"
#include "../../include/input.h"
#include "textures.h"

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
        Time_UpdateSeconds(Time_Current);
    }
}

/**
 * Handle game pad input on port 0
 */
void gamePadInput(void)
{
    int mouseSpeed = 1,
        position_x = Input_Mouse.X,
        position_y = Input_Mouse.Y;

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
    Input_Mouse.X = JO_MAX(JO_MIN(position_x, JO_TV_WIDTH - 3), 0);
    Input_Mouse.Y = JO_MAX(JO_MIN(position_y, JO_TV_HEIGHT - 3), 0);

    // Bind pressed A ,B ,C buttons
    if (jo_is_pad1_key_pressed(JO_KEY_A))
    {
        Input_ButtonPressed(Btn_Left);
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_B) || jo_is_pad1_key_pressed(JO_KEY_C))
    {
        Input_ButtonPressed(Btn_Right);
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_START))
    {
        Input_ButtonPressed(Btn_Escape);
    }

    // Bind clicked A ,B ,C buttons
    if (jo_is_pad1_key_down(JO_KEY_A))
    {
        Input_ButtonClicked(Btn_Left);
    }
    else if (jo_is_pad1_key_down(JO_KEY_B) || jo_is_pad1_key_down(JO_KEY_C))
    {
        Input_ButtonClicked(Btn_Right);
    }
    else if (jo_is_pad1_key_down(JO_KEY_START))
    {
        Input_ButtonClicked(Btn_Escape);
    }
}

/**
 * Handle mouse input from port 0
 */
void mouseInput(void)
{
    Input_Mouse.X = jo_get_mouse_pos_x(0);
    Input_Mouse.Y = jo_get_mouse_pos_y(0);

    // Bind pressed buttons
    if (jo_is_pad1_key_pressed(JO_MOUSE_LEFT_BUTTON))
    {
        Input_ButtonPressed(Btn_Left);
    }
    else if (jo_is_pad1_key_pressed(JO_MOUSE_RIGHT_BUTTON))
    {
        Input_ButtonPressed(Btn_Right);
    }
    else if (jo_is_pad1_key_pressed(JO_MOUSE_START_BUTTON))
    {
        Input_ButtonPressed(Btn_Escape);
    }
    
    // Bind clicked buttons
    if (jo_is_pad1_key_down(JO_MOUSE_LEFT_BUTTON))
    {
        Input_ButtonClicked(Btn_Left);
    }
    else if (jo_is_pad1_key_down(JO_MOUSE_RIGHT_BUTTON))
    {
        Input_ButtonClicked(Btn_Right);
    }
    else if (jo_is_pad1_key_down(JO_MOUSE_START_BUTTON))
    {
        Input_ButtonClicked(Btn_Escape);
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

    jo_sprite_draw3D(Tex_ControllerError, 0, 0, 500);
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
    
    jo_printf(2,2,"%02d:%02d:%02d", Time_Current->Hours, Time_Current->Minutes, Time_Current->Seconds);

    // Draw mouse cursor at Z=100 and where X,Y have origin at top left corner of the screen
    jo_sprite_draw3D2(Input_Mouse.Cursor, Input_Mouse.X - CURSOR_SIZE_HALF, Input_Mouse.Y - CURSOR_SIZE_HALF, 100);
}

/**
 * Load sprites
 */
void loadSprites(void)
{
    // Load cursors
    Input_CursorIdle = jo_sprite_add_tga("UI", "CUR_IDLE.TGA", JO_COLOR_Black);
    Input_CursorHand = jo_sprite_add_tga("UI", "CUR_HAND.TGA", JO_COLOR_Black);
    Input_CursorWait = jo_sprite_add_tga("UI", "CUR_WAIT.TGA", JO_COLOR_Black);
    Input_CursorMove = jo_sprite_add_tga("UI", "CUR_MOVE.TGA", JO_COLOR_Black);

    // Load UI stuff
    Tex_ControllerError = jo_sprite_add_tga("UI", "GP_ERR.TGA", JO_COLOR_Black);
    Tex_UIFont = jo_font_load("UI", "FN_UI.TGA", JO_COLOR_Green, 8, 8, 2, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"?=%&',.()*+-/");

    // Load tiles
    Tex_TileDepoLeft = jo_sprite_add_tga("TILES", "DL.TGA", JO_COLOR_Black);
    Tex_TileDepoUp = jo_sprite_add_tga("TILES", "DU.TGA", JO_COLOR_Black);
    Tex_TileDepoRight = jo_sprite_add_tga("TILES", "DR.TGA", JO_COLOR_Black);
    Tex_TileDepoDown = jo_sprite_add_tga("TILES", "DD.TGA", JO_COLOR_Black);

    Tex_TileSignalEnabledLeft = jo_sprite_add_tga("TILES", "SGLE.TGA", JO_COLOR_Black);
    Tex_TileSignalEnabledUp = jo_sprite_add_tga("TILES", "SGUE.TGA", JO_COLOR_Black);
    Tex_TileSignalEnabledRight = jo_sprite_add_tga("TILES", "SGRE.TGA", JO_COLOR_Black);
    Tex_TileSignalEnabledDown = jo_sprite_add_tga("TILES", "SGDE.TGA", JO_COLOR_Black);

    Tex_TileSignalDisabledLeft = jo_sprite_add_tga("TILES", "SGLD.TGA", JO_COLOR_Black);
    Tex_TileSignalDisabledUp = jo_sprite_add_tga("TILES", "SGUD.TGA", JO_COLOR_Black);
    Tex_TileSignalDisabledRight = jo_sprite_add_tga("TILES", "SGRD.TGA", JO_COLOR_Black);
    Tex_TileSignalDisabledDown = jo_sprite_add_tga("TILES", "SGDD.TGA", JO_COLOR_Black);

    Tex_TileTurnLeft = jo_sprite_add_tga("TILES", "TL.TGA", JO_COLOR_Black);
    Tex_TileTurnUp = jo_sprite_add_tga("TILES", "TU.TGA", JO_COLOR_Black);
    Tex_TileTurnRight = jo_sprite_add_tga("TILES", "TR.TGA", JO_COLOR_Black);
    Tex_TileTurnDown = jo_sprite_add_tga("TILES", "TD.TGA", JO_COLOR_Black);
    Tex_TileTurnLeftMirror = jo_sprite_add_tga("TILES", "TLM.TGA", JO_COLOR_Black);
    Tex_TileTurnUpMirror = jo_sprite_add_tga("TILES", "TUM.TGA", JO_COLOR_Black);
    Tex_TileTurnRightMirror = jo_sprite_add_tga("TILES", "TRM.TGA", JO_COLOR_Black);
    Tex_TileTurnDownMirror = jo_sprite_add_tga("TILES", "TDM.TGA", JO_COLOR_Black);

    Tex_TileStraightHorizontal = jo_sprite_add_tga("TILES", "SH.TGA", JO_COLOR_Black);
    Tex_TileStraightVertical = jo_sprite_add_tga("TILES", "SV.TGA", JO_COLOR_Black);

    Tex_TileStraightDiagonalLeft = jo_sprite_add_tga("TILES", "SDL.TGA", JO_COLOR_Black);
    Tex_TileStraightDiagonalRight = jo_sprite_add_tga("TILES", "SDR.TGA", JO_COLOR_Black);
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
    Time_Current = p_alloc(sizeof(GameTime));
    Time_Reset(Time_Current);

    // Default cursor to center screen and set default cursor image
    Input_Mouse.X = JO_TV_WIDTH_2;
    Input_Mouse.Y = JO_TV_HEIGHT_2;
    Input_Mouse.Cursor = Input_CursorIdle;

    // Setup SGL
    slCurWindow(winNear); 
    slWindow(0, 0, JO_TV_WIDTH-1, JO_TV_HEIGHT-1, 1024, JO_TV_WIDTH_2, JO_TV_HEIGHT_2);
    slScrWindow0(0, 0, JO_TV_WIDTH-1, JO_TV_HEIGHT-1);
    slScrWindowModeNbg1(win0_IN);
    slScrWindowModeNbg0(win1_IN);
    slPerspective(70);
    slZdspLevel(6);

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
