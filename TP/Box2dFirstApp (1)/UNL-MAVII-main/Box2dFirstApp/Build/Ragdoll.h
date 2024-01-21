#pragma once
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "../Build/Avatar.h"
using namespace sf;

class Ragdoll {
public:
	Ragdoll(Vector2f pos, b2World& world);
	void DibujarRagdoll(RenderWindow* window);
	void AplicarFuerza(Vector2f mousePos);

	Vector2f getPosition(int i) const {
		b2Vec2 position = ragPart[i]->GetPosition();	// Obtiene la posición de cualquier parte del ragdoll, utilizado para detectar la colisión.
		return Vector2f(position.x, position.y);
	}

	Vector2f getSize(int i) const {
		b2PolygonShape* shape = static_cast<b2PolygonShape*>(fixPart[i]->GetShape());	// Obtiene las dimensiones de cualquier parte del ragdoll, utilizado para detectar la colisión.
		b2Vec2 size = shape->m_vertices[2] - shape->m_vertices[0];
		return Vector2f(size.x, size.y);
	}
protected:
	RectangleShape* figRag[6];
	Avatar* partAvatar[6];

	b2DistanceJoint* jntRag[5];
	b2DistanceJointDef jntDefRag[5];

	b2Body* ragPart[6];
	b2BodyDef ragPartDef[6];
	b2Fixture* fixPart[6];
	b2FixtureDef fixDefPart[6];
};