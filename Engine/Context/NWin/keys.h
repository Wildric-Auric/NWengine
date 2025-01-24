#pragma once
namespace NWin {
enum Key {
 NWIN_KEY_LBUTTON        =0x01,
 NWIN_KEY_RBUTTON        =0x02,
 NWIN_KEY_CANCEL         =0x03,
 NWIN_KEY_MBUTTON        =0x04,    /* NOT contiguous with L & RBUTTON */
 NWIN_KEY_XBUTTON1       =0x05,    /* NOT contiguous with L & RBUTTON */
 NWIN_KEY_XBUTTON2       =0x06,    /* NOT contiguous with L & RBUTTON */
 NWIN_KEY_BACK           =0x08,
 NWIN_KEY_TAB            =0x09,
 NWIN_KEY_CLEAR          =0x0C,
 NWIN_KEY_RETURN         =0x0D,

 NWIN_KEY_SHIFT          =0x10,
 NWIN_KEY_CONTROL        =0x11,
 NWIN_KEY_MENU           =0x12,
 NWIN_KEY_PAUSE          =0x13,
 NWIN_KEY_CAPITAL        =0x14,

 NWIN_KEY_KANA           =0x15,
 NWIN_KEY_HANGEUL        =0x15,  /* old name - should be here for compatibility */
 NWIN_KEY_HANGUL         =0x15,
 NWIN_KEY_IME_ON         =0x16,
 NWIN_KEY_JUNJA          =0x17,
 NWIN_KEY_FINAL          =0x18,
 NWIN_KEY_HANJA          =0x19,
 NWIN_KEY_KANJI          =0x19,
 NWIN_KEY_IME_OFF        =0x1A,

 NWIN_KEY_ESCAPE         =0x1B,

 NWIN_KEY_CONVERT        =0x1C,
 NWIN_KEY_NONCONVERT     =0x1D,
 NWIN_KEY_ACCEPT         =0x1E,
 NWIN_KEY_MODECHANGE     =0x1F,

 NWIN_KEY_SPACE          =0x20,
 NWIN_KEY_PRIOR          =0x21,
 NWIN_KEY_NEXT           =0x22,
 NWIN_KEY_END            =0x23,
 NWIN_KEY_HOME           =0x24,
 NWIN_KEY_LEFT           =0x25,
 NWIN_KEY_UP             =0x26,
 NWIN_KEY_RIGHT          =0x27,
 NWIN_KEY_DOWN           =0x28,
 NWIN_KEY_SELECT         =0x29,
 NWIN_KEY_PRINT          =0x2A,
 NWIN_KEY_EXECUTE        =0x2B,
 NWIN_KEY_SNAPSHOT       =0x2C,
 NWIN_KEY_INSERT         =0x2D,
 NWIN_KEY_DELETE         =0x2E,
 NWIN_KEY_HELP           =0x2F,

   /*
	* NWIN_KEY_0 - NWIN_KEY_9 are the same as ASCII '0' - '9' (=0x30 - =0x39)
	* =0x3A - =0x40 : unassigned
	* NWIN_KEY_A - NWIN_KEY_Z are the same as ASCII 'A' - 'Z' (=0x41 - =0x5A)
	*/

 NWIN_KEY_LWIN           =0x5B,
 NWIN_KEY_RWIN           =0x5C,
 NWIN_KEY_APPS           =0x5D,

	/*
	 * =0x5E : reserved
	 */

 NWIN_KEY_SLEEP          =0x5F,

 NWIN_KEY_NUMPAD0        =0x60,
 NWIN_KEY_NUMPAD1        =0x61,
 NWIN_KEY_NUMPAD2        =0x62,
 NWIN_KEY_NUMPAD3        =0x63,
 NWIN_KEY_NUMPAD4        =0x64,
 NWIN_KEY_NUMPAD5        =0x65,
 NWIN_KEY_NUMPAD6        =0x66,
 NWIN_KEY_NUMPAD7        =0x67,
 NWIN_KEY_NUMPAD8        =0x68,
 NWIN_KEY_NUMPAD9        =0x69,
 NWIN_KEY_MULTIPLY       =0x6A,
 NWIN_KEY_ADD            =0x6B,
 NWIN_KEY_SEPARATOR      =0x6C,
 NWIN_KEY_SUBTRACT       =0x6D,
 NWIN_KEY_DECIMAL        =0x6E,
 NWIN_KEY_DIVIDE         =0x6F,
 NWIN_KEY_F1             =0x70,
 NWIN_KEY_F2             =0x71,
 NWIN_KEY_F3             =0x72,
 NWIN_KEY_F4             =0x73,
 NWIN_KEY_F5             =0x74,
 NWIN_KEY_F6             =0x75,
 NWIN_KEY_F7             =0x76,
 NWIN_KEY_F8             =0x77,
 NWIN_KEY_F9             =0x78,
 NWIN_KEY_F10            =0x79,
 NWIN_KEY_F11            =0x7A,
 NWIN_KEY_F12            =0x7B,
 NWIN_KEY_F13            =0x7C,
 NWIN_KEY_F14            =0x7D,
 NWIN_KEY_F15            =0x7E,
 NWIN_KEY_F16            =0x7F,
 NWIN_KEY_F17            =0x80,
 NWIN_KEY_F18            =0x81,
 NWIN_KEY_F19            =0x82,
 NWIN_KEY_F20            =0x83,
 NWIN_KEY_F21            =0x84,
 NWIN_KEY_F22            =0x85,
 NWIN_KEY_F23            =0x86,
 NWIN_KEY_F24            =0x87,

