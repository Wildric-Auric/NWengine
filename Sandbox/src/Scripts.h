#pragma once

#include "Script.h"
#include "Components.h"
#include "nwin/timer.h"




#define BG_LAYER 500
#define TREE_LAYER 5
#define TEXT_LAYER 50
#define GROUND_LAYER 5
#define LEAVE_LAYER  -6000
#define WATER_LAYER  -5000


class GameManager : public Scriptable {
public:
	SCRIPT_CONSTR(GameManager)
	void Start()  override;
	void Update() override;
};


class CameraScript: public Scriptable {
public:
	MSAAValue msaa = MSAAValue::NW_MSx8;
	SCRIPT_CONSTR(CameraScript)
	void Start()  override;
	void Update() override;
};

class WaterScript : public Scriptable {
public:
	SCRIPT_CONSTR(WaterScript)
	void Start()  override;
	void Update() override;
};

class Spawner: public Scriptable {
public:
	SCRIPT_CONSTR(Spawner)

	uint32 _count;
	double _counter;

	fVec2        range;
	uint32       maxLeaves;
	NWin::timeMl launchPeriod;

	void Start()  override;
	void Update() override;
};

class Leaf : public Scriptable {
public:
	GameObject* goc; std::string __nwname = "Leaf"; std::string GetName() {
    return __nwname;
}; static Scriptable* GetScript(GameObject* goc); Leaf(GameObject* goc) {
    this->goc = goc;
};

	NWin::timeMl _launchTime = 0;
	Spawner* spawner = nullptr;
	Transform* tr;
	Sprite* spr;
	float dragRandomVal = 0.0;

	void Air(float dt);
	void Water(float dt);
	void Start()  override;
	void Update() override;
};



