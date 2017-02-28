// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_E_EnemyCharacter.h"


// Sets default values
APD_E_EnemyCharacter::APD_E_EnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APD_E_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APD_E_EnemyCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void APD_E_EnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

