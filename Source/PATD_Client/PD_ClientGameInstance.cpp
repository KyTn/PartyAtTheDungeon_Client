// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_ClientGameInstance.h"
#include "NW_NetWorking/PD_NW_ServerActor.h"

#include "NW_NetWorking/Socket/PD_NW_SocketManager.h"
#include "SR_Serializer/PD_SR_UStruct.h"

//Includes of forward declaration
#include "NW_NetWorking/PD_NW_NetworkManager.h"

//Includes de prueba
#include "NW_Networking/EventLayer/PD_NW_iEventObserver.h"

void UPD_ClientGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Init GameInstance ~> "));
	InitializeNetworking();


	class ObservadorPrueba : public PD_NW_iEventObserver
	{
	public:
		UPD_ClientGameInstance *gi;
		ObservadorPrueba(UPD_ClientGameInstance* i) {
			gi = i;
		}
		void handleEvent(FStructGenericoHito2* dataStruct, int inPlayer, UStructType inEventType) {
			

			if (dataStruct->orderType != -1) { //NullOrder
				FStructGenericoHito2 respuesta = FStructGenericoHito2();
				switch (dataStruct->orderType) {
				case 5: //SetClientMaster
					gi->isGameMaster = true;
					gi->numPlayer = dataStruct->stringMap;
					break;

				case 6://Welcome
					gi->numPlayer = dataStruct->stringMap;
					break;
				case 7://ChangeToMainMenu
					gi->LoadMap("LVL_2_MainMenu");
		
					break;
				case 8://ChangeToLobby
					gi->LoadMap("LVL_3_SelectChars_Lobby");

					break;
				case 9://ChangeToMap
					gi->LoadMap("LVL_4_GameMap");

					break;
				case 10://InvalidConnection
					//Que hacemos?

					break;
				}

			}
			else {//No es una order, asi que es un map
				//Cargar el mapa que viene en el string.
				gi->LoadMap(dataStruct->stringMap);
			}

		}
	};
	ObservadorPrueba* obs = new ObservadorPrueba(this);
	obs->setUpObserver(-1, UStructType::AllStructs);
	networkManager->RegisterObserver(obs);







	//PRUEBA
	FStructGenericoHito2* m =  new FStructGenericoHito2();
	m->orderType = 0;
	UE_LOG(LogTemp, Warning, TEXT("Enviando Order %d"), m->orderType);

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


void UPD_ClientGameInstance::LoadMap(FString mapName)
{
	UGameplayStatics::OpenLevel((UObject*)this, FName(*mapName));


}

void UPD_ClientGameInstance::InitClientActoWhenLoadMap()
{
	APD_NW_ServerActor* ServerActorSpawned = (APD_NW_ServerActor*)GetWorld()->SpawnActor(APD_NW_ServerActor::StaticClass());

	//socketManager->InitServerActor(ServerActorSpawned);
	networkManager->GetSocketManager()->InitServerActor(ServerActorSpawned);


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



