#include "../include/engine.h"

/**
 * Input button is pressed
 */
static void ButtonPressed(Button button)
{

}

/**
 * Input button got clicked
 */
static void ButtonClicked(Button button)
{

}

/**
 * Initialize engine
 */
void Core_Initialize()
{
    Input_ButtonPressed = ButtonPressed;
    Input_ButtonClicked = ButtonClicked;
}

/**
 * Game loop
 */
void Core_GameLogic(GameTime *time)
{
    
}