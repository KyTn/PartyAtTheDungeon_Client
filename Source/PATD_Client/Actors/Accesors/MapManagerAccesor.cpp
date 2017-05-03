// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PATD_Client.h"
#include "MapManagerAccesor.h"

#include "MapGeneration/PD_MG_LogicPosition.h"
#include "Actors/PD_E_Character.h"
#include "GM_Game/PD_GM_MapManager.h"
#include "GM_Game/PD_GM_GameManager.h"
#include "GM_Game/PD_GM_EnemyManager.h"
#include "PD_PlayersManager.h"
#include "Structs/PD_ClientStructs.h"//Para todos los structs
#include "GM_Game/LogicCharacter/PD_GM_LogicCharacter.h"
#include "MapInfo/PD_MM_MapInfo.h"

// Sets default values
AMapManagerAccesor::AMapManagerAccesor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapManagerAccesor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapManagerAccesor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

APD_E_Character*  AMapManagerAccesor::GetCharacterPlayerAtPosition(FVector position, bool& existe) {
	PD_MG_LogicPosition lp = mapManager->WorldToLogicPosition(position);
	
	for (int i = 0; i < mapManager->_GAMEMANAGER->playersManager->GetDataPlayers().Num(); i++) {
		if (lp == mapManager->_GAMEMANAGER->playersManager->GetDataPlayers()[i]->logic_Character->GetCurrentLogicalPosition()) {
			existe = true;
			return mapManager->_GAMEMANAGER->playersManager->GetDataPlayers()[i]->logic_Character->GetCharacterBP();
		}
	}
	
	existe = false;
	return nullptr;
}


bool AMapManagerAccesor::IsMyCharacterPlayerAtPosition(FVector position) {

	PD_MG_LogicPosition A = mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->logic_Character->GetCurrentLogicalPosition();
	PD_MG_LogicPosition B = mapManager->WorldToLogicPosition(position);

	UE_LOG(LogTemp, Warning, TEXT("Cheking if (%d,%d) == (%d,%d)"), A.GetX(), A.GetY(), B.GetX(), B.GetY());
	if (A == B) {
		UE_LOG(LogTemp, Warning, TEXT("IsMyCharacterPlayerAtPosition true "));
		return true;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("IsMyCharacterPlayerAtPosition false "));
		return false;
	}
}




int AMapManagerAccesor::GetIdOfCharacterAt(FVector position) {
	PD_MG_LogicPosition lp = mapManager->WorldToLogicPosition(position);

	for (int i = 0; i < mapManager->_GAMEMANAGER->playersManager->GetDataPlayers().Num(); i++) {
		if (lp == mapManager->_GAMEMANAGER->playersManager->GetDataPlayers()[i]->logic_Character->GetCurrentLogicalPosition()) {
			return mapManager->_GAMEMANAGER->playersManager->GetDataPlayers()[i]->ID_Player;
		}
	}
	return -999;
}

FVector AMapManagerAccesor::GetOwnPlayerPosition()
{
	return mapManager->LogicToWorldPosition(mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->logic_Character->GetCurrentLogicalPosition());
}

void AMapManagerAccesor::TransformFVectorToLogicPosition(FVector positionInWorld, int &logicX, int &logicY)
{
	logicX = mapManager->WorldToLogicPosition(positionInWorld).GetX();
	logicY = mapManager->WorldToLogicPosition(positionInWorld).GetY();
}

