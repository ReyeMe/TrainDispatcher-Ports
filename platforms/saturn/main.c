#include <jo/jo.h>

/**
 * Main game loop
 */
void mainLoop(void)
{

}

/**
 * Input process loop
 */
void inputLoop(void)
{

}

/**
 * Render loop
 */
void renderLoop(void)
{

}

/**
 * Saturn application entry point
 */
void jo_main(void)
{
    jo_core_init(JO_COLOR_Black);

    jo_core_add_callback(inputLoop);
    jo_core_add_callback(mainLoop);
    jo_core_add_slave_callback(renderLoop);
    
    jo_core_run();
}
