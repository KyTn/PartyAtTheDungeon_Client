// Fill out your copyright notice in the Description page of Project Settings.
/*
#include "PATD_Client.h"
#include "InitSocketManagerOnLevel.h"

//Includes of forward declaration
#include "NW_Networking/Socket/PD_NW_SocketManager.h"
#include "PD_ClientGameInstance.h"

// Sets default values
AInitSocketManagerOnLevel::AInitSocketManagerOnLevel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInitSocketManagerOnLevel::BeginPlay()
{
	Super::BeginPlay();

	UPD_ClientGameInstance* SGI = Cast<UPD_ClientGameInstance>(GetGameInstance());
	if (SGI)
	{
		SGI->InitClientActoWhenLoadMap();
		FString s = GetWorld()->GetMapName();

		UE_LOG(LogTemp, Warning, TEXT("BeginPlay ActorInitializer. Map: %s"), *s)

		FString currentMapName = SGI->levelsNameDictionary.GetMapName(4);
		if (GetWorld()->GetMapName().Contains(currentMapName)) {
			UE_LOG(LogTemp, Warning, TEXT("If del LVL_4_Gamemap"));
			SGI->InitGameMap();
		}


	

	}

}

// Called every frame
void AInitSocketManagerOnLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

*/