// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PlayerManagerAccesor.h"
#include "PD_PlayersManager.h"
#include "PD_ClientGameInstance.h"
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "Actors/Interactuables/PD_E_Door.h"

// Sets default values
APlayerManagerAccesor::APlayerManagerAccesor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerManagerAccesor::BeginPlay()
{
	Super::BeginPlay();

	CGI = Cast<UPD_ClientGameInstance>(GetWorld()->GetGameInstance());

}

// Called every frame
void APlayerManagerAccesor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


bool APlayerManagerAccesor::CreateConsumableOrder(int id_consumable) { return playersManager->CreateConsumableOrder(id_consumable); }
bool APlayerManagerAccesor::CreateMovementOrder(int positionX, int positionY) { 
	PD_MG_LogicPosition lp = PD_MG_LogicPosition(positionX, positionY);
	//UE_LOG(LogTemp, Warning, TEXT("APlayerManagerAccesor::CreateMovementOrder - Trying to create a move order to (%d:%d)"), lp.GetX(), lp.GetY());

	if (CGI->mapManager->IsLogicPositionATile(lp) || (CGI->mapManager->IsLogicPositionADoor(lp) && CGI->mapManager->MapInfo->doorActorByLogPos[lp]->IsDoorOpen)) {
		//UE_LOG(LogTemp, Warning, TEXT("APlayerManagerAccesor::CreateMovementOrder - tile: %d, dooropen: %d"), CGI->mapManager->IsLogicPositionATile(lp), (CGI->mapManager->IsLogicPositionADoor(lp) && CGI->mapManager->MapInfo->doorActorByLogPos[lp]->IsDoorOpen));
		return playersManager->CreateMovementOrder(positionX, positionY);
	}
	else 
	{
		//UE_LOG(LogTemp, Warning, TEXT("APlayerManagerAccesor::CreateMovementOrder - hey, you DONT move there!"));
		return false;
	}
}
bool APlayerManagerAccesor::DeleteLastMovementOrder() { return playersManager->DeleteLastMovementOrder(); }
bool APlayerManagerAccesor::CreateInteractableOrder(int id_interactable) { return playersManager->CreateInteractableOrder(id_interactable); }
bool APlayerManagerAccesor::CreateActionToPosition(int id_action, TArray<FVector> positions) { return playersManager->CreateActionToPosition(id_action, positions); }
bool APlayerManagerAccesor::CreateActionToCharacter(int id_action, TArray<FString> id_character) { return playersManager->CreateActionToCharacter(id_action, id_character); }


bool APlayerManagerAccesor::ResetAll() { return playersManager->ResetAll(); }
bool APlayerManagerAccesor::ResetConsumables() { return playersManager->ResetConsumables(); }
bool APlayerManagerAccesor::ResetMovements() { return playersManager->ResetMovements(); }
bool APlayerManagerAccesor::ResetInteractuables() { return playersManager->ResetInteractuables(); }
bool APlayerManagerAccesor::ResetActions() { return playersManager->ResetActions(); }

bool APlayerManagerAccesor::DeleteActionTurn(int index_action) { return playersManager->DeleteActionTurn(index_action); };



