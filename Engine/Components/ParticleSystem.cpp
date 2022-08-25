#include "ParticleSystem.h"
ParticleSystem::ParticleSystem(GameObject* attachedObj) {
	this->attachedObj = attachedObj;
}

std::map<GameObject*, ParticleSystem> ParticleSystem::componentList;

void ParticleSystem::Update() {
	int index0 = -1;
	clock += Globals::deltaTime;
	if (clock >= emissionFrequency) Emit();
	for (int index : enabled) {
		index0 += 1;
		Particle* particle = &pool[index];

		if (particle->prop.speedVarDuration <= 0.0001)
			particle->currentSpeed = particle->prop.sSpeed;
		else {
			double temp = particle->clock / particle->prop.speedVarDuration;
			if (abs(temp) < 0.995)
				particle->currentSpeed = lerp<float, double>(particle->prop.sSpeed, particle->prop.eSpeed,
															temp);
		}

		if (particle->prop.scaleVarDuration <= 0.0001)
			particle->currentScale = particle->prop.sScale;
		else {
			double temp = particle->clock / particle->prop.scaleVarDuration;
			if (abs(temp) < 0.995)
				particle->currentScale = lerpVector2<float, double>(particle->prop.sScale, particle->prop.eScale,
					temp);
		}

		if (particle->prop.directionVarDuration <= 0.0001)
			particle->currentDirection = particle->prop.sDirection;
		else {
			double temp = particle->clock / particle->prop.directionVarDuration;
			if (abs(temp) < 0.995)
				particle->currentDirection = lerpVector2<float, double>(particle->prop.sDirection, particle->prop.eScale, temp);
		}
		

		particle->currentPosition = particle->currentPosition +  particle->currentDirection.normalize() * Globals::deltaTime * particle->currentSpeed;
		particle->distance = (particle->currentPosition - particle->prop.sPosition).magnitude();
		particle->clock += Globals::deltaTime;
		if ((particle->clock >= particle->prop.lifetime) || (particle->distance >= particle->prop.lifedistance)) {
			enabled.erase(enabled.begin() + index0);
			disabled.push_back(index);
			particle->Disable();
		}
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
		if (pool.size() > maxParticles) {
			//Often, oldest particles would be at begining of enabled
			if (!recycle) return
			disabled.push_back(enabled.front());
			enabled.pop_front();
			InitParticle();
			continue;
		}
		Init();
	}
}

void ParticleSystem::InitParticle() {
		int temp = disabled.back();
		disabled.pop_back();
		enabled.push_back(temp);
		pool[temp].Enable();
		pool[temp].prop = prop;
		pool[temp].sprite->SetShader(shader);
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
	pool[index].transform->position = pool[index].currentPosition + transform->position;
	pool[index].transform->scale = pool[index].currentScale;
}

void Particle::Disable() {
	isActive = 0;
	go.StopRendering();
}

void Particle::Enable() {
	isActive = 1;
	go.AddToRender();
}