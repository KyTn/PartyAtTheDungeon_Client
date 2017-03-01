// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MapGeneration/PD_MG_LogicPosition.h"
#include "GameFramework/Actor.h"

// FORWARD DECLARATIONS
class PD_MG_LogicPosition;
class PD_MG_StaticMap;
class PD_MM_Room;
class PD_GM_MapManager;

class PATD_CLIENT_API PD_MM_MapInfo
{
public:
	PD_MM_MapInfo();
	~PD_MM_MapInfo();

	PD_GM_MapManager* mapManager;


	TArray<PD_MG_LogicPosition> allLogicPos;
	TArray<PD_MM_Room> rooms;


	// Devuelve un puntero al Room que tenga ese logPos. True si existe. 
	bool RoomOf(PD_MG_LogicPosition logpos, PD_MM_Room* room);

	// Inicializa el vector de rooms dado un staticMap
	void CalculateRooms(PD_MG_StaticMap* sm);
};


class PATD_CLIENT_API PD_MM_Room
{
public:
	PD_MM_Room();
	~PD_MM_Room();


	TArray<PD_MG_LogicPosition> LogicPosInRoom;

	TMap<PD_MG_LogicPosition, AActor*> tiles;
	TMap<PD_MG_LogicPosition, AActor*> walls;
	TMap<PD_MG_LogicPosition, AActor*> interactuables;
};


