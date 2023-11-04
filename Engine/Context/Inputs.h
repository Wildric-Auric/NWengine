#pragma once
#include "Maths.h"

#define INPUT_LEFT_X 0
#define INPUT_LEFT_Y 1
#define INPUT_RIGHT_X 3
#define INPUT_RIGHT_Y 4

typedef int Input_Number;
typedef int Input_Mode;

class Inputs {
public:
	static double mousePosX,
				  mousePosY;
	static bool left,
				right,
				up,
				down,
				d,
				left_click,
				n_0,
				n_1,
				n_2,
				n_3,
				n_4,
				enter,
				f2,
				space,
				usingJoystick;

	static void	 Process(void* window);
	static bool	 GetInputKey(Input_Number key, Input_Mode mode);
	static bool  GetInputMouse(Input_Number key, Input_Mode mode);
	static float joystickAxis[6];
};


#define NW_INPUT_KEY_RELEASE  0
#define NW_INPUT_KEY_PRESS    1
#define NW_INPUT_KEY_REPEAT   2


#define NW_INPUT_KEY_UNKNOWN            -1
#define NW_INPUT_KEY_SPACE              32
#define NW_INPUT_KEY_APOSTROPHE         39  
#define NW_INPUT_KEY_COMMA              44  
#define NW_INPUT_KEY_MINUS              45 
#define NW_INPUT_KEY_PERIOD             46  
#define NW_INPUT_KEY_SLASH              47  
#define NW_INPUT_KEY_0                  48
#define NW_INPUT_KEY_1                  49
#define NW_INPUT_KEY_2                  50
#define NW_INPUT_KEY_3                  51
#define NW_INPUT_KEY_4                  52
#define NW_INPUT_KEY_5                  53
#define NW_INPUT_KEY_6                  54
#define NW_INPUT_KEY_7                  55
#define NW_INPUT_KEY_8                  56
#define NW_INPUT_KEY_9                  57
#define NW_INPUT_KEY_SEMICOLON          59  
#define NW_INPUT_KEY_EQUAL              61  
#define NW_INPUT_KEY_A                  65
#define NW_INPUT_KEY_B                  66
#define NW_INPUT_KEY_C                  67
#define NW_INPUT_KEY_D                  68
#define NW_INPUT_KEY_E                  69
#define NW_INPUT_KEY_F                  70
#define NW_INPUT_KEY_G                  71
#define NW_INPUT_KEY_H                  72
#define NW_INPUT_KEY_I                  73
#define NW_INPUT_KEY_J                  74
#define NW_INPUT_KEY_K                  75
#define NW_INPUT_KEY_L                  76
#define NW_INPUT_KEY_M                  77
#define NW_INPUT_KEY_N                  78
#define NW_INPUT_KEY_O                  79
#define NW_INPUT_KEY_P                  80
#define NW_INPUT_KEY_Q                  81
#define NW_INPUT_KEY_R                  82
#define NW_INPUT_KEY_S                  83
#define NW_INPUT_KEY_T                  84
#define NW_INPUT_KEY_U                  85
#define NW_INPUT_KEY_V                  86
#define NW_INPUT_KEY_W                  87
#define NW_INPUT_KEY_X                  88
#define NW_INPUT_KEY_Y                  89
#define NW_INPUT_KEY_Z                  90
#define NW_INPUT_KEY_LEFT_BRACKET       91
#define NW_INPUT_KEY_BACKSLASH          92  
#define NW_INPUT_KEY_RIGHT_BRACKET      93 
#define NW_INPUT_KEY_GRAVE_ACCENT       96  
#define NW_INPUT_KEY_WORLD_1            161 
#define NW_INPUT_KEY_WORLD_2            162 
#define NW_INPUT_KEY_ESCAPE             256
#define NW_INPUT_KEY_ENTER              257
#define NW_INPUT_KEY_TAB                258
#define NW_INPUT_KEY_BACKSPACE          259
#define NW_INPUT_KEY_INSERT             260
#define NW_INPUT_KEY_DELETE             261
#define NW_INPUT_KEY_RIGHT              262
#define NW_INPUT_KEY_LEFT               263
#define NW_INPUT_KEY_DOWN               264
#define NW_INPUT_KEY_UP                 265
#define NW_INPUT_KEY_PAGE_UP            266
#define NW_INPUT_KEY_PAGE_DOWN          267
#define NW_INPUT_KEY_HOME               268
#define NW_INPUT_KEY_END                269
#define NW_INPUT_KEY_CAPS_LOCK          280
#define NW_INPUT_KEY_SCROLL_LOCK        281
#define NW_INPUT_KEY_NUM_LOCK           282
#define NW_INPUT_KEY_PRINT_SCREEN       283
#define NW_INPUT_KEY_PAUSE              284
#define NW_INPUT_KEY_F1                 290
#define NW_INPUT_KEY_F2                 291
#define NW_INPUT_KEY_F3                 292
#define NW_INPUT_KEY_F4                 293
#define NW_INPUT_KEY_F5                 294
#define NW_INPUT_KEY_F6                 295
#define NW_INPUT_KEY_F7                 296
#define NW_INPUT_KEY_F8                 297
#define NW_INPUT_KEY_F9                 298
#define NW_INPUT_KEY_F10                299
#define NW_INPUT_KEY_F11                300
#define NW_INPUT_KEY_F12                301
#define NW_INPUT_KEY_F13                302
#define NW_INPUT_KEY_F14                303
#define NW_INPUT_KEY_F15                304
#define NW_INPUT_KEY_F16                305
#define NW_INPUT_KEY_F17                306
#define NW_INPUT_KEY_F18                307
#define NW_INPUT_KEY_F19                308
#define NW_INPUT_KEY_F20                309
#define NW_INPUT_KEY_F21                310
#define NW_INPUT_KEY_F22                311
#define NW_INPUT_KEY_F23                312
#define NW_INPUT_KEY_F24                313
#define NW_INPUT_KEY_F25                314
#define NW_INPUT_KEY_KP_0               320
#define NW_INPUT_KEY_KP_1               321
#define NW_INPUT_KEY_KP_2               322
#define NW_INPUT_KEY_KP_3               323
#define NW_INPUT_KEY_KP_4               324
#define NW_INPUT_KEY_KP_5               325
#define NW_INPUT_KEY_KP_6               326
#define NW_INPUT_KEY_KP_7               327
#define NW_INPUT_KEY_KP_8               328
#define NW_INPUT_KEY_KP_9               329
#define NW_INPUT_KEY_KP_DECIMAL         330
#define NW_INPUT_KEY_KP_DIVIDE          331
#define NW_INPUT_KEY_KP_MULTIPLY        332
#define NW_INPUT_KEY_KP_SUBTRACT        333
#define NW_INPUT_KEY_KP_ADD             334
#define NW_INPUT_KEY_KP_ENTER           335
#define NW_INPUT_KEY_KP_EQUAL           336
#define NW_INPUT_KEY_LEFT_SHIFT         340
#define NW_INPUT_KEY_LEFT_CONTROL       341
#define NW_INPUT_KEY_LEFT_ALT           342
#define NW_INPUT_KEY_LEFT_SUPER         343
#define NW_INPUT_KEY_RIGHT_SHIFT        344
#define NW_INPUT_KEY_RIGHT_CONTROL      345
#define NW_INPUT_KEY_RIGHT_ALT          346
#define NW_INPUT_KEY_RIGHT_SUPER        347
#define NW_INPUT_KEY_MENU               348
#define NW_INPUT_KEY_LAST               NW_INPUT_KEY_MENU

#define NW_INPUT_MOUSE_BUTTON_1			    0
#define NW_INPUT_MOUSE_BUTTON_2				1
#define NW_INPUT_MOUSE_BUTTON_3				2
#define NW_INPUT_MOUSE_BUTTON_4				3
#define NW_INPUT_MOUSE_BUTTON_5				4
#define NW_INPUT_MOUSE_BUTTON_6				5
#define NW_INPUT_MOUSE_BUTTON_7				6
#define NW_INPUT_MOUSE_BUTTON_8				7
#define NW_INPUT_MOUSE_BUTTON_LAST      NW_INPUT_MOUSE_BUTTON_8
#define NW_INPUT_MOUSE_BUTTON_LEFT      NW_INPUT_MOUSE_BUTTON_1
#define NW_INPUT_MOUSE_BUTTON_RIGHT     NW_INPUT_MOUSE_BUTTON_2
#define NW_INPUT_MOUSE_BUTTON_MIDDLE    NW_INPUT_MOUSE_BUTTON_3