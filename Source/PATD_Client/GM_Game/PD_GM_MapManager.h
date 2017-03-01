// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PATD_Client/MapInfo/MapInstantiaton/MapInstantiatorActor.h"

// FORWARD DECLARATIONS
class PD_MG_LogicPosition;
class PD_MM_MapInfo;
class PD_MG_StaticMap;
class PD_MG_DynamicMap;
class PD_GM_GameManager;
class PD_GM_EnemyManager;
class APD_GenericController;


/**
 * 
 */
class PATD_CLIENT_API PD_GM_MapManager
{

private:



	// Dada una posición lógica, devuelve el GenericCharacter que está en esa posición, 0 si no hay ninguno. 
	//bool getGenericCharacterAt(PD_MG_LogicPosition* logpos, APD_PLY_GenericCharacter* genCharacter);

public:
	PD_GM_MapManager();
	~PD_GM_MapManager();

	AMapInstantiatorActor* instantiator;

	PD_MM_MapInfo* MapInfo;
	PD_MG_StaticMap* StaticMapRef;
	PD_MG_DynamicMap* DynamicMapRef;

	PD_GM_GameManager* _GAMEMANAGER;


#pragma region GET INFO OF THE MAP


	// Dada una posición lógica, devuelve el GenericCharacter que está en esa posición, que además es un Player. 0 si no hay ninguno. 
	//bool getPlayerAt(PD_MG_LogicPosition* logpos, APD_PLY_GenericCharacter* genCharacter);
	// Dada una posición lógica, devuelve el GenericCharacter que está en esa posición, que además es un Enemy. 0 si no hay ninguno. 
	//bool getEnemyAt(PD_MG_LogicPosition* logpos, APD_PLY_GenericCharacter* genCharacter);

	// Esta funcion devolverá un Interactuable
	AActor* getInteractuableAt(PD_MG_LogicPosition* logpos);

	//Dada un LogicPosition, te da la posición en el mundo.
	FVector* LogicToWorldPosition(PD_MG_LogicPosition* pos);

	//Dada la posición en el mundo, te devuelve la posición lógica que estaría asignada a posición en el mundo, exista o no en el mapa. 
	PD_MG_LogicPosition* WorldToLogicPosition(FVector* pos);


#pragma endregion


#pragma region INSTANTIATE MAP


	void InstantiateMap();
	void InstantiateStaticMap();
	void InstantiateDynamicMap();


#pragma endregion



};
