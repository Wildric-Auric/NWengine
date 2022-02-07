#pragma once
#include "GameObject.h"
bool IsColliding(Collider* collider1, Collider* collider2, Vector2<int> offset1 = Vector2<int>(0,0)) {
	uint8_t overlap = 0;
	int minX1 = collider1->GetPosition().x + offset1.x -  collider1->size->x / 2;
	int maxX1 = collider1->GetPosition().x + offset1.x +  collider1->size->x/2;
	int minY1 = collider1->GetPosition().y + offset1.y -  collider1->size->y/2;
	int maxY1 = collider1->GetPosition().y + offset1.y +  collider1->size->y/2;

	int minX2 = collider2->GetPosition().x - collider2->size->x / 2;
	int maxX2 = collider2->GetPosition().x + collider2->size->x / 2;
	int minY2 = collider2->GetPosition().y - collider2->size->y / 2;
	int maxY2 = collider2->GetPosition().y + collider2->size->y / 2;

	overlap += (  
					( (minX1 < minX2) && (minX2 < maxX1) ) ||  
					( (minX2 < minX1) && (minX1 < maxX2) ) 
			   )
			+ 
			   ( 
					 ( (minY1 < minY2) && (minY2 < maxY1) ) ||
					 ( (minY2 < minY1) && (minY1 < maxY2) )
			   )
		;
	return overlap == 2;
}

GameObject CloneOnStack(GameObject* obj) {
	GameObject cloned = *obj;
	return cloned;
}
