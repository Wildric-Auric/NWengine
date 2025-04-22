#include "UIWindow.h"
#include "Transform.h"
#include "Sprite.h"
#include "Inputs.h"
#include "keyboard.h"
#include "CoordSys.h"
#include "Context.h"
#include "Window.h"
#include "InlineShader.h"

void UIWindow::OnAdd() {
    NW_REQUIRE_COMP(attachedObject, Transform);
    Sprite* spr = NW_REQUIRE_COMP(attachedObject, Sprite);
    spr->SetSize({100,50});
    //Set shader
    InlineShader inlineShader;
    inlineShader.AppFragGlobal("uniform vec2 uRes");
    inlineShader.AppFragGlobal("uniform float uTitleHeight = 20.0;");
    inlineShader.AppFragMain("bool ycond = (1.0-uv.y)*uRes.y < uTitleHeight;");
    inlineShader.AppFragMain("vec3 color = ycond ? vec3(1.0,0.0,0.0) : vec3(1.0);");
    inlineShader.SetFragOut("vec4(color, 1.0)");
    inlineShader.Generate();
    spr->SetShader(inlineShader.GetShader());
}

UIWindow::UIWindow(GameObject* go) {
    attachedObject = go;
}

void UIWindow::Update() {
    Sprite* spr = attachedObject->GetComponent<Sprite>();
    Transform* tr = attachedObject->GetComponent<Transform>();
    spr->GetShader()->Use();
    fVec2 s = fVec2(spr->container.width, spr->container.height);
    fVec2 hs = 0.5*fVec2(spr->container.width, spr->container.height);
    spr->GetShader()->SetVector2("uRes", s.x, s.y);
    spr->GetShader()->SetUniform1f("uTitleHeight", 20.0f);

    fVec2 mpos = Inputs::GetMousePosition();
    fVec2 p    = NWCoordSys::WorldToViewportNonNormalized(mpos);
    fVec2 rpos = -tr->GetPosition() + p;
    bool m = Inputs::GetInputMouse(NWin::Key::NWIN_KEY_LBUTTON,NWin::KeyEventEnum::NWIN_KeyPressed); 
    
    std::cout << "Mpos:" << mpos.x << " " << mpos.y << "\n";
    std::cout << "p: " << p.x << " " << p.y << "\n";
    std::cout << "Scr: " << NWCoordSys::WorldToScreenNonNormalized(mpos).x << " " << NWCoordSys::WorldToScreenNonNormalized(mpos).y << "\n";
    
    bool cond = (abs(rpos.x) < spr->container.width*0.5 && abs(rpos.y) < spr->container.height*0.5);
    bool cond2= abs(rpos.x) > 0.9 * hs.x || abs(rpos.y) > 0.9 * hs.y;
    if (cond2 && cond) {
        ((NWin::Window*)(Context::window))->setCursor(NWin::CursorIcon::RESIZE_WE);
    }
    else {
        ((NWin::Window*)(Context::window))->setCursor(NWin::CursorIcon::ARROW);
    }

    if (state == NWUiWindowState::NONE && m && cond){
        relPos = rpos;
        lsize = fVec2(spr->container.width, spr->container.height);
        lpos  = p;
        lwinPos = tr->GetPosition();
        state = NWUiWindowState::MOVE;
        if (cond2) {
            state = NWUiWindowState::RESIZE;
        }
    }
    if ((state != NWUiWindowState::NONE) && !m) {
        state = NWUiWindowState::NONE;
        ((NWin::Window*)(Context::window))->setCursor(NWin::CursorIcon::ARROW);
    }
    if (state == NWUiWindowState::MOVE) {
        tr->SetPosition(p-relPos);
    }
    if (state == NWUiWindowState::RESIZE) {
        ((NWin::Window*)(Context::window))->setCursor(NWin::CursorIcon::RESIZE_WE);
        fVec2 ps = p - lpos;
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
