// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "MapInstantiatorActor.h"

#include "MapInstantiatorActor.h"
#include "MapGeneration/Static/PD_MG_StaticMap.h"
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "MapGeneration/PD_MG_MapParser.h"
#include "MapGeneration/Dynamic/PD_MG_DynamicMap.h"
#include "Actors/Enemies/PD_E_EnemyCharacter.h"
#include "Actors/PD_E_ElementActor.h"

#pragma region BUILT IN FUNTIONS 


AMapInstantiatorActor::AMapInstantiatorActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBlueprint> TileBlueprint(TEXT("Blueprint'/Game/BluePrints/MapElements/BP_ME_TileActor.BP_ME_TileActor'"));
	if (TileBlueprint.Object) {
		TileClass = (UClass*)TileBlueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> WallBlueprint(TEXT("Blueprint'/Game/BluePrints/MapElements/BP_ME_WallActor.BP_ME_WallActor'"));
	if (WallBlueprint.Object) {
		WallClass = (UClass*)WallBlueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> ArcherBlueprint(TEXT("Blueprint'/Game/Blueprints/Enemies/Archer.Archer'"));
	if (ArcherBlueprint.Object) {
		ArcherClass = (UClass*)ArcherBlueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> ZombieBlueprint(TEXT("Blueprint'/Game/Blueprints/Enemies/Zombie.Zombie'"));
	if (ZombieBlueprint.Object) {
		ZombieClass = (UClass*)ZombieBlueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerBlueprint(TEXT("Blueprint'/Game/Blueprints/Players/Player.Player'"));
	if (PlayerBlueprint.Object) {
		PlayerClass = (UClass*)PlayerBlueprint.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void AMapInstantiatorActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMapInstantiatorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
#pragma endregion



#pragma region INSTANTIATORS

AActor* AMapInstantiatorActor::InstantiateTile(PD_MG_LogicPosition logpos)
{
	return GetWorld()->SpawnActor<APD_E_ElementActor>(TileClass, logpos.ToWorldPosition() , FRotator(0.0f, 0.f, 0.f));
}

AActor* AMapInstantiatorActor::InstantiateWall(PD_MG_LogicPosition logpos)
{
	return GetWorld()->SpawnActor<APD_E_ElementActor>(WallClass, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}

APD_E_EnemyCharacter* AMapInstantiatorActor::InstantiateArcher(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_Character>(ArcherClass, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}

APD_E_EnemyCharacter* AMapInstantiatorActor::InstantiateZombie(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_Character>(ZombieClass, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}

APD_E_EnemyCharacter* AMapInstantiatorActor::InstantiatePlayer(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_Character>(PlayerClass, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}

#pragma endregion




