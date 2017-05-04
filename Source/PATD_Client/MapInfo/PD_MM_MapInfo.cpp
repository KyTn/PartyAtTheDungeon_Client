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

	CalculateRooms_v2();
}

PD_MM_MapInfo::~PD_MM_MapInfo()
{
}

PD_MM_Room* PD_MM_MapInfo::RoomOf(PD_MG_LogicPosition logpos) {
	//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::RoomO %d"), roomByLogPos[logpos].walls.Num());
	//TArray<PD_MG_LogicPosition> positions = TArray<PD_MG_LogicPosition>();
	//roomByLogPos.GenerateKeyArray(positions);

	for (int i = 0; i < rooms.Num(); i++) {

		if (rooms[i].LogicPosInRoom.Contains(logpos)) {
			return &(rooms[i]);
		}

		//for (int j = 0; j < rooms[i].LogicPosInRoom.Num(); j++) {
		//if (rooms[i].LogicPosInRoom[j] == logpos) {
		//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::RoomO %d %d"), rooms[i].LogicPosInRoom[j].GetX(), rooms[i].LogicPosInRoom[j].GetY());

		//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::RoomO %d"), rooms[i].walls.Num());

		//return  &(rooms[i]);
		//}
		//}
	}
	return nullptr;
}

bool PD_MM_MapInfo::AddWall(PD_MG_LogicPosition logpos, AActor *wall)
{
	//UE_LOG(LogTemp, Warning, TEXT("logpos %d %d"), logpos.GetX(), logpos.GetY());

	for (int i = 0; i < rooms.Num(); i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::AddWall %d numm: %d"), rooms[i].GetIDRoom(), rooms[i].LogicPosInRoom.Num());

		for (int j = 0; j < rooms[i].LogicWallPosInRoom.Num(); j++)
		{

			if (rooms[i].LogicWallPosInRoom[j] == logpos) {
				//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::room %d numPos: %d numWalls:%d"), i , rooms[i].LogicPosInRoom.Num(), rooms[i].walls.Num());
				FString s = "Wall (";
				s.AppendInt(logpos.GetX());
				s.Append(",");
				s.AppendInt(logpos.GetY());
				s.Append(")");
				//wall->SetActorLabel(*s);
				//roomByLogPos[logpos].walls.Add(logpos, wall);
				rooms[i].walls.Add(logpos, wall);
				return true;
			}
		}
	}

	/*
	if (roomByLogPos.Contains(logpos)) {
	UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::AddWall"));
	FString s = "Wall (";
	s.AppendInt(logpos.GetX());
	s.Append(",");
	s.AppendInt(logpos.GetY());
	s.Append(")");
	//wall->SetActorLabel(*s);
	roomByLogPos[logpos].walls.Add(logpos, wall);
	return true;
	}
	*/

	return false;
}

