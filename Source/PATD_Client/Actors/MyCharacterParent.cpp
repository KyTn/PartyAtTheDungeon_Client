// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "MyCharacterParent.h"


// Sets default values
AMyCharacterParent::AMyCharacterParent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacterParent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacterParent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AMyCharacterParent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

