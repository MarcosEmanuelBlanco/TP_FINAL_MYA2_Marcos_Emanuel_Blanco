#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

Game::Game(int ancho, int alto, std::string titulo) {
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	wnd->setMouseCursorVisible(false);
	fps = 60;
	tiempo2 = 0;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	fuente.loadFromFile("assets/powerbroker.ttf");
	evento = new Event;
	reloj = new Clock;
	tiempo = new Time;
	habilitarDisparo = false;
	FijarZoom();
	IniciarJuego();
}

void Game::IniciarJuego() {
	InicializarFisicas();
	CargarGraficos();
	ConfigurarTextos();
	avTecho = new Avatar(BodyTecho, FormaTecho);				//}
	avPad = new Avatar(BodyPad, FormaPad);						//}
	avEsqIzq = new Avatar(BodyEsqIzqCanasta, FormaEsqIzq);		//}
	avPiso = new Avatar(BodyPisoCanasta, FormaPiso);			//} Se crean los avatares que siempre son necesarios.
	avEsqDer = new Avatar(BodyEsqDerCanasta, FormaEsqDer);		//}
	for (int i = 0; i < 2; i++) {								//}
		avMuros[i] = new Avatar(BodyMuros[i], FormaMuros[i]);	//}

	}
	Loop();
}

void Game::Loop() {
	while(wnd->isOpen())
	{
		*tiempo = reloj->getElapsedTime();
		if (tiempo2 + frameTime < tiempo->asSeconds()) {
			tiempo2 = tiempo->asSeconds();
			wnd->clear(clearColor);
			ProcesarEventos();
			ActualizarFisicas();
			CondicionCambioDeNivel();

			if (habilitarDisparo) { CambioDeNivel(NivelSuperado);FormaPiso->setFillColor(Color::Red); } // Si se presionó enter luego de concluir un nivel, se permite pasar al siguiente.
																										
			DefinirStrings();
			DibujarComponentes();
			wnd->display();
		}
		
	}
}

