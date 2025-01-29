#include "Scripts.h"
#include "Camera.h"


void CameraScript::Start() {
	Camera* camC = goc->AddComponent<Camera>();
	camC->clearColor = fVec3(0.4, 0.5, 0.6);
	camC->Use();
	camC->ChangeOrthoWithMSAA(720, 480, msaa);
	camC->fbo.AddAttachment({720,480});
	camC->SetClearColor(1, fVec4(0.1,0.1,0.5,0.5));
}

void CameraScript::Update() {

}