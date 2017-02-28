// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_ClientGameInstance.h"
#include "NW_NetWorking/PD_NW_TimerActor.h"

//Includes de uso
#include "MapGeneration/Static/PD_MG_StaticMap.h"
#include "MapGeneration/Dynamic/PD_MG_DynamicMap.h"
#include "NW_NetWorking/Socket/PD_NW_SocketManager.h"


//Includes of forward declaration
#include "Structs/PD_ClientStructs.h" //Para todos los structs y enums
#include "NW_NetWorking/PD_NW_NetworkManager.h"
#include "MapGeneration/PD_MG_MapParser.h"
#include "GM_Game/LogicCharacter/PD_GM_LogicCharacter.h"
//Includes de prueba



bool UPD_ClientGameInstance::SuscribeToEvents(int inPlayer, UStructType inType) {
	return true; //de momento recibe todos, siempre es cierto.
}

void UPD_ClientGameInstance::HandleEvent(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {
	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::HandleEvent:: Evento recibido%d. Estado: %d"), static_cast<uint8>(inEventType), static_cast<uint8>(structClientState->enumClientState));

	if (structClientState->enumClientState == EClientState::NoConnection) {
		if (inEventType == UStructType::FStructOrderMenu) {
			FStructOrderMenu* menuOrder = (FStructOrderMenu*)inDataStruct;
			if (MenuOrderType(menuOrder->orderType) == MenuOrderType::Welcome) {
				structClientState->clientMaster = menuOrder->isClientMaster;
				structClientState->numPlayer = menuOrder->playerIndex;
				this->UpdateState();
			}
		}
	}else if (structClientState->enumClientState == EClientState::ConfigureGame) {
		if (inEventType == UStructType::FStructOrderMenu) {
			FStructOrderMenu* menuOrder = (FStructOrderMenu*)inDataStruct;
			if (MenuOrderType(menuOrder->orderType) == MenuOrderType::ChangeToLobby) {
				structClientState->configurationGameDone = true;
				this->UpdateState();
			}
		}
	}else if (structClientState->enumClientState == EClientState::WaitingConfiguration) {
		if (inEventType == UStructType::FStructOrderMenu) {
			FStructOrderMenu* menuOrder = (FStructOrderMenu*)inDataStruct;
			if (MenuOrderType(menuOrder->orderType) == MenuOrderType::ChangeToLobby) {
				structClientState->configurationGameDone = true;
				this->UpdateState();
			}
		}
	}else if (structClientState->enumClientState == EClientState::ConfigureCharacter) {
		if (inEventType == UStructType::FStructMap) {
			FStructMap* mapStruct = (FStructMap*)inDataStruct;
			structClientState->mapString = mapStruct->stringMap;
			structClientState->configurationCharacterDone = true;
			this->UpdateState();
			
		}
	}
}

void UPD_ClientGameInstance::UpdateState() {
	if (structClientState->enumClientState == EClientState::NoConnection) { //Estado incial.
		//Transiciones de estados
		if (structClientState->clientMaster && structClientState->numPlayer!=-1) {
			this->ChangeState(EClientState::WaitingConfiguration);
		}
		else if (structClientState->clientMaster && structClientState->numPlayer != -1) {
			this->ChangeState(EClientState::ConfigureGame);

		}
	}else  if (structClientState->enumClientState == EClientState::WaitingConfiguration) {//En este estado entran los demas mientras el clientMaster configura.
		//Transiciones de estados
		if (structClientState->configurationGameDone) {
			this->ChangeState(EClientState::ConfigureCharacter);
		}
	}else if (structClientState->enumClientState == EClientState::ConfigureGame) { //En este estado solo entra el clientMaster y configura el juego
		//Transiciones de estados
		if (structClientState->configurationGameDone) {
			this->ChangeState(EClientState::ConfigureCharacter);
		}
	}
	else if (structClientState->enumClientState == EClientState::ConfigureCharacter) { //(Lobby) En este estado se envia el ready y el data del personaje.
		if (structClientState->configurationCharacterDone) {
			this->ChangeState(EClientState::GameInProcess);
		}
	}

}


void UPD_ClientGameInstance::OnBeginState() {
	if (structClientState->enumClientState == EClientState::NoConnection) {

	}else  if (structClientState->enumClientState == EClientState::WaitingConfiguration) {
		this->LoadMap(levelsNameDictionary.GetMapName(2));

	}else if (structClientState->enumClientState == EClientState::ConfigureGame) {
		this->LoadMap(levelsNameDictionary.GetMapName(2));

	}else if (structClientState->enumClientState == EClientState::ConfigureCharacter) { //Lobby
		this->LoadMap(levelsNameDictionary.GetMapName(3));

	
	}else if (structClientState->enumClientState == EClientState::GameInProcess) {

		mapParser = new PD_MG_MapParser();

		staticMapRef = new PD_MG_StaticMap();
		PD_MG_DynamicMap* dynamicMapRef = new PD_MG_DynamicMap();
		
//		mapParser->StartParsingFromFString(&structClientState->mapString, staticMapRef );
	//  mapManager = new PD_GM_MapManager();
		//mapManager->StaticMapRef = staticMapRef;
		//mapManager->DynamicMapRef = dynamicMapRef;

		
		
		//Enviar mapa al cliente
		
		this->LoadMap(levelsNameDictionary.GetMapName(4));//Mapa de juego
	}
	else //Caso indeterminado
	{
		//UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: WARNING: estado sin inicializacion"));
	}

}

void UPD_ClientGameInstance::ChangeState(EClientState newState) {
	structClientState->enumClientState = newState;
	OnBeginState();
}

void UPD_ClientGameInstance::LoadMap(FString mapName)
{
	UGameplayStatics::OpenLevel((UObject*)this, FName(*mapName));
}

//Callback cuando el mapa este cargado
void UPD_ClientGameInstance::OnMapFinishLoad() {
	//Sin importar el estado hacemos:
	APD_NW_TimerActor* TimerActorSpawned = (APD_NW_TimerActor*)GetWorld()->SpawnActor(APD_NW_TimerActor::StaticClass());
	networkManager->GetSocketManager()->InitTimerActor(TimerActorSpawned);

	if (structClientState->enumClientState == EClientState::GameInProcess) {
		//Quizas esto es tarea del gameManager.
		//parserActor = (AParserActor*)GetWorld()->SpawnActor(AParserActor::StaticClass());
//		mapParser->InstantiateStaticMap(parserActor, staticMapRef);
		
		//Aqui cedemos el control al GameManager.
		

	}
}


void UPD_ClientGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Init GameInstance ~> "));
	//InitializeNetworking();

	levelsNameDictionary = LevelsNameDictionary();

	structClientState = new StructClientState();
	structClientState->enumClientState = EClientState::NoConnection;

	playerInfo = new StructPlayer(); //El constructor del StructPlayer inicializa sus variables
	/*
		Construimos el logicCharacter, para luego poder rellenarlo. Lo configuramos como Jugador. 
	*/
	playerInfo->logic_Character = new PD_GM_LogicCharacter();
	playerInfo->logic_Character->SetIsPlayer(true);
	playerInfo->logic_Character->SetTypeCharacter(ECharacterType(0)); //Al ser player. 0 vuelve a indicar que es Jugador.

	InitializeNetworking();

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

void UPD_ClientGameInstance::Shutdown()
{
	Super::Shutdown();

	if (networkManager) {
		delete networkManager;
	}
	//Aqui habria que hacer muchos deletes 

}



void UPD_ClientGameInstance::InitializeNetworking()
{
	networkManager = new PD_NW_NetworkManager();
	
	PD_NW_SocketManager* socketManager = networkManager->GetSocketManager();
	
	socketManager->SetIsServer(false);

	APD_NW_TimerActor* ClientActorSpawned = (APD_NW_TimerActor*)GetWorld()->SpawnActor(APD_NW_TimerActor::StaticClass());

	socketManager->SetNetworkManager(networkManager);
	//Como buscamos la ip para que no tengamos que ponerla a mano en la interfaz?
	socketManager->Init(ClientActorSpawned, serverAddressToConnect, defaultServerPort);//Con esto empezaria el timer, quizas no lo queremos llamar aqui o queremos separarlo entre init y start
	

	
}





/*********************************
****** FUNCIONES BP / UTILIDAD 
*********************************/

void UPD_ClientGameInstance::SetServerAddressToConnect(FString ip) {
	if (ip == "")
		serverAddressToConnect = "127.0.0.1";
	else
		serverAddressToConnect = ip;

	networkManager->ConnectTo(serverAddressToConnect, defaultServerPort);
}


bool UPD_ClientGameInstance::GetIsGameMaster()
{
	return structClientState->clientMaster;
}


void UPD_ClientGameInstance::GoToLobby()
{
	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::GoToLobby()."));
	FStructOrderMenu respuesta = FStructOrderMenu();
	respuesta.orderType = static_cast<uint8>(MenuOrderType::GameConfigurationDone);
	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance:: Enviando: 2 - GameConfigurationDone"));
	networkManager->SendNow(&respuesta, 0);
}


void UPD_ClientGameInstance::GetReadyToParty()
{
	FStructOrderMenu respuesta = FStructOrderMenu();
	respuesta.orderType = respuesta.orderType = static_cast<uint8>(MenuOrderType::ClientReady);
	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance:: Enviando: 4 - ClientReady"));
	networkManager->SendNow(&respuesta, 0);
}

void UPD_ClientGameInstance::FillCharecterStats(int nPOD, int nAGI, int nDES, int nCON, int nPER, int nMAL) 
{
	playerInfo->logic_Character->SetBasicStats(nPOD, nAGI, nDES, nCON, nPER, nMAL);
	playerInfo->logic_Character->SetInitBaseStats(100, 20,5); //HP - DMG
	playerInfo->logic_Character->SetWapon();
	playerInfo->logic_Character->SetTotalStats();
}

void UPD_ClientGameInstance::GetCharacterTotalStats(int &nAP, int &nCH, int &nSA, int &nHP, int &nRAN, int &nDMG)
{
	nAP = playerInfo->logic_Character->GetTotalStats()->APTotal;
	nCH = playerInfo->logic_Character->GetTotalStats()->CH;
	nSA = playerInfo->logic_Character->GetTotalStats()->SA;
	nHP = playerInfo->logic_Character->GetTotalStats()->HPTotal;
	nRAN = playerInfo->logic_Character->GetTotalStats()->RangeTotal;
	nDMG = playerInfo->logic_Character->GetTotalStats()->DMGTotal;

}


bool UPD_ClientGameInstance::SendCharacterToServer()
{
	if (!playerInfo->logic_Character) {
		return false;
	}
	FStructCharacter structCharacterToSend =  FStructCharacter();
	structCharacterToSend.totalStats = *(playerInfo->logic_Character->GetTotalStats());
	structCharacterToSend.initBaseStats = *(playerInfo->logic_Character->GetInitBaseStats());
	structCharacterToSend.skils = *(playerInfo->logic_Character->GetSkills());
	structCharacterToSend.skin = *(playerInfo->logic_Character->GetSkin());
	structCharacterToSend.weapon = *(playerInfo->logic_Character->GetWeapon());

	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance:: Enviando: Character Stats and Data"));
	networkManager->SendNow(&structCharacterToSend, 0);
	
	return true;
}

