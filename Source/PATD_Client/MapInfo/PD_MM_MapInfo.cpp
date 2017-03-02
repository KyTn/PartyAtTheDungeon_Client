// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_MM_MapInfo.h"
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "MapGeneration/Static/PD_MG_StaticMap.h"
#include "GM_Game/PD_GM_MapManager.h"



#pragma region MAP INFO


PD_MM_MapInfo::PD_MM_MapInfo(PD_GM_MapManager* mM)
{
	mapManager = mM;



	allLogicPos = TArray<PD_MG_LogicPosition>();
	rooms = TArray<PD_MM_Room>();
	roomByLogPos = TMap<PD_MG_LogicPosition, PD_MM_Room>();

	CalculateRooms();
}

PD_MM_MapInfo::~PD_MM_MapInfo()
{
}

bool PD_MM_MapInfo::RoomOf(PD_MG_LogicPosition logpos, PD_MM_Room *room)
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

bool PD_MM_MapInfo::AddWall(PD_MG_LogicPosition logpos, AActor *wall)
{
	if (roomByLogPos.Contains(logpos)) {
		roomByLogPos[logpos].walls.Add(logpos, wall);
		return true;
	}


	return false;
}

bool PD_MM_MapInfo::AddTile(PD_MG_LogicPosition logpos, AActor* tile)
{
	if (roomByLogPos.Contains(logpos)) {
		roomByLogPos[logpos].tiles.Add(logpos, tile);
		return true;
	}


	return false;
}

bool PD_MM_MapInfo::AddInteractuable(PD_MG_LogicPosition logpos, AActor* wall)
{
	/*
	if (roomByLogPos.Contains(logpos)) {
	roomByLogPos[logpos].walls.Append(logpos, wall);
	return true;
	}
	*/

	return false;
}

/*

=== CalculateRooms ===

Por cada posicion p(x,y) en el mapa
Si p es un tile o un door
Tener una referencia a una habitacion r
Por cada habitacion r1
Si p está en una habitacion r1
hacer r1 = r
Añadir la lista de posiciones p_a(p) adyacentes de p a r1
break por cada habitacion

Si no está en ninguna habitacion
Crear habitacion r
hacer r1 = r
Añadir p a r1
Añadir la lista de posiciones p_a(p) adyacentes de p a r1

*/

void PD_MM_MapInfo::CalculateRooms()
{
	// Por cada posicion p(x,y) en el mapa
	for (int i = 0; i < mapManager->StaticMapRef->GetLogicPositions().Num(); i++) {

		PD_MG_LogicPosition p = mapManager->StaticMapRef->GetLogicPositions()[i];

		allLogicPos.Add(p);

		// Si p es un tile o un door
		if (mapManager->StaticMapRef->GetXYMap()[p] == '.' ||
			mapManager->StaticMapRef->GetXYMap()[p] == 'd' ||
			mapManager->StaticMapRef->GetXYMap()[p] == 's') {

			// Tener una referencia a una habitacion r
			PD_MM_Room r = PD_MM_Room();
			bool roomFound = false;

			// Por cada habitacion r1
			for (int j = 0; j < rooms.Num(); j++) {
				r = rooms[j]; // hacer r1 = r
							  // Si p está en una habitacion r
				if (r.LogicPosInRoom.Contains(p)) {
					roomFound = true;
					// Añadir la lista de posiciones p_a(p) adyacentes de p a r1
					TArray<PD_MG_LogicPosition> a = mapManager->Get_LogicPosition_Adyacents_To(p);
					for (int k = 0; k < a.Num(); k++) {
						r.LogicPosInRoom.AddUnique(a[k]);
					}
					break;
				}
			}

			// Si no está en ninguna habitacion
			if (!roomFound) {
				// Crear habitacion r
				rooms.Add(r);
				// hacer r1 = r
				// Añadir p a r1
				r.LogicPosInRoom.AddUnique(p);
				// Añadir la lista de posiciones p_a(p) adyacentes de p a r1
				TArray<PD_MG_LogicPosition> a = mapManager->Get_LogicPosition_Adyacents_To(p);
				for (int k = 0; k < a.Num(); k++) {
					r.LogicPosInRoom.AddUnique(a[k]);
				}
			}

			if (!roomByLogPos.Contains(p)) {
				roomByLogPos.Add(p, r);
			}

			if (mapManager->StaticMapRef->GetXYMap()[p] == 's') {
				r.IsSpawnRoom = true;
			}
		}
	}
}


#pragma endregion

#pragma region ROOM

PD_MM_Room::PD_MM_Room()
{
	LogicPosInRoom = TArray<PD_MG_LogicPosition>();

	tiles = TMap<PD_MG_LogicPosition, AActor*>();
	walls = TMap<PD_MG_LogicPosition, AActor*>();
	interactuables = TMap<PD_MG_LogicPosition, AActor*>();

}

PD_MM_Room::~PD_MM_Room()
{
}

#pragma endregion