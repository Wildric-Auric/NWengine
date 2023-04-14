//TODO::
//With babtching it is observed that rendering 1000 particle does not slow the game
//However when particles start to be recycled the fps drops to 30, something wrong in cpu level; fix that
#include "ParticleSystem.h"
#include "Scene.h"
#include "imgui/imgui.h"
#include "NWGui.h"
#include "Utilities.h"

ParticleSystem::ParticleSystem(GameObject* attachedObj) {
	this->attachedObj = attachedObj;
}

std::map<GameObject*, ParticleSystem> ParticleSystem::componentList;

void ParticleSystem::Update() {
	clock += Globals::deltaTime;
	prop.absoluteStartPosition = attachedObj->GetComponent<Transform>()->position; //TODO::Not using GetComponent only once and make sure transofrm exists
	if ((clock >= emissionFrequency) && isActive) Emit();
	auto it = enabled.begin();

#define UPDATE_ATTRIBUTE(duration, value, interpolator)\
	 if (duration > 0.0)  { value = interpolator.Evaluate(particle->clock / duration);}
	while (it != enabled.end()) {
		int index = *it;
		Particle* particle = &pool[index];
		UPDATE_ATTRIBUTE(particle->prop.speed.duration, particle->currentSpeed, particle->prop.speed);

		UPDATE_ATTRIBUTE(particle->prop.colorX.duration, particle->currentColor.x, particle->prop.colorX);
		UPDATE_ATTRIBUTE(particle->prop.colorY.duration, particle->currentColor.y, particle->prop.colorY);
		UPDATE_ATTRIBUTE(particle->prop.colorZ.duration, particle->currentColor.z, particle->prop.colorZ);
		UPDATE_ATTRIBUTE(particle->prop.colorA.duration, particle->currentColor.a, particle->prop.colorA);

		UPDATE_ATTRIBUTE(particle->prop.scaleX.duration, particle->currentScale.x, particle->prop.scaleX);
		UPDATE_ATTRIBUTE(particle->prop.scaleY.duration, particle->currentScale.y, particle->prop.scaleY);

		UPDATE_ATTRIBUTE(particle->prop.directionX.duration, particle->currentDirection.x, particle->prop.directionX);
		UPDATE_ATTRIBUTE(particle->prop.directionY.duration, particle->currentDirection.y, particle->prop.directionY);
#undef UPDATE_ATTRIBUTE(duration, value, interpolator)

		particle->currentPosition = particle->currentPosition +  particle->currentDirection.normalize() * Globals::deltaTime * particle->currentSpeed;
		particle->distance = (particle->currentPosition - particle->prop.sPosition).magnitude();
		particle->clock += Globals::deltaTime;
		if ((particle->clock >= particle->prop.lifetime) || (particle->distance >= particle->prop.lifedistance)) {
			disabled.push_back(index);
			it = enabled.erase(it);
			particle->Disable();
		}
		else it++;
		UpdateParticle(index);
	}
}

void ParticleSystem::Emit() {
	clock = 0.0;
	for (int i = 0; i < emissionQuantity; i++) {
		if (disabled.size() > 0) {
			InitParticle();
			continue;
		}
		if (pool.size() >= maxParticles) {
			//Often, oldest particles would be at begining of enabled
			if (!recycle) return;
			int temp = enabled.front();
			enabled.pop_front();
			enabled.push_back(temp);
			//Copying enable function without add to render function
			pool[temp].clock = 0.0;
			pool[temp].distance = 0.0;

			pool[temp].prop				= prop;
			pool[temp].currentPosition  = prop.sPosition;
			pool[temp].currentScale		= fVec2(prop.scaleX.source, prop.scaleY.source);
			pool[temp].currentDirection = fVec2(prop.directionX.source, prop.directionY.source);
			pool[temp].currentColor     = fVec4(prop.colorX.source, prop.colorY.source, prop.colorZ.source, prop.colorA.source);
			pool[temp].currentSpeed		= prop.speed.source;
			pool[temp].isActive			= 1;
			//---- 
			pool[temp].sprite->SetShader(shader);
			pool[temp].sprite->SetTexture(texture);
			continue;
		}
		Init();
	}
}

