// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PATD_Client/Actors/MyCharacterParent.h"
#include "PD_E_Archer.generated.h"

UCLASS()
class PATD_CLIENT_API APD_E_Archer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APD_E_Archer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/*	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BP Classes")
	UBlueprint* archerClass;
	*/

	AMyCharacterParent* characterP;

	TSubclassOf<class AMyCharacterParent> archerClass;
	
};
