//#pragma once
//#include "Maths.h"
//#include "GameObject.h"
//#include "vector"
//
//#define MAX_PARTICLES 50
//
//
//
//struct Properties {
//	float emissionTime = 1.5f;
//	int emissionNum = 1;
//	float lifeTime = 1.0f;
//	float lifeDistance = 200.0f;
//	iVec2 position = iVec2(0,0);
//	GameObjectClone goc;
//	Vector2<Vector2<int>> velocity;
//	Vector2<Vector3<int>> color;
//
//	Vector2<fVec3>  velocityInterpolation;
//	Vector2<float> colorInterpolation;
//};
//
//
//
//class ParticleSystem  {
//private:
//	float _emissionTime;
//public:
//
//	Properties properties;
//	//bool isActive = true;
//	int currentIndex = 0;
//
//	ParticleSystem(Properties properties) {
//		this->properties = properties;
//	};
//
//
//	GameObject* OriginalGameObject;
//
//	//void Start();
//
//	void Update();
//
//	void Emit(int emission = 1);
//
//	static void Draw();
//
//};
//
//
//static class Particle {
//private:
//	ParticleSystem* ps;
//	float _lifetime;
//	float _lifeDistance;
//	Vector2<int> position;
//public:
//	bool isActive = false;
//	void Activate(ParticleSystem* ps);
//	void Update();
//
//	static Particle pool[MAX_PARTICLES];
//};