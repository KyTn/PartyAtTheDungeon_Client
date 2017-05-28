// Fill out your copyright notice in the Description page of Project Settings.'

#include "PATD_Client.h"
#include "MapGeneration/PD_MG_MapParser.h"
#include "MapGeneration/Static/PD_MG_StaticMap.h"
#include "MapGeneration/Dynamic/PD_MG_DynamicMap.h"
#include "Structs/PD_ClientEnums.h"
#include "GM_Game/LogicCharacter/PD_GM_LogicCharacter.h"
#include "Structs/PD_NetStructs.h"
#include "MapInfo/PD_MM_MapInfo.h"

PD_MG_MapParser::PD_MG_MapParser()
{
}

PD_MG_MapParser::~PD_MG_MapParser()
{
}


#pragma region Parse From File

PD_MG_StaticMap* PD_MG_MapParser::StartParsingFromFile(FString* filepath)
{
	return StartParsingFromFile(filepath, new PD_MG_StaticMap(), new PD_MG_DynamicMap());
}

PD_MG_StaticMap* PD_MG_MapParser::StartParsingFromFile(FString* filepath, PD_MG_StaticMap* staticMapRef, PD_MG_DynamicMap* dynamicMapRef)
{
	staticMapRef->Clear();
	dynamicMapRef->Clear();
	// Leamos el fichero
	//Estamos metiendo el FPaths::GameDir() aqui y en el parserActor, asi que solo hay que ponerlo en uno de los sitios
	FString FilePath = FPaths::GameDir() + *filepath;
	FString FileData = "";

	if (FFileHelper::LoadFileToString(FileData, *FilePath))
	{
		//Agregado para el hito2 MCG
		UE_LOG(LogTemp, Warning, TEXT("PD_MG_MapParser::StartParsingFromFile::  Llamando a SetMapString . Path :%s  Mapa: %s"), *FilePath, *FileData);
		staticMapRef->SetMapString(FileData);

		// Enviar a los clientes el mapa leido ... 


		// Obtenemos la version del parser que se debe usar. 

		TArray<FString> fileSplitted;
		FileData.ParseIntoArray(fileSplitted, TEXT("\n"), true);

		if (fileSplitted[0].Contains("v0.1")) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Using parser version " + fileSplitted[0]);

			Parsing_v_0_1(fileSplitted, staticMapRef, dynamicMapRef);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No parser version registered! Searching for " + fileSplitted[0]);
		}

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("PD_MG_MapParser::StartParsingFromFile::  Error loading map! Failed to load file!. Path :%s "), *FilePath);

	}


	return staticMapRef;
}

PD_MG_StaticMap* PD_MG_MapParser::StartParsingFromChorizo(FString* chorizo, PD_MG_StaticMap*  staticMapRef, PD_MG_DynamicMap* dynamicMapRef) {

	staticMapRef->Clear();
	dynamicMapRef->Clear();
	//Agregado para el hito2 MCG
	//UE_LOG(LogTemp, Warning, TEXT("PD_MG_MapParser::StartParsingFromChorizo::  Llamando a SetMapString. Mapa: %s"), *chorizo);
	staticMapRef->SetMapString(*chorizo);

	// Obtenemos la version del parser que se debe usar. 

	TArray<FString> fileSplitted;
	chorizo->ParseIntoArray(fileSplitted, TEXT("\n"), true);

	if (fileSplitted[0].Contains("v0.1")) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Using parser version " + fileSplitted[0]);

		Parsing_v_0_1(fileSplitted, staticMapRef, dynamicMapRef);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No parser version registered! Searching for " + fileSplitted[0]);
	}

	return staticMapRef;
}


#pragma endregion

#pragma region Parse from FStructMapData

bool PD_MG_MapParser::StartParsingFromFStructMapData(FStructMapData * NETMAPDATA, PD_MM_MapInfo * MapInfoRef, PD_MG_DynamicMap * dynamicMapRef)
{
	MapInfoRef->Clear();
	dynamicMapRef->Clear();

	if (NETMAPDATA->PARSER_VERSION.Contains("v0.2")) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Using parser version " + fileSplitted[0]);

		Parsing_v_0_2(NETMAPDATA, MapInfoRef, dynamicMapRef);
	}

	return true;
}

#pragma endregion

#pragma region VERSION OF PARSERS