void Game::InicializarFisicas() {
	// Inicializamos el mundo con la gravedad por defecto.
	phyWorld = new b2World(b2Vec2(0.0f, 10.0f));

	// Creamos el renderer de debug y le seteamos las banderas para que dibuje TODO.
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	BodyTecho = Box2DHelper::CreateRectangularStaticBody(phyWorld, 800.0f, 40.0f);
	BodyTecho->SetTransform(b2Vec2(400.f, 0.f), 0.0f);

	for (int i = 0; i < 2; i++) {
		BodyMuros[i] = Box2DHelper::CreateRectangularStaticBody(phyWorld, 40.0f, 600.0f);
	}

	BodyMuros[0]->SetTransform(b2Vec2(0.0f, 300.0f), 0.0f);
	BodyMuros[1]->SetTransform(b2Vec2(800.0f, 300.0f), 0.0f);

	BodySuelo = Box2DHelper::CreateRectangularStaticBody(phyWorld, 800.0f, 40.0f);
	BodySuelo->SetTransform(b2Vec2(400.f, 620.f), 0.0f);

	BodyPad = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 100.0f, 20.0f, 0.0f, 0.0f, 1000.0f);
	BodyPad->SetTransform(b2Vec2(400.0f, 560.0f), 0.0f);

	BodyPisoCanasta = Box2DHelper::CreateRectangularStaticBody(phyWorld, 120.0f, 10.0f);
	BodyPisoCanasta->SetTransform(b2Vec2(400.0f, 300.0f), 0.0f);

	BodyEsqIzqCanasta = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10.0f, 60.0f);	//}
	BodyEsqIzqCanasta->SetTransform(b2Vec2(336.0f, 275.0f), 0.0f);							//}
																							//}
	BodyEsqDerCanasta = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10.0f, 60.0f);	//}
	BodyEsqDerCanasta->SetTransform(b2Vec2(464.0f, 275.0f), 0.0f);							//}
																							//} Obstáculo estático que impide lograr fácilmente un acierto con un tiro hacia arriba.
	BodyBordeIzqCanasta = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10.0f, 40.0f); //}
	BodyBordeIzqCanasta->SetTransform(b2Vec2(326.0f, 235.0f), -10.0f);						//}
																							//}
	BodyBordeDerCanasta = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10.0f, 40.0f);	//}
	BodyBordeDerCanasta->SetTransform(b2Vec2(474.0f, 235.0f), 10.0f);						//}

	for (int i = 0; i < 2; i++) {
		BodyRebotines[i] = Box2DHelper::CreateCircularDynamicBody(phyWorld, 20.0f, 1.0f, 0.0f, 50.0f);			//}
	}																											//}
																												//}
	BodyRebotines[0]->SetTransform(b2Vec2(40.0f, 300.0f), 0.0f);												//}
	BodyRebotines[1]->SetTransform(b2Vec2(760.0f, 300.0f), 0.0f);												//}
																												//} Obstáculo con joints, unidos a los muros.
	UnionRebotinIzq = Box2DHelper::CreateDistanceJoint(phyWorld, BodyMuros[0], BodyMuros[0]->GetWorldCenter()	//}
		,BodyRebotines[0], BodyRebotines[0]->GetWorldCenter(), 5.0f, 0.0f, 1.0f);								//}
																												//}
	UnionRebotinDer = Box2DHelper::CreateDistanceJoint(phyWorld, BodyMuros[1], BodyMuros[1]->GetWorldCenter()	//}
		,BodyRebotines[1], BodyRebotines[1]->GetWorldCenter(), 5.0f, 0.0f, 1.0f);								//}
	
	for (int i = 0; i < 2; i++) {
		BodyPelotas[i] = Box2DHelper::CreateCircularDynamicBody(phyWorld, 20.0f, 1.0f, 0.0f, 50.0f);			//}
	}																											//}
																												//} Obstáculo dinámico que empezará a rebotar 
	BodyPelotas[0]->SetTransform(b2Vec2(60.0f, 600.0f), 0.0f);													//} si lo toca algo que no sea el pad.
	BodyPelotas[1]->SetTransform(b2Vec2(740.0f, 600.0f), 0.0f);													//}

	if (nivelActual == 2) {
		for (int i = 0; i < 4; i++) {
		BodyPlats[i] = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 100.0f, 10.0f, 0.0f, 0.0f, 20.0f);		//}
		}																											//}
																													//}
		BodyPlats[0]->SetTransform(b2Vec2(225.0f, 150.0f), 0.0f);													//} Más obstáculos dinámicos, 
		BodyPlats[1]->SetTransform(b2Vec2(225.0f, 450.0f), 0.0f);													//} pero que funcionan como plataformas para que otros objetos reboten.
		BodyPlats[2]->SetTransform(b2Vec2(575.0f, 150.0f), 0.0f);													//}
		BodyPlats[3]->SetTransform(b2Vec2(575.0f, 450.0f), 0.0f);													//}

		for (int i = 0; i < 2; i++) {
			BodyCajas[i] = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 20.0f, 20.0f, 1.0f, 1.0f, 2.0f);		//}
																													//}
		}																											//} Más obstáculos dinámicos, similares a las pelotas
																													//} pero más pequeños.
		BodyCajas[0]->SetTransform(b2Vec2(225.0f, 153.0f), 0.0f);													//}
		BodyCajas[1]->SetTransform(b2Vec2(575.0f, 153.0f), 0.0f);													//}
	}
	
}

void Game::ActualizarFisicas()
{
	phyWorld->Step(frameTime, 8, 8);
	phyWorld->ClearForces();
	phyWorld->DebugDraw();
}

