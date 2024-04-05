#pragma once
#include "GameObject.h"

/**
 * @brief The Collider class represents a base collider component.
 */
class Collider : public GameComponent {
private:

public:
	/**
  * @brief Get the type of the collider.
  * @return The type of the collider.
  */
	static std::string GetType() { return "Collider"; }

	GameObject* attachedObj;

	fVec2 offset = fVec2(0.0f, 0.0f);

	/**
  * @brief Get the position of the collider.
  * @return The position of the collider.
  */
	fVec2 GetPosition();

	/**
  * @brief Get the position of a specific edge of the collider.
  * @param index The index of the edge.
  * @return The position of the edge.
  */
	fVec2 GetEdgePosition(int index);

	Collider() {};
	Collider(GameObject* go);

	std::vector<fVec2> edges = { fVec2(100,100), fVec2(-100,100), fVec2(-100,-100), fVec2(100,-100) }; // Trigonometric orientation

	/**
  * @brief Check if this collider is colliding with another collider using the Separating Axis Theorem (SAT).
  * @param other The other collider to check against.
  * @param depthBuffer A pointer to a vector to store the collision depth.
  * @return True if the colliders are colliding, false otherwise.
  */
	bool Sat(Collider* other, fVec2* depthBuffer = nullptr);

	/**
  * @brief Check if this collider is colliding with another collider.
  * @param other The other collider to check against.
  * @param depthBuffer A pointer to a vector to store the collision depth.
  * @return True if the colliders are colliding, false otherwise.
  */
	bool isColliding(Collider* other, fVec2* depthBuffer = nullptr);

	void Start() override;

	int Serialize(std::fstream* data, int offset) override;

	int Deserialize(std::fstream* data, int offset) override;
};
