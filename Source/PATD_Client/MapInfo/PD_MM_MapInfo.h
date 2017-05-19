// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MapGeneration/PD_MG_LogicPosition.h"
#include "GameFramework/Actor.h"

// FORWARD DECLARATIONS
class PD_MG_LogicPosition;
class PD_MG_StaticMap;
class PD_MM_Room;
class PD_GM_MapManager;
class APD_E_ElementActor;



class PATD_CLIENT_API PD_MM_Room
{
public:
public:
	PD_MM_Room();
	PD_MM_Room(int idRoom);
	~PD_MM_Room();

	bool IsInstantiated;


	bool IsSpawnRoom;
	int IDRoom;
	MapSkinType mapSkin;

	TArray<PD_MG_LogicPosition> LogicPosInRoom; // todas la posiciones logicas que tienen que ver con la habitacion, ya sean tiles, paredes, puertas ... 
	TArray<PD_MG_LogicPosition> LogicWallPosInRoom; //posiciones logicas para las paredes de la habitacion - diferenciarlas de las tiles
	TArray<PD_MG_LogicPosition> LogicDoorPosInRoom;
	TArray<PD_MG_LogicPosition> LogicInteractuablesPosInRoom;
	TMap<PD_MG_LogicPosition, StaticMapElement> PropsAndTilesInRoomByLogicPosition;

	TMap<PD_MG_LogicPosition, APD_E_ElementActor*> tiles;
	TMap<PD_MG_LogicPosition, APD_E_ElementActor*> walls;
	TMap<PD_MG_LogicPosition, APD_E_ElementActor*> interactuables;

	int GetIDRoom() { return IDRoom; };

	bool AddTile(PD_MG_LogicPosition logpos, APD_E_ElementActor* tile);
	bool AddWall(PD_MG_LogicPosition logpos, APD_E_ElementActor* wall);
	bool AddInteractuable(PD_MG_LogicPosition logpos, APD_E_ElementActor* interactuable);



	// TEST PD_MM_ROOM

	void Test() {

		UE_LOG(LogTemp, Warning, TEXT("PD_MM_Room::Test INIT TESTING OF ROOM"));

		if (IsSpawnRoom)
			UE_LOG(LogTemp, Warning, TEXT("PD_MM_Room::Test IsSpawnRoom!"));


		//UE_LOG(LogTemp, Warning, TEXT("PD_MM_Room::Test LogicTilesPosInRoom num %d"), LogicTilesPosInRoom.Num());
		UE_LOG(LogTemp, Warning, TEXT("PD_MM_Room::Test LogicWallPosInRoom num %d"), LogicWallPosInRoom.Num());
		UE_LOG(LogTemp, Warning, TEXT("PD_MM_Room::Test tiles num %d"), tiles.Num());
		UE_LOG(LogTemp, Warning, TEXT("PD_MM_Room::Test walls num %d"), walls.Num());
		UE_LOG(LogTemp, Warning, TEXT("PD_MM_Room::Test interactuables num %d"), interactuables.Num());
	}


};

/*

struct FStruct_Map
- TMap<LogPos, StaticMapElement> mapElements
- TMap<LogPos, Room> mapRooms
- int Total_Height, Total_Width;


*/

class PATD_CLIENT_API PD_MM_MapInfo
{

public:
	PD_MM_MapInfo(PD_GM_MapManager* mM);
	~PD_MM_MapInfo();

	PD_GM_MapManager* mapManager;

	PD_MM_Room* SpawnRoom;
	//int SpawnRoomIndex;

	PD_MG_LogicPosition MAP_SIZE_IN_LOGIC_POSITIONS;

	TMap<uint8, TArray<uint8>> mapAdj;

	TArray<PD_MG_LogicPosition> allLogicPos;
	TArray<PD_MM_Room*> rooms;
	TMap<int, PD_MM_Room*> roomByIDRoom;
	TMap<PD_MG_LogicPosition, PD_MM_Room*> roomByLogPos;

	FStructMapData * NETMAPDATA;

	void ShowMapData() {
		UE_LOG(LogTemp, Log, TEXT("PD_MM_MapInfo::ShowMapData - MISSION_TYPE %d "), (int)NETMAPDATA->MISSION_TYPE);
		UE_LOG(LogTemp, Log, TEXT("PD_MM_MapInfo::ShowMapData - PARSER_VERSION %s "), *(NETMAPDATA->PARSER_VERSION));
		UE_LOG(LogTemp, Log, TEXT("PD_MM_MapInfo::ShowMapData - IDRoomSpawn %d "), NETMAPDATA->IDRoomSpawn);
		UE_LOG(LogTemp, Log, TEXT("PD_MM_MapInfo::ShowMapData - Num skinByRoom %d "), NETMAPDATA->skinByRoom.Num());
		UE_LOG(LogTemp, Log, TEXT("PD_MM_MapInfo::ShowMapData - Num roomsAdj %d "), NETMAPDATA->roomsAdj.Num());
		UE_LOG(LogTemp, Log, TEXT("PD_MM_MapInfo::ShowMapData - Num roomComposition %d "), NETMAPDATA->roomComposition.Num());
		UE_LOG(LogTemp, Log, TEXT("PD_MM_MapInfo::ShowMapData - Num wallComposition %d "), NETMAPDATA->wallComposition.Num());
		UE_LOG(LogTemp, Log, TEXT("PD_MM_MapInfo::ShowMapData - Num doorComposition %d "), NETMAPDATA->doorComposition.Num());

	}




	bool Clear() {
		allLogicPos.Empty();
		rooms.Empty();
		roomByLogPos.Empty();

		SpawnRoom = nullptr;

		return true;
	}

	PD_MM_Room* AddRoom(int IDRoom);

	// Devuelve un puntero al Room que tenga ese logPos. True si existe. 
	//bool RoomOf(PD_MG_LogicPosition logpos, PD_MM_Room *room);
	PD_MM_Room* RoomOf(PD_MG_LogicPosition logpos);

	// Inicializa el vector de rooms dado un staticMap
	//void CalculateRooms();

	//void CalculateRooms_v2();

	//void FindTilesOnRoomByFlowdingAt(PD_MG_LogicPosition initial, TArray<PD_MG_LogicPosition> PoblationSearch, TArray<PD_MG_LogicPosition>* tilesOnRoom);
	//void FindWallsAndDoorsOnRoomByFloodingAndAdyacentTiles(int IDRoom, PD_MG_LogicPosition initial, TArray<PD_MG_LogicPosition> PoblationSearch, TArray<PD_MG_LogicPosition>* WallsOnRoom, TArray<PD_MG_LogicPosition>* DoorsOnRoom);



	bool AddWall(PD_MG_LogicPosition logpos, APD_E_ElementActor* wall);
	bool AddTile(PD_MG_LogicPosition logpos, APD_E_ElementActor* tile);
	bool AddInteractuable(PD_MG_LogicPosition logpos, APD_E_ElementActor* interactuable);

	// TEST PD_MM_MapInfo

	void Test() {

		UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::Test INIT TESTING OF MAPINFO"));

		UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::Test allLogicPos num %d"), allLogicPos.Num());
		UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::Test roomByLogPos num %d"), roomByLogPos.Num());



		UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::Test rooms num %d"), rooms.Num());

		UE_LOG(LogTemp, Warning, TEXT("PD_MM_MapInfo::Test TEST ROOM NON INTENSIVE"));

		for (int i = 0; i < rooms.Num(); i++) {
			rooms[i]->Test();
		}
	}
};


