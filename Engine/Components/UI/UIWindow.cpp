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

bool UIWindow::CacheConditionHasUIWindow(GameObject* obj) {
    return obj->GetComponent<UIWindow>();
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

void UIWindow::SetShaderParams() {
    Sprite* spr = attachedObject->GetComponent<Sprite>();
    spr->GetShader()->Use();
    fVec2 s = fVec2(spr->container.width, spr->container.height);
    spr->GetShader()->SetVector2("uRes", s.x, s.y);
    spr->GetShader()->SetUniform1f("uTitleHeight", metrics.titleBarHeight);
    spr->GetShader()->SetUniform1f("uBorderWidth", metrics.borderWidth);
    spr->GetShader()->SetUniform4f("uCol", bgCol.x, bgCol.y, bgCol.z, bgCol.z);
}

void UIWindow::Update() {
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
        spr->SetSize(lsize + ps);
        tr->SetPosition(lwinPos - m);
    }
}
