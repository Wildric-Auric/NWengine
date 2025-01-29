#include "Scripts.h"
#include "DefaultAssets.h"
#include "NWTime.h"
#include "Scene.h"
#include <random>



static uint32 salt = 0;
static NWin::Timer timer;

static double randomVal() {
	std::srand( timer.getTime() + salt);
	salt += 50.0;
	return (double)std::rand() / (double)RAND_MAX;
}

void Spawner::Start() {
	timer.initialize();

	auto tr      = goc->AddComponent<Transform>();
	tr->position.x   = 250.0;
	tr->position.y   = 50.0;

	range.x          = 90.0;
	range.y          = 90.0;
	maxLeaves        = 80;
	launchPeriod     = 600.0; //Millisec

	_count           = 0;
	_counter         = 0;

}

void Spawner::Update() {
	float dt = NWTime::GetDeltaTime();
	auto tr = goc->AddComponent<Transform>();

	_counter = Max(0.0, _counter - dt * 1000.0);
	if (_counter != 0.0)
		return;

	if (_count >= maxLeaves)
		return;

	//Launch a leaf
	double rx = randomVal() - 0.5;
	double ry = randomVal() - 0.5;

	auto& obj = Scene::GetCurrent()->AddObject();
	Scene::GetCurrent()->Rename("l", &obj);

	obj.AddComponent<Script>()->SetScript<Leaf>();
	auto& l = (Leaf&)*obj.GetComponent<Script>()->script;
	l.Start();
	l.tr->position.x = range.x * rx + tr->position.x;
	l.tr->position.y = range.y * ry + tr->position.y;
	l.spawner = this;
	l._launchTime = NWTime::_currentTime - randomVal() * 900.0;
	_count += 1;
	_counter = launchPeriod;
}

void Leaf::Start() {
	tr  = this->goc->AddComponent<Transform>();
	spr = this->goc->AddComponent<Sprite>();
	spr->SetShader("../Sandbox/assets/Shaders/Leaf.shader");
	spr->SetTexture(&ImageDefault, &TextureDefaultID);	
	spr->SetSortingLayer(LEAVE_LAYER);
	tr->scale = tr->scale * 0.15;
	spr->vertexAttributes.x = randomVal();
}

void Leaf::Air(float dt) {
	float a;
	if ( ( a = randomVal() ) < 0.05)
		dragRandomVal   = randomVal();
	fVec2 drag      = fVec2(-50.0 + (randomVal() - 0.5)  * 10.0, dragRandomVal * 500.0) * dt;
	float grav		= -dt * (30.0 + (randomVal() - 0.5 ) * 10.0);
	tr->position.y +=  grav + drag.y;
	tr->position.x += drag.x;
}

void Leaf::Water(float dt) {

}

void Leaf::Update() {
	float dt  = NWTime::GetDeltaTime();
	Air(dt);
	//Out of bound
	if (tr->position.x > Camera::ActiveCamera->size.x * 0.5
		|| tr->position.x < -Camera::ActiveCamera->size.x * 0.5
		|| tr->position.y > Camera::ActiveCamera->size.y * 0.5
		|| tr->position.y < -Camera::ActiveCamera->size.y * 0.5)
	{
		Scene::GetCurrent()->DeleteCurrentObj();
		spawner->_count -= 1;
	}
}