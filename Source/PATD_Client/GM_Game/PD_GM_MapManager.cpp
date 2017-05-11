// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_GM_MapManager.h"
#include "MapGeneration/Static/PD_MG_StaticMap.h"
#include "MapGeneration/Dynamic/PD_MG_DynamicMap.h"
#include "MapInfo/MapInstantiation/MapInstantiatorActor.h"
#include "Structs/PD_ClientEnums.h"
#include "GM_Game/PD_GM_EnemyManager.h"
#include "GM_Game/PD_GM_GameManager.h"
#include "PD_PlayersManager.h"
#include "GM_Game/LogicCharacter/PD_GM_LogicCharacter.h"
#include "Actors/PD_E_Character.h"
//#include "Actors/PD_SplineActors.h"
//#include "GM_Game/PD_GM_SplineManager.h"
//include of forward declaration
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "Actors/PD_GenericController.h"
#include "MapInfo/PD_MM_MapInfo.h"

PD_GM_MapManager::PD_GM_MapManager()
{
}

PD_GM_MapManager::~PD_GM_MapManager()
{
}



void PD_GM_MapManager::Init(PD_MG_StaticMap* sm, PD_MG_DynamicMap* dm) {

	StaticMapRef = sm;
	DynamicMapRef = dm;

	// Ya tengo la info del mapa cargada por el Struct o por las refs del estatico y dinamico
	MapInfo = new PD_MM_MapInfo(this);
}


#pragma region GET RAW INFO FROM STATIC

bool PD_GM_MapManager::IsLogicPositionAWall(PD_MG_LogicPosition logpos)
{

	if (StaticMapRef->GetXYMap().Contains(logpos)) {

		return StaticMapRef->GetXYMap()[logpos] == 'w' || StaticMapRef->GetXYMap()[logpos] == 'W';
	}



	return false;
}

bool PD_GM_MapManager::IsLogicPositionATile(PD_MG_LogicPosition logpos)
{
	if (StaticMapRef->GetXYMap().Contains(logpos)) {

		return StaticMapRef->GetXYMap()[logpos] == '.' || StaticMapRef->GetXYMap()[logpos] == ',';
		//|| StaticMapRef->GetXYMap()[logpos] == 's' || StaticMapRef->GetXYMap()[logpos] == 'S';
	}
	return false;
}


bool PD_GM_MapManager::IsLogicPositionAProp(PD_MG_LogicPosition logpos)
{
	if (StaticMapRef->GetXYMap().Contains(logpos)) {

		return StaticMapRef->GetXYMap()[logpos] == 'C';
		//|| StaticMapRef->GetXYMap()[logpos] == 'X' || StaticMapRef->GetXYMap()[logpos] == 's' || StaticMapRef->GetXYMap()[logpos] == 'S';
	}
	return false;
}

bool PD_GM_MapManager::IsLogicPositionADoor(PD_MG_LogicPosition logpos)
{
	if (StaticMapRef->GetXYMap().Contains(logpos)) {

		return StaticMapRef->GetXYMap()[logpos] == 'd' || StaticMapRef->GetXYMap()[logpos] == 'D';
		//|| StaticMapRef->GetXYMap()[logpos] == 'X' || StaticMapRef->GetXYMap()[logpos] == 's' || StaticMapRef->GetXYMap()[logpos] == 'S';
	}
	return false;
}

bool PD_GM_MapManager::IsLogicPositionASpawn(PD_MG_LogicPosition logpos)
{
	if (StaticMapRef->GetXYMap().Contains(logpos)) {

		return StaticMapRef->GetXYMap()[logpos] == 's' || StaticMapRef->GetXYMap()[logpos] == 'S';
	}
	return false;
}

#pragma endregion



#pragma region GET INFO OF THE MAP

/*
bool PD_GM_MapManager::getGenericCharacterAt(PD_MG_LogicPosition* logpos, APD_PLY_GenericCharacter* genCharacter) { return false; }
bool PD_GM_MapManager::getPlayerAt(PD_MG_LogicPosition* logpos, APD_PLY_GenericCharacter* genCharacter) { return false; }
bool PD_GM_MapManager::getEnemyAt(PD_MG_LogicPosition* logpos, APD_PLY_GenericCharacter* genCharacter) { return false; }
*/

bool PD_GM_MapManager::IsTherePlayer(uint32 x, uint32 y) {
	PD_MG_LogicPosition logpos;
	logpos.SetX(x);
	logpos.SetY(y);
	if (DynamicMapRef->getEnemies().Contains(logpos))
		if (DynamicMapRef->getEnemies()[logpos].type_Character == ECharacterType::Player)
			return true;
	return false;
}

