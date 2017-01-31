// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_ClientGameInstance.h"
#include "NW_NetWorking/PD_NW_ServerActor.h"

#include "NW_NetWorking/Socket/PD_NW_SocketManager.h"
#include "SR_Serializer/PD_SR_UStruct.h"

//Includes of forward declaration
#include "NW_NetWorking/PD_NW_NetworkManager.h"

//Includes de prueba








void UPD_ClientGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Init GameInstance ~> "));
	InitializeNetworking();

	//PRUEBA
	FStructMap* m =  new FStructMap();

	UE_LOG(LogTemp, Warning, TEXT("Enviando MapStruct "));

	networkManager->SendNow(m,0);


	/* //Pruebas feas del ustructLista

	FStructListaPruebas* generycStructs = new FStructListaPruebas();
	FStructGeneric * n = new FStructGeneric();
	generycStructs->list.Add(*n);

	TArray<uint8>* data = new TArray<uint8>();

	UStruct* FMyStruct = FStructListaPruebas::StaticStruct();
	UE_LOG(LogTemp, Warning, TEXT("indice tras ustruct %d"), generycStructs->list.Num());

	FMemoryWriter ArWriter(*data);
	UE_LOG(LogTemp, Warning, TEXT("indice tras arWriter %d"), generycStructs->list.Num());

	FMyStruct->SerializeBin(ArWriter, &generycStructs);

	UE_LOG(LogTemp, Warning, TEXT("indice tras serializebin %d"), generycStructs->list.Num());

	UE_LOG(LogTemp, Warning, TEXT("indice data tras serializebin %d"), data->Num());

	
	//PRUEBA
	{
		FStructGenericList* gg = generycStructs;

		//FStructGenericList* generyStructs = new FStructGenericList();
		UStruct* FMyStruct = FStructGenericList::StaticStruct();
		FMemoryReader ArReader(*data);
		FMyStruct->SerializeBin(ArReader, &gg);//No estoy seguro si añade las estructuras al array


		UE_LOG(LogTemp, Warning, TEXT("indice tras vuelta a serialize %d"), gg->list.Num());
	}*/

}


void UPD_ClientGameInstance::InitializeNetworking()
{
	networkManager = new PD_NW_NetworkManager();
	PD_NW_SocketManager* socketManager = networkManager->GetSocketManager();
	
	socketManager->SetIsServer(false);

	APD_NW_ServerActor* ServerActorSpawned = (APD_NW_ServerActor*)GetWorld()->SpawnActor(APD_NW_ServerActor::StaticClass());

	socketManager->SetNetworkManager(networkManager);
	//Como buscamos la ip para que no tengamos que ponerla a mano en la interfaz?
	socketManager->Init(ServerActorSpawned, "127.0.0.1", defaultServerPort);//Con esto empezaria el timer, quizas no lo queremos llamar aqui o queremos separarlo entre init y start

	networkManager->ConnectTo("127.0.0.1", defaultServerPort);
}

/*
PD_NW_SocketManager* UPD_ClientGameInstance::GetSocketManager()
{
	return socketManager;
}
*/



