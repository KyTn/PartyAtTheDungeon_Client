#pragma once

//Hacemos (#include "Structs/PD_ServerStructs.h") en el .cpp de las clases para usar los structs.
//Si los necesitamos en el .h los hacemos con forward declaration.
#include "PD_NetStructs.h"
#include "PD_ClientEnums.h"

class PD_GM_LogicCharacter;

//Usado por: PlayerManager
/*
Es el Struct principal del Cliente. 
	Tiene datos referentes a su información de Internet (IP, device, etc)
	Datos del Character
*/

struct StructPlayer {
	//Menu options and configuration
	bool readyMenu;
	int ID_Player;
	FString ID_Client;
	bool clientMaster;

	bool isSetPlayerCharacter; //Para comprobar que se ha enviado al Server el Personaje o no (al menos una vez)
	//Ingame data
	FStructTurnOrders* turnOrders;

	//Define si lo que estas haciendo es un Ataque o un Movimiento
	uint8 typeOfAction;

	//PD_MG_LogicPosition* logicPosition;
	//	PD_GM_GameManager* gameManager;
	//Puntero a Struct de caracteristicas del jugador.

	/*Referencia a la clase GenericCharacter, que es la que tiene acceso a toda la información del personaje
	//creado por el personaje para jugar la partida
	!!!IMPORTANTE: Esta referencia se tendra que ACTUALIZAR cada vez que el jugador cambie de personaje con los nuevos datos
	*/
	FStructCharacter* player_character; //Envio de la informacion LOGICA del personaje
	FStructUpdateCharacter* update_character;
	PD_GM_LogicCharacter* logic_Character;

	///NETWORK
	bool isConnected; //Dice si el cliente se encuentra actualmente conectado con un servidor
	int pingPong; //Para la comprobación de una conexion ping - pong con el servidor
	/*
	0 - Inicializacion
	1 - Ping
	2 - Pong
	*/

	StructPlayer() {
		clientMaster = false;
		readyMenu = false; //¿que es esto?
		turnOrders = new FStructTurnOrders();
		player_character = new FStructCharacter();
		update_character = new FStructUpdateCharacter();
		isSetPlayerCharacter = false;
		isConnected = false;
		pingPong = 0;
	}
};



//Usado por: GameInstance -ServerManager-
struct StructClientState {
	EClientState enumClientState;
	bool clientMaster;
	int numPlayer=-1; //-1 cuando no ha conectado aun.
	bool ConfigMatchDone;
	bool ConfigAllCharactersDone;
	bool AllCharactersIncoming;
	FString mapString;
	FStructMapData* NETMAPDATA;

	//Esta variable sirve para saber si el proceso de carga es el normal o por reconexion. 
	bool reconnected=false;
};

//Usado por: GameManager
struct StructGameState {
	EClientGameState enumGameState;
	FStructUpdateTurn update_turn;

	//Esta variable sirve para hacer un flujo de reconexion distinto al normal
	EClientGameState stateAfterInstantiate_Map;
};