void ParticleSystem::InitParticle() {
		int temp = disabled.back();
		disabled.pop_back();
		enabled.push_back(temp);
		pool[temp].prop = prop;
		pool[temp].Enable();
		pool[temp].sprite->SetShader(shader);
		pool[temp].sprite->SetTexture(texture);
}

void ParticleSystem::Init() {
	int temp = initNum;
	if ( (pool.size() + temp ) >= maxParticles) temp = maxParticles - pool.size(); 
	for (int i = 0; i < temp; i++) {
		pool.push_back(Particle());
		pool.back().prop = prop;
		disabled.push_back(pool.size() - 1);
		pool.back().transform = pool.back().go.AddComponent<Transform>();
		(pool.back().sprite = pool.back().go.AddComponent<Sprite>())->isBatched = 1;	 //TODO::Add batch control
	}
}

void ParticleSystem::UpdateParticle(int index) {
	Transform* transform = attachedObj->GetComponent<Transform>();
	if (transform == nullptr) transform = attachedObj->AddComponent<Transform>();
	pool[index].transform->position = pool[index].currentPosition + pool[index].prop.absoluteStartPosition;
	pool[index].transform->scale = pool[index].currentScale;

	uint32 temp = (uint32)(pool[index].currentColor.g  * 1023.0f) | ((uint32)(pool[index].currentColor.r * 1023.0f) << 0xA); //TODO::formalize these magic values
	uint32 temp0= (uint32)(pool[index].currentColor.a *  1023.0f) | ((uint32)(pool[index].currentColor.b * 1023.0f) << 0xA);
	pool[index].sprite->vertexAttributes.x = *((float*)&temp);
	pool[index].sprite->vertexAttributes.y = *((float*)&temp0);

	pool[index].sprite->Update();
}

void Particle::Disable() {
	isActive = 0;
	sprite->StopRendering();
}

void Particle::Enable() {
	clock = 0.0;
	distance = 0.0;

	currentPosition  = prop.sPosition;

	currentScale     = fVec2(prop.scaleX.source, prop.scaleY.source);
	currentDirection = fVec2(prop.directionX.source, prop.directionY.target);

	currentSpeed     = prop.speed.source;
	currentColor     = fVec4(prop.colorX.source, prop.colorY.source, prop.colorZ.source, prop.colorA.source);

	isActive = 1;
	sprite->Render();
}

void ParticleSystem::Gui() {
	ImGui::Checkbox("Active", &isActive);
	if (ImGui::TreeNode("Particles properties")) {
		GUI_SEP
		NWGui::DragValue("Lifetime", &prop.lifetime, ImGuiDataType_Float);
		GUI_SEP;
		NWGui::DragValue("Lifedistance", &prop.lifedistance, ImGuiDataType_S32);
		GUI_SEP;
		NWGui::DragValue("Relative start position", &prop.sPosition, ImGuiDataType_Float, 2);
		GUI_SEP;
		NWGui::Text("Horizontal direction");
		prop.directionX.Gui();
		ImGui::Text("Vertical direction");
		prop.directionY.Gui();
		GUI_SEP;
		NWGui::Text("Horizontal scale");
		prop.scaleX.Gui();
		NWGui::Text("Vertical scale");
		prop.scaleY.Gui();
		GUI_SEP;
		NWGui::Text("Color: Red channel");
		prop.colorX.Gui();
		NWGui::Text("Color: Green channel");
		prop.colorY.Gui();
		NWGui::Text("Color: Blue channel");
		prop.colorZ.Gui();
		NWGui::Text("Color: Alpha channel");
		prop.colorA.Gui();
		GUI_SEP;
		NWGui::Text("Speed");
		prop.speed.Gui();
		GUI_SEP;
		GUI_SEP; GUI_NEWLINE;
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Emission properties")) {

		if (NWGui::FileHolder("Shader", shader)) {
			std::string path = GetFile("Shader Files\0*.shader\0*.*\0");
			if (path != "") this->shader = path;
		}
		GUI_SEP

		if (NWGui::FileHolder("Texture", this->texture)) {
			std::string path = GetFile("Image Files\0*.png;*.jpeg;*.jpg\0*.*\0");
			if (path != "") path;
		}
		GUI_SEP
		NWGui::DragValue("Emission frequency", &emissionFrequency, ImGuiDataType_Double);
		GUI_SEP;
		NWGui::DragValue("Emission quantity", &emissionQuantity, ImGuiDataType_U16);
		GUI_SEP;
		NWGui::CheckBox("Recycle particles", &recycle);
		GUI_SEP;
		NWGui::DragValue("Max particles(experimental)", &maxParticles, ImGuiDataType_S32);
		ImGui::TreePop();
	}
}




