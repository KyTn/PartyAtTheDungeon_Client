// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NW_NetWorking/EventLayer/PD_NW_iEventObserver.h"
#include "Structs/PD_ClientEnums.h"
#include "Structs/PD_ClientStructs.h"

/// FORWARD DECLARATIONS
class PD_GM_MapManager;
class PD_GM_EnemyManager;
class PD_PlayersManager;
class PD_NW_NetworkManager;
class UPD_ClientGameInstance;
struct StructGameState;
struct StructGamePhase;
/**
 * 
 */
class PATD_CLIENT_API PD_GM_GameManager : public PD_NW_iEventObserver
{
public:
	PD_GM_GameManager(UPD_ClientGameInstance* gameInstance, PD_GM_MapManager* inMapManager, PD_PlayersManager* inPlayerManager, PD_NW_NetworkManager* networkManager);
	~PD_GM_GameManager();

	UPD_ClientGameInstance* _GAMEINSTANCE;

	PD_GM_MapManager* mapManager;
	PD_GM_EnemyManager* enemyManager;
	PD_PlayersManager* playersManager;
	PD_NW_NetworkManager* networkManager;


	//Struct con el estado para la maquina de estados del gameManager
	StructGameState* structGameState;

	//Struct con el estado para la maquina de estados de la fase de juego del GameManager
	StructGamePhase* structGamePhase;



#pragma region HANDLERS
	
	// Suscripcion del handler
	bool SuscribeToEvents(int inPlayer, UStructType inType);

	virtual void HandleEvent(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType);

#pragma endregion



#pragma region Machine State of GM

	//Funciones de configuracion de la maquina
	//Transiciones
	void UpdateState();
	//Acciones al empezar el estado
	void OnBeginState();

	//Funciones auxiliares
	//Control directo del estado, llama a OnBeginState
	void ChangeState(EClientGameState newState);
	void InitState();

#pragma endregion


#pragma region SEND TO SERVER FUNCTIONS

	bool Send_FStructClientMapAlreadyInstantiated();


#pragma endregion 

};