bool PD_GM_MapManager::IsThereWall(uint32 x, uint32 y) {
	PD_MG_LogicPosition logpos;
	logpos.SetX(x);
	logpos.SetY(y);
	if (StaticMapRef->GetXYMap()[logpos] == 'w' || StaticMapRef->GetXYMap()[logpos] == 'W')
		return true;
	return false;
}

TArray<PD_MG_LogicPosition> PD_GM_MapManager::GetSpawnPoints() {
	UE_LOG(LogTemp, Warning, TEXT("PD_GM_MapManager::GetSpawnPoints() - Num Rooms: %d"), MapInfo->rooms.Num());
	/*
	for (int i = 0; i < MapInfo->rooms.Num(); i++) {
	if (MapInfo->rooms[i].IsSpawnRoom) {
	UE_LOG(LogTemp, Warning, TEXT("PD_GM_MapManager::GetSpawnPoints() - Hay una habitacion marcada como spawn"));
	return MapInfo->rooms[i].LogicPosInRoom;
	}
	}
	*/
	//UE_LOG(LogTemp, Warning, TEXT("PD_GM_MapManager::GetSpawnPoints() -  Room %d num spawnpoints: %d"), MapInfo->SpawnRoom->GetIDRoom(), MapInfo->rooms[MapInfo->SpawnRoom->GetIDRoom()].LogicPosInRoom.Num());
	//return MapInfo->rooms[MapInfo->SpawnRoom->GetIDRoom()].LogicPosInRoom;

	//UE_LOG(LogTemp, Warning, TEXT("PD_GM_MapManager::GetSpawnPoints() -  IDSPAN   %d"), MapInfo->SpawnRoomIndex);

	return MapInfo->rooms[MapInfo->SpawnRoomIndex]->LogicPosInRoom;

}

AActor* PD_GM_MapManager::getInteractuableAt(PD_MG_LogicPosition logpos) { return 0; }


const FVector PD_GM_MapManager::LogicToWorldPosition(PD_MG_LogicPosition pos) {
	return pos.ToWorldPosition();
}

PD_MG_LogicPosition PD_GM_MapManager::WorldToLogicPosition(FVector pos) {
	float x, y;

	x = -1.0f * pos.X / 100.f;
	y = pos.Y / 100.f;

	return PD_MG_LogicPosition((int)roundf(x), (int)roundf(y));
}



TArray<PD_MG_LogicPosition> PD_GM_MapManager::Get_LogicPosition_Adyacents_To(PD_MG_LogicPosition logPos) {

	return logPos.GetAdjacentsFromList(StaticMapRef->GetLogicPositions());
}


TArray<PD_MG_LogicPosition> PD_GM_MapManager::Get_LogicPosition_Diagonals_And_Adyacents_To(PD_MG_LogicPosition logPos) {

	return logPos.GetDiagonalsAndAdjacentsFromList(StaticMapRef->GetLogicPositions());
}

#pragma endregion




#pragma region INSTANTIATE MAP


void PD_GM_MapManager::InstantiateMap()
{
	UE_LOG(LogTemp, Log, TEXT("MapManager::InstantiateMap "));

	InstantiateStaticMap();
	InstantiateDynamicMap();

}


void PD_GM_MapManager::InstantiateStaticMap() {

	for (int i = 0; i < StaticMapRef->GetLogicPositions().Num(); i++) {

		/**/
		switch (StaticMapRef->GetXYMap()[StaticMapRef->GetLogicPositions()[i]]) {
		case 'w':
		case 'W':
			MapInfo->AddWall(StaticMapRef->GetLogicPositions()[i], instantiator->InstantiateWall(StaticMapRef->GetLogicPositions()[i]));
			break;

		case '.':
		case ',':
		case 'd':
		case 's':

			MapInfo->AddTile(StaticMapRef->GetLogicPositions()[i], instantiator->InstantiateTile(StaticMapRef->GetLogicPositions()[i]));
			break;

			/*default:

			parserActor->InstantiateTile(staticMap->GetLogicPositions()[i]);
			break;
			*/
		}
	}
}


