#include "Avatar.h"

Avatar::Avatar(b2Body* avatarBody, RectangleShape* form) : BodyAvatar(avatarBody), FormaAvatar(form) { // El body y la forma que use el avatar se corresponder�n con los de los par�metros.
	pos = avatarBody->GetPosition();
	size.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);	//} Definici�n de l�mites de la caja de colisi�n alineada con los ejes (AABB).
	size.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);		//}

	for (b2Fixture* f = avatarBody->GetFixtureList(); f; f = f->GetNext()) {
		size = f->GetAABB(0);	// Asignaci�n de la AABB provista por el body de los par�metros al avatar.
	}

	FormaAvatar->setSize({ size.GetExtents().x * 2, size.GetExtents().y * 2 });				//}
	FormaAvatar->setOrigin(FormaAvatar->getSize().x / 2.f, FormaAvatar->getSize().y / 2.f);	//} Configuraci�n de la forma de SFML del avatar.
	FormaAvatar->setPosition(pos.x, pos.y);													//}
	FormaAvatar->setRotation(RadianesAGrados(avatarBody->GetAngle()));						//}

}

Avatar::~Avatar(void) {

}

float Avatar::RadianesAGrados(float radianes) { // Conversi�n para comunicar cambios de �ngulos entre Box2D y SFML.
	return radianes * 180 / 3.14f;
}

void Avatar::DibujarAvatar(RenderWindow& wnd) {
	pos = BodyAvatar->GetPosition();									//}
	FormaAvatar->setPosition(pos.x, pos.y);								//} Posicionamiento y dibujado del avatar en la escena.
	FormaAvatar->setRotation(RadianesAGrados(BodyAvatar->GetAngle()));	//}
	wnd.draw(*FormaAvatar);												//}
}

