// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_GameStateInitializer.h"

//Includes of forward declaration
#include "PD_ClientGameInstance.h"


void APD_GameStateInitializer::BeginPlay() {
	Super::BeginPlay();


	UPD_ClientGameInstance* SGI = Cast<UPD_ClientGameInstance>(GetGameInstance());
	SGI->OnLoadedLevel();
}