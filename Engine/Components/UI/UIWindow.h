#pragma once
#include "GameObject.h"

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
    iVec2 minSize       = iVec2(20,20);
};

enum class UIItemType {
    NONE,
    TITLE 
};

class UIItem {
    public:
    UIItemType type;
    GameObject obj;

    UIItemType GetType();
    int64 GetLayer();
    void _SetUp(UIItemType type, int64 layer, std::list<UIItem>::iterator);
    std::list<UIItem>::iterator _iter;
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
    int64 GetLayer(); 
    int64 GetUIItemLayer(int64 relative);

    UIItem* AddItem(UIItemType, int64);
    void        DrawItems();

    UIWindowState state = UIWindowState::NONE;
    fVec2 relPos;
    fVec2 rpos;
    fVec2 lsize;
    fVec2 lpos;
    fVec2 lwinPos;


    fVec4 bgCol = fVec4(1.0,1.0,1.0,1.0);
    bool _tmpisFocused = 0;

    UIWindowMetrics metrics;
    std::list<UIItem> items;

    static bool CacheConditionHasUIWindow(GameObject* obj);
    static int UIWindowDrawCallback(void*);

};
