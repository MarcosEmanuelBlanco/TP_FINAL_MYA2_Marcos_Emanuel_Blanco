#pragma once
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "../Build/Avatar.h"
using namespace sf;

class Ragdoll {
public:
	Ragdoll(Vector2f pos, b2World& world);
	void drawRagdoll(RenderWindow* window);
	void applyForce(Vector2f mousePos);
	float rad2deg(float radians);
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