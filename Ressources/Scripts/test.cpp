#include "test.h"
#include "NWengine.h"
#include "Utilities.h"
extern Collider groundCollider[20];

void test::Update() {
	isGrounded = 0;
	static float maxJump = 0.0f;
	iVec2 position = goc->GetComponent<Transform>()->position; //Called everyframe should fix this by calling it only during start()
	fVec2 scale = goc->GetComponent<Transform>()->scale;
	//yspd *= 1-isGrounded;
	if (isGrounded && Inputs::space) {
		isJumping = 1;
		jumpingStartPosition = position.y;
		position.y += 1;
		maxJump = -jumpTime / 2 * (jumpTime / 2 - jumpTime);
	}

	if (isJumping) {
		timer += Globals::deltaTime;

		yspd = -(jumpHeight / maxJump) * timer * (timer - jumpTime);
		if (timer > jumpTime) {
			yspd = 0.0f;
			isJumping = false;
			timer = 0.0f;
		}
		position.y = jumpingStartPosition + yspd;
	}
	position.x -= isRunning * 200 * Globals::deltaTime;
	scale.x = sign(isRunning);


	float a = Inputs::joystickAxis[0];
	isRunning = Clamp((float)(Inputs::right - Inputs::left) + a, -1.0f, 1.0f);
}