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
	TArray<PD_MG_LogicPosition> closed = TArray<PD_MG_LogicPosition>(); // posiciones logicas ya cerradas
	TArray<PD_MG_LogicPosition> open = TArray<PD_MG_LogicPosition>(); //posiciones logicas no se han visitado (abiertas)

	PD_MG_LogicPosition* StartFlood = new PD_MG_LogicPosition(0, 0);

	for (int i = 0; i < sm->GetLogicPositions().Num(); i++) {
		if (open.Contains(*(sm->GetLogicPositions()[i]))) {




		}
	}
}

PD_MM_Room::PD_MM_Room()
{
}

PD_MM_Room::~PD_MM_Room()
{
}
