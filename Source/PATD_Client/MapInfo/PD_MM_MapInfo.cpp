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
		FString s = "Wall (";
		s.AppendInt(logpos.GetX());
		s.Append(",");
		s.AppendInt(logpos.GetY());
		s.Append(")");
		wall->SetActorLabel(*s);
		roomByLogPos[logpos].walls.Add(logpos, wall);
		return true;
	}


	return false;
}

bool PD_MM_MapInfo::AddTile(PD_MG_LogicPosition logpos, AActor* tile)
{
	if (roomByLogPos.Contains(logpos)) {
		FString s = "Tile (";
		s.AppendInt(logpos.GetX());
		s.Append(",");
		s.AppendInt(logpos.GetY());
		s.Append(")");
		tile->SetActorLabel(*s);
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
	int tilesProcess = 0;
	UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Calculating rooms ... "));
	// Por cada posicion p(x,y) en el mapa
	for (int i = 0; i < mapManager->StaticMapRef->GetLogicPositions().Num(); i++) {

		PD_MG_LogicPosition p = mapManager->StaticMapRef->GetLogicPositions()[i];

		allLogicPos.Add(p);

		// Si p es un tile o un door
		if (mapManager->StaticMapRef->GetXYMap()[p] == '.' ||
			mapManager->StaticMapRef->GetXYMap()[p] == 's') {

			// Tener una referencia a una habitacion r
			++tilesProcess;

			bool roomFound = false;

			//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Actual num of Rooms %d"), rooms.Num());
			// Por cada habitacion r1
			for (int j = 0; j < rooms.Num(); j++) {
				//r = rooms[j]; // hacer r1 = r
				// Si p está en una habitacion r
				//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Actual num tiles on Room: %d"), rooms[j].LogicPosInRoom.Num());
				if (rooms[j].LogicPosInRoom.Contains(p)) {
					roomFound = true;
					//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - reusing Room created at (%d,%d) ... "), p.GetX(), p.GetY());

					// Añadir la lista de posiciones p_a(p) adyacentes de p a r1
					TArray<PD_MG_LogicPosition> a = mapManager->Get_LogicPosition_Adyacents_To(p);
					for (int k = 0; k < a.Num(); k++) {
						if (mapManager->StaticMapRef->GetXYMap()[a[k]] == '.' ||
							mapManager->StaticMapRef->GetXYMap()[a[k]] == 's') {
							rooms[j].AddLogicPos(a[k]);
							//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - adding to Room created at (%d,%d) <- (%d,%d) adjacents ... "), p.GetX(), p.GetY(), a[k].GetX(), a[k].GetY());

						}
					}

					if (mapManager->StaticMapRef->GetXYMap()[p] == 's') {
						rooms[j].IsSpawnRoom = true;
						SpawnRoom = &rooms[j];
						UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Spawn Room Found! Room %d"), rooms[j].GetIDRoom());
					}
					break;
				}
			}

			// Si no está en ninguna habitacion
			if (!roomFound) {
				// Crear habitacion r

				PD_MM_Room r = PD_MM_Room(rooms.Num());

				//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - new Room created at (%d,%d) "), p.GetX(), p.GetY());


				// Añadir p a r1
				r.AddLogicPos(p);
				// Añadir la lista de posiciones p_a(p) adyacentes de p a r1
				TArray<PD_MG_LogicPosition> a = mapManager->Get_LogicPosition_Adyacents_To(p);
				//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - \tadding to Room created at (%d,%d) %d adjacents ... "), p.GetX(), p.GetY(), a.Num() );
				for (int k = 0; k < a.Num(); k++) {
					if (mapManager->StaticMapRef->GetXYMap()[a[k]] == '.' ||
						mapManager->StaticMapRef->GetXYMap()[a[k]] == 's') {
						r.AddLogicPos(a[k]);
						//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - adding to Room created at (%d,%d) <- (%d,%d) adjacents ... "), p.GetX(), p.GetY(), a[k].GetX(), a[k].GetY());

					}
				}

				//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - num tiles on room before add adjacents: %d"), r.LogicPosInRoom.Num());

				rooms.Add(r);
				if (!roomByLogPos.Contains(p)) {
					roomByLogPos.Add(p, r);
				}

				if (mapManager->StaticMapRef->GetXYMap()[p] == 's') {
					r.IsSpawnRoom = true;
					SpawnRoom = &r;
					UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Spawn Room Found! (2) Room %d"), r.GetIDRoom());
				}
			}

		}
	}
	UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - num Rooms calculated: %d"), rooms.Num());
	int TotalTilesOnRooms = 0;
	for (int i = 0; i < rooms.Num(); i++) {
		TotalTilesOnRooms += rooms[i].LogicPosInRoom.Num();
		UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Rooms %d: %d tiles "), i, rooms[i].LogicPosInRoom.Num());
		if (rooms[i].IsSpawnRoom) {

			UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Rooms spawn id %d"), i);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Total Tiles on rooms: %d"), TotalTilesOnRooms);
	UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Total Tiles process: %d"), tilesProcess); ;

}


#pragma endregion

#pragma region ROOM

PD_MM_Room::PD_MM_Room()
{
	IsSpawnRoom = false;
	LogicPosInRoom = TArray<PD_MG_LogicPosition>();

	tiles = TMap<PD_MG_LogicPosition, AActor*>();
	walls = TMap<PD_MG_LogicPosition, AActor*>();
	interactuables = TMap<PD_MG_LogicPosition, AActor*>();

	IDRoom = rand() + 100;
}

PD_MM_Room::PD_MM_Room(int idRoom)
{
	PD_MM_Room();
	IDRoom = idRoom;

}


PD_MM_Room::~PD_MM_Room()
{
}

bool PD_MM_Room::AddLogicPos(PD_MG_LogicPosition logpos) { return LogicPosInRoom.AddUnique(logpos) >= 0; }
bool PD_MM_Room::AddTile(PD_MG_LogicPosition logpos, AActor* tile) { return false; }
bool PD_MM_Room::AddWall(PD_MG_LogicPosition logpos, AActor* wall) { return false; }
bool PD_MM_Room::AddInteractuable(PD_MG_LogicPosition logpos, AActor* interactuable) { return false; }
#pragma endregion