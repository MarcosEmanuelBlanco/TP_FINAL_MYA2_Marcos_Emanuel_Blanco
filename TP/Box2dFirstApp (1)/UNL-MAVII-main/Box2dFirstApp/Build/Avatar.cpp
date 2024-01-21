#include "Avatar.h"

Avatar::Avatar(b2Body* avatarBody, RectangleShape* form) : BodyAvatar(avatarBody), FormaAvatar(form) { // El body y la forma que use el avatar se corresponderán con los de los parámetros.
	pos = avatarBody->GetPosition();
	size.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);	//} Definición de límites de la caja de colisión alineada con los ejes (AABB).
	size.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);		//}

	for (b2Fixture* f = avatarBody->GetFixtureList(); f; f = f->GetNext()) {
		size = f->GetAABB(0);	// Asignación de la AABB provista por el body de los parámetros al avatar.
	}

	FormaAvatar->setSize({ size.GetExtents().x * 2, size.GetExtents().y * 2 });				//}
	FormaAvatar->setOrigin(FormaAvatar->getSize().x / 2.f, FormaAvatar->getSize().y / 2.f);	//} Configuración de la forma de SFML del avatar.
	FormaAvatar->setPosition(pos.x, pos.y);													//}
	FormaAvatar->setRotation(RadianesAGrados(avatarBody->GetAngle()));						//}

}

Avatar::~Avatar(void) {

}

float Avatar::RadianesAGrados(float radianes) { // Conversión para comunicar cambios de ángulos entre Box2D y SFML.
	return radianes * 180 / 3.14f;
}

void Avatar::DibujarAvatar(RenderWindow& wnd) {
	pos = BodyAvatar->GetPosition();									//}
	FormaAvatar->setPosition(pos.x, pos.y);								//} Posicionamiento y dibujado del avatar en la escena.
	FormaAvatar->setRotation(RadianesAGrados(BodyAvatar->GetAngle()));	//}
	wnd.draw(*FormaAvatar);												//}
}