PD_MG_StaticMap* PD_MG_MapParser::Parsing_v_0_1(TArray<FString> fileReaded, PD_MG_StaticMap* staticMapRef, PD_MG_DynamicMap* dynamicMapRef)
{
	//if (!fileReaded[0].Equals("v0.1")) { return staticMapRef; }

	uint32 nextIndexRead = 0;
	uint32 mapIndex = 0;
	// Cual es el tipo de misión para este mapa?
	if (fileReaded[1].Contains("defeatBoss")) {
		staticMapRef->SetTypeMission(MATCHCONFIG_MISSIONTYPE::DefeatBoss);
	}
	else if (fileReaded[1].Contains("defeatAll")) {
		staticMapRef->SetTypeMission(MATCHCONFIG_MISSIONTYPE::DefeatAll);
	}
	else if (fileReaded[1].Contains("recoverTreasure")) {
		staticMapRef->SetTypeMission(MATCHCONFIG_MISSIONTYPE::RecoverTreasure);
	}

	// Tamaño de la matriz
	staticMapRef->SetHeight((uint32)FCString::Atoi(*(fileReaded[2])));
	dynamicMapRef->SetHeight((uint32)FCString::Atoi(*(fileReaded[2])));
	staticMapRef->SetWidth((uint32)FCString::Atoi(*(fileReaded[3])));
	dynamicMapRef->SetWidth((uint32)FCString::Atoi(*(fileReaded[3])));

	nextIndexRead = 4;
	mapIndex = nextIndexRead;
	// Cargamos los caractares planos sin referencias de comportamiento
	nextIndexRead = ReadRawMap(fileReaded, nextIndexRead, staticMapRef);

	//Cargamos los enemigos que hay en el map, y los instanciamos en el enemyManager
	nextIndexRead = ReadEnemiesMap(fileReaded, nextIndexRead, dynamicMapRef);

	//Cargamos los objetos interactuables en el mapa
	//nextIndexRead = ReadInteractiveObjectMap(fileReaded, nextIndexRead, dynamicMapRef);


	return staticMapRef;
}