void Game::DibujarComponentes() {
	switch (nivelActual)

	{
	case 0:
		if (nivelActual == 0)
		{
			wnd->draw(*SpriteFondoMenu);
			for (int i = 0; i < 3; i++) { wnd->draw(textoMenuInicio[i]); }
			wnd->draw(*SpriteCursor);
		}
		break;
	case 1:
		if (nivelActual == 1) {
			avTecho->DibujarAvatar(*wnd);			// Se dibujan los avatares y textos necesarios.
			avEsqIzq->DibujarAvatar(*wnd);
			avPad->DibujarAvatar(*wnd);
			avPiso->DibujarAvatar(*wnd);
			avEsqDer->DibujarAvatar(*wnd);
			for (int i = 0; i < 2; i++)
			{
				avMuros[i]->DibujarAvatar(*wnd);
			}
			
			for (int i = 0; i < totalDisparos; i++)
			{
				ragdolls[i]->DibujarRagdoll(wnd);
			}

			if (NivelSuperado)
			{
				wnd->draw(textoNivelSuperado);
				totalDisparos = 0;
			}
			else if (!NivelSuperado && totalDisparos >= 5)
			{
				wnd->draw(textoSinMunicion);
				wnd->draw(textReinicio);
			}
			if (!habilitarDisparo) {
				wnd->draw(textoContinuar);
			}
			wnd->draw(textoMunicion);
			wnd->draw(*SpriteCursor);
		}

	case 2:
		if (nivelActual == 2) {
			avTecho->DibujarAvatar(*wnd);
			avEsqIzq->DibujarAvatar(*wnd);
			avPad->DibujarAvatar(*wnd);
			avPiso->DibujarAvatar(*wnd);
			avEsqDer->DibujarAvatar(*wnd);
			for (int i = 0; i < 2; i++)
			{
				avMuros[i]->DibujarAvatar(*wnd);

			}

			for (int i = 0; i < totalDisparos; i++)
			{
				ragdolls[i]->DibujarRagdoll(wnd);
			}

			if (NivelSuperado)
			{
				wnd->draw(textoJugarDeNuevo);
				totalDisparos = 0;
			}
			else if (!NivelSuperado && sinMunicion)
			{
				wnd->draw(textoSinMunicion);
				wnd->draw(textReinicio);
			}
			if (!habilitarDisparo && nivelActual != 2) {
				wnd->draw(textoContinuar);
			}
			wnd->draw(textoMunicion);
			wnd->draw(*SpriteCursor);
		}
	}
}

void Game::CargarGraficos() {

	TexturaFondoMenu = new Texture;
	TexturaFondoMenu->loadFromFile("assets/FondoMenu.png");

	SpriteFondoMenu = new Sprite(*TexturaFondoMenu);
	SpriteFondoMenu->setPosition(0.0f, 0.0f);

	TexturaCursor = new Texture;
	TexturaCursor->loadFromFile("assets/ImagenCursor.png");

	SpriteCursor = new Sprite(*TexturaCursor);
	SpriteCursor->setOrigin(SpriteCursor->getGlobalBounds().width / 2, SpriteCursor->getGlobalBounds().height / 2);
	SpriteCursor->setScale(0.05f, 0.05f);

	for (int i = 0; i < 2; i++) {
		TexturaOpciones[i] = new Texture;
		TexturaOpciones[i]->loadFromFile("assets/transparencia.png");
		SpriteOpciones[i] = new Sprite(*TexturaOpciones[i]);
		SpriteOpciones[i]->setScale(0.45f, 0.12f);
	}

	SpriteOpciones[0]->setPosition(60.0f, 250.0f);
	SpriteOpciones[1]->setPosition(60.0f, 340.0f);

	// Las figuras de SFML para cada parte del mapa.
	FormaTecho = new RectangleShape;
	FormaPad = new RectangleShape;
	FormaEsqIzq = new RectangleShape;
	FormaPiso = new RectangleShape;
	FormaEsqIzq = new RectangleShape;
	FormaEsqDer = new RectangleShape;
	for (int i = 0; i < 2; i++) {
		FormaMuros[i] = new RectangleShape;
	}

	// Colores de las figuras de SFML.
	FormaTecho->setFillColor(Color::Red);
	FormaPad->setFillColor(Color::Cyan);
	FormaPiso->setFillColor(Color::Red);
	FormaEsqDer->setFillColor(Color::Green);
	FormaEsqIzq->setFillColor(Color::Green);
	for (int i = 0; i < 2; i++)
	{
		FormaMuros[i]->setFillColor(Color::Red);

	}
}

