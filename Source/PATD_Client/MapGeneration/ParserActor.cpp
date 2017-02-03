// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PATD_Client/MapGeneration/ParserActor.h"
#include "PATD_Client/MapGeneration/PD_MG_StaticMap.h"
#include "PATD_Client/MapGeneration/PD_MG_LogicPosition.h"
#include "PATD_Client/MapGeneration/PD_MG_MapParser.h"
#include "PATD_Client/ElementActors/PD_E_TileActor.h"
#include "PATD_Client/ElementActors/PD_E_WallActor.h"



// Sets default values
AParserActor::AParserActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AParserActor::BeginPlay()
{
	Super::BeginPlay();

	StaticMapRef = new PD_MG_StaticMap();


	PD_MG_MapParser* parseMap = new PD_MG_MapParser();

	// Recibir desde el servidor el chorizo del mapa

	FString data;
	parseMap->StartParsingFromFString(&data, StaticMapRef);
	parseMap->InstantiateStaticMap(this);



}



AActor* AParserActor::InstantiateTile(PD_MG_LogicPosition* logpos)
{
	//return GetWorld()->SpawnActor<APD_E_TileActor>(tileActor,FVector(logpos->GetX()*100.0f, logpos->GetY() * 100.0f, 0.f), FRotator(0.0f, 0.f, 0.f), FActorSpawnParameters());

	return GetWorld()->SpawnActor<APD_E_TileActor>(FVector(-1.0f * logpos->GetX()*100.0f, logpos->GetY() * 100.0f, 0.f), FRotator(0.0f, 0.f, 0.f), FActorSpawnParameters());
}


AActor* AParserActor::InstantiateWall(PD_MG_LogicPosition* logpos)
{
	//return GetWorld()->SpawnActor<APD_E_TileActor>(tileActor,FVector(logpos->GetX()*100.0f, logpos->GetY() * 100.0f, 0.f), FRotator(0.0f, 0.f, 0.f), FActorSpawnParameters());

	return GetWorld()->SpawnActor<APD_E_WallActor>(FVector(-1.0f * logpos->GetX()*100.0f, logpos->GetY() * 100.0f, 0.f), FRotator(0.0f, 0.f, 0.f), FActorSpawnParameters());
}



void AParserActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}