	 /*
	  * =0x88 - =0x8F : UI navigation
	  */

 NWIN_KEY_NAVIGATION_VIEW     =0x88, // reserved
 NWIN_KEY_NAVIGATION_MENU     =0x89, // reserved
 NWIN_KEY_NAVIGATION_UP       =0x8A, // reserved
 NWIN_KEY_NAVIGATION_DOWN     =0x8B, // reserved
 NWIN_KEY_NAVIGATION_LEFT     =0x8C, // reserved
 NWIN_KEY_NAVIGATION_RIGHT    =0x8D, // reserved
 NWIN_KEY_NAVIGATION_ACCEPT   =0x8E, // reserved
 NWIN_KEY_NAVIGATION_CANCEL   =0x8F, // reserved

 NWIN_KEY_NUMLOCK        =0x90,
 NWIN_KEY_SCROLL         =0x91,

	  /*
	   * NEC PC-9800 kbd definitions
	   */
 NWIN_KEY_OEM_NEC_EQUAL  =0x92,   // '=' key on numpad

	   /*
		* Fujitsu/OASYS kbd definitions
		*/
 NWIN_KEY_OEM_FJ_JISHO   =0x92,   // 'Dictionary' key
 NWIN_KEY_OEM_FJ_MASSHOU =0x93,   // 'Unregister word' key
 NWIN_KEY_OEM_FJ_TOUROKU =0x94,   // 'Register word' key
 NWIN_KEY_OEM_FJ_LOYA    =0x95,   // 'Left OYAYUBI' key
 NWIN_KEY_OEM_FJ_ROYA    =0x96,   // 'Right OYAYUBI' key

		/*
		 * =0x97 - =0x9F : unassigned
		 */

		 /*
		  * NWIN_KEY_L* & NWIN_KEY_R* - left and right Alt, Ctrl and Shift virtual keys.
		  * Used only as parameters to GetAsyncKeyState() and GetKeyState().
		  * No other API or message will distinguish left and right keys in this way.
		  */
 NWIN_KEY_LSHIFT         =0xA0,
 NWIN_KEY_RSHIFT         =0xA1,
 NWIN_KEY_LCONTROL       =0xA2,
 NWIN_KEY_RCONTROL       =0xA3,
 NWIN_KEY_LMENU          =0xA4,
 NWIN_KEY_RMENU          =0xA5,


 NWIN_KEY_BROWSER_BACK        =0xA6,
 NWIN_KEY_BROWSER_FORWARD     =0xA7,
 NWIN_KEY_BROWSER_REFRESH     =0xA8,
 NWIN_KEY_BROWSER_STOP        =0xA9,
 NWIN_KEY_BROWSER_SEARCH      =0xAA,
 NWIN_KEY_BROWSER_FAVORITES   =0xAB,
 NWIN_KEY_BROWSER_HOME        =0xAC,

 NWIN_KEY_VOLUME_MUTE         =0xAD,
 NWIN_KEY_VOLUME_DOWN         =0xAE,
 NWIN_KEY_VOLUME_UP           =0xAF,
 NWIN_KEY_MEDIA_NEXT_TRACK    =0xB0,
 NWIN_KEY_MEDIA_PREV_TRACK    =0xB1,
 NWIN_KEY_MEDIA_STOP          =0xB2,
 NWIN_KEY_MEDIA_PLAY_PAUSE    =0xB3,
 NWIN_KEY_LAUNCH_MAIL         =0xB4,
 NWIN_KEY_LAUNCH_MEDIA_SELECT =0xB5,
 NWIN_KEY_LAUNCH_APP1         =0xB6,
 NWIN_KEY_LAUNCH_APP2         =0xB7,

		  /*
		   * =0xB8 - =0xB9 : reserved
		   */

 NWIN_KEY_OEM_1          =0xBA,   // ';:' for US
 NWIN_KEY_OEM_PLUS       =0xBB,   // '+' any country
 NWIN_KEY_OEM_COMMA      =0xBC,   // ',' any country
 NWIN_KEY_OEM_MINUS      =0xBD,   // '-' any country
 NWIN_KEY_OEM_PERIOD     =0xBE,   // '.' any country
 NWIN_KEY_OEM_2          =0xBF,   // '/?' for US
 NWIN_KEY_OEM_3          =0xC0,   // '`~' for US

		   /*
			* =0xC1 - =0xC2 : reserved
			*/

			/*
			 * =0xC3 - =0xDA : Gamepad input
			 */

