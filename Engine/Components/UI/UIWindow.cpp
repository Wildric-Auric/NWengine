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
    spr->SetSize({100,100});
    //Set shader
    InlineShader inlineShader;
    inlineShader.AppFragMain("vec3 color = uv.y > 0.9 ? vec3(1.0,0.0,0.0) : vec3(1.0)");
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
    fVec2 mpos = Inputs::GetMousePosition();
    fVec2 p    = NWCoordSys::WorldToViewportNonNormalized(mpos);
    fVec2 rpos = -tr->GetPosition() + p;
    bool m = Inputs::GetInputMouse(NWin::Key::NWIN_KEY_LBUTTON,NWin::KeyEventEnum::NWIN_KeyPressed); 
    
    std::cout << "Mpos:" << mpos.x << " " << mpos.y << "\n";
    std::cout << "p: " << p.x << " " << p.y << "\n";
    std::cout << "Scr: " << NWCoordSys::WorldToScreenNonNormalized(mpos).x << " " << NWCoordSys::WorldToScreenNonNormalized(mpos).y << "\n";

    if (state == 0 && m && abs(rpos.x) < spr->container.width*0.5 && abs(rpos.y) < spr->container.height*0.5) {
        state = 2;
        relPos = rpos;
        lsize = fVec2(spr->container.width, spr->container.height);
        lpos  = p;
    }
    if ((state == 1 || state == 2) && !m) {
        state = 0;
    }
    if (state == 1) {
        tr->SetPosition(p+relPos);
    }
    if (state == 2) {
        fVec2 ps = p - lpos;
        ps.x = 2.0*(ps.x);
        ps.y = 2.0*(ps.y);
        if (relPos.x < 0.0) {
            ps.x = -(ps.x);
        }
        if (relPos.y < 0.0) {
            ps.y = -(ps.y);
        }
        spr->SetSize(lsize + ps);
    }

}
