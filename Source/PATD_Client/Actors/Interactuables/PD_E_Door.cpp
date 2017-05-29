// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_E_Door.h"
#include "Actors/PD_E_ElementActor.h"
#include "PD_ClientGameInstance.h"
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "GM_Game/PD_GM_MapManager.h"
#include "MapInfo/PD_MM_MapInfo.h"
#include "Actors/Interactuables/PD_E_Interactuable.h"




// Sets default values
APD_E_Door::APD_E_Door()
{
	//Super::APD_E_Interactuable();
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APD_E_Door::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APD_E_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APD_E_Door::Set_DoorInfo(TArray<APD_E_Interactuable*> otherInteractuables, PD_MM_DoorInfo* dInfo)
{

	UE_LOG(LogTemp, Log, TEXT("APD_E_Door::Set_DoorInfo"));

	this->doorInfo = dInfo;
	this->ID_Interactuable = dInfo->IDInteractuable;

	for (int i = 0; i < this->doorInfo->reactuables.Num(); i++) {
		for (APD_E_Interactuable* other : otherInteractuables) {
			if (other->ID_Interactuable == this->doorInfo->reactuables[i]) {
				ActivateThisReactorsWhenActive.Add(other);
				break;
			}
		}
	}

	InteractFromThisLogicPositions = this->doorInfo->logpos.GenerateAdjacents();

}

void APD_E_Door::InteractToActivate(AActor * interactor, bool overwriteState)
{
}

void APD_E_Door::InteractToDeactivate(AActor * interactor, bool overwriteState)
{
}


void APD_E_Door::ChangeRotationToReal(PD_MG_LogicPosition lp) {

	FOutputDeviceDebug  debug;

	Super::ActualLogicPosition = lp;
	Super::SGI = Cast<UPD_ClientGameInstance>(GetWorld()->GetGameInstance());

	TArray<PD_MG_LogicPosition> adj_reales = Super::SGI->mapManager->Get_LogicPosition_Adyacents_To(Super::ActualLogicPosition);
	PD_MG_LogicPosition N = PD_MG_LogicPosition(Super::ActualLogicPosition.GetX() - 1, Super::ActualLogicPosition.GetY());
	PD_MG_LogicPosition S = PD_MG_LogicPosition(Super::ActualLogicPosition.GetX() + 1, Super::ActualLogicPosition.GetY());
	PD_MG_LogicPosition E = PD_MG_LogicPosition(Super::ActualLogicPosition.GetX(), Super::ActualLogicPosition.GetY() + 1);
	PD_MG_LogicPosition W = PD_MG_LogicPosition(Super::ActualLogicPosition.GetX(), Super::ActualLogicPosition.GetY() - 1);

	UE_LOG(LogTemp, Log, TEXT("APD_E_WallActor::ChangeMaterialProperties_MapSkin - adj_reales %d isTile %d isProp %d && "),
		adj_reales.Contains(N), Super::SGI->mapManager->IsLogicPositionATile(N), Super::SGI->mapManager->IsLogicPositionAProp(N));

	// Tienes una tile en el norte o el sur?
	if ((adj_reales.Contains(N) && (Super::SGI->mapManager->IsLogicPositionATile(N) || Super::SGI->mapManager->IsLogicPositionAProp(N)))
		|| (adj_reales.Contains(S) && (Super::SGI->mapManager->IsLogicPositionATile(S) || Super::SGI->mapManager->IsLogicPositionAProp(S)))
		) {
		this->CallFunctionByNameWithArguments(TEXT("BP_ChangeRotationToReal"), debug, this, true);
	}


	// Tienes una tile en el este o el oeste?
	else if ((adj_reales.Contains(E) && (Super::SGI->mapManager->IsLogicPositionATile(E) || Super::SGI->mapManager->IsLogicPositionAProp(E)))
		|| (adj_reales.Contains(W) && (Super::SGI->mapManager->IsLogicPositionATile(W) || Super::SGI->mapManager->IsLogicPositionAProp(W)))
		) {
		this->CallFunctionByNameWithArguments(TEXT("BP_ChangeRotationToReal"), debug, this, true);
	}
}


void APD_E_Door::GetInteractuableDoorInfo(int &id_interactable, FString &name_interactable)
{
	if (doorInfo)
	{
		id_interactable = doorInfo->IDInteractuable;
		name_interactable = "door_";
		name_interactable.Append(FString::FromInt(id_interactable));
	}
	else
	{
		id_interactable = -1;
		name_interactable = "none_info";
	}

}