// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "Actors/PD_E_ElementActor.h"
#include "PD_ClientGameInstance.h"
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "PD_E_WallActor.h"
#include "GM_Game/PD_GM_MapManager.h"
#include "MapInfo/PD_MM_MapInfo.h"

APD_E_WallActor::APD_E_WallActor() {

}

// Called when the game starts or when spawned
void APD_E_WallActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APD_E_WallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void APD_E_WallActor::SetMaterialSkin(PD_MG_LogicPosition lp) {
	ChangeMaterialProperties_MapSkin(lp);
};



void APD_E_WallActor::ChangeMaterialProperties_MapSkin(PD_MG_LogicPosition lp) {
	FOutputDeviceDebug  debug;
	Super::ActualLogicPosition = lp;
	Super::SGI = Cast<UPD_ClientGameInstance>(GetWorld()->GetGameInstance());
	TArray<PD_MG_LogicPosition> adj_reales = Super::SGI->mapManager->Get_LogicPosition_Adyacents_To(Super::ActualLogicPosition);
	PD_MG_LogicPosition N = PD_MG_LogicPosition(Super::ActualLogicPosition.GetX() - 1, Super::ActualLogicPosition.GetY());
	PD_MG_LogicPosition S = PD_MG_LogicPosition(Super::ActualLogicPosition.GetX() + 1, Super::ActualLogicPosition.GetY());
	PD_MG_LogicPosition E = PD_MG_LogicPosition(Super::ActualLogicPosition.GetX(), Super::ActualLogicPosition.GetY() + 1);
	PD_MG_LogicPosition W = PD_MG_LogicPosition(Super::ActualLogicPosition.GetX(), Super::ActualLogicPosition.GetY() - 1);

	/*
	bool founded = false;
	UE_LOG(LogTemp, Log, TEXT("APD_E_WallActor::ChangeMaterialProperties_MapSkin - Super::ActualLogicPosition (%d,%d)"), Super::ActualLogicPosition.GetX(), Super::ActualLogicPosition.GetY());
	for (int i = 0; i < adj_reales.Num(); i++) {
	UE_LOG(LogTemp, Log, TEXT("APD_E_WallActor::ChangeMaterialProperties_MapSkin - adjreales[%d] - (%d,%d)"), i, adj_reales[i].GetX(), adj_reales[i].GetY());
	}
	*/

	//UE_LOG(LogTemp, Log, TEXT("APD_E_WallActor::ChangeMaterialProperties_MapSkin - adj_reales %d isTile %d isProp %d && "), adj_reales.Contains(N), Super::SGI->mapManager->IsLogicPositionATile(N), Super::SGI->mapManager->IsLogicPositionAProp(N));

	// Tienes una tile en el norte ?
	if (adj_reales.Contains(N)) {
		if (Super::SGI->mapManager->IsLogicPositionATile(N) || Super::SGI->mapManager->IsLogicPositionAProp(N)) {
			mapSkin_N = APD_E_ElementActor::SGI->mapManager->MapInfo->roomByLogPos[N]->mapSkin;
		}
		else
		{
			this->CallFunctionByNameWithArguments(TEXT("BP_N_NotVisible"), debug, this, true);
		}
	}


	// Tienes una tile en el sur?
	if (adj_reales.Contains(S)) {
		if (Super::SGI->mapManager->IsLogicPositionATile(S) || Super::SGI->mapManager->IsLogicPositionAProp(S)) {
			mapSkin_S = APD_E_ElementActor::SGI->mapManager->MapInfo->roomByLogPos[S]->mapSkin;
		}
		else
		{
			this->CallFunctionByNameWithArguments(TEXT("BP_S_NotVisible"), debug, this, true);
		}
	}

	// Tienes una tile en el este?
	if (adj_reales.Contains(E)) {
		if (Super::SGI->mapManager->IsLogicPositionATile(E) || Super::SGI->mapManager->IsLogicPositionAProp(E)) {
			mapSkin_E = APD_E_ElementActor::SGI->mapManager->MapInfo->roomByLogPos[E]->mapSkin;
		}
		else
		{
			this->CallFunctionByNameWithArguments(TEXT("BP_E_NotVisible"), debug, this, true);
		}
	}


	// Tienes una tile en el oeste?
	if (adj_reales.Contains(W)) {
		if (Super::SGI->mapManager->IsLogicPositionATile(W) || Super::SGI->mapManager->IsLogicPositionAProp(W)) {
			mapSkin_W = APD_E_ElementActor::SGI->mapManager->MapInfo->roomByLogPos[W]->mapSkin;
		}
		else
		{
			this->CallFunctionByNameWithArguments(TEXT("BP_W_NotVisible"), debug, this, true);
		}
	}

	this->CallFunctionByNameWithArguments(TEXT("BP_ChangeMaterialProperties_MapSkin"), debug, this, true);
}

