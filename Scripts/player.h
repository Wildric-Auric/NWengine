#include "GameObject.h"
#include "Utilities.h"
#include "Inputs.h"

extern Collider groundCollider[20];

class player : Updatable {
public:
	float isRunning = 0.0f;
	int isJumping = 0;
	int isGrounded = 1;
	float jumpingStartPosition = 0.0f;
	float jumpTime = 0.8f;
	float timer = 0.0f;
	float jumpHeight = 50.0f;
	GameObjectClone* goc;
	float yspd = 0.0f;
	player(GameObjectClone* goc) {
		this->goc = goc;
		goc->AddComponent<Collider>();
	};
	void Update() {

		isGrounded = 0;
		for (int i = 0; i < 15; i++) {
			while (IsColliding(goc->GetComponent<Collider>(), &groundCollider[i])) {
				goc->position.y += 1;
				//DevNote::By setting up the collider offset I understood finally what Shaun Spalding used to do with placemeeting...
			
			};
			if (IsColliding(goc->GetComponent<Collider>(), &groundCollider[i], Vector2<int>(0, -1))) {
				isGrounded = 1;
				break;
			}
		}
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
		goc->position.x += isRunning * 200 * deltaTime;
		goc->scale.x = sign(isRunning);


		float a = joystickAxis[0];
		isRunning = Clamp((float)(input_right - input_left) + a, -1.0f, 1.0f);
	};
};