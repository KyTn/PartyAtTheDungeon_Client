// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_MM_MapInfo.h"
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "MapGeneration/Static/PD_MG_StaticMap.h"
#include "GM_Game/PD_GM_MapManager.h"
#include "Actors/Interactuables/PD_E_Interactuable.h"
#include "Actors/Interactuables/PD_E_Door.h"


#pragma region INTERACTUABLE INFO

PD_MM_InteractuableInfo::PD_MM_InteractuableInfo(PD_MG_LogicPosition lp, int id, StaticMapElement t, FString name) {
	Name_Interactuable = name;
	logpos = lp;
	IDInteractuable = id;
	type = t;
	reactuables = TArray<int>();
}

PD_MM_InteractuableInfo::~PD_MM_InteractuableInfo() {}


void PD_MM_InteractuableInfo::AddReactuableID(int id) {
	reactuables.Add(id);
}



#pragma endregion

#pragma region DOOR INFO


PD_MM_DoorInfo::PD_MM_DoorInfo(PD_MG_LogicPosition lp, int id, StaticMapElement t, PD_MM_Room* roomA, PD_MM_Room* roomB) {
	logpos = lp;
	IDInteractuable = id;
	reactuables = TArray<int>();
	type = t;
	room_ConnA = roomA;
	room_ConnB = roomB;
	CalculateConnectors();
}



PD_MM_DoorInfo::~PD_MM_DoorInfo() {}


void PD_MM_DoorInfo::CalculateConnectors() {

	TArray<PD_MG_LogicPosition>lps = logpos.GenerateAdjacents();
	for (int i = 0; i < lps.Num(); i++) {
		if (room_ConnA->PropsAndTilesInRoomByLogicPosition.Contains(lps[i])) {
			connA = lps[i];
		}
		if (room_ConnB->PropsAndTilesInRoomByLogicPosition.Contains(lps[i])) {
			connB = lps[i];
		}
	}
}

void PD_MM_DoorInfo::AddReactuableID(int id) {
	reactuables.Add(id);
}

#pragma endregion


#pragma region MAP INFO


PD_MM_MapInfo::PD_MM_MapInfo(PD_GM_MapManager* mM)
{
	mapManager = mM;

	// Rooms
	mapAdj = TMap<uint8, TArray<uint8>>();
	allLogicPos = TArray<PD_MG_LogicPosition>();
	rooms = TArray<PD_MM_Room*>();
	roomByLogPos = TMap<PD_MG_LogicPosition, PD_MM_Room*>();
	roomByIDRoom = TMap<int, PD_MM_Room*>();

	// Doors e interactaubles

	doorInfoInMap = TArray<PD_MM_DoorInfo*>();
	interactuableInfoInMap = TArray<PD_MM_InteractuableInfo*>();

	// door localisation & id
	doorActorByLogPos = TMap<PD_MG_LogicPosition, APD_E_Door*>();
	doorActorByID = TMap<int, APD_E_Door*>();

	//door logic info
	doorInfoByLogPos = TMap<PD_MG_LogicPosition, PD_MM_DoorInfo*>();
	doorInfoByID = TMap<int, PD_MM_DoorInfo*>();

	// interactuable localisation & id
	interactuableActorByID = TMap<int, APD_E_Interactuable*>();
	interactuableActorByLogicPosition = TMap<PD_MG_LogicPosition, APD_E_Interactuable*>();

	//door logic info
	interactuableInfoByLogPos = TMap<PD_MG_LogicPosition, PD_MM_InteractuableInfo*>();
	interactuableInfoByID = TMap<int, PD_MM_InteractuableInfo*>();

}

PD_MM_MapInfo::~PD_MM_MapInfo()
{
}

PD_MM_Room* PD_MM_MapInfo::AddRoom(int IDRoom) {
	PD_MM_Room* nr = new PD_MM_Room(IDRoom);
	rooms.Add(nr);

	if (!roomByIDRoom.Contains(IDRoom)) {
		roomByIDRoom.Add(IDRoom, nr);
	}

	return nr;
}


