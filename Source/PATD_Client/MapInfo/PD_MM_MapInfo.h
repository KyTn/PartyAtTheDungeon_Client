// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Structs/PD_NetStructs.h"
#include "MapGeneration/PD_MG_LogicPosition.h"

// FORWARD DECLARATIONS
class PD_MG_LogicPosition;
class PD_MG_StaticMap;
class PD_GM_MapManager;
class APD_E_ElementActor;
class APD_E_Interactuable;
class APD_E_Door;

class PD_MM_Room;


// guarda la info logica de un interactuable
class PATD_CLIENT_API PD_MM_InteractuableInfo
{
public:

	// VARS

	FString Name_Interactuable;
	PD_MG_LogicPosition logpos;
	int IDInteractuable;
	StaticMapElement type;

	TArray<int> reactuables;


	// CONSTR
	PD_MM_InteractuableInfo(PD_MG_LogicPosition lp, int id, StaticMapElement t, FString name);
	~PD_MM_InteractuableInfo();


	//FUNCT

	void AddReactuableID(int id);
};

// guarda la info logica de una puerta 
class PATD_CLIENT_API PD_MM_DoorInfo
{
public:

	//VARS
	PD_MG_LogicPosition logpos;
	int IDInteractuable;
	StaticMapElement type;

	PD_MG_LogicPosition connA, connB;

	PD_MM_Room *room_ConnA, *room_ConnB;

	TArray<int> reactuables;

	PD_MM_InteractuableInfo* interactuableInfo;

	//CONSTR

	PD_MM_DoorInfo(PD_MG_LogicPosition lp, int id, StaticMapElement t, PD_MM_Room* roomA, PD_MM_Room* roomB);
	~PD_MM_DoorInfo();


	//FUNCT

	void CalculateConnectors();
	void AddReactuableID(int id);

};


class PATD_CLIENT_API PD_MM_Room
{
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

	TArray<PD_MG_LogicPosition> LogicDoorPosInRoom; // posiciones logicas de las puertas de la habitacion
	TArray<PD_MG_LogicPosition> LogicInteractuablesPosInRoom; // posiciones logicas de los interactuables de la habitacion

	TMap<PD_MG_LogicPosition, StaticMapElement> PropsAndTilesInRoomByLogicPosition; // mapa de props y tiles de la habitacion, accesibles desde su posicion logica
	TMap<PD_MG_LogicPosition, PD_MM_InteractuableInfo*> InteractuableInfoInRoomByLogicPosition; // mapa de interactuableInfo de la habitacion, accesibles desde su posicion logica
	TMap<PD_MG_LogicPosition, PD_MM_DoorInfo*> DoorInfoInRoomByLogicPosition; // mapa de doorInfo de la habitacion, accesibles desde su posicion logica


																			  // elementos instanciados
	TMap<PD_MG_LogicPosition, APD_E_ElementActor*> tiles;
	TMap<PD_MG_LogicPosition, APD_E_ElementActor*> walls;
	TMap<PD_MG_LogicPosition, APD_E_Interactuable*> interactuables;

	int GetIDRoom() { return IDRoom; };

	bool AddTile(PD_MG_LogicPosition logpos, APD_E_ElementActor* tile);
	bool AddWall(PD_MG_LogicPosition logpos, APD_E_ElementActor* wall);
	bool AddInteractuable(PD_MG_LogicPosition logpos, APD_E_Interactuable* interactuable);



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


class PATD_CLIENT_API PD_MM_MapInfo
{

public:
	PD_MM_MapInfo(PD_GM_MapManager* mM);
	~PD_MM_MapInfo();

	PD_GM_MapManager* mapManager;

	PD_MM_Room* SpawnRoom; // habitacion marcada como spawn

	PD_MG_LogicPosition MAP_SIZE_IN_LOGIC_POSITIONS; // tamaño del mapa ya trimeado

													 // Rooms
	TMap<uint8, TArray<uint8>> mapAdj; // dado un IDRoom, cuales son los IDRoom de las rooms adyacentes
	TArray<PD_MG_LogicPosition> allLogicPos; // todas las posiciones logicas usadas del mapa
	TArray<PD_MM_Room*> rooms; // todas las habitaciones del mapa 
	TMap<int, PD_MM_Room*> roomByIDRoom; // dado un IDRoom, cual es su IDRoom asociado
	TMap<PD_MG_LogicPosition, PD_MM_Room*> roomByLogPos; // dada una posicion logica, a que room pertenece


														 // DOORS E INTERACTUABLES

	TArray<PD_MM_DoorInfo*> doorInfoInMap; // una lista con la info logica de las puertas del mapa
	TArray<PD_MM_InteractuableInfo*>interactuableInfoInMap; // una lista con la info logica de los interactuables del mapa

															// door localisation & id
	TMap<PD_MG_LogicPosition, APD_E_Door*> doorActorByLogPos; // dada una posicion logica, que DoorActor es  (SOLO CUANDO YA LO HAS INSTACIADO!)
	TMap<int, APD_E_Door*> doorActorByID; // dado un idInteractuable, que puerta es (SOLO CUANDO YA LO HAS INSTACIADO!)

										  //door logic info
	TMap<PD_MG_LogicPosition, PD_MM_DoorInfo*> doorInfoByLogPos; // dada una posicion logica, la info logica de la puerta asociada
	TMap<int, PD_MM_DoorInfo*> doorInfoByID; // dado un idInteractuable, la info logica asociada de la puerta asociada



											 // interactuable localisation & id
	TMap<int, APD_E_Interactuable*> interactuableActorByID; // dado un idInteractuable, que InteractuableActor es (SOLO CUANDO YA LO HAS INSTACIADO!)
	TMap<PD_MG_LogicPosition, APD_E_Interactuable*> interactuableActorByLogicPosition; // dado uan posicion logica, que idInteractuable es

																					   //interactuable logic info
	TMap<PD_MG_LogicPosition, PD_MM_InteractuableInfo*> interactuableInfoByLogPos; // dada una posicion logica, la info logica del interactuable asociado
	TMap<int, PD_MM_InteractuableInfo*> interactuableInfoByID; // dado un idInteractuable, la info logica asociada del interactuable asociado


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

	bool IsDoorInstantiatedAt(PD_MG_LogicPosition lp);

	bool AddWall(PD_MG_LogicPosition logpos, APD_E_ElementActor* wall);
	bool AddTile(PD_MG_LogicPosition logpos, APD_E_ElementActor* tile);
	bool AddInteractuable(PD_MG_LogicPosition logpos, APD_E_Interactuable* interactuable);
	bool AddDoor_WithoutLink(PD_MG_LogicPosition logpos, APD_E_Door* interactuable);

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