int ParticleSystem::Serialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;

	WRITE_ON_BIN(data,"ParticleSystem", 14,sizeBuffer);
	WRITE_ON_BIN(data, &(isActive), sizeof(isActive), sizeBuffer);
	//serializes particles prop

	WRITE_ON_BIN(data, &(prop.lifetime),     sizeof(prop.lifetime), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.lifedistance), sizeof(prop.lifedistance), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.sPosition.x),  sizeof(prop.sPosition.x), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.sPosition.y),  sizeof(prop.sPosition.y), sizeBuffer);

	prop.directionX.Serialize(data, offset);
	prop.directionY.Serialize(data, offset);
	prop.scaleX.Serialize(data, offset);
	prop.scaleY.Serialize(data, offset);
	prop.speed.Serialize(data, offset);
	prop.colorX.Serialize(data, offset);
	prop.colorY.Serialize(data, offset);
	prop.colorZ.Serialize(data, offset);
	prop.colorA.Serialize(data, offset);

	//emitter prop
	WRITE_ON_BIN(data, &(emissionFrequency), sizeof(emissionFrequency), sizeBuffer);
	WRITE_ON_BIN(data, &(emissionQuantity), sizeof(emissionQuantity), sizeBuffer);
	WRITE_ON_BIN(data, &(recycle), sizeof(recycle), sizeBuffer);
	WRITE_ON_BIN(data, &(emissionFrequency), sizeof(emissionFrequency), sizeBuffer);
	WRITE_ON_BIN(data, &(maxParticles), sizeof(maxParticles), sizeBuffer);

	return 0;
}

int ParticleSystem::Deserialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	READ_FROM_BIN(data, &(isActive), sizeBuffer);
	READ_FROM_BIN(data, &(prop.lifetime), sizeBuffer);
	READ_FROM_BIN(data, &(prop.lifedistance), sizeBuffer);
	READ_FROM_BIN(data, &(prop.sPosition.x), sizeBuffer);
	READ_FROM_BIN(data, &(prop.sPosition.y), sizeBuffer);

	prop.directionX.Deserialize(data, offset);
	prop.directionY.Deserialize(data, offset);
	prop.scaleX.Deserialize(data, offset);
	prop.scaleY.Deserialize(data, offset);
	prop.speed.Deserialize(data, offset);

	prop.colorX.Deserialize(data, offset);
	prop.colorY.Deserialize(data, offset);
	prop.colorZ.Deserialize(data, offset);
	prop.colorA.Deserialize(data, offset);

	READ_FROM_BIN(data, &(emissionFrequency), sizeBuffer);
	READ_FROM_BIN(data, &(emissionQuantity), sizeBuffer);
	READ_FROM_BIN(data, &(recycle), sizeBuffer);
	READ_FROM_BIN(data, &(emissionFrequency), sizeBuffer);
	READ_FROM_BIN(data, &(maxParticles), sizeBuffer);
	return 0;
}

ParticleSystem::~ParticleSystem() {
	//TODO::Better solution
	for (int i = 0; i < pool.size(); i++) {
		pool[i].Disable();
		pool[i].go.DeleteComponents();
	}

	auto it = Scene::currentScene->drawList.begin();
	while (it != Scene::currentScene->drawList.end()) {
		if (((*it) == nullptr) || !(*it)->isRendered) {
			it = Scene::currentScene->drawList.erase(it);
			continue;
		}
		it++;
	}
}