void Game::ProcesarEventos() {

	Vector2i posicion_m = Mouse::getPosition(*wnd);;// La posicion del ratón en coordenadas de la ventana.
	Vector2f posicion_mc = wnd->mapPixelToCoords(posicion_m);;// La posicion del ratón en coordenadas del mundo.

	while (wnd->pollEvent(*evento)) {
		switch (evento->type) {
		case Event::Closed:
			wnd->close();
			break;
		case sf::Event::MouseMoved:	
			SpriteCursor->setPosition((Vector2f)Mouse::getPosition(*wnd));	// Se fija la posición del sprite que corresponde al cursor a la posición del mismo.
			// Se detecta si el sprite del cursor se encuentra en contacto
			// con el de alguna opción del menú y se cambia el color de su texto.
			if (SpriteCursor->getGlobalBounds().intersects(SpriteOpciones[0]->getGlobalBounds())) {
				textoMenuInicio[1].setFillColor(Color::Yellow);
			}
			else {
				textoMenuInicio[1].setFillColor(Color::White);
			}

			if (SpriteCursor->getGlobalBounds().intersects(SpriteOpciones[1]->getGlobalBounds())) {
				textoMenuInicio[2].setFillColor(Color::Yellow);
			}
			else {
				textoMenuInicio[2].setFillColor(Color::White);
			}
			break;
		case Event::MouseButtonPressed:
			if (nivelActual == 0) {	// Se detecta si se da click sobre los sprites de las opciones del menú.
				if (SpriteCursor->getGlobalBounds().intersects(SpriteOpciones[0]->getGlobalBounds())) {	// Jugar, habilita el nivel 1.
					nivelActual = 1;
				}

				if (SpriteCursor->getGlobalBounds().intersects(SpriteOpciones[1]->getGlobalBounds())) {	// Salir.
					wnd->close();
				}
			}
			if (totalDisparos < 5 && habilitarDisparo) {	// Si se presionó Enter anteriormente y aún hay munición...

				ragdolls[totalDisparos] = new Ragdoll({ BodyPad->GetPosition().x,BodyPad->GetPosition().y - 60.f }, *phyWorld); // Se toma la posición del pad y el mundo activo para generar el ragdoll allí.
				ragdolls[totalDisparos]->AplicarFuerza({ (posicion_mc.x - BodyPad->GetPosition().x) * 25,(posicion_mc.y - BodyPad->GetPosition().y) * 25 }); /* Usando la posición del ratón en el mundo menos la del pad(sin eso queda flotando sobre el pad),
																																								se calcula la fuerza del tiro.*/
				totalDisparos++; // El total se incrementa...
				auxMunicion--;	 // ...y la variable para el texto se reduce...
				textoMunicion.setString("Munición: " + to_string(auxMunicion)); // ...para luego actualizar al mismo.

			}
			break;
		case Event::KeyPressed:

			if (Keyboard::isKeyPressed(Keyboard::Escape)) {	// Con Escape se cierra la ventana.
				wnd->close();
				break;
			}
			else if (Keyboard::isKeyPressed(Keyboard::R)) {
				wnd->close();
				new Game(800,600, "Ragkanoid"); // Si se presiona R se abre un nuevo juego.
				break;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Enter)) { // Al comenzar el nivel 1 permite disparar, al terminarlo, permite tambien pasar de nivel.
				if (nivelActual != 0 && nivelActual != 2) {
					habilitarDisparo = true;
				}
				break;
			}
			// El movimiento y la rotación del pad, tomando la posición y rotación actual del mismo e incrementándolas o reduciéndolas al presionar la tecla correspondiente.
			else if (Keyboard::isKeyPressed(Keyboard::A)) {
				BodyPad->SetTransform(b2Vec2(BodyPad->GetPosition().x - 15.0f, BodyPad->GetPosition().y), BodyPad->GetAngle());
				break;
			}
			else if (Keyboard::isKeyPressed(Keyboard::D)) {
				BodyPad->SetTransform(b2Vec2(BodyPad->GetPosition().x + 15.0f, BodyPad->GetPosition().y), BodyPad->GetAngle());
				break;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Q)) {
				BodyPad->SetTransform(b2Vec2(BodyPad->GetPosition().x, BodyPad->GetPosition().y), BodyPad->GetAngle() - 0.2f);
			}
			else if (Keyboard::isKeyPressed(Keyboard::E)) {
				BodyPad->SetTransform(b2Vec2(BodyPad->GetPosition().x, BodyPad->GetPosition().y), BodyPad->GetAngle() + 0.2f);
			}
			break;
		}
	}
}

