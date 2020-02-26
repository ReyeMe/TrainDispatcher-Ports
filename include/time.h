#ifndef __TIME_TRAINS__
#define __TIME_TRAINS__

/**
 * Maximum length of seconds or minutes
 */
#define TIME_LENGTH 60

/**
 * Maximum length of hours in day
 */
#define TIME_LENGTH_DAY 24

/**
 * Contains information about current time 
 */
typedef struct
{
    /**
     *  Number of hours 
     */
    unsigned char Hours;

    /**
     * Number of minutes 
     */
    unsigned char Minutes;

    /**
     * Number of seconds 
     */
    unsigned char Seconds;
} GameTime;

/**
 * Current time 
 */
static GameTime *Time_Current;

/**
 * Reset time back to zero
 * @param Item to reset
 */
void Time_Reset(GameTime *time);

/**
 * Move time forward by 1 second
 * @param Item to update
 */
void Time_UpdateSeconds(GameTime *time);

#endif
