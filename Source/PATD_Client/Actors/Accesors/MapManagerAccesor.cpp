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
#include "Actors/PD_E_ElementActor.h"

// Sets default values
AMapManagerAccesor::AMapManagerAccesor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	lastMoveWasClean = false;
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
		int playerRange = mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->logic_Character->GetWeapon()->RangeWeapon;

		if (playerRange >= 1) //melee
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
	bool isLastPosition = false;
	if (!lastMoveWasClean)// Si en la ultima hemos añadido hay que quitar dos, sino solo una
	{
		if (positionMoves.Num() > 1 && currentPositionLogic == positionMoves[positionMoves.Num() - 2]) {

			positionMoves.RemoveAt(positionMoves.Num() - 1);
		}
	}
	if (positionMoves.Num() > 0 && currentPositionLogic == positionMoves[positionMoves.Num() - 1]) {
		positionMoves.RemoveAt(positionMoves.Num() - 1);
		isLastPosition = true;
		lastMoveWasClean = true;
	}
	else if (positionMoves.Num() > 0 && currentPositionLogic == positionMoves[0]) {
		positionMoves.Empty();
	}
	else
		lastMoveWasClean = false;


	for (int i = 0; i < tilesNear.Num(); i++)
	{
		PD_MM_Room* roomOfTile = nullptr;
		roomOfTile = mapManager->MapInfo->RoomOf(tilesNear[i]);
		//UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ShowAdjenctsTile 2"));
		if (roomOfTile) //si ha pillado la room a partir de la posicion logica dada..
		{
			//UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ShowAdjenctsTile 3"));

			APD_E_ElementActor* currentTile = nullptr;

			if (roomOfTile->tiles.Contains(tilesNear[i]))
			{
				UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement lo tiene el TMAP tiles - borrar "));
				currentTile = Cast<APD_E_ElementActor>(roomOfTile->tiles[tilesNear[i]]);
			}

			UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ShowAdjenctsTile 4 %d"), roomOfTile->tiles.Num());

			if (currentTile)
			{
				//UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ShowAdjenctsTile 4"));

				//Comprobar aqui se se puede añadir a las posible o no
				FOutputDeviceNull ar;
				const FString command = FString::Printf(TEXT("DrawMovementMaterial %d"), 0);
				if (currentTile->CallFunctionByNameWithArguments(*command, ar, NULL, true))
				{
					UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ShowAdjenctsTile -- EXITO EN LLAMAR A LA FUNCION DrawMovementMaterial (2)"));
				}
				else {
					UE_LOG(LogTemp, Error, TEXT(" AMapManagerAccesor::ShowAdjenctsTile - EEROR EN LLAMATR A LA FUNCION - DrawMovementMaterial(2)"));
				}
			}
		}
	}

	tilesNear.Empty();

	tilesNear = mapManager->Get_LogicPosition_Adyacents_To(currentPositionLogic);

	for (int i = 0; i < tilesNear.Num(); i++)
	{
		PD_MM_Room* roomOfTile = nullptr;
		roomOfTile = mapManager->MapInfo->RoomOf(tilesNear[i]);
			//UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ShowAdjenctsTile 2"));
			if (roomOfTile) //si ha pillado la room a partir de la posicion logica dada..
			{
				//UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ShowAdjenctsTile 3"));

				APD_E_ElementActor* currentTile = nullptr;

				if (roomOfTile->tiles.Contains(tilesNear[i]))
				{
					UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement lo tiene el TMAP tiles - borrar "));
					currentTile = Cast<APD_E_ElementActor>(roomOfTile->tiles[tilesNear[i]]);
				}

				UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ShowAdjenctsTile 4 %d"), roomOfTile->tiles.Num());

				if (currentTile) 
				{
					
					if (positionMoves.Num() > 0 && tilesNear[i] == positionMoves[positionMoves.Num() - 1]) {
						FOutputDeviceNull ar;
						const FString command = FString::Printf(TEXT("DrawMovementMaterial %d"), 3);
						if (currentTile->CallFunctionByNameWithArguments(*command, ar, NULL, true))
						{
							UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ShowAdjenctsTile -- EXITO EN LLAMAR A LA FUNCION DrawMovementMaterial (2)"));
						}
						else {
							UE_LOG(LogTemp, Error, TEXT(" AMapManagerAccesor::ShowAdjenctsTile - EEROR EN LLAMATR A LA FUNCION - DrawMovementMaterial(2)"));
						}
					}
					else {
						FOutputDeviceNull ar;
						const FString command = FString::Printf(TEXT("DrawMovementMaterial %d"), 1);
						if (currentTile->CallFunctionByNameWithArguments(*command, ar, NULL, true))
						{
							UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ShowAdjenctsTile -- EXITO EN LLAMAR A LA FUNCION DrawMovementMaterial (2)"));
						}
						else {
							UE_LOG(LogTemp, Error, TEXT(" AMapManagerAccesor::ShowAdjenctsTile - EEROR EN LLAMATR A LA FUNCION - DrawMovementMaterial(2)"));
						}
					}
				}	
			}
	}
	if (!isLastPosition) {
		positionMoves.Add(currentPositionLogic);
	}
}