void Game::CondicionCambioDeNivel() {
	for (int i = 0; i < totalDisparos; i++) {

		FloatRect hitboxRag((Vector2f)ragdolls[i]->getPosition(i), (Vector2f)ragdolls[i]->getSize(i)); // El área de contacto del avatar del ragdoll.
		if (FormaPiso->getGlobalBounds().intersects(hitboxRag)) {
			NivelSuperado = true; // Cuando el ragdoll choca con el piso de la canasta, esta bandera lo informa.
		}
		else
		{
			NivelSuperado = false;
		}

		if(auxMunicion == 0) {	// Al quedarse sin munición, se informa al juego con esta bandera para lanzar el mensaje correspondiente.
			sinMunicion = true;
		}

		if (NivelSuperado) // Si la bandera informa sobre el contacto, se reestablecen las otras banderas y se cambia el color del piso.
		{
			habilitarDisparo = false;
			sinMunicion = false;
			auxMunicion = 5;
			FormaPiso->setFillColor(Color::Green);
		}
	}

	
}

void Game::CambioDeNivel(bool condicion) {	// Se permite pasar al siguiente nivel usando la bandera de Nivel Superado, alterando y reestableciendo variables.
	if (NivelSuperado && nivelActual < 2)
	{
		NivelSuperado = false;
		nivelActual++;
		IniciarJuego(); 
		totalDisparos = 0;// Esto es lo que hace que se borre el avatar de los muñecos al ganar.
		textoMunicion.setString("Munición: " + to_string(auxMunicion)); // Se actualiza el texto.
	}
}

// Definimos el area del mundo que veremos en nuestro juego
// Box2D tiene problemas para simular magnitudes muy grandes
void Game::FijarZoom() {
	View camara;
	// Posicion del view.
	camara.setSize(800.0f, 600.0f);
	camara.setCenter(400.0f, 300.0f);
	wnd->setView(camara); // Asignamos la camara.
}

void Game::DefinirStrings() { 
	textoMenuInicio[0].setString("Ragkanoid");
	textoMenuInicio[1].setString("Jugar");
	textoMenuInicio[2].setString("Salir");
	textoNivelSuperado.setString("Nivel completado");
	textoSinMunicion.setString("Te quedaste sin munición. Empezá a ponerte creativo o");
	textReinicio.setString("pulsá R para reiniciar el juego.");
	textoJugarDeNuevo.setString("Juego completado Pulsá R para volver a jugar");
	textoContinuar.setString("Pulsá Enter para continuar");
	textoMunicion.setString("Munición: " + to_string(auxMunicion));
}

void Game::ConfigurarTextos() {

	textoNivelSuperado.setFont(fuente);
	textoNivelSuperado.setScale(1.0f, 1.0f);
	textoNivelSuperado.setPosition(90.f, 90.f);

	textoSinMunicion.setFont(fuente);
	textoSinMunicion.setScale(0.5f, 0.5f);
	textoSinMunicion.setPosition(50.f, 90.f);

	textReinicio.setFont(fuente);
	textReinicio.setScale(0.5f, 0.5f);
	textReinicio.setPosition(50.f, 120.f);

	textoJugarDeNuevo.setFont(fuente);
	textoJugarDeNuevo.setScale(0.5f, 0.5f);
	textoJugarDeNuevo.setPosition(50.f, 90.f);

	textoContinuar.setFont(fuente);
	textoContinuar.setScale(1.0f, 1.0f);
	textoContinuar.setPosition(90.f, 150.f);

	textoMunicion.setFont(fuente);
	textoMunicion.setScale(0.7f, 0.7f);
	textoMunicion.setPosition(50.0f, 50.0f);

	//0: Título, 1: Jugar, 2: Salir 
	for (int i = 0; i < 3; i++) {
		textoMenuInicio[i].setFont(fuente);
	}
	textoMenuInicio[0].setPosition(100.0f, 100.f);
	textoMenuInicio[0].setCharacterSize(85);
	textoMenuInicio[1].setPosition(50.f, 250.f);
	textoMenuInicio[1].setCharacterSize(55);
	textoMenuInicio[2].setPosition(50.f, 320.f);
	textoMenuInicio[2].setCharacterSize(55);

}

Game::~Game(void)
{ }






