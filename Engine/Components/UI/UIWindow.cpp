#include "UIWindow.h"
#include "Transform.h"
#include "Sprite.h"
#include "Inputs.h"
#include "keyboard.h"
#include "CoordSys.h"
#include "Context.h"
#include "Window.h"
#include "InlineShader.h"
#include "UISys.h"
#include "Scene.h"
#include "Text.h"
#include "DefaultAssets.h"
#include "Utilities.h"

int64 (*UIItem::DefaultUIItemGetLayerProc)(UIItem*) = [](UIItem* item)->int64{ return item->obj.GetComponent<Sprite>()->GetSortingLayer();};

bool UIWindow::CacheConditionHasUIWindow(GameObject* obj) {
    return obj->GetComponent<UIWindow>();
}

int UIWindow::UIWindowDrawCallback(void* obj) {
    UIWindow* win = ((GameObject*)obj)->GetComponent<UIWindow>();

    fMat4 tempview = Camera::GetActiveCamera()->viewMatrix; //Temporary, should use UIManager instead
    Camera::GetActiveCamera()->viewMatrix = fMat4(1.0f);

    Context::EnableStencilTest(1);
    Context::SetStencilMask(1);
    Context::ClearStencilBuff();
    Context::SetStencilFunc(NWStencilBehaviour::NW_ALWAYS);
    int ret = Sprite::DefaultSpriteDrawCallback(obj); 
    Context::SetStencilFunc(NWStencilBehaviour::NW_EQUAL);
    win->DrawItems();
    Context::EnableStencilTest(0);

    Camera::GetActiveCamera()->viewMatrix = tempview;

    return ret;
}

void UIWindow::DrawItems() {
    for (auto iter = this->items.begin(); iter != this->items.end(); iter++) {
        if (iter->type == UIItemType::TITLE) {
            iter->obj.GetComponent<Text>()->DirectDraw();
        }
        iter->obj.Draw();
    }
}
    
int64 UIWindow::GetLayer() {
    return attachedObject->GetComponent<Sprite>()->GetSortingLayer();
}

int64 UIWindow::GetUIItemLayer(int64 relative) {
    return GetLayer() - relative;
}

void UIWindow::OnAdd() {
    NW_REQUIRE_COMP(attachedObject, Transform);
    Sprite* spr = NW_REQUIRE_COMP(attachedObject, Sprite);
    spr->SetSize({100,50});
    //Set shader
    InlineShader inlineShader;
    inlineShader.AppFragGlobal("uniform vec2 uRes");
    inlineShader.AppFragGlobal("uniform float uTitleHeight = 20.0;");
    inlineShader.AppFragGlobal("uniform float uBorderWidth = 1.0;");
    inlineShader.AppFragGlobal("uniform vec4  uCol = vec4(1.0);");
    inlineShader.AppFragMain("bool ycond = (1.0-uv.y)*uRes.y < uTitleHeight;");
    inlineShader.AppFragMain("bool condBorder = (1.0-uv.y)*uRes.y <= uBorderWidth || uv.y * uRes.y <= uBorderWidth;");
    inlineShader.AppFragMain("condBorder = condBorder || (1.0-uv.x)*uRes.x <= uBorderWidth || uv.x * uRes.x <= uBorderWidth;");
    inlineShader.AppFragMain("vec4 color = ycond ? vec4(1.0,0.0,0.0,1.0) : uCol;");
    inlineShader.AppFragMain("color = condBorder ? vec4(0.0,0.0,0.0,1.0): color;");
    inlineShader.SetFragOut("color");
    inlineShader.Generate();
    spr->SetShader(inlineShader.GetShader());
    Scene::GetCurrent()->AddToCache(UIWindow::CacheConditionHasUIWindow, *attachedObject);
    spr->SetSortingLayerFull(UISys::GetAvailableLayer());

    UIItem* item = AddItem(UIItemType::TITLE,-1);

    GameObject* obj = &item->obj;
    Text* te = obj->AddComponent<Text>();
    te->SetShader(ShaderTextDefaultStr, &ShaderTextDefaultID);
    te->isBatched = false;
    std::string fdir;
    GetSystemFontDir(&fdir); 
    fdir += "Arial.ttf";
	te->SetFont({fdir.c_str(),15}, te->_shader);
    te->layerOrder = spr->sortingLayer - 1;
    attachedObject->SetDrawCallback(UIWindowDrawCallback);
}

UIItemType UIItem::GetType() {
    return type;
}

int64 UIItem::GetLayer() {
    return obj.GetComponent<Sprite>()->GetSortingLayer();
}

void UIItem::_SetUp(UIItemType ptype, int64 layer, std::list<UIItem>::iterator it) {
    if (ptype == UIItemType::TITLE) _GetLayerProc = 
    [](UIItem* item)->int64 {return item->obj.GetComponent<Text>()->layerOrder;};    
    type = ptype;
    _iter = it;
}

UIItem* UIWindow::AddItem(UIItemType type, int64 layer) {
    //insert at beginning
    if (items.size() == 0 || items.front().GetLayer() <= layer) {
        items.push_front({});
        items.front()._SetUp(type, layer, items.begin());
        return &items.front();
    }
    //insert at the middle
    for (auto it = ++items.begin(); it != items.end(); ++it) {
        if (it->GetLayer() > layer)
            continue;
        auto newIt = items.insert(it,{});
        newIt->_SetUp(type, layer, newIt);
        return &*newIt;
    }
    //insert at the end
    items.push_back({});
    (--items.end())->_SetUp(type,layer, --items.end());
    return &items.back();
}