PD_MM_Room* PD_MM_MapInfo::RoomOf(PD_MG_LogicPosition logpos) {
	//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::RoomO %d"), roomByLogPos[logpos].walls.Num());
	//TArray<PD_MG_LogicPosition> positions = TArray<PD_MG_LogicPosition>();
	//roomByLogPos.GenerateKeyArray(positions);

	for (int i = 0; i < rooms.Num(); i++) {

		if (rooms[i]->LogicPosInRoom.Contains(logpos)) {
			return rooms[i];
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


bool PD_MM_MapInfo::IsDoorInstantiatedAt(PD_MG_LogicPosition lp) {

	return doorActorByLogPos.Contains(lp);
}




bool PD_MM_MapInfo::AddWall(PD_MG_LogicPosition logpos, APD_E_ElementActor *wall)
{
	//UE_LOG(LogTemp, Warning, TEXT("logpos %d %d"), logpos.GetX(), logpos.GetY());

	for (int i = 0; i < rooms.Num(); i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::AddWall %d numm: %d"), rooms[i].GetIDRoom(), rooms[i].LogicPosInRoom.Num());

		for (int j = 0; j < rooms[i]->LogicWallPosInRoom.Num(); j++)
		{

			if (rooms[i]->LogicWallPosInRoom[j] == logpos) {
				//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::room %d numPos: %d numWalls:%d"), i , rooms[i].LogicPosInRoom.Num(), rooms[i].walls.Num());
				FString s = "Wall (";
				s.AppendInt(logpos.GetX());
				s.Append(",");
				s.AppendInt(logpos.GetY());
				s.Append(")");
				//wall->SetActorLabel(*s);
				//roomByLogPos[logpos].walls.Add(logpos, wall);
				rooms[i]->walls.Add(logpos, wall);
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

bool PD_MM_MapInfo::AddTile(PD_MG_LogicPosition logpos, APD_E_ElementActor* tile)
{
	for (int i = 0; i < rooms.Num(); i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::AddWall %d numm: %d"), rooms[i].GetIDRoom(), rooms[i].LogicPosInRoom.Num());

		for (int j = 0; j < rooms[i]->LogicPosInRoom.Num(); j++)
		{
			if (rooms[i]->LogicPosInRoom[j] == logpos)
			{
				//currentTileUE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::room %d numPos: %d numTile:%d"), i , rooms[i].LogicPosInRoom.Num(), rooms[i].tiles.Num());
				FString s = "Tile (";
				s.AppendInt(logpos.GetX());
				s.Append(",");
				s.AppendInt(logpos.GetY());
				s.Append(")");
				//wall->SetActorLabel(*s);
				//roomByLogPos[logpos].walls.Add(logpos, wall);
				rooms[i]->tiles.Add(logpos, tile);
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

bool PD_MM_MapInfo::AddInteractuable(PD_MG_LogicPosition logpos, APD_E_ElementActor* wall)
{
	/*
	if (roomByLogPos.Contains(logpos)) {
	roomByLogPos[logpos].walls.Append(logpos, wall);
	return true;
	}
	*/

	return false;
}



bool PD_MM_MapInfo::AddDoor_WithoutLink(PD_MG_LogicPosition logpos, APD_E_Door* door) {
	doorActorByLogPos.Add(logpos, door);
	doorActorByID.Add(door->ID_Interactuable, door);
	interactuableActorByID.Add(door->ID_Interactuable, door);
	interactuableActorByLogicPosition.Add(logpos, door);

	return true;
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



//void PD_MM_MapInfo::CalculateRooms()
//{
//	int tilesProcess = 0;
//	UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Calculating rooms ... "));
//	// Por cada posicion p(x,y) en el mapa
//	for (int i = 0; i < mapManager->StaticMapRef->GetLogicPositions().Num(); i++) {
//		
//		PD_MG_LogicPosition p = mapManager->StaticMapRef->GetLogicPositions()[i];
//
//		allLogicPos.Add(p);
//
//		// Si p es un tile
//		if (mapManager->StaticMapRef->GetXYMap()[p] == '.' ||
//			mapManager->StaticMapRef->GetXYMap()[p] == ',' ||
//			mapManager->StaticMapRef->GetXYMap()[p] == 's')
//			{
//
//			// Tener una referencia a una habitacion r
//			++tilesProcess;
//
//			bool roomFound = false;
//
//			//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Actual num of Rooms %d"), rooms.Num());
//			// Por cada habitacion r1
//			for (int j = 0; j < rooms.Num(); j++) {
//				//r = rooms[j]; // hacer r1 = r
//				// Si p está en una habitacion r
//				//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Actual num tiles on Room: %d"), rooms[j].LogicPosInRoom.Num());
//				if (rooms[j]->LogicPosInRoom.Contains(p)) {
//					roomFound = true;
//					//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - reusing Room created at (%d,%d) ... "), p.GetX(), p.GetY());
//
//					// Añadir la lista de posiciones p_a(p) adyacentes de p a r1
//					TArray<PD_MG_LogicPosition> a = mapManager->Get_LogicPosition_Adyacents_To(p);
//					for (int k = 0; k < a.Num(); k++) {
//						// Si p es un tile
//						if (mapManager->StaticMapRef->GetXYMap()[p] == '.' ||
//							mapManager->StaticMapRef->GetXYMap()[p] == ',' ||
//							mapManager->StaticMapRef->GetXYMap()[p] == 's')
//						{
//							rooms[j]->AddLogicPos(a[k]);
//							//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - adding to Room created at (%d,%d) <- (%d,%d) adjacents ... "), p.GetX(), p.GetY(), a[k].GetX(), a[k].GetY());
//
//						}
//						
//					}
//
//					if (mapManager->StaticMapRef->GetXYMap()[p] == 's') {
//						rooms[j]->IsSpawnRoom = true;
//						SpawnRoom = rooms[j] ;
//						SpawnRoomIndex = j;
//						UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Spawn Room Found! Room %d"), rooms[j]->GetIDRoom());
//					}
//					break;
//				}
//			}
//
//			// Si no está en ninguna habitacion
//			if (!roomFound) {
//				// Crear habitacion r
//
//				PD_MM_Room r = PD_MM_Room(rooms.Num());
//
//				//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - new Room created at (%d,%d) "), p.GetX(), p.GetY());
//				
//
//				// Añadir p a r1
//				r.AddLogicPos(p);
//				// Añadir la lista de posiciones p_a(p) adyacentes de p a r1
//				TArray<PD_MG_LogicPosition> a = mapManager->Get_LogicPosition_Adyacents_To(p);
//				//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - \tadding to Room created at (%d,%d) %d adjacents ... "), p.GetX(), p.GetY(), a.Num() );
//				for (int k = 0; k < a.Num(); k++) {
//					if (mapManager->StaticMapRef->GetXYMap()[a[k]] == '.' ||
//						mapManager->StaticMapRef->GetXYMap()[a[k]] == 's'  ) {
//							r.AddLogicPos(a[k]);
//							//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - adding to Room created at (%d,%d) <- (%d,%d) adjacents ... "), p.GetX(), p.GetY(), a[k].GetX(), a[k].GetY());
//					}
//					
//				}
//
//				//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - num tiles on room before add adjacents: %d"), r.LogicPosInRoom.Num());
//
//				rooms.Add(&r);
//				if (!roomByLogPos.Contains(p)) {
//					roomByLogPos.Add(p, rooms[rooms.Num()-1] );
//				}
//
//				if (mapManager->StaticMapRef->GetXYMap()[p] == 's') {
//					r.IsSpawnRoom = true;
//					SpawnRoom = rooms[rooms.Num()-1];
//					SpawnRoomIndex = rooms.Num() - 1;
//					UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Spawn Room Found! (2) Room %d"), r.GetIDRoom());
//				}
//			}
//
//		}
//	}
//	UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - num Rooms calculated: %d"), rooms.Num());
//	int TotalTilesOnRooms = 0;
//	for (int i = 0; i < rooms.Num(); i++) {
//		TotalTilesOnRooms += rooms[i]->LogicPosInRoom.Num();
//		UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Rooms %d: %d tiles "), i, rooms[i]->LogicPosInRoom.Num());
//		if (rooms[i]->IsSpawnRoom) {
//
//			UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Rooms spawn id %d"), i);
//		}
//	}
//	
//	UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Total Tiles on rooms: %d"), TotalTilesOnRooms);
//	UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms() - Total Tiles process: %d"), tilesProcess); ;
//
//}





//void PD_MM_MapInfo::CalculateRooms_v2() {
//
//	TArray<PD_MG_LogicPosition> pos, pos_wall;
//	for (int i = 0; i < mapManager->StaticMapRef->GetLogicPositions().Num(); i++) {
//		allLogicPos.Add(mapManager->StaticMapRef->GetLogicPositions()[i]);
//		pos.Add(mapManager->StaticMapRef->GetLogicPositions()[i]);
//		pos_wall.Add(mapManager->StaticMapRef->GetLogicPositions()[i]);
//	}
//
//	#pragma region Create rooms by flooding 
//
//	while (pos.Num() > 0) {
//		TArray<PD_MG_LogicPosition> *tiles = new TArray<PD_MG_LogicPosition>();
//		//pos.RemoveAt(0);
//		while (!mapManager->IsLogicPositionATile(pos[0])) {
//			//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms_v2() - Removing NOT A TILE from pos at (%d, %d)"), pos[0].GetX(), pos[0].GetY());
//			
//			pos.RemoveAt(0);
//
//			if (pos.Num() == 0) break;
//		}
//
//		if (pos.Num() == 0) break;
//
//		//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms_v2() - searching tiles by flowing at (%d, %d) on a poblation of %d elements"), pos[0].GetX(), pos[0].GetY(), pos.Num());
//		FindTilesOnRoomByFlowdingAt(pos[0], mapManager->StaticMapRef->GetLogicPositions(), tiles);
//		UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms_v2() - %d tiles found"), tiles->Num());
//
//		PD_MM_Room* r = new PD_MM_Room(rooms.Num());
//
//		for (int i = 0; i < tiles->Num(); i++) {
//			r->AddLogicPos((*tiles)[i]);
//			r->AddLogicTilePos((*tiles)[i]);
//			pos.Remove((*tiles)[i]);
//
//			// si la posicion lógica es un punto de spawn, marcamos la habitacion como de spawn
//			if (mapManager->IsLogicPositionASpawn((*tiles)[i])) {
//				r->IsSpawnRoom = true;
//			}
//		}
//
//
//
//		// añadimos la habitacion a la lista de habitaciones una vez está totalmente completa. Si es de spawn, guardamos una referencia desde el array
//		rooms.Add(r);
//		if (r->IsSpawnRoom) {
//			SpawnRoom = rooms[rooms.Num() - 1];
//			SpawnRoomIndex = rooms.Num() - 1;
//		}
//
//		// añadimos las logicpos de las tiles para busquedas inversas 
//		for (int i = 0; i < tiles->Num(); i++) {
//			if (!roomByLogPos.Contains((*tiles)[i])) {
//				roomByLogPos.Add((*tiles)[i], rooms[rooms.Num()-1] );
//			}
//		}
//		tiles->Empty();
//	}
//	
//#pragma endregion
//
//
//
//
//	#pragma region Searching Walls and Doors by Adjacent Tiles 
//
//	/*	por cada logic pos en el mapa
//		si es un muro / puerta
//			obtener las habitaciones con las que colinda (adjc+diag)
//			actualizar 
//	*/
//
//
//	for (int i_lp = 0; i_lp < mapManager->StaticMapRef->GetLogicPositions().Num(); i_lp++) {
//		PD_MG_LogicPosition p = mapManager->StaticMapRef->GetLogicPositions()[i_lp];
//		if (mapManager->IsLogicPositionAWall(p) || mapManager->IsLogicPositionADoor(p)) {
//
//			TArray<PD_MM_Room*> idrooms = TArray<PD_MM_Room*>();
//
//			TArray<PD_MG_LogicPosition> adjacents = mapManager->Get_LogicPosition_Diagonals_And_Adyacents_To(p);
//
//			for (int i_adj = 0; i_adj < adjacents.Num(); i_adj++) {
//				PD_MG_LogicPosition adj = adjacents[i_adj];
//				if (mapManager->IsLogicPositionATile(adj)) {
//					idrooms.AddUnique(roomByLogPos[adj]);
//				}
//			}
//
//			for (int i_idr = 0; i_idr < idrooms.Num(); i_idr++) {
//				idrooms[i_idr]->AddLogicPos(p);
//				if (mapManager->IsLogicPositionAWall(p)) 
//					idrooms[i_idr]->AddLogicWallPos(p);
//				if (mapManager->IsLogicPositionADoor(p)) 
//					idrooms[i_idr]->AddLogicDoorPos(p);
//			}
//		}
//	}
//
//
//
//
//
//
//
//
//	/*
//
//	// Por cada habitación 
//	for (int i_R = 0; i_R < rooms.Num(); i_R++) {
//		TArray<PD_MG_LogicPosition> *walls = new TArray<PD_MG_LogicPosition>();
//		TArray<PD_MG_LogicPosition> *doors = new TArray<PD_MG_LogicPosition>();
//
//		// buscamos una tile que esté pegada a una pared 
//		PD_MG_LogicPosition initialWallorDoor;
//		bool found = false;
//
//		// lo buscamos ... 
//		for (int i_T = 0; i_T < rooms[i_R].LogicPosInRoom.Num(); i_T++) {
//
//			TArray<PD_MG_LogicPosition> adj = mapManager->Get_LogicPosition_Diagonals_And_Adyacents_To(rooms[i_R].LogicTilesPosInRoom[i_T]);
//
//			for (int i_Adj = 0; i_Adj < adj.Num(); i_Adj++) {
//				if (mapManager->IsLogicPositionAWall(adj[i_Adj]) || mapManager->IsLogicPositionADoor(adj[i_Adj])) {
//					initialWallorDoor = adj[i_Adj];
//					found = true;
//					break;
//				}
//			}
//
//			if (found) break;
//		}
//
//
//		// aqui debemos tener encontrado al menos uno
//		if (!found) {
//			UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::CalculateRooms_v2() ERROR: Searching walls for room id:%d not found"), rooms[i_R].GetIDRoom());
//		}
//
//
//		FindWallsAndDoorsOnRoomByFloodingAndAdyacentTiles(rooms[i_R].GetIDRoom(), initialWallorDoor, pos_wall, walls, doors);
//
//		// añadimos todas las paredes de la habitacion en cuestion
//		for (int i = 0; i < walls->Num(); i++) {
//			rooms[i_R].AddLogicPos((*walls)[i]);
//			rooms[i_R].AddLogicWallPos((*walls)[i]);
//			//roomByLogPos.Add((*walls)[i], rooms[i_R]);
//		}
//
//		// y añadimos todas las puertas de la habitacion en cuestion
//		for (int i = 0; i < doors->Num(); i++) {
//			rooms[i_R].AddLogicPos((*doors)[i]);
//			rooms[i_R].AddLogicWallPos((*doors)[i]);
//			//roomByLogPos.Add((*doors)[i], rooms[i_R]);
//		}
//	}
//	*/
//
//#pragma endregion
//
//
//}

//void PD_MM_MapInfo::FindTilesOnRoomByFlowdingAt(PD_MG_LogicPosition initial, TArray<PD_MG_LogicPosition> PoblationSearch, TArray<PD_MG_LogicPosition>* tilesOnRoom) {
//
//	// Si el conjunto de salida no contiene al initial, se añade como parte del conjunto final. 
//	if(!(*tilesOnRoom).Contains(initial)) (*tilesOnRoom).Add(initial);
//
//	// Generamos los adjacentes a initial que puedan serlo segun el conjunto de busqueda.
//	TArray<PD_MG_LogicPosition> adjacents = initial.GetAdjacentsFromList(PoblationSearch);
//
//	// por cada posible adjacente que pueda serlo, añadirlo al conjunto de salida y continuar el flowding desde el adjacent
//	for (int i = 0; i < adjacents.Num(); i++) {
//		PD_MG_LogicPosition adj = adjacents[i];
//
//		
//		
//
//		// Si es un tile, abrimos el flow por aqui. 
//		if (mapManager->IsLogicPositionATile(adj) || mapManager->IsLogicPositionASpawn(adj))
//		{
//
//			// Si el adjacente ya está en el conjunto final, es que he pasado por ahi antes, por lo que cierra esta rama del flow
//			if ((*tilesOnRoom).Contains(adj)) continue;
//
//			FindTilesOnRoomByFlowdingAt(adj, PoblationSearch, tilesOnRoom);
//		}
//
//
//	}
//
//
//}


//void PD_MM_MapInfo::FindWallsAndDoorsOnRoomByFloodingAndAdyacentTiles(int IDRoom, PD_MG_LogicPosition initial, TArray<PD_MG_LogicPosition> PoblationSearch, TArray<PD_MG_LogicPosition>* WallsOnRoom, TArray<PD_MG_LogicPosition>* DoorsOnRoom) {
//
//	// Si el conjunto de salida no contiene al initial, se añade como parte del conjunto final. 
//	if (mapManager->IsLogicPositionADoor(initial) && !(*DoorsOnRoom).Contains(initial)) (*DoorsOnRoom).Add(initial);
//	if (mapManager->IsLogicPositionAWall(initial) && !(*WallsOnRoom).Contains(initial)) (*WallsOnRoom).Add(initial);
//
//
//	// Generamos los adjacentes a initial que puedan serlo segun el conjunto de busqueda.
//	TArray<PD_MG_LogicPosition> adjacents = initial.GetAdjacentsFromList(PoblationSearch);
//
//	// por cada posible adjacente que pueda serlo, añadirlo al conjunto de salida y continuar el flowding desde el adjacent
//	for (int i = 0; i < adjacents.Num(); i++) {
//		PD_MG_LogicPosition adj = adjacents[i];
//
//
//
//		// Si es una pared, abrimos el flow por aqui. 
//		if (mapManager->IsLogicPositionAWall(adj) )
//		{
//
//			// Si el adjacente ya está en el conjunto final, es que he pasado por ahi antes, por lo que cierra esta rama del flow
//			if ((*WallsOnRoom).Contains(adj)) continue;
//
//			// si no está en el conjunto inicial, tenemos que ver si es adjacente a algun tile de la habitacion que estamos buscando
//			// generamos los adjacentes y diagonales al muro que estamos preguntando
//			TArray<PD_MG_LogicPosition> adjacents_w = mapManager->Get_LogicPosition_Diagonals_And_Adyacents_To(adj);
//
//			UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::FindWallsAndDoorsOnRoomByFloodingAndAdyacentTiles() sacando adjacentes para (%d,%d) - roomByLogPos with %d elements "), adj.GetX(), adj.GetY(), roomByLogPos.Num());
//			for (int i_w = 0; i_w < adjacents_w.Num(); i++) {
//				PD_MG_LogicPosition adj_w = adjacents_w[i_w];
//
//				// si el adjacente del muro si es un tile, y ademas, es de la misma habitacion que el muro, el muro es de esa habitacion. 
//
//				//UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::FindWallsAndDoorsOnRoomByFloodingAndAdyacentTiles() .... adj_w (%d,%d) esta en la habitacion"), adj_w.GetX(), adj_w.GetY());
//				if (mapManager->IsLogicPositionATile(adj_w)) {
//					UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::FindWallsAndDoorsOnRoomByFloodingAndAdyacentTiles() .... adj_w (%d,%d) esta en la habitacion y es un tile "), adj_w.GetX(), adj_w.GetY());
//				}
//				else {
//					UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::FindWallsAndDoorsOnRoomByFloodingAndAdyacentTiles() .... adj_w (%d,%d) esta en la habitacion y NO es un tile "), adj_w.GetX(), adj_w.GetY());
//				}
//				if (mapManager->IsLogicPositionATile(adj_w) && roomByLogPos[adj_w]->GetIDRoom() == IDRoom) {
//					FindWallsAndDoorsOnRoomByFloodingAndAdyacentTiles(IDRoom, adj, PoblationSearch, WallsOnRoom, DoorsOnRoom);
//					break;
//				}
//			}
//		}
//
//
//		// Si es una puerta, abrimos el flow por aqui. 
//		if (mapManager->IsLogicPositionADoor(adj))
//		{
//
//			// Si el adjacente ya está en el conjunto final, es que he pasado por ahi antes, por lo que cierra esta rama del flow
//			if ((*DoorsOnRoom).Contains(adj)) continue;
//
//			// si no está en el conjunto inicial, tenemos que ver si es adjacente a algun tile de la habitacion que estamos buscando
//			// generamos los adjacentes y diagonales de la puerta que estamos preguntando
//			TArray<PD_MG_LogicPosition> adjacents_w = mapManager->Get_LogicPosition_Diagonals_And_Adyacents_To(adj);
//			for (int i_w = 0; i_w < adjacents_w.Num(); i++) {
//				PD_MG_LogicPosition adj_w = adjacents_w[i];
//
//				// si el adjacente de la puerta si es un tile, y ademas, es de la misma habitacion que la puerta, es de esa habitacion. 
//				if (mapManager->IsLogicPositionATile(adj_w) && roomByLogPos[adj_w]->GetIDRoom() == IDRoom) {
//					FindWallsAndDoorsOnRoomByFloodingAndAdyacentTiles(IDRoom, adj, PoblationSearch, WallsOnRoom, DoorsOnRoom);
//				}
//			}
//		}
//
//
//	}
//
//}


#pragma endregion

#pragma region ROOM

PD_MM_Room::PD_MM_Room()
{
	IsInstantiated = false;
	IsSpawnRoom = false;
	LogicPosInRoom = TArray<PD_MG_LogicPosition>();
	//LogicTilesPosInRoom = TArray<PD_MG_LogicPosition>();
	LogicWallPosInRoom = TArray<PD_MG_LogicPosition>();
	LogicDoorPosInRoom = TArray<PD_MG_LogicPosition>();
	LogicInteractuablesPosInRoom = TArray<PD_MG_LogicPosition>();

	PropsAndTilesInRoomByLogicPosition = TMap<PD_MG_LogicPosition, StaticMapElement>();
	InteractuableInfoInRoomByLogicPosition = TMap<PD_MG_LogicPosition, PD_MM_InteractuableInfo*>();
	DoorInfoInRoomByLogicPosition = TMap<PD_MG_LogicPosition, PD_MM_DoorInfo*>();


	tiles = TMap<PD_MG_LogicPosition, APD_E_ElementActor*>();
	walls = TMap<PD_MG_LogicPosition, APD_E_ElementActor*>();
	interactuables = TMap<PD_MG_LogicPosition, APD_E_Interactuable*>();

	IDRoom = rand() + 100;
}

PD_MM_Room::PD_MM_Room(int idRoom)
{
	IsInstantiated = false;
	IsSpawnRoom = false;
	LogicPosInRoom = TArray<PD_MG_LogicPosition>();
	//LogicTilesPosInRoom = TArray<PD_MG_LogicPosition>();
	LogicWallPosInRoom = TArray<PD_MG_LogicPosition>();
	LogicDoorPosInRoom = TArray<PD_MG_LogicPosition>();
	LogicInteractuablesPosInRoom = TArray<PD_MG_LogicPosition>();

	PropsAndTilesInRoomByLogicPosition = TMap<PD_MG_LogicPosition, StaticMapElement>();
	InteractuableInfoInRoomByLogicPosition = TMap<PD_MG_LogicPosition, PD_MM_InteractuableInfo*>();
	DoorInfoInRoomByLogicPosition = TMap<PD_MG_LogicPosition, PD_MM_DoorInfo*>();


	tiles = TMap<PD_MG_LogicPosition, APD_E_ElementActor*>();
	walls = TMap<PD_MG_LogicPosition, APD_E_ElementActor*>();
	interactuables = TMap<PD_MG_LogicPosition, APD_E_Interactuable*>();

	IDRoom = idRoom;

}


PD_MM_Room::~PD_MM_Room()
{
}

bool PD_MM_Room::AddTile(PD_MG_LogicPosition logpos, APD_E_ElementActor* tile) { return tiles.Add(logpos, tile) != nullptr; }
bool PD_MM_Room::AddWall(PD_MG_LogicPosition logpos, APD_E_ElementActor* wall) { return walls.Add(logpos, wall) != nullptr; }
bool PD_MM_Room::AddInteractuable(PD_MG_LogicPosition logpos, APD_E_Interactuable* interactuable) { return interactuables.Add(logpos, interactuable) != nullptr; }
#pragma endregion