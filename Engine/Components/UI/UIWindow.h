#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "ds.h"

#define UI_ITEM_HEAP_UNIT_SIZE 512
#define UNWRP_COL(col)		   (col).x, (col).y, (col).z, (col).a

class UIManager;

struct UIColorScheme {
	v4f bg;
	v4f fg;
	v4f win;
	v4f winRest;
	v4f winHover;
	v4f winSelect;
	v4f winBar;
	v4f winBrdr;
	v4f winBrdrResize;
	v4f text;
	v4f titleText;
};

extern UIColorScheme uiColorSchemePreset_Test;
extern UIColorScheme uiColorSchemePreset_Light;
extern UIColorScheme uiColorSchemePreset_Dark;

extern UIColorScheme currentUIColorScheme;

#define SetBitFieldVal(token, off) token = 0x1 << off
enum UIWindowState : i32 {
	SetBitFieldVal(Window_State_MOVE_X, 1),
	SetBitFieldVal(Window_State_MOVE_Y, 2),
	SetBitFieldVal(Window_State_RESIZE_X, 3),
	SetBitFieldVal(Window_State_RESIZE_Y, 4),
};

enum WindowProp : i32 {
	SetBitFieldVal(Window_Prop_ResizableX, 1),
	SetBitFieldVal(Window_Prop_ResizableY, 2),
	SetBitFieldVal(Window_Prop_MovableX, 3),
	SetBitFieldVal(Window_Prop_MovableY, 4),
};

enum ItemProp : i32 { SetBitFieldVal(Item_Prop_Selectable, 1) };

enum UIItemState : i32 { SetBitFieldVal(Item_State_Selected, 1) };

#undef SetBitFieldVal

enum class CurAdvanceStrat { None, BreakOnHorizontalEnd, FixedWidth };

enum UIItemType {
	UIItemType_None,
	UIItemType_Title,
	UIItemType_Label,
	UIItemType_TestZone,
	UIItemType_Slider,
	UIItemType_Checkbox
};

class UIItem;

typedef UIItem UIItemLabel;
typedef UIItem UIItemTestZone;
typedef i32	   UIItemProp;
typedef i32	   UIWindowProp;
struct UIWindowMetrics {
	int	  titleBarHeight  = 20;
	int	  resizeAreaWidth = 5;
	int	  borderWidth	  = 2;
	iVec2 itemSpacing	  = fVec2(3, 3);
	iVec2 minSize		  = iVec2(20, 20);
};

class UIWindow;
class UIItem {
  public:
	UIItemType	 type;
	UIWindowProp prop;
	GameObject	 obj;
	int8		 endln = 0;
	UIWindow*	 _owner;
	void*		 data = 0;
	UIItemType	 GetType();
	int64		 GetLayer();
	inline v2f	 GetSize() { return _GetSizeProc(this); };
	inline int	 Update() { return _UpdateProc(this); }
	inline int	 LateUpdate() { return _LateUpdateProc(this); }
	inline void	 Draw() { _DrawProc(this); };
	int64 (*_GetLayerProc)(UIItem*) = DefaultUIItemGetLayerProc;
	v2f (*_GetSizeProc)(UIItem*)	= DefaultUIItemGetSizeProc;
	int (*_UpdateProc)(UIItem*)		= [](UIItem*) -> int { return 0; };
	int (*_LateUpdateProc)(UIItem*) = [](UIItem* it) -> int { return it->endln; };
	void (*_DrawProc)(UIItem*)		= [](UIItem* iter) { iter->obj.Draw(); };
	std::list<UIItem>::iterator _iter;

	static int64 DefaultUIItemGetLayerProc(UIItem*);
	static v2f	 DefaultUIItemGetSizeProc(UIItem*);
};

class UIWindow;
class UICursor {
  public:
	UICursor() = default;
	UICursor(UIWindow*);
	fVec2			origin		  = 0;
	fVec2			pos			  = fVec2(0.0, 0.0);
	float			fixedWidth	  = 200.0f;
	float			lineBreakSize = 0.0;
	UIWindow*		win;
	CurAdvanceStrat strat;
	void			SetWindow(UIWindow*);
	void			SetPos(const fVec2&);
	void			SetCursorTopLeftWin();
	void			SetCursorOnNextLineBeg();
	void			Advance(const fVec2&);
	void			SetLineBreakSize(const float);
	fVec2			GetAbsolutePos();
	fVec2*			GetPos();

	bool CalcNextPosition(const fVec2 offset);
	void CalcAdvanceBr();
};

class UIWindow : public GameComponent {
  public:
	NW_ST_GET_TYPE_IMPL(UIWindow);
	void OnAdd() override;
	void OnDelete() override;
	UIWindow() = default;
	UIWindow(GameObject*);

	void Update() override;

	i32	  ComputeResizeState();
	i32	  ComputeMoveState();
	int	  IsCursorOnTitleBar();
	int	  IsCursorOnWindow();
	int	  IsCursorOnResize();
	bool  IsFocused();
	i32	  GetState();
	fVec2 GetSize();
	fVec2 GetPosition();
	void  SetShaderParams();
	void  SetTitle(const char* str);
	int64 GetLayer();
	int64 GetUIItemLayer(int64 relative);

	UIItem* _PushItem(UIItemType, int64);
	UIItem* _SetUpItem(UIItem*, UIItemType, int64);
	UIItem* AddItem(UIItemType, int64);
	UIItem* AddItem(UIItemType type, int64 layer, uint8 endln);
	void	DrawItems();

	void					   SetPosition(const fVec2&);
	void					   SetSize(const fVec2&);
	inline UICursor*		   GetCursor() { return &cursor; }
	inline const UIWindowProp& GetProp() { return prop; }
	inline UIWindowProp*	   GetPropRef() { return &prop; }

	i32			 state = 0; // UIWindowState
	UIWindowProp prop  = Window_Prop_MovableX | Window_Prop_MovableY | Window_Prop_ResizableX | Window_Prop_ResizableY;

	v2f relPos;
	v2f rpos;
	v2f lsize;
	v2f lpos;
	v2f lwinPos;

	v2f lPosItemBfSelect;

	MemoryRegion itemsHeap;
	UICursor	 cursor		  = UICursor(this);
	UIItem*		 hoveredItem  = 0;
	UIItem*		 selectedItem = 0;
	UIItem*		 draggedItem  = 0;
	UIItem*		 clickedItem  = 0;

	bool _tmpisFocused = 0;

	UIWindowMetrics		 metrics;
	std::list<UIItem>	 items;
	std::vector<UIItem*> itemsOrd;

	UIManager* attachedUIManager = 0;

	static bool CacheConditionHasUIWindow(GameObject* obj);
	static int	UIWindowDrawCallback(void*);
};

inline const float& GetSliderPosX(UIItem* it) { return READ(float, it->data); }
inline void			SetSliderPosX(UIItem* it, float val) { READ(float, it->data) = val; }
inline const bool&	GetCheckBoxState(UIItem* it) { return READ(bool, it->data); }
inline void			SetCheckBoxState(UIItem* it, float val) { READ(bool, it->data) = val; }

//-------------------UIManager-------------------

class UIManager : public GameComponent {
  public:
	GameObject _camContainer;
	Camera*	   _lastCam = 0;

	Camera* GetCamera();
	Camera* GetTmpCamera();
	void	OnAdd() override;
	void	Update() override;
	void	Bind();
	void	Unbind();
};
