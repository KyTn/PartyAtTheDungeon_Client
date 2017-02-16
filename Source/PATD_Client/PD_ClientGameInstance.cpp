// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_ClientGameInstance.h"
#include "NW_NetWorking/PD_NW_TimerActor.h"

#include "NW_NetWorking/Socket/PD_NW_SocketManager.h"
#include "SR_Serializer/PD_SR_UStruct.h"

//Includes of forward declaration
#include "NW_NetWorking/PD_NW_NetworkManager.h"

//Includes de prueba
#include "NW_Networking/EventLayer/PD_NW_iEventObserver.h"
#include "MapGeneration/ParserActor.h"




void UPD_ClientGameInstance::HandleEvent(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {


	FStructGenericoHito2* dataStruct = (FStructGenericoHito2*)inDataStruct;

	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance:: Recibido order tipo:%d"), dataStruct->orderType);
	FStructGenericoHito2 respuesta = FStructGenericoHito2();
	if (dataStruct->orderType != 255) { //NullOrder
		switch (dataStruct->orderType) {
		case 5: //SetClientMaster
			this->isGameMaster = true;
			this->numPlayer = dataStruct->stringMap;

			respuesta.orderType = 1;//GoToMainMenu
			UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance:: Enviando: 1 - GoToMainMenu"));
			this->networkManager->SendNow(&respuesta, 0);
			break;

		case 6://Welcome
			this->numPlayer = dataStruct->stringMap;
			break;
		case 7://ChangeToMainMenu
			this->LoadMap("LVL_2_MainMenu");

			break;
		case 8://ChangeToLobby
			this->LoadMap("LVL_3_SelectChars_Lobby");

			break;
		case 9://ChangeToMap
			this->LoadMap("LVL_4_GameMap");

			break;
		case 10://InvalidConnection
				//Que hacemos?

			break;
		}

	}
	else {//No es una order, asi que es un map
		  //Cargar el mapa que viene en el string.
		UE_LOG(LogTemp, Warning, TEXT("Recibido mapa"), *dataStruct->stringMap);
		this->mapString = dataStruct->stringMap;
		UE_LOG(LogTemp, Warning, TEXT("Mapa recibido %s"), *this->mapString);

		respuesta.orderType = 11;
		UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance:: Enviando: 11 ??"));
		this->networkManager->SendNow(&respuesta, 0);
	}

}

bool UPD_ClientGameInstance::SuscribeToEvents(int inPlayer, UStructType inType) {
	return true; //de momento recibe todos, siempre es cierto.
}



void UPD_ClientGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Init GameInstance ~> "));
	//InitializeNetworking();

	levelsNameDictionary = LevelsNameDictionary();


	

	networkManager = new PD_NW_NetworkManager();
	
	networkManager->RegisterObserver(this);

	//PRUEBA
	//FStructGenericoHito2* m =  new FStructGenericoHito2();
	//m->orderType = 0;
	//UE_LOG(LogTemp, Warning, TEXT("Enviando Order %d"), m->orderType);

	//networkManager->SendNow(m,0);


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


void UPD_ClientGameInstance::LoadMap(FString mapName)
{
	UGameplayStatics::OpenLevel((UObject*)this, FName(*mapName));
}

void UPD_ClientGameInstance::InitClientActoWhenLoadMap()
{
	APD_NW_TimerActor* ClientActorSpawned = (APD_NW_TimerActor*)GetWorld()->SpawnActor(APD_NW_TimerActor::StaticClass());

	//socketManager->InitServerActor(ServerActorSpawned);
	networkManager->GetSocketManager()->InitTimerActor(ClientActorSpawned);
}

void UPD_ClientGameInstance::InitGameMap()
{
	FString s = GetWorld()->GetMapName();
	UE_LOG(LogTemp, Warning, TEXT("Init GameMap %s"), *s);


	AParserActor* ParseActor = (AParserActor*)GetWorld()->SpawnActor(AParserActor::StaticClass());
	ParseActor->InitGameMap(mapString);

}

void UPD_ClientGameInstance::InitializeNetworking()
{
	
	PD_NW_SocketManager* socketManager = networkManager->GetSocketManager();
	
	socketManager->SetIsServer(false);

	APD_NW_TimerActor* ClientActorSpawned = (APD_NW_TimerActor*)GetWorld()->SpawnActor(APD_NW_TimerActor::StaticClass());

	socketManager->SetNetworkManager(networkManager);
	//Como buscamos la ip para que no tengamos que ponerla a mano en la interfaz?
	socketManager->Init(ClientActorSpawned, serverAddressToConnect, defaultServerPort);//Con esto empezaria el timer, quizas no lo queremos llamar aqui o queremos separarlo entre init y start
	networkManager->ConnectTo(serverAddressToConnect, defaultServerPort);

	FStructGenericoHito2 respuesta = FStructGenericoHito2();
	
}





/*********************************
****** FUNCIONES BP / UTILIDAD 
*********************************/

void UPD_ClientGameInstance::SetServerAddressToConnect(FString ip) {
	if (ip == "")
		serverAddressToConnect = "127.0.0.1";
	else
		serverAddressToConnect = ip;

	InitializeNetworking();
}


bool UPD_ClientGameInstance::GetIsGameMaster()
{
	return isGameMaster;
}
/*
PD_NW_SocketManager* UPD_ClientGameInstance::GetSocketManager()
{
	return socketManager;
}
*/

void UPD_ClientGameInstance::GoToLobby()
{
	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::GoToLobby()."));
	FStructGenericoHito2 respuesta = FStructGenericoHito2();
	respuesta.orderType = 2;
	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance:: Enviando: 2 - GoToLobby"));
	networkManager->SendNow(&respuesta, 0);
}


void UPD_ClientGameInstance::GetReadyToParty()
{
	FStructGenericoHito2 respuesta = FStructGenericoHito2();
	respuesta.orderType = 4;
	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance:: Enviando: 4 - ClientReady"));
	networkManager->SendNow(&respuesta, 0);
}
