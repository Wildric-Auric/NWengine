#include "Renderer.h"
#include "Sprite.h"
#include "Camera.h"
#include "Transform.h"
#include "Script.h"

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
	if (temp == nullptr) return;

	if (temp != inputCamera) {
		inputCamera = temp;
		sprite->container = Quad(iVec2(0, 0), temp->fbo.RenderedImage.size.x, temp->fbo.RenderedImage.size.y);
	}

	cam->ChangeOrtho(temp->fbo.RenderedImage.size.x * strechCoeff.x, temp->fbo.RenderedImage.size.y * strechCoeff.y);
	cam->Update();
	//Setting srpite texture
	sprite->SetTexture(&inputCamera->fbo.RenderedImage);
	if (sprite->shader->name != shaderName)
		sprite->SetShader(shaderName);

	Context::SetViewPort(Max<int>(Context::WINDOW_WIDTH - cam->viewPortSize.x, 0) * 0.5, 
					     Max<int>(Context::WINDOW_HEIGHT - cam->viewPortSize.y, 0)* 0.5,
				         cam->viewPortSize.x, cam->viewPortSize.y);

	Context::Clear(cam->clearColor.x, cam->clearColor.y, cam->clearColor.z, 1.0);
	attachedObj->Draw();
}

void Renderer::CaptureOnCamFrame() {
	//Adding or getting components
	Camera* cam			  = attachedObj->AddComponent<Camera>();
	Sprite* sprite		  = attachedObj->AddComponent<Sprite>();

	//set up renderer quad 
	Camera* temp = Camera::ActiveCamera;
	if (temp == nullptr)
		return;
	if (temp != inputCamera) {
		inputCamera = temp;
		sprite->container = Quad(iVec2(0, 0), temp->fbo.RenderedImage.size.x, temp->fbo.RenderedImage.size.y);
	}

	cam->ChangeOrtho(temp->fbo.RenderedImage.size.x * strechCoeff.x, temp->fbo.RenderedImage.size.y * strechCoeff.y);
	cam->Update();
	//Setting srpite texture
	sprite->SetTexture(&inputCamera->fbo.RenderedImage);
	if (sprite->shader->name != shaderName)
		sprite->SetShader(shaderName);

	Context::SetViewPort(0, 0, cam->viewPortSize.x, cam->viewPortSize.y);
	Context::Clear(cam->clearColor.x, cam->clearColor.y, cam->clearColor.z, 1.0);

	cam->fbo.Bind();
		Context::Clear(cam->clearColor.x, cam->clearColor.y, cam->clearColor.z, 1.0);
		attachedObj->Draw();
	cam->fbo.Unbind();

}

Renderer::~Renderer() {
	if (Renderer::currentRenderer == this)
		Renderer::currentRenderer = Renderer::defaultRenderer;
}

Renderer* Renderer::defaultRenderer = nullptr;
Renderer* Renderer::currentRenderer = nullptr;

void Renderer::Gui() {};
int Renderer::Serialize(std::fstream* data, int offset) { return 0; };
int Renderer::Deserialize(std::fstream* data, int offset) { return 0; };
