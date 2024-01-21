#include "Ragdoll.h"

Ragdoll::Ragdoll(Vector2f pos, b2World& world) { // La posición y mundo en los parámetros se usan en el momento del disparo.
	ragPartDef[0].position = b2Vec2(pos.x, pos.y - 7.5f); // Cabeza
	ragPartDef[1].position = b2Vec2(pos.x, pos.y + 3.25f); // Torso
	ragPartDef[2].position = b2Vec2(pos.x + 1.75, pos.y + 1.0f); // Brazo Derecho
	ragPartDef[3].position = b2Vec2(pos.x - 1.75, pos.y + 1.0f); // Brazo Izquierdo
	ragPartDef[4].position = b2Vec2(pos.x + 1.75, pos.y + 25.0f); // Pierna Derecha
	ragPartDef[5].position = b2Vec2(pos.x - 1.75, pos.y + 25.0f); // Pierna Izquierda

	for (int i = 0; i < 6; i++)
	{
		ragPartDef[i].type = b2_dynamicBody;			//Se establece que las partes sean del tipo dinámico.
		ragPart[i] = world.CreateBody(&ragPartDef[i]); // Se le indica al mundo del parámetro que cree las partes.

	}

	b2PolygonShape ragShape[6];

	// Dimensiones del Cuerpo
	ragShape[0].SetAsBox(5.0f, 5.0f); // Eje x, Eje y.
	ragShape[1].SetAsBox(5.0f, 10.0f);
	ragShape[2].SetAsBox(1.75f, 7.5f);
	ragShape[3].SetAsBox(1.75f, 7.5f);
	ragShape[4].SetAsBox(1.75f, 10.0f);
	ragShape[5].SetAsBox(1.75f, 10.0f);

	for (int i = 0; i < 6; i++)
	{
		fixDefPart[i].shape = &ragShape[i];	//}
		fixDefPart[i].density = 2.0f;		//} Se fijan los parámetros para las partes.
		fixDefPart[i].friction = 0.3f;		//}
		fixDefPart[i].restitution = 0.0f;	//}

		fixPart[i] = ragPart[i]->CreateFixture(&fixDefPart[i]); //Se crean las partes con los parámetros.


	}
	// Se crean las uniones:
	jntDefRag[0].Initialize(ragPart[0], ragPart[1],									// Cuello
		b2Vec2(ragPart[0]->GetPosition().x, ragPart[0]->GetPosition().y),
		b2Vec2(ragPart[1]->GetPosition().x, ragPart[1]->GetPosition().y - 9.75));

	jntDefRag[1].Initialize(ragPart[1], ragPart[2],									// Hombro derecho
		b2Vec2(ragPart[1]->GetPosition().x + 5, ragPart[1]->GetPosition().y - 10),
		b2Vec2(ragPart[2]->GetPosition().x, ragPart[2]->GetPosition().y - 7.5));

	jntDefRag[2].Initialize(ragPart[1], ragPart[3],									// Hombro izquierdo
		b2Vec2(ragPart[1]->GetPosition().x - 5, ragPart[1]->GetPosition().y - 10),
		b2Vec2(ragPart[3]->GetPosition().x, ragPart[3]->GetPosition().y - 7.5));

	jntDefRag[3].Initialize(ragPart[1], ragPart[4],									// Cadera derecha
		b2Vec2(ragPart[1]->GetPosition().x, ragPart[1]->GetPosition().y + 10),
		b2Vec2(ragPart[4]->GetPosition().x, ragPart[4]->GetPosition().y - 10));

	jntDefRag[4].Initialize(ragPart[1], ragPart[5],									// Cadera izquierda
		b2Vec2(ragPart[1]->GetPosition().x, ragPart[1]->GetPosition().y + 10),
		b2Vec2(ragPart[5]->GetPosition().x, ragPart[5]->GetPosition().y - 10));

	for (int i = 0; i < 5; i++)
	{
		jntDefRag[i].stiffness = 40.0f;									//}
		jntDefRag[i].damping = 0.3f;									//} Se fijan los parámetros de las uniones y se las crea.
		jntDefRag[i].collideConnected = true;							//}
		jntDefRag[i].length = 0.1f;										//}
		jntRag[i] = (b2DistanceJoint*)world.CreateJoint(&jntDefRag[i]); //}
	}

	for (int i = 0; i < 6; i++)
	{
		figRag[i] = new RectangleShape;	// Se asigna el tipo de figura de SFML que se usará.


	}
	figRag[0]->setFillColor(Color::Blue); // Cabeza
	figRag[1]->setFillColor(Color::White);// Resto del cuerpo
	figRag[2]->setFillColor(Color::White);
	figRag[3]->setFillColor(Color::White);
	figRag[4]->setFillColor(Color::White);
	figRag[5]->setFillColor(Color::White);

	for (int i = 0; i < 6; i++) {
		partAvatar[i] = new Avatar(ragPart[i], figRag[i]); // Se asigna al avatar el body y las figuras de SFML creadas.
	}
}

void Ragdoll::DibujarRagdoll(RenderWindow* window) {
	for (int i = 0; i < 6; i++) {
		partAvatar[i]->DibujarAvatar(*window); // Se usa el método de Avatar para dibujarlo, pero desde el ragdoll mismo.
	}
}

void Ragdoll::AplicarFuerza(Vector2f mousePos) {
		ragPart[0]->ApplyForceToCenter(b2Vec2(mousePos.x * 2000, mousePos.y * 2000), false); // Se aplica fuerza a la cabeza. La posición en el parámetro se toma al momento del disparo para calcular el ángulo.
}

