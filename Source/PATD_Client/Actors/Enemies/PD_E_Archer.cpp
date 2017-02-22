// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_E_Archer.h"


// Sets default values
APD_E_Archer::APD_E_Archer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APD_E_Archer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APD_E_Archer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

