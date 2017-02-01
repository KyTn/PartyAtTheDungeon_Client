// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PATD_Client/MapGeneration/PD_MG_LogicPosition.h"
#include "PATD_Client/MapGeneration/PD_MG_Tile.h"

#pragma region ClassReferences

class PD_MG_Tile;
class PD_MG_LogicPosition;

#pragma endregion


class PATD_CLIENT_API PD_MG_Room
{
private:
	TMap<PD_MG_LogicPosition, PD_MG_Tile> _tilesInMap;
	TArray<PD_MG_Tile*> _tiles;

public:
	PD_MG_Room();
	~PD_MG_Room();
};
