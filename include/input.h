#ifndef __INPUT__
#define __INPUT__

/**
 * Available buttons
 */
typedef enum
{
    Btn_None,
    Btn_Left,
    Btn_Middle,
    Btn_Right,
    Btn_Escape
} Button;

/**
 * Mouse position
 */
typedef struct
{
    int X;
    int Y;
    int Cursor;
} Mouse;

/**
 * Cursor texture indexes and half size
 */
#define CURSOR_SIZE_HALF 16
int Input_CursorIdle;
int Input_CursorHand;
int Input_CursorWait;
int Input_CursorMove;

/**
 * Mouse data
 */
Mouse Input_Mouse = { 0, 0, 0 };

/**
 * Called when button is pressed
 */
void (*Input_ButtonPressed)(Button button);

/**
 * Called when button is clicked
 */
void (*Input_ButtonClicked)(Button button);

#endif