bool PD_MG_MapParser::Parsing_v_0_2(FStructMapData * NETMAPDATA, PD_MM_MapInfo * MapInfoRef, PD_MG_DynamicMap * DynamicMapRef)
{
	MapInfoRef->MAP_SIZE_IN_LOGIC_POSITIONS.SetIn16bits(NETMAPDATA->MAP_SIZE_IN_LOGIC_POSITIONS);

	int spawnIDRoom = NETMAPDATA->IDRoomSpawn;
	//MapInfoRef->SpawnRoomIndex = spawnIDRoom;

	//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - NETMAPDATA->IDRoomSpawn %d"), spawnIDRoom);



	//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Num skinByRoom %d"), NETMAPDATA->skinByRoom.Num());
	for (int i = 0; i < NETMAPDATA->skinByRoom.Num(); i++) {
		uint16 data = NETMAPDATA->skinByRoom[i];


		int IDRoom = (data >> 8) & 0x00FF;
		MapSkinType mapSkin = MapSkinType((data) & 0x00FF);

		PD_MM_Room* r = MapInfoRef->AddRoom(IDRoom);
		r->mapSkin = mapSkin;

		if (r->IDRoom == spawnIDRoom) {
			r->IsSpawnRoom = r->IDRoom == spawnIDRoom;
			MapInfoRef->SpawnRoom = r;
			//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Room Spawn founded: r->IsSpawnRoom %d"), r->IsSpawnRoom);
		}
	}
	//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Num MapInfoRef->rooms %d"), MapInfoRef->rooms.Num());
	//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Num MapInfoRef->SpawnRoom %d"), MapInfoRef->SpawnRoom->IDRoom);


	// sacamos las habitaciones adyacentes 

	//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Num roomsAdj %d"), NETMAPDATA->roomsAdj.Num());
	for (int i = 0; i < NETMAPDATA->roomsAdj.Num(); i++) {
		uint8 IDRoom = (NETMAPDATA->roomsAdj[i] & 0xFF00) >> 8;
		uint8 IDRoomAdj = (NETMAPDATA->roomsAdj[i] & 0x00FF);

		if (MapInfoRef->mapAdj.Contains(IDRoom)) {
			MapInfoRef->mapAdj[IDRoom].Add(IDRoomAdj);
		}
		else {
			TArray<uint8> d = TArray<uint8>();
			d.Add(IDRoomAdj);
			MapInfoRef->mapAdj.Add(IDRoom, d);

		}
	}

	//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Num MapInfoRef->mapAdj %d"), MapInfoRef->mapAdj.Num());


	//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Num roomComposition %d"), NETMAPDATA->roomComposition.Num());
	for (int i = 0; i < NETMAPDATA->roomComposition.Num(); i++) {
		// Desempaquetamos el paquete
		PD_MG_LogicPosition pos = PD_MG_LogicPosition(NETMAPDATA->roomComposition[i] >> 16);
		int IDRoom = NETMAPDATA->roomComposition[i] & 0x000000FF;
		StaticMapElement mapElement = StaticMapElement((NETMAPDATA->roomComposition[i] >> 8) & 0x000000FF);

		//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Num roomComposition PARSING pos uint16 (%u) idroom (%u) mapElement (%u)"), (NETMAPDATA->roomComposition[i] >> 16), NETMAPDATA->roomComposition[i] & 0x000000FF, (NETMAPDATA->roomComposition[i] >> 8) & 0x000000FF);

		//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Num roomComposition PARSING (%u) -> (%d, %d) - %d - %d"), NETMAPDATA->roomComposition[i], pos.GetX(), pos.GetY(), IDRoom, (int)mapElement);

		/*if (i == 0) {
		UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Num MapInfoRef->roomComposition[0] (%d, %d) - %d - %d"), pos.GetX(), pos.GetY(), IDRoom, (int)mapElement);
		}

		if (i == NETMAPDATA->roomComposition.Num() - 1) {
		UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Num MapInfoRef->roomComposition[-1] (%d, %d) - %d - %d"), pos.GetX(), pos.GetY(), IDRoom, (int)mapElement);
		}*/

		// buscamos la habitacion a la que pertenece
		PD_MM_Room* room = nullptr;

		for (int j = 0; j < MapInfoRef->rooms.Num(); j++) {
			if (IDRoom == MapInfoRef->rooms[j]->IDRoom) {
				room = MapInfoRef->rooms[j];
				break;
			}
		}

		MapInfoRef->allLogicPos.AddUnique(pos);

		MapInfoRef->roomByLogPos.Add(pos, room);
		room->LogicPosInRoom.AddUnique(pos);


		//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Num MapInfoRef->allLogicPos %d"), MapInfoRef->allLogicPos.Num());
		//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - room->PropsAndTilesInRoomByLogicPosition NUM %d"), room->PropsAndTilesInRoomByLogicPosition.Num());

		// Añadimos la posicion 
		if (room->PropsAndTilesInRoomByLogicPosition.Contains(pos)) {
			//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - room->PropsAndTilesInRoomByLogicPosition (%d, %d)"), pos.GetX(), pos.GetY());
		}
		else {
			room->PropsAndTilesInRoomByLogicPosition.Add(pos, mapElement);
			//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - ADDING room->PropsAndTilesInRoomByLogicPosition (%d, %d) - %d"), pos.GetX(), pos.GetY(), (int)mapElement);
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Num MapInfoRef->allLogicPos %d"), MapInfoRef->allLogicPos.Num());
	//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Num MapInfoRef->roomByLogPos %d"), MapInfoRef->roomByLogPos.Num());

	//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Num MapInfoRef->roomComposition[-1] (%d, %d) - %d - %d"), MapInfoRef->allLogicPos[0].GetX(), MapInfoRef->allLogicPos[0].GetY());



	//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Num wallComposition %d"), NETMAPDATA->wallComposition.Num());
	for (int i = 0; i < NETMAPDATA->wallComposition.Num(); i++) {
		// Desempaquetamos el paquete
		PD_MG_LogicPosition pos = PD_MG_LogicPosition((uint16)(NETMAPDATA->wallComposition[i] >> 16));
		int IDRoomB = NETMAPDATA->wallComposition[i] & 0x000000FF;
		int IDRoomA = (NETMAPDATA->wallComposition[i] >> 8) & 0x000000FF;

		// buscamos la habitacion A a la que pertenece
		PD_MM_Room* room = nullptr;

		//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - wallComposition: searching IDRoomA %d"), IDRoomA);


		for (int j = 0; j < MapInfoRef->rooms.Num(); j++) {
			if (IDRoomA == MapInfoRef->rooms[j]->IDRoom) {
				room = MapInfoRef->rooms[j];

				//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - wallComposition: RoomA Founded!"));
				break;
			}
		}

		room->LogicWallPosInRoom.Add(pos);

		// Si hay habitacion B, la buscamos 
		//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - wallComposition: searching IDRoomB %d"), IDRoomB);

		if (IDRoomB != 0xFF) {
			room = nullptr;

			for (int j = 0; j < MapInfoRef->rooms.Num(); j++) {
				if (IDRoomB == MapInfoRef->rooms[j]->IDRoom) {
					room = MapInfoRef->rooms[j];
					//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::Parsing_v_0_2 - wallComposition: RoomB Founded!"));
					break;
				}
			}

			room->LogicWallPosInRoom.Add(pos);
		}



		MapInfoRef->allLogicPos.AddUnique(pos);
	}


	UE_LOG(LogTemp, Error, TEXT("PD_MG_MapParser::Parsing_v_0_2 - interactuableids num %d"), NETMAPDATA->interactuableId.Num());
	for (int i = 0; i < NETMAPDATA->interactuableId.Num(); i++) {

		UE_LOG(LogTemp, Error, TEXT("PD_MG_MapParser::Parsing_v_0_2 - interactuableids %d"), NETMAPDATA->interactuableId[i]);

		PD_MG_LogicPosition pos = PD_MG_LogicPosition((uint16)(NETMAPDATA->interactuableId[i] >> 16) & 0x0000FFFF);
		int idInter = (NETMAPDATA->interactuableId[i] >> 8) & 0x000000FF;
		StaticMapElement typeInter = StaticMapElement(NETMAPDATA->interactuableId[i] & 0x000000FF);

		UE_LOG(LogTemp, Error, TEXT("PD_MG_MapParser::Parsing_v_0_2 - interactuableids (%d,%d) - %d - %d"), pos.GetX(), pos.GetY(), idInter, (int)typeInter);


		// Si el interactuable es una puerta, tiene un tratamiento adicional ... 
		if (typeInter == StaticMapElement::DOOR) {
			int IDRoomA = -1, IDRoomB = -1;

			// aqui buscamos a que puerta se refiere, y nos quedamos sus ids
			for (int idoor = 0; idoor < NETMAPDATA->doorComposition.Num(); idoor++) {
				// Desempaquetamos el paquete
				PD_MG_LogicPosition pos_door = PD_MG_LogicPosition((uint16)(NETMAPDATA->doorComposition[idoor] >> 16) & 0x0000FFFF);

				UE_LOG(LogTemp, Error, TEXT("PD_MG_MapParser::Parsing_v_0_2 - interactuableids pos (%d,%d) - interactuableids pos (%d,%d)"), pos.GetX(), pos.GetY(), pos_door.GetX(), pos_door.GetY());



				if (pos_door == pos) {
					IDRoomB = NETMAPDATA->doorComposition[idoor] & 0x000000FF;
					IDRoomA = (NETMAPDATA->doorComposition[idoor] >> 8) & 0x000000FF;


					UE_LOG(LogTemp, Error, TEXT("PD_MG_MapParser::Parsing_v_0_2 - interactuableids: room founded. value: %d -> (%d,%d) - %d - %d"), NETMAPDATA->doorComposition[idoor], pos.GetX(), pos.GetY(), IDRoomA, IDRoomB);
					break;
				}
			}
			UE_LOG(LogTemp, Error, TEXT("PD_MG_MapParser::Parsing_v_0_2 - interactuableids IDRoomA - %d - IDRoomB %d"), IDRoomA, IDRoomB);

			PD_MM_Room *roomA = MapInfoRef->roomByIDRoom[IDRoomA], *roomB = MapInfoRef->roomByIDRoom[IDRoomB];
			PD_MM_DoorInfo* doorInfo = new PD_MM_DoorInfo(pos, idInter, typeInter, roomA, roomB);

			FString n = "I_";
			n.AppendInt((int)typeInter);
			n.Append("_");
			n.AppendInt(idInter);

			PD_MM_InteractuableInfo* interactuableInfo = new PD_MM_InteractuableInfo(pos, idInter, typeInter, n);

			doorInfo->interactuableInfo = interactuableInfo;

			MapInfoRef->allLogicPos.AddUnique(pos);

			MapInfoRef->doorInfoInMap.Add(doorInfo);
			MapInfoRef->doorInfoByLogPos.Add(pos, doorInfo);
			MapInfoRef->doorInfoByID.Add(doorInfo->IDInteractuable, doorInfo);

			MapInfoRef->roomByLogPos.Add(pos, roomA);

			roomA->DoorInfoInRoomByLogicPosition.Add(pos, doorInfo);
			roomA->LogicDoorPosInRoom.Add(pos);
			roomB->DoorInfoInRoomByLogicPosition.Add(pos, doorInfo);
			roomB->LogicDoorPosInRoom.Add(pos);


			MapInfoRef->interactuableInfoInMap.Add(interactuableInfo);
			MapInfoRef->interactuableInfoByLogPos.Add(pos, interactuableInfo);
			MapInfoRef->interactuableInfoByID.Add(interactuableInfo->IDInteractuable, interactuableInfo);

			roomA->InteractuableInfoInRoomByLogicPosition.Add(pos, interactuableInfo);
			roomB->InteractuableInfoInRoomByLogicPosition.Add(pos, interactuableInfo);

			UE_LOG(LogTemp, Error, TEXT("PD_MG_MapParser::Parsing_v_0_2 - door added!"));
			continue;
		}
		else {
			FString n = "I_";
			n.AppendInt((int)typeInter);
			n.Append("_");
			n.AppendInt(idInter);

			// añadamos el interactuable al mapa 
			PD_MM_InteractuableInfo* interactuableInfo = new PD_MM_InteractuableInfo(pos, idInter, typeInter, n);

			MapInfoRef->interactuableInfoInMap.Add(interactuableInfo);
			MapInfoRef->interactuableInfoByLogPos.Add(pos, interactuableInfo);
			MapInfoRef->interactuableInfoByID.Add(interactuableInfo->IDInteractuable, interactuableInfo);

			if (MapInfoRef->roomByLogPos.Contains(pos)) {
				MapInfoRef->roomByLogPos[pos]->InteractuableInfoInRoomByLogicPosition.Add(pos, interactuableInfo);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Error al añadir interactuable con id %d: No hay room con logpos (%d,%d)"),
					interactuableInfo->IDInteractuable, pos.GetX(), pos.GetY());
			}

		}

	}

	// terminamos de configurar los interactuables con sus reactuables
	for (int i = 0; i < NETMAPDATA->interactuableComposition.Num(); i++) {
		int idInteractuable = (NETMAPDATA->interactuableComposition[i] >> 8) & 0x00FF;
		int idReactuable = NETMAPDATA->interactuableId[i] & 0x00FF;

		// si el interactuable es una puerta ...
		if (MapInfoRef->doorInfoByID.Contains(idInteractuable)) {
			MapInfoRef->doorInfoByID[idInteractuable]->AddReactuableID(idReactuable);
		}


		// tiene que existir un interactuable con ese id
		if (MapInfoRef->interactuableInfoByID.Contains(idInteractuable)) {
			MapInfoRef->interactuableInfoByID[idInteractuable]->AddReactuableID(idReactuable);
		}
		else
		{
			// si no existe, ES UN PROBLEMON
			UE_LOG(LogTemp, Error, TEXT("PD_MG_MapParser::Parsing_v_0_2 - Error al añadir reactuable con id %d: No hay interactuable con id %d"),
				idReactuable, idInteractuable);
		}
	}




	DynamicMapRef->SetHeight(MapInfoRef->MAP_SIZE_IN_LOGIC_POSITIONS.GetX());
	DynamicMapRef->SetWidth(MapInfoRef->MAP_SIZE_IN_LOGIC_POSITIONS.GetY());

	for (int i = 0; i < NETMAPDATA->enemyComposition.Num(); i++) {
		// Desempaquetamos el paquete
		PD_MG_LogicPosition pos = PD_MG_LogicPosition((uint16)(NETMAPDATA->enemyComposition[i] >> 16));
		uint8 type = NETMAPDATA->enemyComposition[i] & 0x000000FF;
		ECharacterType TypeEnemy = ECharacterType(type);
		uint8 IDEnemy = (NETMAPDATA->enemyComposition[i] >> 8);

		FString id = "Enemy_";
		id.AppendInt(IDEnemy);

		DynamicMapRef->AddNewEnemy(pos, TypeEnemy, id);
	}

	return true;
}

#pragma endregion 

//Rellena el diccionario interno del staticMapRef con la info en crudo del mapa, sin referencias de comportamiento. 
uint32 PD_MG_MapParser::ReadRawMap(TArray<FString> fileReaded, uint32 firstIndex, PD_MG_StaticMap* staticMapRef)
{

	for (uint32 i = firstIndex; i < staticMapRef->GetHeight() + firstIndex; i++) {
		TArray<TCHAR> ta = fileReaded[i].GetCharArray();
		for (uint32 j = 0; j < staticMapRef->GetWidth(); j++) {
			if (ta[j] != ' ' && ta[j] != '\n' && ta[j] != '\0')
			{
				//UE_LOG(LogTemp, Log, TEXT("Mapa pos (%d,%d) char %c"), i - firstIndex, j, ta[j]);
				staticMapRef->AddNewLogicPosition(i - firstIndex, j, ta[j]);
			}
		}
	}
	return staticMapRef->GetHeight() + firstIndex;
}

//Rellena la parte de los enemigos del DynamicMap pasado por parametros
uint32 PD_MG_MapParser::ReadEnemiesMap(TArray<FString> fileReaded, uint32 firstIndex, PD_MG_DynamicMap* dynamicMapRef) {
	TArray <TCHAR> enemyLine;
	FString f;
	uint32 Enemynum = (uint32)FCString::Atoi(*(fileReaded[firstIndex]));
	uint32 intType, x, y;
	int32 num, j;
	firstIndex++;
	ECharacterType type;
	for (uint32 i = firstIndex; i < Enemynum + firstIndex; i++) {
		/*enemyValue = fileReaded[i].RightChop(2);
		enemyValue.RemoveAt(enemyValue.Len()-1);//Hay que hacer esto, dado que guardaba basura, un "\n", o un "\0" que se comportaba al hacer el cout como "\n"
		dynamicMapRef->AddEnemyDictionary(fileReaded[i].GetCharArray()[0], enemyValue);*/
		enemyLine = fileReaded[i].GetCharArray();
		enemyLine.RemoveAt(enemyLine.Num() - 1);
		//enemyLine.RemoveAt(enemyLine.Num() - 1);

		j = 0; num = 1; intType = 0; x = 0; y = 0;
		while (enemyLine[j] != ':')
		{
			intType = enemyLine[j] - '0' + (intType*num);
			j++;
			num = num * 10;
		}
		type = ECharacterType(intType);
		num = 1; j++;
		while (enemyLine[j] != ',')
		{
			x = enemyLine[j] - '0' + (x*num);
			j++;
			num = num * 10;
		}
		num = 1; j++;
		while (j<enemyLine.Num())
		{
			y = enemyLine[j] - '0' + (y*num);
			j++;
			num = num * 10;
		}

		PD_MG_LogicPosition lp = PD_MG_LogicPosition(x, y);
		UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::ReadEnemiesMap: %d"), static_cast<uint8>(type));

		/*switch (type) {///En este switch metemos la IA lógica de cada uno
		case ECharacterType::Archer: {

		FString id = "Arch" + FString::FromInt(i - firstIndex);
		//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::ReadEnemiesMap:Archer %s in pos (%d,%d)"), *id, lp.GetX(),lp.GetY());
		//enemyMan->AddEnemie(ch);
		dynamicMapRef->AddNewEnemy(lp, type,id);
		break;
		}
		case ECharacterType::Zombie: {

		FString id = "Zomb" + FString::FromInt(i - firstIndex);
		//enemyMan->AddEnemie(ch);
		dynamicMapRef->AddNewEnemy(lp, type,id);
		//UE_LOG(LogTemp, Log, TEXT("PD_MG_MapParser::ReadEnemiesMap:Zombie %s in pos (%d,%d)"), *id, lp.GetX(), lp.GetY());
		break;
		}
		}*/
	}
	return firstIndex + Enemynum + 1;
}


/*uint32 PD_MG_MapParser::ReadInteraciveObjectMap(TArray<FString> fileReaded, uint32 firstIndex, PD_MG_DynamicMap* dynamicMapRef) {
TArray <TCHAR> intObjLine;
FString f;
uint32 intObjNum = (uint32)FCString::Atoi(*(fileReaded[firstIndex]));
uint32 intType, x, y;
int32 num, j;
firstIndex++;
for (uint32 i = firstIndex; i < intObjNum + firstIndex; i++) {
enemyValue = fileReaded[i].RightChop(2);
enemyValue.RemoveAt(enemyValue.Len()-1);//Hay que hacer esto, dado que guardaba basura, un "\n", o un "\0" que se comportaba al hacer el cout como "\n"
dynamicMapRef->AddEnemyDictionary(fileReaded[i].GetCharArray()[0], enemyValue);
intObjLine = fileReaded[i].GetCharArray();
intObjLine.RemoveAt(intObjLine.Num() - 1);
intObjLine.RemoveAt(intObjLine.Num() - 1);

///Falta decidir como se va a definir en el fichero, el parseo y guardarlo en el mapa dinámico
}
return firstIndex + intObjNum + 1;
}*/