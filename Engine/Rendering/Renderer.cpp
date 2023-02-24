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
	sprite->container.UpdateSize(temp->fbo.RenderedImage.size.x, temp->fbo.RenderedImage.size.y);
	cam->ChangeOrtho(temp->fbo.RenderedImage.size.x, temp->fbo.RenderedImage.size.y);
	cam->Update();
	//Setting sprite texture
	sprite->SetTexture(&temp->fbo.RenderedImage);
	if (sprite->shader->name != shaderName)
		sprite->SetShader(shaderName);

	cam->viewPortSize.x = cam->size.x * strechCoeff.x;
	cam->viewPortSize.y = cam->size.y * strechCoeff.y;

	Context::SetViewPort(Max<int>(Context::WINDOW_WIDTH - cam->viewPortSize.x, 0) * 0.5, 
					     Max<int>(Context::WINDOW_HEIGHT - cam->viewPortSize.y, 0)* 0.5,
				         cam->viewPortSize.x, cam->viewPortSize.y);

	Context::Clear(cam->clearColor.x, cam->clearColor.y, cam->clearColor.z, 1.0);
	Camera::ActiveCamera = cam;
	attachedObj->Draw();
	Camera::ActiveCamera = temp;
}
#include "glfw3.h"
void Renderer::CaptureOnCamFrame() {
	//Adding or getting components
	Camera* cam			  = attachedObj->AddComponent<Camera>();
	Sprite* sprite		  = attachedObj->AddComponent<Sprite>();
	Transform* transform  = attachedObj->AddComponent<Transform>();
	//set up renderer quad 
	Camera* temp = Camera::ActiveCamera;
	if (temp == nullptr)
		return;
	 
	//TODO::Downscale the texture to which post processing shader is applied and then pass it to imgui
	sprite->container.UpdateSize(temp->fbo.RenderedImage.size.x, temp->fbo.RenderedImage.size.y);
	cam->ChangeOrtho(temp->fbo.RenderedImage.size.x, temp->fbo.RenderedImage.size.y);
	cam->Update();

	//Setting srpite texture
	sprite->SetTexture(&temp->fbo.RenderedImage);
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
