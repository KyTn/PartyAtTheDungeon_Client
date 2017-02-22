#pragma once

#include "PD_NetStructs.h"
//Incluyendo este archivo en cualquier otro #include "Structs/PD_ServerStructs.h" tenemos todos los structs (porque hace include de los demas)

//#include "GM_Game/LogicCharacter/PD_GM_iLogicCharacter.h"
//#include "GM_Game/LogicCharacter/PD_GM_PlayerLogicCharacter.h"

//class PD_GM_PlayerLogicCharacter;



/*
//Usado por: PlayersManager
struct StructPlayer {

	//Menu options and configuration
	bool readyMenu;

	//Ingame data
	FStructTurnOrders* turnOrders;

	//Puntero a Struct de caracteristicas del jugador.
	PD_GM_PlayerLogicCharacter* logicCharacter;
	//Puntero a su fisicCharacter (actor de unreal)


};


enum class EGameState {  WaitingPlayerOrders, ExecutingActionOrders };

//Usado por: GameManager
struct StructGameState {
	EGameState enumGameState;
};

enum class EServerState {WaitingClientMaster,WaitingStartGame,GameInProcess };
//Usado por: GameInstance -ServerManager-
struct StructServerState {
	EServerState enumServerState;
};
*/