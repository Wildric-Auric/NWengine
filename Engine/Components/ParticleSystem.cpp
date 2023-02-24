//Memory leak?
#include "ParticleSystem.h"
#include "Scene.h"
#include "imgui/imgui.h"
#include "NWGui.h"

ParticleSystem::ParticleSystem(GameObject* attachedObj) {
	this->attachedObj = attachedObj;
}

std::map<GameObject*, ParticleSystem> ParticleSystem::componentList;

void ParticleSystem::Update() {
	clock += Globals::deltaTime;
	prop.absoluteStartPosition = attachedObj->GetComponent<Transform>()->position; //TODO::Not using GetComponent only once and make sure transofrm exists
	if ((clock >= emissionFrequency) && isActive) Emit();
	auto it = enabled.begin();
	while (it != enabled.end()) {
		int index = *it;
		Particle* particle = &pool[index];
		if (particle->prop.speedVarDuration >= 0.01) {
			double temp = particle->clock / particle->prop.speedVarDuration;
			if (abs(temp) < 0.99)
				particle->currentSpeed = lerp<float, double>(particle->prop.sSpeed, particle->prop.eSpeed,
															temp);
		}

		if (particle->prop.alphaVarDuration >= 0.01) {
			double temp = particle->clock / particle->prop.alphaVarDuration;
			if (abs(temp) < 0.99)
				particle->currentAlpha = lerp<float, double>(particle->prop.sAlpha, particle->prop.eAlpha, temp);
			else
				particle->currentAlpha = particle->prop.eAlpha;
		}

		if (particle->prop.scaleVarDuration >= 0.01) {
			double temp = particle->clock / particle->prop.scaleVarDuration;
			if (abs(temp) < 0.99)
				particle->currentScale = lerpVector2<float, double>(particle->prop.sScale, particle->prop.eScale,
											Vector2<double>(temp, temp));
		}

		if (particle->prop.directionVarDuration >= 0.01) {
			double temp = particle->clock / particle->prop.directionVarDuration;
			if (abs(temp) < 0.99)
				particle->currentDirection = lerpVector2<float, double>(particle->prop.sDirection, particle->prop.eDirection, 
												Vector2<double>(temp, temp));
		}


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

			pool[temp].prop = prop;
			pool[temp].currentPosition = prop.sPosition;
			pool[temp].currentScale = prop.sScale;
			pool[temp].currentAlpha = prop.sAlpha;
			pool[temp].currentSpeed = prop.sSpeed;
			pool[temp].currentDirection = prop.sDirection;
			pool[temp].isActive = 1;
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
		pool.back().sprite = pool.back().go.AddComponent<Sprite>();	
	}
}

void ParticleSystem::UpdateParticle(int index) {
	Transform* transform = attachedObj->GetComponent<Transform>();
	if (transform == nullptr) transform = attachedObj->AddComponent<Transform>();
	pool[index].transform->position = pool[index].currentPosition + pool[index].prop.absoluteStartPosition;
	pool[index].transform->scale = pool[index].currentScale;

	//Can't work like this, TODO::Add uniform call stack
	/*pool[index].sprite->shader->Use();
	pool[index].sprite->shader->SetUniform1f("uAlpha", pool[index].currentAlpha);*/
}

void Particle::Disable() {
	isActive = 0;
	go.StopRendering();
}

void Particle::Enable() {
	clock = 0.0;
	distance = 0.0;

	currentPosition = prop.sPosition;
	currentScale = prop.sScale;
	currentSpeed = prop.sSpeed;
	currentAlpha = prop.sAlpha;
	currentDirection = prop.sDirection;
	isActive = 1;
	go.AddToRender();
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
		NWGui::DragValue("Starting direction", &prop.sDirection.x, ImGuiDataType_Float, 2, 0.05f, -1.0f, 1.0f);
		GUI_SEP;
		NWGui::DragValue("End direction", &prop.eDirection.x, ImGuiDataType_Float, 2, 0.05f, -1.0f, 1.0f);
		GUI_SEP;
		NWGui::DragValue("Direction variation duration", &prop.directionVarDuration, ImGuiDataType_Float);
		GUI_SEP;

		NWGui::DragValue("Starting scale", &prop.sScale.x, ImGuiDataType_Float, 2);
		GUI_SEP;
		NWGui::DragValue("End scale", &prop.eScale.x, ImGuiDataType_Float, 2);
		GUI_SEP;
		NWGui::DragValue("Scale variation duration", &prop.scaleVarDuration, ImGuiDataType_Float);
		GUI_SEP;
		NWGui::DragValue("Starting Alpha", &prop.sAlpha, ImGuiDataType_Float);
		GUI_SEP;
		NWGui::DragValue("End Alpha", &prop.eAlpha, ImGuiDataType_Float);
		GUI_SEP;
		NWGui::DragValue("Alpha variation duration", &prop.alphaVarDuration, ImGuiDataType_Float);
		GUI_SEP
		NWGui::DragValue("Starting speed", &prop.sSpeed, ImGuiDataType_Float);
		GUI_SEP;
		NWGui::DragValue("End speed", &prop.eSpeed, ImGuiDataType_Float);
		GUI_SEP;
		NWGui::DragValue("speed variation duration", &prop.speedVarDuration, ImGuiDataType_Float);
		GUI_SEP; GUI_NEWLINE;
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Emission properties")) {
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
	WRITE_ON_BIN(data, &(prop.lifetime), sizeof(prop.lifetime), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.lifedistance), sizeof(prop.lifedistance), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.sPosition.x), sizeof(prop.sPosition.x), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.sPosition.y), sizeof(prop.sPosition.y), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.sDirection.x), sizeof(prop.sPosition.x), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.sDirection.y), sizeof(prop.sDirection.y), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.eDirection.x), sizeof(prop.eDirection.x), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.directionVarDuration), sizeof(prop.directionVarDuration), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.sScale.x), sizeof(prop.sPosition.x), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.sScale.y), sizeof(prop.sPosition.y), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.eScale.x), sizeof(prop.eScale.x), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.eScale.y), sizeof(prop.eScale.y), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.eScale.x), sizeof(prop.eScale.x), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.scaleVarDuration), sizeof(prop.scaleVarDuration), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.sSpeed),	 sizeof(prop.sSpeed), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.eSpeed),	 sizeof(prop.eSpeed), sizeBuffer);
	WRITE_ON_BIN(data, &(prop.speedVarDuration), sizeof(prop.speedVarDuration), sizeBuffer);
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
	READ_FROM_BIN(data, &(prop.sDirection.x), sizeBuffer);
	READ_FROM_BIN(data, &(prop.sDirection.y), sizeBuffer);
	READ_FROM_BIN(data, &(prop.eDirection.x), sizeBuffer);
	READ_FROM_BIN(data, &(prop.directionVarDuration), sizeBuffer);
	READ_FROM_BIN(data, &(prop.sScale.x), sizeBuffer);
	READ_FROM_BIN(data, &(prop.sScale.y), sizeBuffer);
	READ_FROM_BIN(data, &(prop.eScale.x), sizeBuffer);
	READ_FROM_BIN(data, &(prop.eScale.y), sizeBuffer);
	READ_FROM_BIN(data, &(prop.eScale.x), sizeBuffer);
	READ_FROM_BIN(data, &(prop.scaleVarDuration), sizeBuffer);
	READ_FROM_BIN(data, &(prop.sSpeed), sizeBuffer);
	READ_FROM_BIN(data, &(prop.eSpeed), sizeBuffer);
	READ_FROM_BIN(data, &(prop.speedVarDuration), sizeBuffer);
	
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