void PD_GM_MapManager::InstantiateDynamicMap() {
	ECharacterType enemyType;

	UE_LOG(LogTemp, Warning, TEXT("PD_GM_MapManager::InstantiateDynamicMap - Players Num %d"), _GAMEMANAGER->playersManager->GetNumPlayers());

	for (int i = 0; i < _GAMEMANAGER->playersManager->GetNumPlayers(); i++)
	{
		FOutputDeviceNull ar;

		_GAMEMANAGER->playersManager->GetDataPlayers()[i]->logic_Character->SetCharacterBP(instantiator->InstantiatePlayer(
			_GAMEMANAGER->playersManager->GetDataPlayers()[i]->logic_Character->GetCurrentLogicalPosition()));

		Cast<APD_E_Character>(_GAMEMANAGER->playersManager->GetDataPlayers()[i]->logic_Character->GetCharacterBP())->SetLogicCharacter(
			_GAMEMANAGER->playersManager->GetDataPlayers()[i]->logic_Character);

		_GAMEMANAGER->playersManager->GetDataPlayers()[i]->logic_Character->SetController(Cast<APD_GenericController>(
			_GAMEMANAGER->playersManager->GetDataPlayers()[i]->logic_Character->GetCharacterBP()->GetController()));

		//Seteamos el spline de los jugadores.
		//Cast<APD_GenericController>(_GAMEMANAGER->playersManager->GetDataPlayers()[i]->logic_Character->GetCharacterBP()->GetController())->SetSpline(
		//_GAMEMANAGER->splineManager->GetSpline());

		//Set de color of the Character --> Esto se deberia llevar a una fucnion del CharacterLogic para cambiar desde ahi la skin
		/*const FString command = FString::Printf(TEXT("ChangeMaterial %d"), i);
		if (_GAMEMANAGER->playersManager->GetDataPlayers()[i]->logic_Character->GetCharacterBP()->CallFunctionByNameWithArguments(*command, ar, NULL, true))
		{
		UE_LOG(LogTemp, Warning, TEXT("PD_GM_MapManager::InstantiateDynamicMap -- EXITO EN LLAMAR A LA FUNCION"));
		}
		else{
		UE_LOG(LogTemp, Error, TEXT("PD_GM_MapManager::InstantiateDynamicMap - EEROR EN LLAMATR A LA FUNCION"), _GAMEMANAGER->playersManager->GetNumPlayers());
		}
		*/
		///actualizamos la referencia del BP

	}

	for (int i = 0; i < DynamicMapRef->GetLogicPositions().Num(); i++) {

		enemyType = DynamicMapRef->getEnemies()[DynamicMapRef->GetLogicPositions()[i]].type_Character; ///Cogemos el tipo

		switch (enemyType)
		{
		case ECharacterType::Archer:
		{
			APD_E_Character* charac = instantiator->InstantiateArcher(DynamicMapRef->GetLogicPositions()[i]);
			PD_GM_LogicCharacter* logicCha = new PD_GM_LogicCharacter();
			logicCha->SetIsPlayer(false);
			logicCha->SetTypeCharacter(DynamicMapRef->getEnemies()[DynamicMapRef->GetLogicPositions()[i]].type_Character);
			logicCha->SetIDCharacter(DynamicMapRef->getEnemies()[DynamicMapRef->GetLogicPositions()[i]].ID_Character);
			UE_LOG(LogTemp, Log, TEXT("PD_GM_MapManager::InstantiateDynamicMap: Id Dinamicmap: %s"), *logicCha->GetIDCharacter());
			logicCha->SetCharacterBP(charac);
			logicCha->SetController(Cast<APD_GenericController>(charac->GetController()));
			logicCha->SetCurrentLogicalPosition(DynamicMapRef->GetLogicPositions()[i]);
			///SETEAR AQUI TODOS LOS STATS- WEAPONS- SKILLS DE CADA TIOPO DE ENEMIGO ENE SU LOGIC CHARACTER
			charac->SetLogicCharacter(logicCha);
			_GAMEMANAGER->enemyManager->AddEnemy(logicCha);
			break;
		}
		case ECharacterType::Zombie:
		{
			APD_E_Character* charac = instantiator->InstantiateZombie(DynamicMapRef->GetLogicPositions()[i]);
			PD_GM_LogicCharacter* logicCha = new PD_GM_LogicCharacter();
			logicCha->SetIsPlayer(false);
			logicCha->SetTypeCharacter(DynamicMapRef->getEnemies()[DynamicMapRef->GetLogicPositions()[i]].type_Character);
			logicCha->SetIDCharacter(DynamicMapRef->getEnemies()[DynamicMapRef->GetLogicPositions()[i]].ID_Character);
			UE_LOG(LogTemp, Log, TEXT("PD_GM_MapManager::InstantiateDynamicMap: Id Dinamicmap: %s"), *logicCha->GetIDCharacter());
			logicCha->SetCharacterBP(charac);
			logicCha->SetController(Cast<APD_GenericController>(charac->GetController()));
			logicCha->SetCurrentLogicalPosition(DynamicMapRef->GetLogicPositions()[i]);

			///SETEAR AQUI TODOS LOS STATS- WEAPONS- SKILLS DE CADA TIOPO DE ENEMIGO ENE SU LOGIC CHARACTER
			charac->SetLogicCharacter(logicCha);

			_GAMEMANAGER->enemyManager->AddEnemy(logicCha);
			break;
		}
		}
	}
}

#pragma endregion