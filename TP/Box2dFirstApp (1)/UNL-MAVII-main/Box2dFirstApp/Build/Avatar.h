#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
using namespace sf;
class Avatar {
public:
	Avatar(b2Body* avatarBody, RectangleShape* form);
	~Avatar(void);
	float RadianesAGrados(float radians);
	void DibujarAvatar(RenderWindow& wnd);
protected:
	b2Body* BodyAvatar;
	RectangleShape* FormaAvatar;
	b2AABB size;
	b2Vec2 pos;
};