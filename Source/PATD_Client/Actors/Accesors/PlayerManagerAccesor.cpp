// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PlayerManagerAccesor.h"
#include "PD_PlayersManager.h"

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
	
}

// Called every frame
void APlayerManagerAccesor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


bool APlayerManagerAccesor::CreateConsumableOrder(int id_consumable) { return playersManager->CreateConsumableOrder(id_consumable); }
bool APlayerManagerAccesor::CreateMovementOrder(int positionX, int positionY) { return playersManager->CreateMovementOrder(positionX, positionY); }
bool APlayerManagerAccesor::DeleteLastMovementOrder() { return playersManager->DeleteLastMovementOrder(); }
bool APlayerManagerAccesor::CreateInteractableOrder(int id_interactable) { return playersManager->CreateInteractableOrder(id_interactable); }
bool APlayerManagerAccesor::CreateActionToPosition(int id_action, TArray<FVector> positions) { return playersManager->CreateActionToPosition(id_action, positions); }
bool APlayerManagerAccesor::CreateActionToCharacter(int id_action, TArray<FString> id_character) { return playersManager->CreateActionToCharacter(id_action, id_character); }


bool APlayerManagerAccesor::ResetAll() { return playersManager->ResetAll(); }
bool APlayerManagerAccesor::ResetConsumables() { return playersManager->ResetConsumables(); }
bool APlayerManagerAccesor::ResetMovements() { return playersManager->ResetMovements(); }
bool APlayerManagerAccesor::ResetInteractuables() { return playersManager->ResetInteractuables(); }
bool APlayerManagerAccesor::ResetActions() { return playersManager->ResetActions(); }




