#ifndef _WYN_H
#define _WYN_H
#include <stdint.h>

#ifndef __cplusplus
#else
extern "C" {
#endif
typedef unsigned long Word;
typedef unsigned char uchar;
typedef unsigned int  uint;
typedef uint64_t	  ui64;
typedef unsigned char boolean;

#define WYN_RESIZE_CBK_IDX 0
#define WYN_UPDATE_CBK_IDX 1

typedef struct {
	int x;
	int y;
} wyn_vec2;

typedef struct {
	wyn_vec2 pos;
	wyn_vec2 size;
} wyn_rect;

typedef struct {
	boolean should_close;
    boolean valid;
} wyn_state;

enum wyn_key_id {
    Wyn_Key_RMouse = 511,    
    Wyn_Key_MMouse = 510,
    Wyn_Key_LMouse = 509,    
    Wyn_Key_FMouse = 508,    
    Wyn_Key_BMouse = 507,    
    Wyn_Key_LCtrl  = 500,
    Wyn_Key_RCtrl  = 501,
    Wyn_Key_LShift = 502,
    Wyn_Key_RShift = 503,
    Wyn_Key_LAlt   = 504,
    Wyn_Key_RAlt   = 505,
    Wyn_Key_RArrow = 499,
    Wyn_Key_LArrow = 498,
    Wyn_Key_UArrow = 497,
    Wyn_Key_DArrow = 496,
    Wyn_Key_Ret    = 495,
    Wyn_Key_Esc    = 494,
    Wyn_Key_Del    = 493,
    Wyn_Key_Space  = 492,
};

enum wyn_event_mask {
	Wyn_KeyPressed	   = 1 << 0,
	Wyn_OnKeyRelease   = 1 << 1,
	Wyn_OnKeyPress	   = 1 << 2,
	Wyn_LastUpdate	   = 1 << 3,
	Wyn_OnKeyPressLock = 1 << 4
};

typedef struct {
	int events[512];
	int update_list[512];
	int update_idx;
} wyn_kboard;

typedef struct {
	ui64	   handle;
	void*	   extra;
	wyn_state  state;
	wyn_kboard keyboard;
	void*	   _callbacks[16];
	void*	   _callbakcs_usr_data[16];
} wyndow;

typedef struct {
	char*	 desc;
	wyn_rect rect;
} wyn_crt_info;

typedef struct {
	wyndow* owner;
	void*	handle;
} wyn_glctx;

typedef struct {
	int	 major;
	int	 minor;
	boolean compatibilityProfile;
} wyn_glctx_crt_info;

typedef void (*wyn_rz_callback_proc)(wyndow* w, wyn_vec2* new_size, void* usr_data);

#ifdef PLTFRM_WIN32
typedef enum { WYN_DWMWCP_DEFAULT = 0, WYN_DWMWCP_DONOTROUND = 1, WYN_DWMWCP_ROUND = 2, WYN_DWMWCP_ROUNDSMALL = 3 } WYN_DWM_WINDOW_CORNER_PREFERENCE;
int wyn_win32_create(wyndow*, wyn_crt_info*);
int wyn_win32_destroy(wyndow*);
#endif

#ifdef PLTFRM_LINUX
int wyn_linux_create(wyndow*, wyn_crt_info*);
int wyn_linux_destroy(wyndow* w);
#endif

extern int (*wyn_create)(wyndow*, wyn_crt_info*);
extern int (*wyn_destroy)(wyndow* w);
extern int (*wyn_show)(wyndow* w, boolean flag);
extern int (*wyn_set_title)(wyndow* w, const char* desc);
extern int (*wyn_get_metrics)(wyndow* w, wyn_vec2* out_size, wyn_vec2* out_pos);
extern int (*wyn_update)(wyndow* w);
extern int (*wyn_swap)(wyndow* w);
extern int (*wyn_glctx_create)(wyndow* w, wyn_glctx* glc, wyn_glctx_crt_info* crt_info);
extern void (*wyn_glctx_make_current)(wyndow* w, wyn_glctx* glc);
extern void (*wyn_glctx_destroy)(wyn_glctx* glc);
extern void (*wyn_set_vsync)(wyndow* w, int interval);
extern boolean (*wyn_key_pressed)(wyndow* w, int key);
extern boolean (*wyn_on_key_press)(wyndow* w, int key);
extern boolean (*wyn_on_key_release)(wyndow* w, int key);
extern boolean (*wyn_get_mouse_pos)(wyndow* w, wyn_vec2* pos);
void wyn_rz_cbk_reg(wyndow* w, wyn_rz_callback_proc, void* usr_data);
void wyn_set_callback(wyndow* w, int cbk_ind, void* proc, void* usr_data);
#ifdef __cplusplus
}
#endif

#endif