void UIWindow::OnDelete() {
    Scene::GetCurrent()->DeleteFromCache(UIWindow::CacheConditionHasUIWindow, *attachedObject);
}

UIWindow::UIWindow(GameObject* go) {
    attachedObject = go;
}

int UIWindow::IsCursorOnTitleBar() {
    //dist from top
    if (relPos.y < 0.0) return 0;
    return lsize.y * 0.5 - relPos.y <= metrics.titleBarHeight;
}

int UIWindow::IsCursorOnResize() {
    Sprite* spr = attachedObject->GetComponent<Sprite>();
    return IsCursorOnWindow() && abs(rpos.x) > (spr->container.width*0.5-metrics.resizeAreaWidth) || abs(rpos.y) > (spr->container.height*0.5-metrics.resizeAreaWidth);
}

int UIWindow::IsCursorOnWindow() {
    Sprite* spr = attachedObject->GetComponent<Sprite>();
    return (abs(rpos.x) < spr->container.width*0.5 && abs(rpos.y) < spr->container.height*0.5);
}

bool UIWindow::IsFocused() {
    return UISys::focusedWindow == this;
}

UIWindowState UIWindow::GetState() {
    return state;
}

fVec2 UIWindow::GetSize() {
    Sprite* spr = attachedObject->GetComponent<Sprite>();
    return fVec2(spr->container.width, spr->container.height);
}

void UIWindow::SetShaderParams() {
    Sprite* spr = attachedObject->GetComponent<Sprite>();
    spr->GetShader()->Use();
    fVec2 s = fVec2(spr->container.width, spr->container.height);
    spr->GetShader()->SetVector2("uRes", s.x, s.y);
    spr->GetShader()->SetUniform1f("uTitleHeight", metrics.titleBarHeight);
    spr->GetShader()->SetUniform1f("uBorderWidth", metrics.borderWidth);
    spr->GetShader()->SetUniform4f("uCol", bgCol.x, bgCol.y, bgCol.z, bgCol.z);
}

fVec2 UIWindow::GetPosition() {
    Transform* tr = attachedObject->GetComponent<Transform>();
    return tr->GetPosition();
}

void UIWindow::_SetTitlePosition() {
    if (items.size() == 0) return;
    UIItem& item = items.back();
    if (item.type != UIItemType::TITLE) return;
    Text* te = item.obj.GetComponent<Text>();
    fVec2 s = GetSize();
    NWCoordSys::BoundingBox& bb = te->GetBBRef();
    te->SetPosition(GetPosition() + fVec2(-s.x * 0.5 + bb.size.x * 0.5, s.y * 0.5 - bb.size.y * 0.5));
    te->UpdateGlyphs(1);
}

void UIWindow::SetTitle(const char* c) {
    Text* te = items.back().obj.GetComponent<Text>();
    te->SetContent(c);
    te->UpdateGlyphs(1);
}

void UIWindow::Update() {
    for (UIItem& item : items) {
        Text* t = item.obj.GetComponent<Text>();
        if (t) {_SetTitlePosition();t->Update();}
    }

    Sprite* spr = attachedObject->GetComponent<Sprite>();
    Transform* tr = attachedObject->GetComponent<Transform>();
    NWin::Window* win = ((NWin::Window*)(Context::window));
    fVec2 s = fVec2(spr->container.width, spr->container.height);
    fVec2 hs = 0.5*fVec2(spr->container.width, spr->container.height);
    SetShaderParams();

    rpos = -tr->GetPosition() + UISys::curPos;
    bool m = Inputs::GetInputMouse(NWin::Key::NWIN_KEY_LBUTTON,NWin::KeyEventEnum::NWIN_KeyPressed); 
    
    if (IsCursorOnWindow() && UISys::GetClickEvent()) {
        UISys::Focus(this);
    }
    if (IsCursorOnWindow()) {
        UISys::Hover(this);
    }

    bgCol.x = IsFocused();

    if (state == UIWindowState::NONE && m && IsCursorOnWindow() && UISys::focusedWindow == this) {
        relPos = rpos;
        lsize = fVec2(spr->container.width, spr->container.height);
        lpos  = UISys::curPos;
        lwinPos = tr->GetPosition();
        if (IsCursorOnTitleBar())
            state = UIWindowState::MOVE;
        if (IsCursorOnResize()) {
            state = UIWindowState::RESIZE;
        }
    }
    if ((state != UIWindowState::NONE) && !m) {
        state = UIWindowState::NONE;
    }
    if (state == UIWindowState::MOVE) {
        tr->SetPosition(UISys::GetCurPos() - relPos);
    }
    if (state == UIWindowState::RESIZE) {
        ((NWin::Window*)(Context::window))->setCursor(NWin::CursorIcon::RESIZE_WE);
        fVec2 ps = UISys::curPos - lpos;
        ps.x = (ps.x);
        ps.y = (ps.y);
        fVec2 m = -0.5 * ps;
        if (relPos.x < 0.0) {
            ps.x = -(ps.x);
        }
        if (relPos.y < 0.0) {
            ps.y = -(ps.y);
        }
        fVec2 newsize = lsize + ps;
        newsize.x = Max<int>(newsize.x, metrics.minSize.x);
        newsize.y = Max<int>(newsize.y, metrics.minSize.y);
        spr->SetSize(newsize);
        tr->SetPosition(lwinPos - m);
//      if (newsize.x != metrics.minSize.x && newsize.y != metrics.minSize.y)
    }
}
