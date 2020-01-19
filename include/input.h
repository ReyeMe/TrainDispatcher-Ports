#ifndef __INPUT__
#define __INPUT__

#include "platformhacks.h"

static unsigned char active_inputs[8] = {0, 0, 0, 0, 0, 0, 0, 0};

static unsigned char active_inputs_depth = 0;

static unsigned short input_mouse_x = 0;
static unsigned short input_mouse_y = 0;

enum KeyInput {
    KI_SELECT = 1,
    KI_CANCEL = 2,
    KI_ESCAPE = 4,

    /* Debug keys! */
    KI_SHOWHIDE_DEBUG_MENU = 128,
    KI_STOP_TRAIN_SPAWNING = 129,
    KI_FORCE_TRAIN_SPAWN = 130,
};

void push_key_input(char input);

void process_inputs();

#endif
