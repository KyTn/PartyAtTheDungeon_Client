// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PATD_Client.h"
#include "MapManagerAccesor.h"

#include "MapGeneration/PD_MG_LogicPosition.h"
#include "Actors/PD_E_Character.h"
#include "GM_Game/PD_GM_MapManager.h"
#include "GM_Game/PD_GM_GameManager.h"
#include "PD_PlayersManager.h"
#include "Structs/PD_ClientStructs.h"//Para todos los structs
#include "GM_Game/LogicCharacter/PD_GM_LogicCharacter.h"

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

	return mapManager->_GAMEMANAGER->playersManager->MyPlayerInfo->logic_Character->GetCurrentLogicalPosition() == mapManager->WorldToLogicPosition(position);
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
