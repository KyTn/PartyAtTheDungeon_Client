// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MapInstantiatorActor.generated.h"


// FORWARD DECLARATIONS //

class PD_MG_StaticMap;
class PD_MG_DynamicMap;
class PD_MG_LogicPosition;
class APD_E_Character;
class APD_E_ElementActor;
class APD_E_Door;
class PD_MM_Room;
class APD_E_WallActor;

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

#pragma region DEFAULT - COMMON

	TSubclassOf<class APD_E_ElementActor> TileClass;
	TSubclassOf<class APD_E_ElementActor> WallClass;

	TSubclassOf<class APD_E_Door> DoorClass;

	TSubclassOf<class APD_E_ElementActor> Default_Prop_Column_01_Class;
	TSubclassOf<class APD_E_ElementActor> Default_Prop_Column_02_1_Class;
	TSubclassOf<class APD_E_ElementActor> Default_Prop_Column_02_2_Class;
	TSubclassOf<class APD_E_ElementActor> Default_Prop_Column_03_Class;
	TSubclassOf<class APD_E_ElementActor> Default_Prop_Column_04_1_Class;
	TSubclassOf<class APD_E_ElementActor> Default_Prop_Column_04_2_Class;
#pragma endregion


#pragma region MAPSKIN DUNGEON

	TSubclassOf<class APD_E_ElementActor> Dungeon_SpecialTile_Class;


	//Props
	TSubclassOf<class APD_E_ElementActor> Dungeon_Prop_Treasure_01_Class;

#pragma endregion 


#pragma region ENEMIES

	//Enemies
	/// seria la class generica
	TSubclassOf<class APD_E_Character> OrcBowClass;
	TSubclassOf<class APD_E_Character> OrcGunsClass;
	TSubclassOf<class APD_E_Character> OrcMeleeClass;
#pragma endregion 


#pragma region PLAYERS
	TSubclassOf<class APD_E_Character> PlayerClass;
#pragma endregion 
#pragma endregion 

	APD_E_ElementActor* InstantiateTile(PD_MG_LogicPosition logpos);

	APD_E_WallActor* InstantiateWall(PD_MG_LogicPosition logpos);

	APD_E_Door* InstantiateDoor(PD_MG_LogicPosition logpos);

#pragma region Instantiate Dungeon MapElements 

	APD_E_ElementActor* Instantiate_Dungeon_Prop_Treasure_01(PD_MG_LogicPosition logpos);
	APD_E_ElementActor* Instantiate_Dungeon_SpecialTile(PD_MG_LogicPosition logpos);
	APD_E_ElementActor* Instantiate_Dungeon_Prop_Column_01(PD_MG_LogicPosition logpos);
	APD_E_ElementActor* Instantiate_Dungeon_Prop_Column_02_1(PD_MG_LogicPosition logpos);
	APD_E_ElementActor* Instantiate_Dungeon_Prop_Column_03(PD_MG_LogicPosition logpos);



#pragma endregion 

	APD_E_Character* InstantiateOrcBow(PD_MG_LogicPosition logpos);
	APD_E_Character* InstantiateOrcGuns(PD_MG_LogicPosition logpos);
	APD_E_Character* InstantiateOrcMelee(PD_MG_LogicPosition logpos);
	APD_E_Character* InstantiatePlayer(PD_MG_LogicPosition logpos);
};
