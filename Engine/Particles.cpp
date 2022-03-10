#include "Particles.h"
#include "NWengine.h"

Particle Particle::pool[MAX_PARTICLES] = {Particle()};

void ParticleSystem::Update() {
	_emissionTime += deltaTime;
	if (_emissionTime >= properties.emissionTime) {
		_emissionTime = 0.0f;
		Emit(properties.emissionNum);
	}
};

void ParticleSystem::Emit(int emissionNum) {
	for (int i = 0; i < emissionNum; i++) {
		Particle::pool[currentIndex].Activate(this);
		properties.position.x += 5 * currentIndex;
		currentIndex += 1;
		if (currentIndex > MAX_PARTICLES - 1) currentIndex = 0;
	}
}

void Particle::Activate(ParticleSystem* ps) {
	this->ps = ps;
	isActive = true;
	_lifetime = ps->properties.lifeTime;
	_lifeDistance = ps->properties.lifeDistance;
	position = ps->properties.position;
}

void Particle::Update() {
	_lifetime -= deltaTime;
	if (_lifetime <= 0) isActive = false;

	ps->properties.goc.position = position;
	ps->properties.goc.sortingLayer = 100;

	Shader* last = ps->properties.goc.originalGameObject->shader; //Just testing for now, not optimized

	ps->properties.goc.originalGameObject->shader = &shaders["shader_particle"];
	glUseProgram(shaders["shader_particle"].shaderProgram); //TODO:: Fix gl functions calls; no one of these should be called inside highlevel NWengine code
	ps->properties.goc.originalGameObject->texture->Bind(0);
	ps->properties.goc.originalGameObject->shader->SetUniform1f("uA", _lifetime);
	ps->properties.goc.originalGameObject->shader->SetUniform1i("uTex0", 0);
	ps->properties.goc.BasicDraw(0);
	ps->properties.goc.originalGameObject->shader = last;
}


void ParticleSystem::Draw() {
	for (int i = 0; i < MAX_PARTICLES; i++) {
		if (Particle::pool[i].isActive) Particle::pool[i].Update();
	}
}