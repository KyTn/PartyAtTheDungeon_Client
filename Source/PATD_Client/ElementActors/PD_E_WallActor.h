// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PD_E_WallActor.generated.h"

UCLASS()
class PATD_CLIENT_API APD_E_WallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APD_E_WallActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;
	
};