void AMapManagerAccesor::SetCurrentPositionAsPathMovement(FVector currentPosition)
{
	PD_MG_LogicPosition currentPositionLogic = mapManager->WorldToLogicPosition(currentPosition);

	UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement 1"));

	//limpiar las posibles tiles de la anterior tile seleccionada
	if (mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->turnOrders->positionsToMove.Num() > 0)
	{
		PD_MG_LogicPosition lastPositionLogic = PD_MG_LogicPosition(mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->turnOrders->positionsToMove.Last().positionX,
			mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->turnOrders->positionsToMove.Last().positionY);

		/*TArray<PD_MG_LogicPosition>*/  tilesNear = mapManager->Get_LogicPosition_Adyacents_To(lastPositionLogic);
		PD_MM_Room* roomOfTileLast = nullptr;

		UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement Tiles Near - %d"), tilesNear.Num());

		for (int j = 0; j < tilesNear.Num(); j++)
		{
			roomOfTileLast = mapManager->MapInfo->RoomOf(tilesNear[j]);
			if (roomOfTileLast)
			{
				UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement Tiles room - %d"), roomOfTileLast->tiles.Num());
				AActor* tileAdjecntToUpdate = nullptr;
				if (roomOfTileLast->tiles.Contains(tilesNear[j]))
				{
					UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement lo tiene el TMAP tiles - borrar "));
					tileAdjecntToUpdate = Cast<APD_E_ElementActor>(roomOfTileLast->tiles[tilesNear[j]]);
				}
				if (tileAdjecntToUpdate)
				{
					UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement TileAdyacent exist  borrar -"));

					FOutputDeviceNull ar2;
					const FString command2 = FString::Printf(TEXT("DrawMovementMaterial %d"), 1);
					if (tileAdjecntToUpdate->CallFunctionByNameWithArguments(*command2, ar2, NULL, true))
					{
						UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement -- EXITO EN LLAMAR A LA FUNCION DrawMovementMaterial (0)"));
					}
					else {
						UE_LOG(LogTemp, Error, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement - EEROR EN LLAMATR A LA FUNCION - DrawMovementMaterial(0)"));
					}
				}
				
			}
			
		}
	}
	
	//Añadimos la tile al camino del movimiento
	mapManager->_GAMEMANAGER->playersManager->CreateMovementOrder(currentPositionLogic.GetX(), currentPositionLogic.GetY()); 

	//Seteamos el State a 1 (Azul) y pintamos
	PD_MM_Room* roomOfTile = nullptr;
	roomOfTile = mapManager->MapInfo->RoomOf(currentPositionLogic);
	UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement 2"));
	if (roomOfTile) //si ha pillado la room a partir de la posicion logica dada..
	{
		if (roomOfTile->tiles.Num() > 0)
		{
			AActor* currentTile = nullptr;
			if (roomOfTile->tiles.Contains(currentPositionLogic))
			{
				UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement lo tiene el TMAP tiles "));
				currentTile = Cast<APD_E_ElementActor>(roomOfTile->tiles[currentPositionLogic]);
			}
			FOutputDeviceNull ar;
			if (currentTile)
			{
				UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement Si se ha encontrado una tile"));

				const FString command = FString::Printf(TEXT("DrawMovementMaterial %d"), 1);
				if (currentTile->CallFunctionByNameWithArguments(*command, ar, NULL, true))
				{
					UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement -- EXITO EN LLAMAR A LA FUNCION DrawMovementMaterial (1)"));
				}
				else {
					UE_LOG(LogTemp, Error, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement - EEROR EN LLAMATR A LA FUNCION - DrawMovementMaterial(1)"));
				}
			}
		}	
	}

	//Visualizar las siguientes posibles casillas
	ShowAdjenctsTiles(currentPosition);

	//LLamar a actualizar el camino de tiles seleccionado de momento
	UpdatePathMovement();


}


void AMapManagerAccesor::DeleteLastTileSelecteInPathMovenebt()
{
	TArray<FStructLogicPosition>* tilesSelected = &(mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->turnOrders->positionsToMove);

	PD_MG_LogicPosition  tileTodelete;
	PD_MM_Room* roomOfTile = nullptr;

	if (tilesSelected->Num() > 0)
	{
		//limpiar las posibles tiles de la anterior tile seleccionada
		if (mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->turnOrders->positionsToMove.Num() > 0)
		{
			PD_MG_LogicPosition lastPositionLogic = PD_MG_LogicPosition(mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->turnOrders->positionsToMove.Last().positionX,
				mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->turnOrders->positionsToMove.Last().positionY);

			/*TArray<PD_MG_LogicPosition>*/  tilesNear = mapManager->Get_LogicPosition_Adyacents_To(lastPositionLogic);
			PD_MM_Room* roomOfTileLast = nullptr;
			for (int j = 0; j < tilesNear.Num(); j++)
			{
				roomOfTileLast = mapManager->MapInfo->RoomOf(tilesNear[j]);
				if (roomOfTileLast)
				{
					AActor* tileAdjecntToUpdate = nullptr;
				    tileAdjecntToUpdate = Cast<APD_E_ElementActor>(roomOfTileLast->tiles[tilesNear[j]]);
					if (tileAdjecntToUpdate)
					{
						FOutputDeviceNull ar2;
						const FString command2 = FString::Printf(TEXT("DrawMovementMaterial %d"), 0);
						if (tileAdjecntToUpdate->CallFunctionByNameWithArguments(*command2, ar2, NULL, true))
						{
							UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement -- EXITO EN LLAMAR A LA FUNCION DrawMovementMaterial (0)"));
						}
						else {
							UE_LOG(LogTemp, Error, TEXT(" AMapManagerAccesor::SetCurrentPositionAsPathMovement - EEROR EN LLAMATR A LA FUNCION - DrawMovementMaterial(0)"));
						}
					}

				}

			}
		}

		// Borramos la ultima posicion del array de posiciones y actualizamos la tile para que solo se vea la textura de esta+
		tileTodelete = PD_MG_LogicPosition(tilesSelected->Last().positionX, tilesSelected->Last().positionY);
		roomOfTile = mapManager->MapInfo->RoomOf(tileTodelete);
		if (roomOfTile) //si ha pillado la room a partir de la posicion logica dada..
		{
			AActor* tileToUpdate = nullptr;
			tileToUpdate = Cast<APD_E_ElementActor>(roomOfTile->tiles[tileTodelete]);
			FOutputDeviceNull ar;
			const FString command = FString::Printf(TEXT("DrawMovementMaterial %d"), 0);
			if (tileToUpdate->CallFunctionByNameWithArguments(*command, ar, NULL, true))
			{
				UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::DeleteLastTileSelecteInPathMovenebt -- EXITO EN LLAMAR A LA FUNCION DrawMovementMaterial (0)"));
			}
			else {
				UE_LOG(LogTemp, Error, TEXT(" AMapManagerAccesor::DeleteLastTileSelecteInPathMovenebt - EEROR EN LLAMATR A LA FUNCION - DrawMovementMaterial(0)"));
			}

			//borramos la ultima tile del array
			mapManager->_GAMEMANAGER->playersManager->DeleteLastMovementOrder();
		}

		UpdatePathMovement(); //volvemos a pintar el nuevo camino ahora

		//visualizar el camuino nuevo
		FVector positionToShowAdjents = mapManager->LogicToWorldPosition(PD_MG_LogicPosition(
			mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->turnOrders->positionsToMove.Last().positionX,
			mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->turnOrders->positionsToMove.Last().positionY));
		ShowAdjenctsTiles(positionToShowAdjents);
	}

}



void AMapManagerAccesor::UpdatePathMovement()
{
	TArray<FStructLogicPosition> tilesSelected = mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->turnOrders->positionsToMove;
	
	PD_MG_LogicPosition  tileToMove;
	PD_MM_Room* roomOfTile = nullptr;

	if (tilesSelected.Num() > 0) //hay tiles seleccionadas
	{
		for (int i = 0; i < tilesSelected.Num(); i++)
		{
			tileToMove = PD_MG_LogicPosition(tilesSelected[i].positionX, tilesSelected[i].positionY); 
			roomOfTile = mapManager->MapInfo->RoomOf(tileToMove);
			UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::UpdatePathMovement 2"));
			if (roomOfTile) //si ha pillado la room a partir de la posicion logica dada..
			{
				if (i == tilesSelected.Num() - 1) //SI es la ultima tile - pintar de azul (estado a 1)
				{
					AActor* tileToUpdate = nullptr;
					tileToUpdate = Cast<APD_E_ElementActor>(roomOfTile->tiles[tileToMove]);
					if (tileToUpdate)
					{
						FOutputDeviceNull ar;
						const FString command = FString::Printf(TEXT("DrawMovementMaterial %d"), 1);
						if (tileToUpdate->CallFunctionByNameWithArguments(*command, ar, NULL, true))
						{
							UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::UpdatePathMovement -- EXITO EN LLAMAR A LA FUNCION DrawMovementMaterial (1)"));
						}
						else {
							UE_LOG(LogTemp, Error, TEXT(" AMapManagerAccesor::UpdatePathMovement - EEROR EN LLAMATR A LA FUNCION - DrawMovementMaterial(1)"));
						}
					}
				}
				else if (i == tilesSelected.Num() - 2) //Es la penultima tile - pintar en rojo para borrar (estado a 3)
				{
					AActor* tileToUpdate = nullptr;
					tileToUpdate = Cast<APD_E_ElementActor>(roomOfTile->tiles[tileToMove]);
					if (tileToUpdate)
					{
						FOutputDeviceNull ar;
						const FString command = FString::Printf(TEXT("DrawMovementMaterial %d"), 3);
						if (tileToUpdate->CallFunctionByNameWithArguments(*command, ar, NULL, true))
						{
							UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::UpdatePathMovement -- EXITO EN LLAMAR A LA FUNCION DrawMovementMaterial (3)"));
						}
						else {
							UE_LOG(LogTemp, Error, TEXT(" AMapManagerAccesor::UpdatePathMovement - EEROR EN LLAMATR A LA FUNCION - DrawMovementMaterial(3)"));
						}
					}
				}
				else  //Pintalas de verde
				{
					AActor* tileToUpdate = nullptr;
					tileToUpdate = Cast<APD_E_ElementActor>(roomOfTile->tiles[tileToMove]);
					if (tileToUpdate)
					{
						FOutputDeviceNull ar;
						const FString command = FString::Printf(TEXT("DrawMovementMaterial %d"), 4);
						if (tileToUpdate->CallFunctionByNameWithArguments(*command, ar, NULL, true))
						{
							UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::UpdatePathMovement -- EXITO EN LLAMAR A LA FUNCION DrawMovementMaterial (4)"));
						}
						else {
							UE_LOG(LogTemp, Error, TEXT(" AMapManagerAccesor::UpdatePathMovement - EEROR EN LLAMATR A LA FUNCION - DrawMovementMaterial(4)"));
						}
					}
				}
			}
		}
	}
}

void AMapManagerAccesor::ClearAllOverTilesInPathMovement()
{
	//Recorre el array de posiciones y limpia los iconos o pintura por encima de la tile.
	//Esta funcion se llama cuando se envian las ordenes. Para limpiar todas las tiles.

	TArray<FStructLogicPosition> tilesSelected = mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->turnOrders->positionsToMove;

	PD_MG_LogicPosition  tileToMove;
	PD_MM_Room* roomOfTile = nullptr;

	if (tilesSelected.Num() > 0) //hay tiles seleccionadas
	{
		for (int i = 0; i < tilesSelected.Num(); i++)
		{
			tileToMove = PD_MG_LogicPosition(tilesSelected[i].positionX, tilesSelected[i].positionY);
			roomOfTile = mapManager->MapInfo->RoomOf(tileToMove);
			UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ClearAllOverTilesInPathMovement 2"));

			if (roomOfTile) //si ha pillado la room a partir de la posicion logica dada..
			{
				AActor* tileToUpdate = nullptr;
				tileToUpdate = Cast<APD_E_ElementActor>(roomOfTile->tiles[tileToMove]);
				FOutputDeviceNull ar;
				const FString command = FString::Printf(TEXT("DrawMovementMaterial %d"), 0);
				if (tileToUpdate->CallFunctionByNameWithArguments(*command, ar, NULL, true))
				{
					UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ClearAllOverTilesInPathMovement -- EXITO EN LLAMAR A LA FUNCION DrawMovementMaterial (0)"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT(" AMapManagerAccesor::ClearAllOverTilesInPathMovement - EEROR EN LLAMATR A LA FUNCION - DrawMovementMaterial(0)"));
				}

				if (i == tilesSelected.Num() - 1) //SI es la ultima tile - quitar pintura tambien de adyacentes
				{
					/*TArray<PD_MG_LogicPosition>  tilesNear = mapManager->Get_LogicPosition_Adyacents_To(tileToMove);
					for (int j = 0; j < tilesNear.Num(); j++)
					{
						PD_MM_Room* roomOfTileAdjencts = mapManager->MapInfo->RoomOf(tilesNear[j]);
						if (roomOfTileAdjencts)
						{
							AActor* tileAdjecntToUpdate = nullptr;
							tileAdjecntToUpdate = Cast<APD_E_ElementActor>(roomOfTileAdjencts->tiles[tilesNear[j]]);
							if (tileAdjecntToUpdate)
							{
								FOutputDeviceNull ar2;
								const FString command2 = FString::Printf(TEXT("DrawMovementMaterial %d"), 0);
								if (tileAdjecntToUpdate->CallFunctionByNameWithArguments(*command2, ar2, NULL, true))
								{
									UE_LOG(LogTemp, Warning, TEXT(" AMapManagerAccesor::ClearAllOverTilesInPathMovement -- EXITO EN LLAMAR A LA FUNCION DrawMovementMaterial (0)"));
								}
								else
								{
									UE_LOG(LogTemp, Error, TEXT(" AMapManagerAccesor::ClearAllOverTilesInPathMovement - EEROR EN LLAMATR A LA FUNCION - DrawMovementMaterial(0)"));
								}
							}
							
						}
						
					} */
				}
			}
		}
	}
}
