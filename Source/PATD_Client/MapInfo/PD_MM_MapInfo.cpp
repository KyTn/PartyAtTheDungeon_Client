// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_MM_MapInfo.h"
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "MapGeneration/Static/PD_MG_StaticMap.h"

PD_MM_MapInfo::PD_MM_MapInfo()
{
}

PD_MM_MapInfo::~PD_MM_MapInfo()
{
}

bool PD_MM_MapInfo::RoomOf(PD_MG_LogicPosition logpos, PD_MM_Room * room)
{
	for (int i = 0; i < rooms.Num(); i++) {
		for (int j = 0; j < rooms[i].LogicPosInRoom.Num(); j++) {
			if (rooms[i].LogicPosInRoom[j] == logpos) {
				room = &rooms[i];
				return true;
			}
		}
	}
	return false;
}

void PD_MM_MapInfo::CalculateRooms(PD_MG_StaticMap * sm)
{
	TArray<PD_MG_LogicPosition> visited;
	visited.Empty();
	PD_MG_LogicPosition* StartFlood = new PD_MG_LogicPosition(0, 0);

	for (int i = 0; i < sm->GetLogicPositions().Num(); i++) {
		//if (visited.Contains(sm->GetLogicPositions()[i])) {

		//}
	}



}

PD_MM_Room::PD_MM_Room()
{
}

PD_MM_Room::~PD_MM_Room()
{
}
