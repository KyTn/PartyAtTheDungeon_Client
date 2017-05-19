// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MapInstantiatorActor.generated.h"

// FORWARD DECLARATIONS //

class PD_MG_StaticMap;
class PD_MG_DynamicMap;
class PD_MG_LogicPosition;
class APD_E_Character;
class APD_E_WallActor;
class APD_E_TileActor;
class APD_E_ElementActor;
//////////////////////////


UCLASS()
class PATD_CLIENT_API AMapInstantiatorActor : public AActor
{
	GENERATED_BODY()

public:

#pragma region BUILT IN FUNCTIONS											 

	AMapInstantiatorActor();

	// Called when the game starts or when spawned							 
	virtual void BeginPlay() override;

	// Called every frame													 
	virtual void Tick(float DeltaSeconds) override;

#pragma endregion															 


#pragma region TSUBCLASSOF Static & Dynamic									 

	//Static																 
	TSubclassOf<class APD_E_ElementActor> TileClass;
	TSubclassOf<class APD_E_ElementActor> WallClass;
	//Enemies
	/// seria la class generica
	TSubclassOf<class APD_E_Character> ArcherClass;
	TSubclassOf<class APD_E_Character> ZombieClass;
	TSubclassOf<class APD_E_Character> PlayerClass;
#pragma endregion 


	APD_E_ElementActor* InstantiateTile(PD_MG_LogicPosition logpos);
	APD_E_ElementActor* InstantiateWall(PD_MG_LogicPosition logpos);

	APD_E_Character* InstantiateArcher(PD_MG_LogicPosition logpos);
	APD_E_Character* InstantiateZombie(PD_MG_LogicPosition logpos);
	APD_E_Character* InstantiatePlayer(PD_MG_LogicPosition logpos);
};
