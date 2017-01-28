// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_ClientGameInstance.h"
#include "NW_NetWorking/PD_NW_ServerActor.h"
#include "NW_NetWorking/Socket/PD_NW_SocketManager.h"



void UPD_ClientGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Init GameInstance ~> "));
	initializeNetworking();
}


void UPD_ClientGameInstance::initializeNetworking()
{
	socketManager = new PD_NW_SocketManager();
	socketManager->SetIsServer(false);

	APD_NW_ServerActor* ServerActorSpawned = (APD_NW_ServerActor*)GetWorld()->SpawnActor(APD_NW_ServerActor::StaticClass());

	//Como buscamos la ip para que no tengamos que ponerla a mano en la interfaz?
	socketManager->Init(ServerActorSpawned, "127.0.0.1", 8891);//Con esto empezaria el timer, quizas no lo queremos llamar aqui o queremos separarlo entre init y start
//	socketManager->
}

PD_NW_SocketManager* UPD_ClientGameInstance::GetSocketManager()
{
	return socketManager;
}



