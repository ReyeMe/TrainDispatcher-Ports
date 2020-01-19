#ifndef __WM__
#define __WM__

#include "platformhacks.h"
#include "input.h"

/**
 * WM messages
 */
enum WM_Message {
    WM_ME_Empty = 0,    /* NULL parameter */
    WM_ME_Update,
    WM_ME_Focus,        /* WM_MEP_GenericMessage as parameter */
    WM_ME_Create,       /* NULL parameter */
    WM_ME_Destroy,      /* WM_MEP_GenericMessage as parameter, NULL if forced */
    WM_ME_Mouse,
    WM_ME_Key
};

/**
 * WM type
 */
enum WM_Type {
    WM_Type_Window = 0
};

/**
 * Generic WM control
 */
typedef struct WM_Control {
    /* Location */
    short X;
    short Y;
    
    /* Size */
    unsigned short Width;
    unsigned short Height;

    /* Control data and functions */
    char *Text;
    enum WM_Type Type;
    struct WM_List *Children;
    struct WM_Control *Parent;
    void (*Proc)(struct WM_Control *control, enum WM_Message message, void *params);
} WM_Control;

/**
 * List of WM controls
 */
typedef struct WM_List {
    struct WM_List *Prev;
    struct WM_Control *Current;
    struct WM_List *Next;
} WM_List;

/**
 * Generic message parameter
 */
typedef struct {
    bool Handled;
} WM_MEP_GenericMessage;

/**
 * List of control on desktop 
 */
WM_List *Desktop;

/**
 * Initialize WM desktop 
 */
void WM_Initialize();

/**
 * WM control has mouse
 * @param x Position of control on X axis
 * @param y Position of control on Y axis
 * @param width Width of the control
 * @param height Height of the control
 * @param type Type of the control
 * @param parent Parent control
 * @returns Pointer to new control
 */
WM_Control * WM_CreateWindow(short x, short y, unsigned short width, unsigned short height, enum WM_Type type, WM_Control *parent,
                             void (*proc)(WM_Control *control, enum WM_Message message, void *params));

/**
 * Destroy WM control
 * @param control Control to destroy 
 */
void WM_Destroy(WM_Control *control);

/**
 * Brings control to foreground
 * @param control Control to focus on
 * @returns True if focused
 */
bool WM_FocusControl(WM_Control *control);

/**
 * WM control has mouse
 * @param control Pointer to WM control
 * @returns True if mouse is in bounds
 */
bool WM_HasHasMouse(WM_Control *control);

#endif
