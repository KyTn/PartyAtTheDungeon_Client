// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MapGeneration/PD_MG_LogicPosition.h"


// FORWARD DECLARATIONS
class PD_MG_LogicPosition;
class PD_MG_StaticMap;
class PD_MM_Room;


class PATD_CLIENT_API PD_MM_MapInfo
{
public:
	PD_MM_MapInfo();
	~PD_MM_MapInfo();

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

	TMap<PD_MG_LogicPosition, uint32> tiles;
	TMap<PD_MG_LogicPosition, uint32> walls;
	TMap<PD_MG_LogicPosition, uint32> interactuables;
	TMap<PD_MG_LogicPosition, uint32> players;
	TMap<PD_MG_LogicPosition, uint32> enemies;
};


