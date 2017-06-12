// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "MapInstantiatorActor.h"
#include "MapGeneration/Static/PD_MG_StaticMap.h"
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "MapGeneration/PD_MG_MapParser.h"
#include "MapGeneration/Dynamic/PD_MG_DynamicMap.h"
#include "Actors/PD_E_Character.h"
#include "Actors/PD_E_ElementActor.h"
#include "MapInfo/PD_MM_MapInfo.h"
#include "Actors/MapElements/PD_E_WallActor.h"


#pragma region BUILT IN FUNTIONS 


AMapInstantiatorActor::AMapInstantiatorActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



#pragma region DEFAULT - COMMON
	static ConstructorHelpers::FObjectFinder<UBlueprint> TileBlueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/BP_ME_TileActor.BP_ME_TileActor'"));
	if (TileBlueprint.Object) {
		TileClass = (UClass*)TileBlueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> WallBlueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/BP_ME_WallActor.BP_ME_WallActor'"));
	if (WallBlueprint.Object) {
		WallClass = (UClass*)WallBlueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> DoorBlueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/DEFA_COMM/PD_E_Door.PD_E_Door'"));
	if (DoorBlueprint.Object) {
		DoorClass = (UClass*)DoorBlueprint.Object->GeneratedClass;
	}



	static ConstructorHelpers::FObjectFinder<UBlueprint> Default_Prop_Column_01_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/DEFA_COMM/COLUMN/BP_Column_01.BP_Column_01'"));
	if (Default_Prop_Column_01_Blueprint.Object) {
		Default_Prop_Column_01_Class = (UClass*)Default_Prop_Column_01_Blueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> Default_Prop_Column_02_1_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/DEFA_COMM/COLUMN/BP_Column_02_1.BP_Column_02_1'"));
	if (Default_Prop_Column_02_1_Blueprint.Object) {
		Default_Prop_Column_02_1_Class = (UClass*)Default_Prop_Column_02_1_Blueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> Default_Prop_Column_02_2_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/DEFA_COMM/COLUMN/BP_Column_02_2.BP_Column_02_2'"));
	if (Default_Prop_Column_02_2_Blueprint.Object) {
		Default_Prop_Column_02_2_Class = (UClass*)Default_Prop_Column_02_2_Blueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> Default_Prop_Column_03_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/DEFA_COMM/COLUMN/BP_Column_03.BP_Column_03'"));
	if (Default_Prop_Column_03_Blueprint.Object) {
		Default_Prop_Column_03_Class = (UClass*)Default_Prop_Column_03_Blueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> Default_Prop_Column_04_1_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/DEFA_COMM/COLUMN/BP_Column_04_1.BP_Column_04_1'"));
	if (Default_Prop_Column_04_1_Blueprint.Object) {
		Default_Prop_Column_04_1_Class = (UClass*)Default_Prop_Column_04_1_Blueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> Default_Prop_Column_04_2_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/DEFA_COMM/COLUMN/BP_Column_04_2.BP_Column_04_2'"));
	if (Default_Prop_Column_04_2_Blueprint.Object) {
		Default_Prop_Column_04_2_Class = (UClass*)Default_Prop_Column_04_2_Blueprint.Object->GeneratedClass;
	}


	static ConstructorHelpers::FObjectFinder<UBlueprint> Default_Prop_Obstruction_02_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/DEFA_COMM/OBSTRUCTION/BP_Obstruction_02.BP_Obstruction_02'"));
	if (Default_Prop_Obstruction_02_Blueprint.Object) {
		Default_Prop_Obstruction_02_Class = (UClass*)Default_Prop_Obstruction_02_Blueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> Default_Prop_Obstruction_03_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/DEFA_COMM/OBSTRUCTION/BP_Obstruction_03.BP_Obstruction_03'"));
	if (Default_Prop_Obstruction_03_Blueprint.Object) {
		Default_Prop_Obstruction_03_Class = (UClass*)Default_Prop_Obstruction_03_Blueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> Default_Prop_Obstruction_04_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/DEFA_COMM/OBSTRUCTION/BP_Obstruction_04.BP_Obstruction_04'"));
	if (Default_Prop_Obstruction_04_Blueprint.Object) {
		Default_Prop_Obstruction_04_Class = (UClass*)Default_Prop_Obstruction_04_Blueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> Default_Prop_Obstruction_12_1_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/DEFA_COMM/OBSTRUCTION/BP_Obstruction_12_1.BP_Obstruction_12_1'"));
	if (Default_Prop_Obstruction_12_1_Blueprint.Object) {
		Default_Prop_Obstruction_12_1_Class = (UClass*)Default_Prop_Obstruction_12_1_Blueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> Default_Prop_Obstruction_12_2_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/DEFA_COMM/OBSTRUCTION/BP_Obstruction_12_2.BP_Obstruction_12_2'"));
	if (Default_Prop_Obstruction_12_2_Blueprint.Object) {
		Default_Prop_Obstruction_12_2_Class = (UClass*)Default_Prop_Obstruction_12_2_Blueprint.Object->GeneratedClass;
	}
#pragma endregion


#pragma region MAPSKIN DUNGEON

	static ConstructorHelpers::FObjectFinder<UBlueprint> Dungeon_SpecialTile_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/SK_DUNG/BP_Dungeon_SpecialTile_01.BP_Dungeon_SpecialTile_01'"));
	if (Dungeon_SpecialTile_Blueprint.Object) {
		Dungeon_SpecialTile_Class = (UClass*)Dungeon_SpecialTile_Blueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> Dungeon_Prop_Treasure_01_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/SK_DUNG/BP_Dungeon_Prop_Treasure_01.BP_Dungeon_Prop_Treasure_01'"));
	if (Dungeon_Prop_Treasure_01_Blueprint.Object) {
		Dungeon_Prop_Treasure_01_Class = (UClass*)Dungeon_Prop_Treasure_01_Blueprint.Object->GeneratedClass;
	}






#pragma region MAPSKIN GARDEN

	static ConstructorHelpers::FObjectFinder<UBlueprint> Garden_Prop_Treasure_01_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/SK_GARD/BP_Garden_Prop_Treasure_01.BP_Garden_Prop_Treasure_01'"));
	if (Garden_Prop_Treasure_01_Blueprint.Object) {
		Garden_Prop_Treasure_01_Class = (UClass*)Garden_Prop_Treasure_01_Blueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> Garden_Prop_Tree_01_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/SK_GARD/BP_Garden_Tree_01.BP_Garden_Tree_01'"));
	if (Garden_Prop_Tree_01_Blueprint.Object) {
		Garden_Prop_Tree_01_Class = (UClass*)Garden_Prop_Tree_01_Blueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> Garden_Prop_Bush_01_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/SK_GARD/BP_Garden_bush_01.BP_Garden_Bush_01'"));
	if (Garden_Prop_Bush_01_Blueprint.Object) {
		Garden_Prop_Bush_01_Class = (UClass*)Garden_Prop_Bush_01_Blueprint.Object->GeneratedClass;
	}



#pragma endregion 
#pragma region MAPSKIN LIBRARY
	static ConstructorHelpers::FObjectFinder<UBlueprint> Library_Prop_Treasure_01_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/SK_LIBR/BP_Library_Prop_Treasure_01.BP_Library_Prop_Treasure_01'"));
	if (Library_Prop_Treasure_01_Blueprint.Object) {
		Library_Prop_Treasure_01_Class = (UClass*)Library_Prop_Treasure_01_Blueprint.Object->GeneratedClass;
	}
#pragma endregion 
#pragma region MAPSKIN SACRIFICE

	static ConstructorHelpers::FObjectFinder<UBlueprint> Sacrifice_Prop_Treasure_01_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/SK_SACR/BP_Sacrifice_Prop_Treasure_01.BP_Sacrifice_Prop_Treasure_01'"));
	if (Sacrifice_Prop_Treasure_01_Blueprint.Object) {
		Sacrifice_Prop_Treasure_01_Class = (UClass*)Sacrifice_Prop_Treasure_01_Blueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> Sacrifice_Prop_Jail_01_Blueprint(TEXT("Blueprint'/Game/BluePrints/PD_E/ESC/SK_SACR/BP_Sacrifice_Prop_Jail_01.BP_Sacrifice_Prop_Jail_01'"));
	if (Sacrifice_Prop_Jail_01_Blueprint.Object) {
		Sacrifice_Prop_Jail_01_Class = (UClass*)Sacrifice_Prop_Jail_01_Blueprint.Object->GeneratedClass;
	}



#pragma endregion 
#pragma region CHARACTERS

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

	static ConstructorHelpers::FObjectFinder<UBlueprint> OrcBossBlueprint(TEXT("Blueprint'/Game/Blueprints/Enemies/Orco_Boss.Orco_Boss'"));
	if (OrcBossBlueprint.Object) {
		OrcBossClass = (UClass*)OrcBossBlueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerBlueprint(TEXT("Blueprint'/Game/Blueprints/Players/Player.Player'"));
	if (PlayerBlueprint.Object) {
		PlayerClass = (UClass*)PlayerBlueprint.Object->GeneratedClass;
	}


#pragma endregion 
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



#pragma region DEFAULT - COMMON

APD_E_ElementActor* AMapInstantiatorActor::InstantiateTile(PD_MG_LogicPosition logpos)
{
	return GetWorld()->SpawnActor<APD_E_ElementActor>(TileClass, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}

APD_E_WallActor* AMapInstantiatorActor::InstantiateWall(PD_MG_LogicPosition logpos)
{
	return GetWorld()->SpawnActor<APD_E_WallActor>(WallClass, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}

APD_E_Door* AMapInstantiatorActor::InstantiateDoor(PD_MG_LogicPosition logpos)
{
	return GetWorld()->SpawnActor<APD_E_Door>(DoorClass, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}

#pragma endregion



#pragma region MAPSKIN DUNGEON

APD_E_ElementActor* AMapInstantiatorActor::Instantiate_Dungeon_Prop_Treasure_01(PD_MG_LogicPosition logpos)
{

	UE_LOG(LogTemp, Warning, TEXT("AMapInstantiatorActor::Instantiate_Dungeon_Prop_Treasure_01"));
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Dungeon_Prop_Treasure_01_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}

APD_E_ElementActor* AMapInstantiatorActor::Instantiate_Dungeon_SpecialTile(PD_MG_LogicPosition logpos)
{
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Dungeon_SpecialTile_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}

APD_E_ElementActor* AMapInstantiatorActor::Instantiate_Dungeon_Prop_Column_01(PD_MG_LogicPosition logpos)
{
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Default_Prop_Column_01_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}

APD_E_ElementActor * AMapInstantiatorActor::Instantiate_Dungeon_Prop_Column_02_1(PD_MG_LogicPosition logpos)
{
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Default_Prop_Column_02_1_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}
APD_E_ElementActor * AMapInstantiatorActor::Instantiate_Dungeon_Prop_Column_03(PD_MG_LogicPosition logpos)
{
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Default_Prop_Column_03_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}



APD_E_ElementActor * AMapInstantiatorActor::Instantiate_Dungeon_Prop_Column_04_1(PD_MG_LogicPosition logpos)
{
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Default_Prop_Column_04_1_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}


APD_E_ElementActor* AMapInstantiatorActor::Instantiate_Dungeon_Prop_Obstruction_02(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Default_Prop_Obstruction_02_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));

}
#pragma endregion 



#pragma region MAPSKIN GARDEN

APD_E_ElementActor* AMapInstantiatorActor::Instantiate_Garden_Prop_Tree_01(PD_MG_LogicPosition logpos)
{
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Garden_Prop_Tree_01_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}
APD_E_ElementActor* AMapInstantiatorActor::Instantiate_Garden_Prop_Treasure_01(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Garden_Prop_Treasure_01_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));

}
APD_E_ElementActor* AMapInstantiatorActor::Instantiate_Garden_Prop_Bush_01(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Garden_Prop_Bush_01_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));

}
#pragma endregion 

#pragma region MAPSKIN LIBRARY

APD_E_ElementActor * AMapInstantiatorActor::Instantiate_Library_Prop_Column_04_2(PD_MG_LogicPosition logpos)
{
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Default_Prop_Column_04_2_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));
}
APD_E_ElementActor* AMapInstantiatorActor::Instantiate_Library_Prop_Treasure_01(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Library_Prop_Treasure_01_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));

}

APD_E_ElementActor* AMapInstantiatorActor::Instantiate_Library_Prop_Obstruction_12_1(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Default_Prop_Obstruction_12_1_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));

}

#pragma endregion 

#pragma region MAPSKIN SACRIFICE

APD_E_ElementActor* AMapInstantiatorActor::Instantiate_Sacrifice_Prop_Treasure_01(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Sacrifice_Prop_Treasure_01_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));

}
APD_E_ElementActor* AMapInstantiatorActor::Instantiate_Sacrifice_Prop_Jail_01(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Sacrifice_Prop_Jail_01_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));

}

APD_E_ElementActor* AMapInstantiatorActor::Instantiate_Sacrifice_Prop_Obstruction_12_2(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_ElementActor>(Default_Prop_Obstruction_12_2_Class, logpos.ToWorldPosition(), FRotator(0.0f, 0.f, 0.f));

}
#pragma endregion 





#pragma region CHARACTERS

APD_E_Character* AMapInstantiatorActor::InstantiateOrcBow(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_Character>(OrcBowClass, logpos.ToWorldPosition(true), FRotator(0.0f, 0.f, 0.f));
}

APD_E_Character* AMapInstantiatorActor::InstantiateOrcGuns(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_Character>(OrcGunsClass, logpos.ToWorldPosition(true), FRotator(0.0f, 0.f, 0.f));
}

APD_E_Character* AMapInstantiatorActor::InstantiateOrcMelee(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_Character>(OrcMeleeClass, logpos.ToWorldPosition(true), FRotator(0.0f, 0.f, 0.f));
}

APD_E_Character* AMapInstantiatorActor::InstantiateOrcBoss(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_Character>(OrcBossClass, logpos.ToWorldPosition(true), FRotator(0.0f, 0.f, 0.f));
}

APD_E_Character* AMapInstantiatorActor::InstantiatePlayer(PD_MG_LogicPosition logpos) {
	return GetWorld()->SpawnActor<APD_E_Character>(PlayerClass, logpos.ToWorldPosition(true), FRotator(0.0f, 0.f, 0.f));
}

#pragma endregion

#pragma endregion




