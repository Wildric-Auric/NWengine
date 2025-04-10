#pragma once
#include "GameObject.h"
#include <vector>
#include "ComponentTypes.h"

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

    NW_ST_GET_TYPE_IMPL(Collider);

    uint32      tag = 0;
	/**
     * @brief Position offset in pixels relaltive to Transform position
     */
	fVec2 offset = fVec2(0.0f, 0.0f); 

	/**
  * @brief Get the position of the collider.
  * @return The position of the collider.
  */
	fVec2 GetPosition();

	/**
  * @brief Get the position of a specific vertex of the collider.
  * @param index The index of the edge.
  * @return The position of the vertex.
  */
	fVec2 GetEdgePosition(int index);

	Collider() {};
	Collider(GameObject* go);

	/**
	 * @brief A vector that contains the data of vertex offset relative to Transform + offset member.
	 */
	std::vector<fVec2> edges = { fVec2(100,100), fVec2(-100,100), fVec2(-100,-100), fVec2(100,-100) }; // Trigonometric orientation

	/**
  * @brief Checks if this collider is colliding with another collider using the Separating Axis Theorem (SAT).
  * @note SAT is applied only on the current Collider, objects may collide even though return value 
  * is false. To get full collision info, you should call isColliding() instead.
  * @param other The other collider to check against.
  * @param depthBuffer A pointer to a vector to store the collision depth; the user should translate
  * the transform position using this vector so that colliding objects are separated.
  * If no collision is taking place, this vector is accessed or written on by this component.
  * @return True if the colliders are colliding, false otherwise.
  */
	bool Sat(Collider* other, fVec2* depthBuffer = nullptr);

	/**
  * @brief Higher level of SAT(), checks if this collider is colliding with another collider.
  * @param other The other collider to check against.
  * @param depthBuffer A pointer to a vector to store the collision depth.
  * @return True if the colliders are colliding, false otherwise.
  */
	bool isColliding(Collider* other, fVec2* depthBuffer = nullptr);

    /**
    * @brief checks if a point is inside the polygon
    */
    bool isInside(const fVec2& point);


	/**
  * @brief Sets edges according to rectangular shape.
  * @param size The size of the rectangle.
  * @param rotation The rotation of the box.
  */
    void SetEdgesRect(const fVec2& size, const float rotation); 

	/**
  * @brief Sets edges according to the sprite of the attached GameObject.
  */
    void SetEdgesSprite(); 

	void Start() override;
};


class CircleCollider: public GameComponent {
    public:
        float radius = 32.0f;
        NW_ST_GET_TYPE_IMPL(CircleCollider);
        CircleCollider() {};
        CircleCollider(GameObject* obj) {attachedObject = obj;}
        float Sat(const fVec2& axis, const fVec2& origin, Collider*);
        bool isColliding(Collider* other, fVec2* depthBuffer = nullptr);
        bool isColliding(CircleCollider* other, fVec2* depthBuffer = nullptr);
        bool isInside(const fVec2&);

        void SetRadius(float);
        fVec2 GetPosition();
        void Start();
};
