// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_E_Interactuable.h"
#include "Actors/PD_E_ElementActor.h"
#include "MapInfo/PD_MM_MapInfo.h"


// Sets default values
APD_E_Interactuable::APD_E_Interactuable()
{
	//Super::APD_E_ElementActor();
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APD_E_Interactuable::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APD_E_Interactuable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APD_E_Interactuable::Set_InteractuableInfo(TArray<APD_E_Interactuable*> otherInteractuables, PD_MM_InteractuableInfo * interInfo)
{
	interactuableInfo = interInfo;
	Name = interactuableInfo->Name_Interactuable;
	for (int i = 0; i < interactuableInfo->reactuables.Num(); i++) {
		for (APD_E_Interactuable* other : otherInteractuables) {
			if (other->ID_Interactuable == interactuableInfo->reactuables[i]) {
				ActivateThisReactorsWhenActive.Add(other);
				break;
			}
		}
	}


}


// Se llamará a esta funcion para activar el interactuable
void APD_E_Interactuable::InteractToActivate(AActor* interactor, bool overwriteState) {}

// Se llamará a esta función para desactivar el interactuable
void APD_E_Interactuable::InteractToDeactivate(AActor* interactor, bool overwriteState) {}


void APD_E_Interactuable::GetInteractuableInfo(int &id_interactable, FString &name_interactable)
{
	if (interactuableInfo)
	{
		id_interactable = interactuableInfo->IDInteractuable;
		name_interactable = interactuableInfo->Name_Interactuable;
	}
	else
	{
		id_interactable = -1;
		name_interactable = "";
	}

}
