#include "player.h"
#include "NWengine.h"
#include "Utilities.h"


void player::Update() {
	isGrounded = 0;
	int vCol = 0;
	int direction = (input_right - input_left);
	for (auto it = Collider::componentList.begin(); it!= Collider::componentList.end(); it++) {
		if (it->first != goc) {
			if (direction != 0) {
				if (IsColliding(goc->GetComponent<Collider>(), &it->second, Vector2<int>(direction, 0))) {
					while (IsColliding(goc->GetComponent<Collider>(), &it->second)) {
						goc->position.x -= direction;
					};
					vCol = direction;
				};
			}

			if (IsColliding(goc->GetComponent<Collider>(), &it->second, Vector2<int>(0, -1))) {
				while (IsColliding(goc->GetComponent<Collider>(), &it->second)) {
					goc->position.y += 1;
				};
				isGrounded = 1;
			}
		}
	}
	static float maxJump = 0.0f;

	if (isGrounded && input_space) {
		isJumping = 1;
		jumpingStartPosition = goc->position.y;
		goc->position.y += 1;
		//maxJump = -jumpTime / 2 * (jumpTime / 2 - jumpTime);
		
		//temp jump code
		yspd = 5;
		isGrounded = 0;
	}
	yspd -= deltaTime*30;
	yspd *= 1-isGrounded;

	goc->position.y += yspd;
	//if (isJumping) {
	//	timer += deltaTime;

	//	yspd = -(jumpHeight / maxJump) * timer * (timer - jumpTime);
	//	if (timer > jumpTime) {
	//		yspd = 0.0f;
	//		isJumping = false;
	//		timer = 0.0f;
	//	}
	//	goc->position.y = jumpingStartPosition + yspd;
	//}
	goc->position.x += isRunning * 200 * deltaTime;
	goc->scale.x = sign(isRunning);


	float a = joystickAxis[0];
	if (direction == vCol) direction = 0;
	isRunning = Clamp((float)direction + a, -1.0f, 1.0f);
}