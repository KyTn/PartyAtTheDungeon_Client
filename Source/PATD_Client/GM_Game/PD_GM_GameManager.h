// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NW_NetWorking/EventLayer/PD_NW_iEventObserver.h"
#include "Structs/PD_ClientEnums.h"

/// FORWARD DECLARATIONS
class PD_GM_MapManager;
class PD_GM_EnemyManager;
class PD_PlayersManager;
struct StructGameState;
/**
 * 
 */
class PATD_CLIENT_API PD_GM_GameManager : public PD_NW_iEventObserver
{
public:
	PD_GM_GameManager(PD_GM_MapManager* mapManager, PD_PlayersManager* playersManager);
	~PD_GM_GameManager();


	PD_GM_MapManager* mapManager;
	PD_GM_EnemyManager* enemyManager;
	PD_PlayersManager* playersManager;

	bool SuscribeToEvents(int inPlayer, UStructType inType);
	virtual void HandleEvent(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType);

	//Struct con el estado para la maquina de estados del gameManager
	StructGameState* structGameState;

	//Funciones de gestion del estado (maquina de estados)

	//Funciones de configuracion de la maquina
	//Transiciones
	void UpdateState();
	//Acciones al empezar el estado
	void OnBeginState();

	//Funciones auxiliares
	//Control directo del estado, llama a OnBeginState
	void ChangeState(EGameState newState);
	void InitState();


};
