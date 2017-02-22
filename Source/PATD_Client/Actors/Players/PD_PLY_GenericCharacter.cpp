// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_PLY_GenericCharacter.h"


// Sets default values
APD_PLY_GenericCharacter::APD_PLY_GenericCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APD_PLY_GenericCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APD_PLY_GenericCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

