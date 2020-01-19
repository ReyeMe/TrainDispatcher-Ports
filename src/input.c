#include "../include/input.h"

void push_key_input(char input) {
    if (active_inputs_depth >= 7) {
        p_panic("Input buffer overflow", __FUNCTION__);
    }
    active_inputs[active_inputs_depth++] = input;
}

void process_inputs() {
    active_inputs_depth = 0;
    // Do a thing!
}
