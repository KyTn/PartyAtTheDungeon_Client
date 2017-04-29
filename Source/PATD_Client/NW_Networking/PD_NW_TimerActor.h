// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PD_NW_TimerActor.generated.h"

//forward declarations
class PD_NW_SocketManager;


UCLASS()
class PATD_CLIENT_API APD_NW_TimerActor : public AActor
{
	GENERATED_BODY()

private:
	PD_NW_SocketManager* SocketManager;


public:
	// Sets default values for this actor's properties
	APD_NW_TimerActor();


	FTimerHandle TimerHandleActor;



	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;


	//Setea e inicia el timer.
	void InitTimerActor();

	//Funcion a la que llama el timer
	void CheckForReceivedData();

	//Referencia al socketManager
	void SetSocketManager(PD_NW_SocketManager* InSocketManager);

	//Funcion que determina si el timer esta activo.
	bool isTimerActive();


	//Comprobar al X tiempo que se ha recibido un pong desde el cliente
	void SetTimerToCheckPong();
	void CallFunctionCheckPongByNetworkManager();
};
