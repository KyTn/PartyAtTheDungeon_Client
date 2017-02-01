// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_NW_ClientActor.h"

//Includes of forward declaration
#include "NW_Networking/Socket/PD_NW_SocketManager.h"


//Includes de prueba

#include "PD_ClientGameInstance.h"

// Sets default values
APD_NW_ClientActor::APD_NW_ClientActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APD_NW_ClientActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APD_NW_ClientActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APD_NW_ClientActor::InitTimerActor()
{
	GetWorldTimerManager().SetTimer(TimerHandleActor, this, &APD_NW_ClientActor::CheckForReceivedData, 0.01f, true);

	//Timers de prueba

	GetWorldTimerManager().SetTimer(TimerHandleActor3, this, &APD_NW_ClientActor::SendPruebaSockets, 5.0f, true);
}


void APD_NW_ClientActor::CheckForReceivedData()
{
	SocketManager->TimerRefreshFunction();
}


void APD_NW_ClientActor::SetSocketManager(PD_NW_SocketManager* InSocketManager)
{
	SocketManager = InSocketManager;
}


bool APD_NW_ClientActor::isTimerActive() {
	return GetWorldTimerManager().IsTimerActive(TimerHandleActor);
}

//Funciones de prueba




void APD_NW_ClientActor::SendPruebaSockets()
{

	//PRUEBA


}