#ifndef __TEXTURES__
#define __TEXTURES__


/**
 * UI textures
 */
jo_font *Tex_UIFont;
int Tex_ControllerError;

/**
 * Rail tile sprite indexes
 */
#define TILE_STRAIGHT_ROTATION 2
int Tex_TileStraightHorizontal;
int Tex_TileStraightVertical;
int Tex_TileStraightDiagonalLeft;
int Tex_TileStraightDiagonalRight;

#define TILE_TURN_ROTATION 4
int Tex_TileTurnLeft;
int Tex_TileTurnUp;
int Tex_TileTurnRight;
int Tex_TileTurnDown;
int Tex_TileTurnLeftMirror;
int Tex_TileTurnUpMirror;
int Tex_TileTurnRightMirror;
int Tex_TileTurnDownMirror;

/**
 * Special rail tile sprite indexes
 */
#define TILE_SPECIAL_ROTATION 4
int Tex_TileDepoLeft;
int Tex_TileDepoUp;
int Tex_TileDepoRight;
int Tex_TileDepoDown;

int Tex_TileSignalEnabledLeft;
int Tex_TileSignalEnabledUp;
int Tex_TileSignalEnabledRight;
int Tex_TileSignalEnabledDown;

int Tex_TileSignalDisabledLeft;
int Tex_TileSignalDisabledUp;
int Tex_TileSignalDisabledRight;
int Tex_TileSignalDisabledDown;

#endif