 NWIN_KEY_GAMEPAD_A                         =0xC3, // reserved
 NWIN_KEY_GAMEPAD_B                         =0xC4, // reserved
 NWIN_KEY_GAMEPAD_X                         =0xC5, // reserved
 NWIN_KEY_GAMEPAD_Y                         =0xC6, // reserved
 NWIN_KEY_GAMEPAD_RIGHT_SHOULDER            =0xC7, // reserved
 NWIN_KEY_GAMEPAD_LEFT_SHOULDER             =0xC8, // reserved
 NWIN_KEY_GAMEPAD_LEFT_TRIGGER              =0xC9, // reserved
 NWIN_KEY_GAMEPAD_RIGHT_TRIGGER             =0xCA, // reserved
 NWIN_KEY_GAMEPAD_DPAD_UP                   =0xCB, // reserved
 NWIN_KEY_GAMEPAD_DPAD_DOWN                 =0xCC, // reserved
 NWIN_KEY_GAMEPAD_DPAD_LEFT                 =0xCD, // reserved
 NWIN_KEY_GAMEPAD_DPAD_RIGHT                =0xCE, // reserved
 NWIN_KEY_GAMEPAD_MENU                      =0xCF, // reserved
 NWIN_KEY_GAMEPAD_VIEW                      =0xD0, // reserved
 NWIN_KEY_GAMEPAD_LEFT_THUMBSTICK_BUTTON    =0xD1, // reserved
 NWIN_KEY_GAMEPAD_RIGHT_THUMBSTICK_BUTTON   =0xD2, // reserved
 NWIN_KEY_GAMEPAD_LEFT_THUMBSTICK_UP        =0xD3, // reserved
 NWIN_KEY_GAMEPAD_LEFT_THUMBSTICK_DOWN      =0xD4, // reserved
 NWIN_KEY_GAMEPAD_LEFT_THUMBSTICK_RIGHT     =0xD5, // reserved
 NWIN_KEY_GAMEPAD_LEFT_THUMBSTICK_LEFT      =0xD6, // reserved
 NWIN_KEY_GAMEPAD_RIGHT_THUMBSTICK_UP       =0xD7, // reserved
 NWIN_KEY_GAMEPAD_RIGHT_THUMBSTICK_DOWN     =0xD8, // reserved
 NWIN_KEY_GAMEPAD_RIGHT_THUMBSTICK_RIGHT    =0xD9, // reserved
 NWIN_KEY_GAMEPAD_RIGHT_THUMBSTICK_LEFT     =0xDA, // reserved


 NWIN_KEY_OEM_4          =0xDB,  //  '[{' for US
 NWIN_KEY_OEM_5          =0xDC,  //  '\|' for US
 NWIN_KEY_OEM_6          =0xDD,  //  ']}' for US
 NWIN_KEY_OEM_7          =0xDE,  //  ''"' for US
 NWIN_KEY_OEM_8          =0xDF,

			 /*
			  * =0xE0 : reserved
			  */

			  /*
			   * Various extended or enhanced keyboards
			   */
 NWIN_KEY_OEM_AX         =0xE1,  //  'AX' key on Japanese AX kbd
 NWIN_KEY_OEM_102        =0xE2,  //  "<>" or "\|" on RT 102-key kbd.
 NWIN_KEY_ICO_HELP       =0xE3,  //  Help key on ICO
 NWIN_KEY_ICO_00         =0xE4,  //  00 key on ICO

 NWIN_KEY_PROCESSKEY     =0xE5,

 NWIN_KEY_ICO_CLEAR      =0xE6,


 NWIN_KEY_PACKET         =0xE7,
			   /*
				* =0xE8 : unassigned
				*/

				/*
				 * Nokia/Ericsson definitions
				 */
 NWIN_KEY_OEM_RESET      =0xE9,
 NWIN_KEY_OEM_JUMP       =0xEA,
 NWIN_KEY_OEM_PA1        =0xEB,
 NWIN_KEY_OEM_PA2        =0xEC,
 NWIN_KEY_OEM_PA3        =0xED,
 NWIN_KEY_OEM_WSCTRL     =0xEE,
 NWIN_KEY_OEM_CUSEL      =0xEF,
 NWIN_KEY_OEM_ATTN       =0xF0,
 NWIN_KEY_OEM_FINISH     =0xF1,
 NWIN_KEY_OEM_COPY       =0xF2,
 NWIN_KEY_OEM_AUTO       =0xF3,
 NWIN_KEY_OEM_ENLW       =0xF4,
 NWIN_KEY_OEM_BACKTAB    =0xF5,

 NWIN_KEY_ATTN           =0xF6,
 NWIN_KEY_CRSEL          =0xF7,
 NWIN_KEY_EXSEL          =0xF8,
 NWIN_KEY_EREOF          =0xF9,
 NWIN_KEY_PLAY           =0xFA,
 NWIN_KEY_ZOOM           =0xFB,
 NWIN_KEY_NONAME         =0xFC,
 NWIN_KEY_PA1            =0xFD,
 NWIN_KEY_OEM_CLEAR      =0xFE

	};
};