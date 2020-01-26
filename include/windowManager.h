#ifndef __WM__
#define __WM__

#include "platformhacks.h"
#include "input.h"

/**
 * Use this to call control message procesor instead of calling it directly
 * @param C Target control
 * @param M Message to send
 * @param P Message parameter
 */
#define WM_CALLPROC(C,M,P) if(C->Proc != NULL) C->Proc(C,M,P)

/**
 * WM messages
 */
typedef enum {
    WM_ME_Invalid = 0,  /* NULL parameter, if control recieves this message, something got horribly broken */
    WM_ME_Draw,         /* Called at the end of drawing control (will not get called if control is not visible). WM_Rect in global coordinates as parameter */
    WM_ME_Focus,        /* Called before focus is recieved. WM_MEP_GenericMessage as parameter */
    WM_ME_Create,       /* Called after control is created and added to child tree. NULL parameter */
    WM_ME_Destroy,      /* Called before control is destroyed. WM_MEP_GenericMessage as parameter, NULL if forced */
    WM_ME_MouseEnter,
    WM_ME_MouseMove,
    WM_ME_MouseLeave,
    WM_ME_Mouse,
    WM_ME_KeyPressed
} WM_Message;

/**
 * WM type
 */
typedef enum {
    WM_Type_Window = 0,
    WM_Type_Button,
    WM_Type_Canvas,
    WM_Type_Label
} WM_Type;

/**
 * WM type
 */
typedef enum {
    WM_Att_Visible = 1,
    WM_Att_InputEnabled = 2,
} WM_Attributes;

/**
 * WM rectangle ()
 */
typedef struct {
    short Left;
    short Top;
    short Right;
    short Bottom;
} WM_Rect;

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
    WM_Attributes Attributes;
    char *Text;
    WM_Type Type;
    struct WM_List *Children;
    struct WM_Control *Parent;
    void (*Proc)(struct WM_Control *control, WM_Message message, void *params);
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
WM_List *WM_Desktop;

/**
 * Initialize WM desktop 
 */
void WM_Initialize();

/**
 * Create WM control
 * @param x Position of control on X axis
 * @param y Position of control on Y axis
 * @param width Width of the control
 * @param height Height of the control
 * @param type Type of the control
 * @param parent Parent control
 * @returns Pointer to new control
 */
WM_Control * WM_CreateWidget(short x, short y, unsigned short width, unsigned short height, WM_Type type, WM_Control *parent,
                             void (*proc)(WM_Control *control, WM_Message message, void *params));

/**
 * Destroy WM control
 * @param control Control to destroy 
 */
void WM_Destroy(WM_Control *control);

/**
 * TODO: Implement function to process mouse and button input
 */
void WM_ProcessInput();

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
