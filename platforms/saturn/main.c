#include <jo/jo.h>

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

    // Run game logic on main CPU
    jo_core_add_callback(checkDriveDoor);
    jo_core_add_callback(inputLoop);
    jo_core_add_callback(mainLoop);

    // Render stuff on slave CPU
    jo_core_add_slave_callback(renderLoop);
    
    jo_core_run();
}
