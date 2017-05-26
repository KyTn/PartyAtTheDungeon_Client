// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_E_ElementActor.h"
#include "PD_ClientGameInstance.h"

// Sets default values
APD_E_ElementActor::APD_E_ElementActor()
{
	//SGI = Cast<UPD_ServerGameInstance>(GetWorld()->GetGameInstance());
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APD_E_ElementActor::BeginPlay()
{
	Super::BeginPlay();
	SGI = Cast<UPD_ClientGameInstance>(GetWorld()->GetGameInstance());
}

// Called every frame
void APD_E_ElementActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void APD_E_ElementActor::SetMaterialSkin(MapSkinType inMaterialSkin) {

	mapSkin = inMaterialSkin;

	ChangeMaterialProperties_MapSkin();

};



void APD_E_ElementActor::ChangeMaterialProperties_MapSkin() {

	FOutputDeviceDebug  debug;
	this->CallFunctionByNameWithArguments(TEXT("BP_ChangeMaterialProperties_MapSkin"), debug, this, true);
}


