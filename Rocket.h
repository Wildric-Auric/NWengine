#include "Script.h"

struct phyData {
	float lastAcc = 0;
	float lastVel = 0;
	float vel = 0;
	float acc = 0;
	float mass = 10.0;
	float pos = 0;
	float lastPos = 0;
};

;


class Rocket : public Scriptable {
public:
	SCRIPT_CONSTR(Rocket)
	phyData data{};
	void Start()  override;
	void Update() override;


};