bool PD_MM_MapInfo::AddTile(PD_MG_LogicPosition logpos, AActor* tile)
{
	for (int i = 0; i < rooms.Num(); i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::AddWall %d numm: %d"), rooms[i].GetIDRoom(), rooms[i].LogicPosInRoom.Num());

		for (int j = 0; j < rooms[i].LogicPosInRoom.Num(); j++)
		{
			if (rooms[i].LogicPosInRoom[j] == logpos)
			{
				//currentTileUE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::room %d numPos: %d numTile:%d"), i , rooms[i].LogicPosInRoom.Num(), rooms[i].tiles.Num());
				FString s = "Tile (";
				s.AppendInt(logpos.GetX());
				s.Append(",");
				s.AppendInt(logpos.GetY());
				s.Append(")");
				//wall->SetActorLabel(*s);
				//roomByLogPos[logpos].walls.Add(logpos, wall);
				rooms[i].tiles.Add(logpos, tile);
				return true;
			}
		}
	}
	




	/*if (roomByLogPos.Contains(logpos)) {
		FString s = "Tile (";
		s.AppendInt(logpos.GetX());
		s.Append(",");
		s.AppendInt(logpos.GetY());
		s.Append(")");
		//tile->SetActorLabel(*s);
		roomByLogPos[logpos].tiles.Add(logpos, tile);
		return true;
	}
	*/

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

		// Si p es un tile
		if (mapManager->StaticMapRef->GetXYMap()[p] == '.' ||
			mapManager->StaticMapRef->GetXYMap()[p] == ',' ||
			mapManager->StaticMapRef->GetXYMap()[p] == 's')
		{

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
						// Si p es un tile
						if (mapManager->StaticMapRef->GetXYMap()[p] == '.' ||
							mapManager->StaticMapRef->GetXYMap()[p] == ',' ||
							mapManager->StaticMapRef->GetXYMap()[p] == 's')
						{
							rooms[j].AddLogicPos(a[k]);
							//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - adding to Room created at (%d,%d) <- (%d,%d) adjacents ... "), p.GetX(), p.GetY(), a[k].GetX(), a[k].GetY());

						}

					}

					if (mapManager->StaticMapRef->GetXYMap()[p] == 's') {
						rooms[j].IsSpawnRoom = true;
						SpawnRoom = &rooms[j];
						SpawnRoomIndex = j;
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
					SpawnRoom = &rooms[rooms.Num() - 1];
					SpawnRoomIndex = rooms.Num() - 1;
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


void PD_MM_MapInfo::CalculateRooms_v2() {

	TArray<PD_MG_LogicPosition> pos;
	for (int i = 0; i < mapManager->StaticMapRef->GetLogicPositions().Num(); i++) {
		pos.Add(mapManager->StaticMapRef->GetLogicPositions()[i]);
	}

	while (pos.Num() > 0) {
		TArray<PD_MG_LogicPosition> *tiles = new TArray<PD_MG_LogicPosition>(), *walls = new TArray<PD_MG_LogicPosition>();
		//pos.RemoveAt(0);
		while (!mapManager->IsLogicPositionATile(pos[0])) {
			UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms_v2() - Removing NOT A TILE from pos at (%d, %d)"), pos[0].GetX(), pos[0].GetY());
			pos.RemoveAt(0);

			if (pos.Num() == 0) break;
		}

		if (pos.Num() == 0) break;

		UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms_v2() - searching tiles by flowing at (%d, %d) on a poblation of %d elements"), pos[0].GetX(), pos[0].GetY(), pos.Num());
		FindTilesOnRoomByFlowdingAt(pos[0], mapManager->StaticMapRef->GetLogicPositions(), tiles, walls);
		UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms_v2() - %d tiles found, %d walls found"), tiles->Num(), walls->Num());

		PD_MM_Room r = PD_MM_Room(rooms.Num());

		for (int i = 0; i < tiles->Num(); i++) {
			r.AddLogicPos((*tiles)[i]);
			pos.Remove((*tiles)[i]);

			// si la posicion lógica es un punto de spawn, marcamos la habitacion como de spawn
			if (mapManager->IsLogicPositionASpawn((*tiles)[i])) {
				r.IsSpawnRoom = true;
			}
		}

		for (int i = 0; i < walls->Num(); i++) {
			r.AddLogicWallPos((*walls)[i]);
		}



		// añadimos la habitacion a la lista de habitaciones una vez está totalmente completa. Si es de spawn, guardamos una referencia desde el array
		rooms.Add(r);
		if (r.IsSpawnRoom) {
			SpawnRoom = &rooms[rooms.Num() - 1];
			SpawnRoomIndex = rooms.Num() - 1;
		}

		// añadimos las logicpos de las tiles para busquedas inversas 
		for (int i = 0; i < tiles->Num(); i++) {
			if (!roomByLogPos.Contains((*tiles)[i])) {
				roomByLogPos.Add((*tiles)[i], r);
			}
		}


	}
}

void PD_MM_MapInfo::FindTilesOnRoomByFlowdingAt(PD_MG_LogicPosition initial, TArray<PD_MG_LogicPosition> PoblationSearch, TArray<PD_MG_LogicPosition>* tilesOnRoom, TArray<PD_MG_LogicPosition>* wallsOnRoom) {

	// Si el conjunto de salida no contiene al initial, se añade como parte del conjunto final. 
	if (!(*tilesOnRoom).Contains(initial)) (*tilesOnRoom).Add(initial);

	// Generamos los adjacentes a initial que puedan serlo segun el conjunto de busqueda.
	TArray<PD_MG_LogicPosition> adjacents = initial.GetAdjacentsFromList(PoblationSearch);

	// por cada posible adjacente que pueda serlo, añadirlo al conjunto de salida y continuar el flowding desde el adjacent
	for (int i = 0; i < adjacents.Num(); i++) {
		PD_MG_LogicPosition adj = adjacents[i];




		// Si es un tile, abrimos el flow por aqui. 
		if (mapManager->IsLogicPositionATile(adj) || mapManager->IsLogicPositionASpawn(adj))
		{

			// Si el adjacente ya está en el conjunto final, es que he pasado por ahi antes, por lo que cierra esta rama del flow
			if ((*tilesOnRoom).Contains(adj)) continue;

			FindTilesOnRoomByFlowdingAt(adj, PoblationSearch, tilesOnRoom, wallsOnRoom);
		}

		// Si es un wall, lo añadimos como puerta, pero no abrimos el flow por aqui

		if (mapManager->IsLogicPositionAWall(adj))
		{

		}

	}


}


#pragma endregion

#pragma region ROOM

PD_MM_Room::PD_MM_Room()
{
	IsSpawnRoom = false;
	LogicPosInRoom = TArray<PD_MG_LogicPosition>();
	LogicWallPosInRoom = TArray<PD_MG_LogicPosition>();

	tiles = TMap<PD_MG_LogicPosition, AActor*>();
	walls = TMap<PD_MG_LogicPosition, AActor*>();
	interactuables = TMap<PD_MG_LogicPosition, AActor*>();

	IDRoom = rand() + 100;
}

PD_MM_Room::PD_MM_Room(int idRoom)
{
	IsSpawnRoom = false;
	LogicPosInRoom = TArray<PD_MG_LogicPosition>();
	LogicWallPosInRoom = TArray<PD_MG_LogicPosition>();

	tiles = TMap<PD_MG_LogicPosition, AActor*>();
	walls = TMap<PD_MG_LogicPosition, AActor*>();
	interactuables = TMap<PD_MG_LogicPosition, AActor*>();

	IDRoom = idRoom;

}


PD_MM_Room::~PD_MM_Room()
{
}

bool PD_MM_Room::AddLogicPos(PD_MG_LogicPosition logpos) { return LogicPosInRoom.AddUnique(logpos) >= 0; }
bool PD_MM_Room::AddLogicWallPos(PD_MG_LogicPosition logpos) { return LogicWallPosInRoom.AddUnique(logpos) >= 0; }
bool PD_MM_Room::AddTile(PD_MG_LogicPosition logpos, AActor* tile) { return tiles.Add(logpos, tile) != nullptr; }
bool PD_MM_Room::AddWall(PD_MG_LogicPosition logpos, AActor* wall) { return walls.Add(logpos, wall) != nullptr; }
bool PD_MM_Room::AddInteractuable(PD_MG_LogicPosition logpos, AActor* interactuable) { return interactuables.Add(logpos, interactuable) != nullptr; }
#pragma endregion