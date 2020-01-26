#include "WM_render.h"

/* Local function definitions */
static void WM_RenderList(WM_List *list, int offsetX, int offsetY, WM_Rect *rect, int depth);
static void WM_RenderControl(WM_Control *control, int offsetX, int offsetY, WM_Rect *rect, int depth);

/**
 * Some other draw methods using SGL
 */

/**
 * Draw rectangle line
 * @param x1 X coordinate of first point
 * @param y1 Y coordinate of first point
 * @param x2 X coordinate of second point
 * @param y2 Y coordinate of second point
 * @param color Line color
 */
static void WM_DrawLine(int x1, int y1, int x2, int y2, int z, jo_color color)
{
    SPRITE line;
    line.COLR = color;
    line.CTRL = FUNC_Line;
    line.PMOD = Window_In;
    line.XA = x1 - JO_TV_WIDTH_2;
    line.YA = y1 - JO_TV_HEIGHT_2;
    line.XB = x2 - JO_TV_WIDTH_2;
    line.YB = y2 - JO_TV_HEIGHT_2;
    slSetSprite(&line, JO_MULT_BY_65536(z));
}

/**
 * Specific control renders
 */

/**
 * Render window
 * @param control Window data
 * @param offsetX Horizontal offset from global coordinates to top left corner of control
 * @param offsetY Vertical offset from global coordinates to top left corner of control
 * @param rect Real drawing area
 * @param depth Drawing depth
 */
static void WM_RenderTypeWindow(WM_Control *control, int offsetX, int offsetY, WM_Rect* rect, int depth)
{
    int tx = offsetX,
        ty = offsetY,
        right = offsetX + control->Width,
        bottom = offsetY + control->Height;

    // Cap at clipping bounds
    while (tx < rect->Right)
    {
        while (ty < rect->Bottom)
        {
            jo_sprite_draw3D2(tex_cnt_window, tx, ty, depth);
            ty += 32;
        }
        
        tx += 32; 
        ty = offsetY;
    }

    /* Draw borders */
    WM_DrawLine(offsetX, offsetY, right, offsetY, depth, JO_COLOR_White);
    WM_DrawLine(offsetX, offsetY, offsetX, bottom - 1, depth, JO_COLOR_White);
    WM_DrawLine(offsetX + 1, bottom - 1, right, bottom - 1, depth, JO_COLOR_Gray);
    WM_DrawLine(right - 1, offsetY + 1, right - 1, bottom - 1, depth, JO_COLOR_Gray);
}

/**
 * Render label
 * @param control Label data
 * @param offsetX Horizontal offset from global coordinates to top left corner of control
 * @param offsetY Vertical offset from global coordinates to top left corner of control
 * @param depth Drawing depth
 * @param isButton Is label a button
 */
static void WM_RenderLabel(WM_Control *control, int offsetX, int offsetY, int depth, bool isButton)
{
    if (isButton)
    {
        tex_ui_fontButton->z_index = depth;
        jo_font_print(tex_ui_fontButton, offsetX, offsetY, 1.0f, control->Text);
    }
    else
    {
        tex_ui_font->z_index = depth;
        jo_font_print(tex_ui_font, offsetX, offsetY, 1.0f, control->Text);
    }
}

/**
 * Core render logic
 */

/**
 * Render WM control with specified offset 
 * @param control WM control
 * @param offsetX Horizontal offset from global coordinates to top left corner of control
 * @param offsetY Vertical offset from global coordinates to top left corner of control
 * @param rect Draw area
 * @param depth Rendering Z position
 */
static void WM_RenderControl(WM_Control *control, int offsetX, int offsetY, WM_Rect *rect, int depth)
{
    // Set control clipping area
    jo_sprite_set_clipping_area(rect->Left, rect->Top, rect->Right - rect->Left, rect->Bottom - rect->Top, depth);
    jo_sprite_enable_clipping(false);
    
    switch (control->Type)
    {
        case WM_Type_Window:
            WM_RenderTypeWindow(control, offsetX, offsetY, rect, depth);
            break;

        case WM_Type_Button:
            WM_RenderLabel(control, offsetX, offsetY, depth, true);
            break;

        case WM_Type_Label:
            WM_RenderLabel(control, offsetX, offsetY, depth, false);
            break;

        /* Canvas is used for user drawing */
        case WM_Type_Canvas:
        default:
            break;
    }

    /* Call draw update on window, so user can draw custom stuff */
    WM_Rect drawRectangle = { offsetX, offsetY, offsetX + control->Width, offsetY + control->Height };
    WM_CALLPROC(control, WM_ME_Draw, &drawRectangle);

    // Reset clipping
    jo_sprite_disable_clipping();

    // This is here to prevent user clip to fall apart
    jo_sprite_draw3D2(tex_cursor_wait, -50, 0, depth);

    // Render child controls
    WM_RenderList(control->Children, offsetX, offsetY, rect, depth--);
}

/**
 * Check if control is visible
 * @param control Control to check
 * @param offsetX Horizontal offset from global coordinates to top left corner of control
 * @param offsetY Vertical offset from global coordinates to top left corner of control
 * @param rect Drawing rectangle
 */
static bool WM_ControlIsVisible(WM_Control *control, int offsetX, int offsetY, WM_Rect *rect)
{
    return control->Attributes & WM_Att_Visible != 0 &&
        offsetX < rect->Right && 
        offsetY < rect->Bottom &&
        offsetX + control->Width > rect->Left &&
        offsetY + control->Height > rect->Top;
}

/**
 * Render WM list
 * @param list Control list
 * @param offsetX Horizontal offset from global coordinates to top left corner of control
 * @param offsetY Vertical offset from global coordinates to top left corner of control
 * @param rect Drawing rectangle
 * @param depth Rendering Z position
 */
static void WM_RenderList(WM_List *list, int offsetX, int offsetY, WM_Rect *rect, int depth)
{
    WM_List *control = list;
    
    while (control != NULL && control->Current != NULL)
    {
        int originX = offsetX + control->Current->X;
        int originY = offsetY + control->Current->Y;

        if (WM_ControlIsVisible(control->Current, originX, originY, rect))
        {
            // Create drawing rectangle
            WM_Rect areaRectangle = { 
                JO_MAX(rect->Left, originX),
                JO_MAX(rect->Top, originY),
                JO_MIN(rect->Right, originX + control->Current->Width),
                JO_MIN(rect->Bottom, originY + control->Current->Height)
            };

            WM_RenderControl(control->Current, originX, originY, &areaRectangle, depth);
        }

        control = control->Next;
    }
}

/**
 * Render controls
 */
void WM_Render()
{
    // Set clipping bounds, Do not allow user clip to get out of bounds or else hilarity ensues!
    WM_Rect rectangle = { 0, 0, JO_TV_WIDTH - 1, JO_TV_HEIGHT - 1 };
    
    // Draw controls
    WM_RenderList(WM_Desktop, 0, 0, &rectangle, WINDOW_DEPTH_LEVEL);
}
