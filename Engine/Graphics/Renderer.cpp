#include "Renderer.h"
#include "Sprite.h"
#include "Camera.h"
#include "Transform.h"
#include "DefaultAssets.h"


//TODO::Default constructor creates the gameobject
void Renderer::SetUp() {
	Sprite* spr;
	spr = componentContainer.AddComponent<Sprite>();
	componentContainer.AddComponent<Transform>();
	spr->SetShader(ShaderTexturedDefaultStr, &ShaderTexturedDefaultID);
	componentContainer.AddComponent<Camera>();
    Clear();
}

void Renderer::Clear() {
	Camera* cam = componentContainer.GetComponent<Camera>();
    cam->fbo.Bind();
		Context::Clear(cam->clearColor.x, cam->clearColor.y, cam->clearColor.z, 1.0);
	cam->fbo.Unbind();
}

void Renderer::Clean() {
    componentContainer.DeleteComponents();
}

void Renderer::SetShader(const std::string& shaderPath) {
	componentContainer.GetComponent<Sprite>()->SetShader(shaderPath);
}

void Renderer::SetShader(const ShaderText& st, ShaderIdentifier* id) { 
	componentContainer.GetComponent<Sprite>()->SetShader(st,id);
}

void Renderer::Use() {
	Renderer::currentRenderer = this;
}

void Renderer::Unuse() {
	Renderer::currentRenderer = Renderer::defaultRenderer;
}

Camera* Renderer::GetCamera() {
    return componentContainer.GetComponent<Camera>();
}

void Renderer::SetOffScreenSizeMultiplier(const fVec2& val) {
    _offscreenCoeff = val;
}

void Renderer::SetOnScreenSizeMultiplier(const fVec2& val) {
    stretchCoeff = val; 
}

void Renderer::SetTexParams(const TexMinFilter mi, const TexMaxFilter ma) {
    _minFilter = mi;
    _magFilter = ma;
}

void Renderer::SetStretch(const fVec2& v) {
    stretchCoeff = v;
}

bool Renderer::_DrawPrep() {
    Camera* cam = componentContainer.AddComponent<Camera>();
	Sprite* sprite = componentContainer.GetComponent<Sprite>();

	//set up renderer quad 
	Camera* temp = Camera::ActiveCamera;
	if (target == nullptr)
		target = temp;
	if (target == nullptr)
		return 0;

	target->fbo.Resolve();

    fVec2 nativeSize = fVec2(target->fbo.GetAtt(0).tex._size.x, target->fbo.GetAtt(0).tex._size.y) * _offscreenCoeff;
	sprite->container.UpdateSize(nativeSize.x, nativeSize.y);
	cam->ChangeOrtho(nativeSize.x, nativeSize.y);
	cam->Update();

	sprite->SetTexture(&target->fbo.GetAtt(0).tex);
    sprite->texture->SetMaxFilter(_magFilter);
    sprite->texture->SetMinFilter(_minFilter);
	cam->viewPortSize.x = cam->size.x;
	cam->viewPortSize.y = cam->size.y;
    return 1;
}

void Renderer::DrawOnDefaultFrame() {
	Camera* cam = componentContainer.AddComponent<Camera>();
    Camera* temp = Camera::ActiveCamera;

    bool ready = _DrawPrep();
    if (!ready) return;


	cam->viewPortSize.x = cam->size.x * stretchCoeff.x;
	cam->viewPortSize.y = cam->size.y * stretchCoeff.y;
	Context::SetViewPort((Context::WINDOW_WIDTH  - cam->viewPortSize.x) * 0.5, 
					     (Context::WINDOW_HEIGHT - cam->viewPortSize.y) * 0.5,
				         cam->viewPortSize.x, cam->viewPortSize.y);

	Context::Clear(cam->clearColor.x, cam->clearColor.y, cam->clearColor.z, 1.0);
	Camera::ActiveCamera = cam;
	componentContainer.Draw();
	Camera::ActiveCamera = temp;
	target               = nullptr; //May be temporary solution
}

void Renderer::CaptureOnCamFrame() {
	//Adding or getting components
	Camera* cam			  = componentContainer.AddComponent<Camera>();
	Transform* transform  = componentContainer.AddComponent<Transform>();
	//set up renderer quad 
	Camera* temp = Camera::ActiveCamera;
    
    bool ready = _DrawPrep();
    if (!ready) return;

	Context::SetViewPort(0, 0, cam->viewPortSize.x, cam->viewPortSize.y);
	cam->fbo.Bind();
		Context::Clear(cam->clearColor.x, cam->clearColor.y, cam->clearColor.z, 1.0);
		componentContainer.Draw();
	cam->fbo.Unbind();

	Camera::ActiveCamera = temp;
	target = nullptr;
}

Renderer::~Renderer() {
	if (Renderer::currentRenderer == this)
		Renderer::currentRenderer = Renderer::defaultRenderer;
}

Renderer* Renderer::operator()(Renderer* renderer, bool captureOnDefaultFrame) {
	Camera* temp		 = Camera::ActiveCamera;
	Camera::ActiveCamera = renderer->componentContainer.GetComponent<Camera>();

	if (captureOnDefaultFrame)
		this->DrawOnDefaultFrame();
	else
		this->CaptureOnCamFrame();

	Camera::ActiveCamera = temp;
	return this;
}

Renderer* Renderer::operator()(bool captureOnDefaultFrame) {
	if (captureOnDefaultFrame)
		this->DrawOnDefaultFrame();
	else
		this->CaptureOnCamFrame();
	return this;
};


void Renderer::Init() {
	Renderer::defaultRenderer = new Renderer();
	Renderer::defaultRenderer->SetUp();
	Renderer::currentRenderer = Renderer::defaultRenderer;
}

void Renderer::Destroy() {
	if (Renderer::currentRenderer == Renderer::defaultRenderer)
		Renderer::currentRenderer = nullptr;
	delete Renderer::defaultRenderer;
}

Renderer* Renderer::defaultRenderer = nullptr;
Renderer* Renderer::currentRenderer = nullptr;
