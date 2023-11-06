#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include "Transform.h"
#include "Globals.h"
#include "Interpolation.h"
#include <deque>

#define SHADER_PARTICLES_DEFAULT  "Ressources\\Shaders\\ParticleBatched.shader"
#define TEXTURE_PARTICLES_DEFAULT "Ressources\\Images\\DefaultBox10x10.png"

struct ParticleProperties {
	fVec2 absoluteStartPosition = fVec2(0.0f, 0.0f); //Read only; buffer of position  used to pass beginning absolute position to particle

	float lifetime = 1.0f;
	int lifedistance = 1000; //in pixels
	//s stands for start and e for end; notice that position is relative to particle system position
	fVec2 sPosition = fVec2(0.0f,0.0f); 
	
	BezierInterpolator       directionX = BezierInterpolator(0.0f, 0.0f, 0.0f);
	BezierInterpolator       directionY = BezierInterpolator(1.0f, 1.0f, 0.0f);
	BezierInterpolator       scaleX	    = BezierInterpolator(1.0f, 1.0f, 0.0f);
	BezierInterpolator       scaleY     = BezierInterpolator(1.0f, 1.0f, 0.0f);	

	BezierInterpolator       colorX     = BezierInterpolator(1.0f, 1.0f, 0.0f);
	BezierInterpolator       colorY     = BezierInterpolator(1.0f, 1.0f, 0.0f);
	BezierInterpolator       colorZ     = BezierInterpolator(1.0f, 1.0f, 0.0f);
	BezierInterpolator       colorA     = BezierInterpolator(1.0f, 1.0f, 0.0f);
	
	BezierInterpolator       speed      = BezierInterpolator(100.0f, 0.0f, 0.0f);
};

class Particle {	
private:
public:
	GameObject go;
	Sprite* sprite;
	Transform* transform;
	bool isActive = 0;
	ParticleProperties prop;
	fVec2 currentPosition;
	fVec2 currentScale;
	float currentSpeed;
	fVec4 currentColor;
	fVec2 currentDirection;

	double clock = 0.0;
	float distance = 0.0;

	void Disable();
	void Enable();
};


class ParticleSystem : public GameComponent {
private:
	double clock = 0.0;
public:
	static std::string GetType() { return "ParticleSystem"; };

	GameObject* attachedObj = nullptr;
	ParticleSystem() {};
	~ParticleSystem();
	ParticleSystem(GameObject* attachedObj);
    static std::map<GameObject*, ParticleSystem> componentList;

	bool isActive = true;
	std::deque<Particle> pool;
	std::deque<int> disabled;
	std::deque<int> enabled;
	int maxParticles = 100;

	double emissionFrequency = 1.0; //every emissionf Emission is called
	uint16 emissionQuantity =  1;
	int initNum = 5;
	ParticleProperties prop;
	bool recycle = 1;
	std::string shader  =	SHADER_PARTICLES_DEFAULT;
	std::string texture =   TEXTURE_PARTICLES_DEFAULT;

	void Update();
	void UpdateParticle(int index);
	void Emit();
	void Init();
	void InitParticle();

	OVERRIDE_GUI
	int  Serialize(std::fstream* data, int offset)   override; 
	int  Deserialize(std::fstream* data, int offset) override;
};