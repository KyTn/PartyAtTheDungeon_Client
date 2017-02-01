// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_E_WallActor.h"


// Sets default values
APD_E_WallActor::APD_E_WallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = mesh;
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/Models/MapElements/Wall_Model.Wall_Model'"));
	mesh->SetStaticMesh(MeshObj.Object);
}

// Called when the game starts or when spawned
void APD_E_WallActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APD_E_WallActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

