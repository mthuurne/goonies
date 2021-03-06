#ifndef _THEGOONIES_CONSTANTS
#define _THEGOONIES_CONSTANTS

#if defined(PANDORA)
#define SCREEN_X    800
#define SCREEN_Y    480
#else
#define SCREEN_X    640
#define SCREEN_Y    480
#endif
#if defined(GCW)
#define VIEWPORT_W  320
#define VIEWPORT_H  240
#else
#define VIEWPORT_W  SCREEN_X
#define VIEWPORT_H  SCREEN_Y
#endif

#define PLAYER_MAX_ENERGY  64
#define PLAYER_MAX_EXPERIENCE 16
#define PLAYER_CRITICAL_ENERGY 8
#define PLAYER_SPEED   (1.6f)
#define PLAYER_CLIMBING_SPEED (1.6f)
#define BIGROCK_SPEED (0.9f)
#define SKULL_SPEED    (2.0f)
#define SKELETON_SPEED   (2.0f)
#define BONE_SPEED    (2.75f)
#define BAT_SPEED    (1.5f)
#define COIN_SPEED    (1.0f)

#define YELLOW_FRATELLI_SPEED (1.75f)
#define BLUE_FRATELLI_SPEED  (2.00f)
#define GREEN_FRATELLI_SPEED (2.5f)
#define RED_FRATELLI_SPEED  (2.25f)
#define GREY_FRATELLI_SPEED  (1.5f)
#define WHITE_FRATELLI_SPEED (1.75f)
#define BULLET_SPEED   (4.0f)
#define MUSICALNOTE_SPEED  (4.0f)

#endif

