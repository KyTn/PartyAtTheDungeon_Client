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


	TSubclassOf<class APD_E_ElementActor> Default_Prop_Obstruction_02_Class;
	TSubclassOf<class APD_E_ElementActor> Default_Prop_Obstruction_03_Class;
	TSubclassOf<class APD_E_ElementActor> Default_Prop_Obstruction_04_Class;
	TSubclassOf<class APD_E_ElementActor> Default_Prop_Obstruction_12_1_Class;
	TSubclassOf<class APD_E_ElementActor> Default_Prop_Obstruction_12_2_Class;
#pragma endregion


#pragma region MAPSKIN DUNGEON

	TSubclassOf<class APD_E_ElementActor> Dungeon_SpecialTile_Class;


	//Props
	TSubclassOf<class APD_E_ElementActor> Dungeon_Prop_Treasure_01_Class;

#pragma endregion 
#pragma region MAPSKIN GARDEN


	//Props
	TSubclassOf<class APD_E_ElementActor> Garden_Prop_Treasure_01_Class;
	TSubclassOf<class APD_E_ElementActor> Garden_Prop_Tree_01_Class;
	TSubclassOf<class APD_E_ElementActor> Garden_Prop_Bush_01_Class;
#pragma endregion
#pragma region MAPSKIN LIBRARY
	TSubclassOf<class APD_E_ElementActor> Library_Prop_Treasure_01_Class;

#pragma endregion

#pragma region MAPSKIN SACRIFICE


	//Props
	TSubclassOf<class APD_E_ElementActor> Sacrifice_Prop_Treasure_01_Class;
	TSubclassOf<class APD_E_ElementActor> Sacrifice_Prop_Jail_01_Class;
#pragma endregion
#pragma region MAPSKIN BOSS


#pragma endregion

#pragma region ENEMIES

	//Enemies
	/// seria la class generica
	TSubclassOf<class APD_E_Character> OrcBowClass;
	TSubclassOf<class APD_E_Character> OrcGunsClass;
	TSubclassOf<class APD_E_Character> OrcMeleeClass;
	TSubclassOf<class APD_E_Character> OrcBossClass;
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
	APD_E_ElementActor* Instantiate_Dungeon_Prop_Column_04_1(PD_MG_LogicPosition logpos);

	APD_E_ElementActor* Instantiate_Dungeon_Prop_Obstruction_02(PD_MG_LogicPosition logpos);
#pragma region Instantiate Garden MapElements 
	APD_E_ElementActor* Instantiate_Garden_Prop_Treasure_01(PD_MG_LogicPosition logpos);
	APD_E_ElementActor* Instantiate_Garden_Prop_Tree_01(PD_MG_LogicPosition logpos);
	APD_E_ElementActor* Instantiate_Garden_Prop_Bush_01(PD_MG_LogicPosition logpos);
#pragma endregion

#pragma region Instantiate Library MapElements 
	APD_E_ElementActor* Instantiate_Library_Prop_Treasure_01(PD_MG_LogicPosition logpos);
	APD_E_ElementActor * Instantiate_Library_Prop_Column_04_2(PD_MG_LogicPosition logpos);
	APD_E_ElementActor* Instantiate_Library_Prop_Obstruction_12_1(PD_MG_LogicPosition logpos);
#pragma endregion
#pragma region Instantiate Sacrifice MapElements 
	APD_E_ElementActor* Instantiate_Sacrifice_Prop_Treasure_01(PD_MG_LogicPosition logpos);
	APD_E_ElementActor* Instantiate_Sacrifice_Prop_Jail_01(PD_MG_LogicPosition logpos);
	APD_E_ElementActor* Instantiate_Sacrifice_Prop_Obstruction_12_2(PD_MG_LogicPosition logpos);
#pragma region Instantiate Boss MapElements 

#pragma endregion

#pragma endregion 

	APD_E_Character* InstantiateOrcBow(PD_MG_LogicPosition logpos);
	APD_E_Character* InstantiateOrcGuns(PD_MG_LogicPosition logpos);
	APD_E_Character* InstantiateOrcMelee(PD_MG_LogicPosition logpos);
	APD_E_Character* InstantiateOrcBoss(PD_MG_LogicPosition logpos);
	APD_E_Character* InstantiatePlayer(PD_MG_LogicPosition logpos);
};
