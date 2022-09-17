#include "ParticleSystem.h"
ParticleSystem::ParticleSystem(GameObject* attachedObj) {
	this->attachedObj = attachedObj;
}

std::map<GameObject*, ParticleSystem> ParticleSystem::componentList;

void ParticleSystem::Update() {
	clock += Globals::deltaTime;
	prop.absoluteStartPosition = attachedObj->GetComponent<Transform>()->position; //TODO::Not using GetComponent only once and make sure transofrm exists
	if (clock >= emissionFrequency) Emit();
	auto it = enabled.begin();
	while (it != enabled.end()) {
		int index = *it;
		Particle* particle = &pool[index];
		if (particle->prop.speedVarDuration >= 0.0000001) {
			double temp = particle->clock / particle->prop.speedVarDuration;
			if (abs(temp) < 0.999995)
				particle->currentSpeed = lerp<float, double>(particle->prop.sSpeed, particle->prop.eSpeed,
															temp);
		}

		if (particle->prop.scaleVarDuration >= 0.0000001) {
			double temp = particle->clock / particle->prop.scaleVarDuration;
			if (abs(temp) < 0.9999995)
				particle->currentScale = lerpVector2<float, double>(particle->prop.sScale, particle->prop.eScale,
											Vector2<double>(temp, temp));
		}

		if (particle->prop.directionVarDuration >= 0.0000001) {
			double temp = particle->clock / particle->prop.directionVarDuration;
			if (abs(temp) < 0.999995)
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
	currentDirection = prop.sDirection;
	isActive = 1;
	go.AddToRender();
}