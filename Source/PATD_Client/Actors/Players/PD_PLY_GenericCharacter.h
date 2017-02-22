// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PD_PLY_Controller.h"
#include "../MyCharacterParent.h"
#include "PD_PLY_GenericCharacter.generated.h"

UCLASS()
class PATD_CLIENT_API APD_PLY_GenericCharacter : public AActor
{
	GENERATED_BODY()

/* ////// Variables ////// */
	//Posicion, Rotacion para realizar el spawn del character
	FVector spawnLoc;
	FRotator spawnRot;

	//Character
	AMyCharacterParent* characterParent;
	TSubclassOf<class AMyCharacterParent> character_Player_BP; //BP donde esta el PROP del Character

	//Controller para el personaje
	APD_PLY_Controller* playerController;

	//Character Stats and Logic
	//Character_Logic LogicCharacter;

//Metodos


public:	
	// Sets default values for this actor's properties
	APD_PLY_GenericCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Variables


	//Metodos
	
	
};
