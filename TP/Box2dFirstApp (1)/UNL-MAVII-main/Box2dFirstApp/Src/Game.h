#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include <list>
#include "../Build/Ragdoll.h"
#include "../Build/Avatar.h"
using namespace std;
using namespace sf;
class Game
{
private:
	// Propiedades de la ventana.
	int alto;
	int ancho;
	RenderWindow *wnd;
	Color clearColor;
	Time* tiempo;
	Clock* reloj;
	Font fuente;
	Event* evento;
	// Textos para el Menu y la interfaz.
	Text
		textoMenuInicio[3],
		textoNivelSuperado,
		textoSinMunicion,
		textReinicio,
		textoJugarDeNuevo,
		textoContinuar,
		textoMunicion;
	// Objetos de box2d y sus formas de SFML.
	b2World *phyWorld;
	SFMLRenderer *debugRender;

	Ragdoll* ragdolls[5];
	
	b2Body* BodyTecho; RectangleShape* FormaTecho; Avatar* avTecho;
	b2Body* BodyMuros[2]; RectangleShape* FormaMuros[2]; Avatar* avMuros[2];
	b2Body* BodySuelo;
	
	b2Body* BodyPisoCanasta; RectangleShape* FormaPiso; Avatar* avPiso;
	b2Body* BodyEsqIzqCanasta; RectangleShape* FormaEsqIzq; Avatar* avEsqIzq;
	b2Body* BodyEsqDerCanasta; RectangleShape* FormaEsqDer; Avatar* avEsqDer;
	b2Body* BodyBordeIzqCanasta;
	b2Body* BodyBordeDerCanasta;

	b2Body* BodyPad; RectangleShape* FormaPad; Avatar* avPad;
	
	b2Body* BodyRebotines[2]; b2DistanceJoint* UnionRebotinIzq; b2DistanceJoint* UnionRebotinDer;
	
	b2Body* BodyPlats[4];

	b2Body* BodyCajas[2];

	b2Body* BodyPelotas[2];

	// Tiempo de frame.
	float frameTime,
		tiempo2;
	int fps;

	int totalDisparos = 0;// Variable que indica la cantidad de ragdolls disparados usada para determinar la cantidad máxima de disparos disponible.
	int nivelActual = 0; // Determina si se está en el menú de inicio, el nivel 1 o el 2.
	int auxMunicion = 5; // Se usa para mostrar la munición restante en la interfaz.
	bool sinMunicion = false; // Se usa para habilitar el reinicio del juego al quedarse sin munición.
	bool habilitarDibujado = false; // Permite dibujar el avatar del ragdoll cuando es necesario.
	bool habilitarDisparo; // Se usa para activar el disparo y que no se comience el nivel 1 ya habiendo disparado por dar click a Jugar, y para acceder al nivel 2.
	bool NivelSuperado = false; // Indicador para el cambio de nivel.

	Sprite* SpriteFondoMenu;
	Texture* TexturaFondoMenu;

	Sprite* SpriteOpciones[2];	// El fondo transparente que delimita la forma de los botones del menú principal.
	Texture* TexturaOpciones[2];

	Sprite* SpriteCursor;
	Texture* TexturaCursor;

public:

	// Constructores, destructores e inicializadores.
	Game(int ancho, int alto,std::string titulo);
	
	~Game(void);
	void InicializarFisicas();

	// Main game loop.
	void Loop();

	void DibujarComponentes();
	void ActualizarFisicas();
	void ProcesarEventos();
	void FijarZoom();
	void CargarGraficos();
	void DefinirStrings();
	void ConfigurarTextos();
	void IniciarJuego();
	void CondicionCambioDeNivel();
	void CambioDeNivel(bool condicion);
};

