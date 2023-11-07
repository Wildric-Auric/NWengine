#include "Renderer.h"
#include "Sprite.h"
#include "Camera.h"
#include "Transform.h"
#include "Script.h"
#ifndef NW_GAME_BUILD
#include "Utilities.h"
#endif

Renderer::Renderer(GameObject* go) {
	this->attachedObj = go;

	this->attachedObj->AddComponent<Transform>();
	this->attachedObj->AddComponent<Camera>();
	this->attachedObj->AddComponent<Sprite>();
	this->attachedObj->AddComponent<Script>();
}

void Renderer::Use() {
	Renderer::currentRenderer = this;
}

void Renderer::Unuse() {
	Renderer::currentRenderer = Renderer::defaultRenderer;
}

void Renderer::DrawOnDefaultFrame() {
	//Adding or getting components
	Camera* cam = attachedObj->AddComponent<Camera>();
	Sprite* sprite = attachedObj->AddComponent<Sprite>();

	//set up renderer quad 
	Camera* temp = Camera::ActiveCamera;
	if (target == nullptr)
		target = temp;
	if (target == nullptr)
		return; //TODO::Show Blank image instead

	sprite->container.UpdateSize(target->fbo.RenderedImage.size.x, target->fbo.RenderedImage.size.y);
	cam->ChangeOrtho(target->fbo.RenderedImage.size.x, target->fbo.RenderedImage.size.y);
	cam->Update();
	//Setting sprite texture
	sprite->SetTexture(&target->fbo.RenderedImage);
	if (sprite->shader->name != shaderName)
		sprite->SetShader(shaderName);

	cam->viewPortSize.x = cam->size.x * strechCoeff.x;
	cam->viewPortSize.y = cam->size.y * strechCoeff.y;
	Context::SetViewPort((Context::WINDOW_WIDTH  - cam->viewPortSize.x) * 0.5, 
					     (Context::WINDOW_HEIGHT - cam->viewPortSize.y) * 0.5,
				         cam->viewPortSize.x, cam->viewPortSize.y);

	Context::Clear(cam->clearColor.x, cam->clearColor.y, cam->clearColor.z, 1.0);
	Camera::ActiveCamera = cam;
	attachedObj->Draw();
	Camera::ActiveCamera = temp;
	target               = nullptr; //May be temporary solution
}

void Renderer::CaptureOnCamFrame() {
	//Adding or getting components
	Camera* cam			  = attachedObj->AddComponent<Camera>();
	Sprite* sprite		  = attachedObj->AddComponent<Sprite>();
	Transform* transform  = attachedObj->AddComponent<Transform>();
	//set up renderer quad 
	Camera* temp = Camera::ActiveCamera;
	if (target == nullptr)
		target = temp;
	if (target == nullptr)
		return;
	//TODO::Downscale the texture to which post processing shader is applied and then pass it to imgui
	sprite->container.UpdateSize(target->fbo.RenderedImage.size.x, target->fbo.RenderedImage.size.y);
	cam->ChangeOrtho(target->fbo.RenderedImage.size.x, target->fbo.RenderedImage.size.y);
	cam->Update();

	//Setting srpite texture
	sprite->SetTexture(&target->fbo.RenderedImage);
	if (sprite->shader->name != shaderName)
		sprite->SetShader(shaderName);

	Camera::ActiveCamera = cam;

	cam->viewPortSize.x = cam->size.x * strechCoeff.x;
	cam->viewPortSize.y = cam->size.y * strechCoeff.y;

	//Context::SetViewPort(0, 0, cam->viewPortSize.x, cam->viewPortSize.y);
	cam->fbo.Bind();
		Context::Clear(cam->clearColor.x, cam->clearColor.y, cam->clearColor.z, 1.0);
		attachedObj->Draw();
	cam->fbo.Unbind();

	Camera::ActiveCamera = temp;
	target = nullptr;
}

Renderer::~Renderer() {
	if (Renderer::currentRenderer == this)
		Renderer::currentRenderer = Renderer::defaultRenderer;
}

Renderer* Renderer::defaultRenderer = nullptr;
Renderer* Renderer::currentRenderer = nullptr;