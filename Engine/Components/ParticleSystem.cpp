//TODO::
//With babtching it is observed that rendering 1000 particle does not slow the game
//However when particles start to be recycled the fps drops to 30, something wrong in cpu level; fix that
#include "ParticleSystem.h"
#include "Scene.h"
#include "Utilities.h"
#include "NWTime.h"

ParticleSystem::ParticleSystem(GameObject* obj) {
	this->attachedObject = obj;
}

void ParticleSystem::Update() {
	clock += NWTime::GetDeltaTime();
	prop.absoluteStartPosition = attachedObject->AddComponent<Transform>()->position; //TODO::Not using GetComponent only once and make sure transofrm exists
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
#undef UPDATE_ATTRIBUTE

		particle->currentPosition = particle->currentPosition +  particle->currentDirection.normalize() * NWTime::GetDeltaTime() * particle->currentSpeed;
		particle->distance = (particle->currentPosition - particle->prop.sPosition).magnitude();
		particle->clock += NWTime::GetDeltaTime();
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
		(pool.back().sprite = pool.back().go.AddComponent<Sprite>())->Batch();
	}
}

void ParticleSystem::UpdateParticle(int index) {
	Transform* transform = attachedObject->AddComponent<Transform>();
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

ParticleSystem::~ParticleSystem() {
	//TODO::Better solution
	for (int i = 0; i < pool.size(); i++) {
		pool[i].Disable();
		pool[i].go.DeleteComponents();
	}
	Scene::currentScene->ForceRenderStop();
}
