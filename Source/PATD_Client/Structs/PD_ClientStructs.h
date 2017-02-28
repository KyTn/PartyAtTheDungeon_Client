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

	bool clientMaster;
	//Ingame data
	FStructTurnOrders* turnOrders;

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

	StructPlayer() {
		clientMaster = false;
		readyMenu = false; //¿que es esto?
		turnOrders = new FStructTurnOrders();
		player_character = new FStructCharacter();
		update_character = new FStructUpdateCharacter();
	}
};



//Usado por: GameInstance -ServerManager-
struct StructClientState {
	EClientState enumClientState;
	bool clientMaster;
	int numPlayer=-1; //-1 cuando no ha conectado aun.
	bool configurationGameDone;
	bool configurationCharacterDone;
	FString mapString;
};

//Usado por: GameManager
struct StructGameState {
	EGameState enumGameState;
	EActionPhase enumActionPhase;

};