// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "MapInstantiatorActor.h"

#include "MapInstantiatorActor.h"
#include "MapGeneration/Static/PD_MG_StaticMap.h"
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "MapGeneration/PD_MG_MapParser.h"
#include "MapGeneration/Dynamic/PD_MG_DynamicMap.h"
#include "Actors/PD_E_Character.h"
#include "ElementActors/PD_E_TileActor.h"
#include "ElementActors/PD_E_WallActor.h"
#include"Actors/PD_E_ElementActor.h"

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
	static ConstructorHelpers::FObjectFinder<UBlueprint> OrcBowBlueprint(TEXT("Blueprint'/Game/Blueprints/Enemies/Orco_Arco.Orco_Arco'"));
	if (OrcBowBlueprint.Object) {
		OrcBowClass = (UClass*)OrcBowBlueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> OrcGunsBlueprint(TEXT("Blueprint'/Game/Blueprints/Enemies/Orco_Pistola.Orco_Pistola'"));
	if (OrcGunsBlueprint.Object) {
		OrcGunsClass = (UClass*)OrcGunsBlueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> OrcMeleeBlueprint(TEXT("Blueprint'/Game/Blueprints/Enemies/Orco_Melee.Orco_Melee'"));
	if (OrcMeleeBlueprint.Object) {
		OrcMeleeClass = (UClass*)OrcMeleeBlueprint.Object->GeneratedClass;
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



APD_E_ElementActor* AMapInstantiatorActor::InstantiateTile(PD_MG_LogicPosition logpos)
{
	return GetWorld()->SpawnActor<APD_E_ElementActor>(TileClass, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}

APD_E_ElementActor* AMapInstantiatorActor::InstantiateWall(PD_MG_LogicPosition logpos)
{
	return GetWorld()->SpawnActor<APD_E_ElementActor>(WallClass, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}

APD_E_Character* AMapInstantiatorActor::InstantiateOrcBow(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_Character>(OrcBowClass, logpos.ToWorldPosition(true), FRotator(0.0f, 0.f, 0.f));
}

APD_E_Character* AMapInstantiatorActor::InstantiateOrcGuns(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_Character>(OrcGunsClass, logpos.ToWorldPosition(true), FRotator(0.0f, 0.f, 0.f));
}

APD_E_Character* AMapInstantiatorActor::InstantiateOrcMelee(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_Character>(OrcMeleeClass, logpos.ToWorldPosition(true), FRotator(0.0f, 0.f, 0.f));
}

APD_E_Character* AMapInstantiatorActor::InstantiatePlayer(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_Character>(PlayerClass, logpos.ToWorldPosition(true), FRotator(0.0f, 0.f, 0.f));
}
#pragma endregion