bool AMapManagerAccesor::GetPossibleEnemiesToAttack(TArray<AActor*> &possibleEnemies)
{

	if (mapManager->_GAMEMANAGER->enemyManager->GetEnemies().Num() > 0)
	{
		int playerRange = mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->logic_Character->GetTotalStats()->RangeTotal;

		if (playerRange == 1) //melee
		{
			UE_LOG(LogTemp, Warning, TEXT("apManagerAccesor::GetPossibleEnemiesToAttack - player range %d"), playerRange);
			TArray<PD_MG_LogicPosition> possibleNewPositionToMove = TArray<PD_MG_LogicPosition>();
			if (mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->turnOrders->positionsToMove.Num() > 0) 
			{
				UE_LOG(LogTemp, Warning, TEXT("apManagerAccesor::GetPossibleEnemiesToAttack - te has movido antes"));

				//si es mayor que 0, la posicion a comprobar para el rango es la ultima a la que se va a mover
				int maxIndexPosToMove = mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->turnOrders->positionsToMove.Num() - 1;
				possibleNewPositionToMove = mapManager->Get_LogicPosition_Adyacents_To(PD_MG_LogicPosition(
					mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->turnOrders->positionsToMove[maxIndexPosToMove].positionX,
					mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->turnOrders->positionsToMove[maxIndexPosToMove].positionY));		
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("apManagerAccesor::GetPossibleEnemiesToAttack - desde la posicion actual"));

				possibleNewPositionToMove = mapManager->Get_LogicPosition_Adyacents_To(
					mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->logic_Character->GetCurrentLogicalPosition());
			}

			UE_LOG(LogTemp, Warning, TEXT("apManagerAccesor::GetPossibleEnemiesToAttack - possible tiles to check - %d"), possibleNewPositionToMove.Num());

			for (int j = 0; j < possibleNewPositionToMove.Num(); j++)
			{
				for (int i = 0; i < mapManager->_GAMEMANAGER->enemyManager->GetEnemies().Num(); i++)
				{
					if (possibleNewPositionToMove[j] == mapManager->_GAMEMANAGER->enemyManager->GetEnemies()[i]->GetCurrentLogicalPosition())
					{
						possibleEnemies.Add(mapManager->_GAMEMANAGER->enemyManager->GetEnemies()[i]->GetCharacterBP());
						UE_LOG(LogTemp, Warning, TEXT("apManagerAccesor::GetPossibleEnemiesToAttack - adding posible enemy to defenestreiting"));
					}

				}
			}
		}
		}
	/*
		for (int i = 0; i <  mapManager->_GAMEMANAGER->enemyManager->GetEnemies().Num(); i++)
		{
			possibleEnemies.Add(mapManager->_GAMEMANAGER->enemyManager->GetEnemies()[i]->GetCharacterBP());
		}
		
		*/
	UE_LOG(LogTemp, Warning, TEXT("apManagerAccesor::GetPossibleEnemiesToAttack - enemies %d added  %d"), mapManager->_GAMEMANAGER->enemyManager->GetEnemies().Num(),
		possibleEnemies.Num());

	if (possibleEnemies.Num() > 0)
		return true;
	else
		return false;
}

bool AMapManagerAccesor::GetIDCharFromEnemy(AActor* enemyToCheck, FString &id_char)
{
	APD_E_Character* enemy = Cast<APD_E_Character>(enemyToCheck);

	if (enemy)
	{
		id_char = enemy->GetLogicCharacter()->GetIDCharacter();

		PD_GM_LogicCharacter* enemyLogic = nullptr;
		enemyLogic = mapManager->_GAMEMANAGER->enemyManager->GetCharacterByID(id_char);
		if (enemyLogic)
		{
			return true;
		}
		else {
			id_char = "none";
		}
	}

	return false;
}

void AMapManagerAccesor::ShowAdjenctsTiles(FVector currentPosition)
{
	UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ShowAdjenctsTile "));

	PD_MG_LogicPosition  currentPositionLogic = mapManager->WorldToLogicPosition(currentPosition);

	TArray<PD_MG_LogicPosition>  tilesNear = mapManager->Get_LogicPosition_Adyacents_To(currentPositionLogic);

	for (int i = 0; i < tilesNear.Num(); i++)
	{

		PD_MM_Room* roomOfTile = nullptr;
		if (mapManager->MapInfo->RoomOf(currentPositionLogic, roomOfTile))
		{
			UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ShowAdjenctsTile 2"));

			if (roomOfTile) //si ha pillado la room a partir de la posicion logica dada..
			{
				AActor* currentTile = roomOfTile->tiles[currentPositionLogic];
				//Comprobar aqui se se puede añadir a las posible o no
				FOutputDeviceNull ar;
				const FString command = FString::Printf(TEXT("DrawMovementMaterial %d"), 2);
				if (currentTile->CallFunctionByNameWithArguments(*command, ar, NULL, true))
				{
					UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ShowAdjenctsTile -- EXITO EN LLAMAR A LA FUNCION DrawMovementMaterial ()"));
				}
				else {
					UE_LOG(LogTemp, Error, TEXT(" AMapManagerAccesor::ShowAdjenctsTile - EEROR EN LLAMATR A LA FUNCION - DrawMovementMaterial()"));
				}

			}
		}

	}
	
}