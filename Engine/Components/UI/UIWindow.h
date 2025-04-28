#pragma once
#include "GameObject.h"

class UIManager;

enum class UIWindowState {
    NONE,
    MOVE,
    RESIZE,
    RESIZE_X,
    RESIZE_Y,
};

struct UIWindowMetrics {
    int titleBarHeight = 20;
    int resizeAreaWidth = 5;
    int borderWidth     = 2;
    iVec2 itemSpacing   = fVec2(3,3);
    iVec2 minSize       = iVec2(20,20);
};

enum class UIItemType {
    NONE,
    TITLE,
    TEST_ZONE
};

class UIItem {
    public:
    UIItemType type;
    GameObject obj;
    UIItemType GetType();
    int64 GetLayer();
    void _SetUp(UIItemType type, int64 layer, std::list<UIItem>::iterator);
    int64 (*_GetLayerProc)(UIItem*) = DefaultUIItemGetLayerProc; 
    std::list<UIItem>::iterator _iter;

    static int64 (*DefaultUIItemGetLayerProc)(UIItem*);
};

class UIWindow : public GameComponent {
    public:
    NW_ST_GET_TYPE_IMPL(UIWindow);
    void OnAdd() override;
    void OnDelete() override;
    UIWindow() = default;
    UIWindow(GameObject*);

    void Update() override;

    int IsCursorOnTitleBar();
    int IsCursorOnResize();
    int IsCursorOnWindow();
    bool IsFocused();
    UIWindowState GetState(); 
    fVec2  GetSize();
    fVec2  GetPosition();
    void SetShaderParams(); 
    void SetTitle(const char* str);
    void _SetTitlePosition();
    int64 GetLayer(); 
    int64 GetUIItemLayer(int64 relative);

    UIItem* AddItem(UIItemType, int64);
    void    DrawItems();

    void SetPosition(const fVec2&);
    void SetSize(const fVec2&);
    void SetCursor(const fVec2&);
    void SetCursorTopLeft();
    void IncCursor(const fVec2&);
    void GetCursorPositionFree(const fVec2&);
    fVec2 GetAbsoluteCursor();
    fVec2 GetCursor();

    UIWindowState state = UIWindowState::NONE;
    fVec2 relPos;
    fVec2 rpos;
    fVec2 lsize;
    fVec2 lpos;
    fVec2 lwinPos;


    fVec4 bgCol = fVec4(1.0,1.0,1.0,1.0);
    bool _tmpisFocused = 0;

    UIWindowMetrics metrics;
    fVec2 cursor = fVec2(0.0,0.0);
    std::list<UIItem> items;

    UIManager* attachedUIManager = 0;

    static bool CacheConditionHasUIWindow(GameObject* obj);
    static int UIWindowDrawCallback(void*);

};
