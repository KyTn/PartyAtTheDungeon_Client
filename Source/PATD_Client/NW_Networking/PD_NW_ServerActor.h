// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PD_NW_ServerActor.generated.h"

//forward declarations
class PD_NW_SocketManager;

UCLASS()
class PATD_CLIENT_API APD_NW_ServerActor : public AActor
{
	GENERATED_BODY()
	
private:
	PD_NW_SocketManager* SocketManager;


public:
	// Sets default values for this actor's properties
	APD_NW_ServerActor();


	FTimerHandle TimerHandleActor;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void InitTimerActor();
	void CheckForReceivedData();

	void SetSocketManager(PD_NW_SocketManager* InSocketManager);

	
	
};
