#include "../include/time.h"
#include "../include/platformhacks.h"

/**
 * Reset time back to zero 
 */
void time_Reset(GameTime *time)
{
    time->Hours = 20;
    time->Minutes = 0;
    time->Seconds = 0;
}

/**
 * Move time forward by 1 second 
 */
void time_UpdateSeconds(GameTime *time)
{
    if (++time->Seconds >= TIME_LENGTH)
    {
        time->Seconds = 0;

        if (++time->Minutes >= TIME_LENGTH)
        {
            time->Minutes = 0;
            
            if (++time->Hours >= TIME_LENGTH_DAY)
            {
                time->Hours = 0;
            }
        }
    }
}
