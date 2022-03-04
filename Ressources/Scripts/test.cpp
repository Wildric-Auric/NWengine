#include "test.h"
#include "NWengine.h"
#include "Utilities.h"
extern Collider groundCollider[20];

void test::Update() {
	isGrounded = 0;
	static float maxJump = 0.0f;
	//yspd *= 1-isGrounded;
	if (isGrounded && input_space) {
		isJumping = 1;
		jumpingStartPosition = goc->position.y;
		goc->position.y += 1;
		maxJump = -jumpTime / 2 * (jumpTime / 2 - jumpTime);
	}

	if (isJumping) {
		timer += deltaTime;

		yspd = -(jumpHeight / maxJump) * timer * (timer - jumpTime);
		if (timer > jumpTime) {
			yspd = 0.0f;
			isJumping = false;
			timer = 0.0f;
		}
		goc->position.y = jumpingStartPosition + yspd;
	}
	goc->position.x -= isRunning * 200 * deltaTime;
	goc->scale.x = sign(isRunning);


	float a = joystickAxis[0];
	isRunning = Clamp((float)(input_right - input_left) + a, -1.0f, 1.0f);
}