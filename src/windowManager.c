#include "../include/windowManager.h"

/**
 * Initialize WM desktop 
 */
void WM_Initialize()
{
    Desktop = p_alloc(sizeof(WM_List));
    Desktop->Prev = NULL;
    Desktop->Current = NULL;
    Desktop->Next = NULL;
}

/**
 * WM control has mouse
 * @param x Position of control on X axis
 * @param y Position of control on Y axis
 * @param width Width of the control
 * @param height Height of the control
 * @param type Type of the control
 * @param parent Parent control
 * @param proc Control message process
 * @returns Pointer to new control
 */
WM_Control * WM_CreateWindow(short x, short y, unsigned short width, unsigned short height, 
                             enum WM_Type type, WM_Control *parent, 
                             void (*proc)(WM_Control *control, enum WM_Message message, void *params))
{
    WM_Control *control = p_alloc(sizeof(WM_Control));

    control->Type = type;
    control->Proc = proc;
    control->Parent = parent;
    control->Children = NULL;

    control->X = x;
    control->Y = y;
    control->Width = width;
    control->Height = height;

    if (parent != NULL)
    {
        /* Add to control */
        WM_List *item = p_alloc(sizeof(WM_List));
        item->Prev = NULL;
        item->Next = NULL;
        item->Current = control;

        if (parent->Children == NULL)
        {
            parent->Children = item;
        }
        else
        {
            WM_List *current = parent->Children;

            while (current->Next != NULL)
            {
                current = current->Next;
            }
            
            current->Next = item;
            item->Prev = current;
        }
    }
    else if (Desktop != NULL)
    {
        /* Add to desktop */
        WM_List *desktop = Desktop;
        
        while (desktop->Current != NULL)
        {
            desktop = desktop->Next;
        }
    }
    else
    {
        p_panic("WM not initialized!", __FUNCTION__);
        p_free(control);
        return NULL;
    }
    
    /* Send create message to control */
    control->Proc(control, WM_ME_Create, NULL);

    return control;
}

/**
 * Destroy control (Used by WM_Destroy, internal only)
 * @param control Control to destroy
 */
static void WM_Internal_Destroy(WM_Control *control)
{
    control->Proc(control, WM_ME_Destroy, NULL);

    WM_List *destroy = control->Children;

    while (destroy != NULL)
    {
        WM_Internal_Destroy(destroy->Current);
        destroy = destroy->Next;

        if (destroy != NULL)
        {
            p_free(destroy->Prev);
        }
    }

    p_free(control->Text);
    p_free(control);
}

/**
 * Destroy WM control
 * @param control Control to destroy 
 */
void WM_Destroy(WM_Control *control)
{
    WM_MEP_GenericMessage handled;
    handled.Handled = false;
    control->Proc(control, WM_ME_Destroy, &handled);

    /* Message has been handeled, do not destroy WM */
    if (handled.Handled)
    {
        return;
    }

    /* Remove from parent */
    if (control->Parent != NULL)
    {
        WM_List *current = control->Parent->Children;

        while (current != NULL && current->Current != control)
        {
            current = current->Next;
        }

        if (current != NULL)
        {
            current->Prev->Next = current->Next;
            p_free(current);
        }
    }

    /* Free children */
    WM_List *destroy = control->Children;

    while (destroy != NULL)
    {
        WM_Internal_Destroy(destroy->Current);
        destroy = destroy->Next;

        if (destroy != NULL)
        {
            p_free(destroy->Prev);
        }
    }

    WM_Internal_Destroy(control);
}

/**
 * Brings control to foreground
 * @param control Control to focus on
 * @returns True if focused
 */
bool WM_FocusControl(WM_Control *control)
{
    WM_List *order = NULL;
    WM_List *found = NULL;

    WM_MEP_GenericMessage handled;
    handled.Handled = false;
    control->Proc(control, WM_ME_Focus, &handled);

    if (handled.Handled)
    {
        return false;
    }

    if (control->Parent == NULL)
    {
        /* Control is on desktop */
        order = Desktop;
    }
    else
    {
        /* Control is in other control */
        order = control->Parent->Children;
        
        if (!WM_FocusControl(control->Parent))
        {
            return false;
        }
    }

    while (order != NULL)
    {
        if (order->Current == control)
        {
            found = order;
        }

        if (order->Next == NULL)
        {
            break;
        }

        order = order->Next;
    }
    
    if (order == NULL)
    {
        p_panic("Control does not belong to any parent!", __FUNCTION__);
        return false;
    }

    if (found != order)
    {
        /* Reorder list */
        found->Prev->Next = found->Next;
        order->Next = found;
        found->Prev = order;
    }

    return true;
}

/**
 * WM control has mouse
 * @param control Pointer to WM control
 * @returns True if mouse is in bounds
 */
bool WM_HasHasMouse(WM_Control *control)
{
    return control->X >= input_mouse_x &&
        control->Y >= input_mouse_y &&
        control->X + control->Width <= input_mouse_x &&
        control->Y + control->Height <= input_mouse_y;
}
