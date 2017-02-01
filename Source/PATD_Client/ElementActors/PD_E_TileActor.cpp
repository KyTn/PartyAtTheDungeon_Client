// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_E_TileActor.h"


// Sets default values
APD_E_TileActor::APD_E_TileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = mesh;
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/Models/MapElements/Tile_Model.Tile_Model'"));
	mesh->SetStaticMesh(MeshObj.Object);
}

// Called when the game starts or when spawned
void APD_E_TileActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APD_E_TileActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

