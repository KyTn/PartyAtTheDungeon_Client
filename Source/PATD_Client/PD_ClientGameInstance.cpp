// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_SaveCharacterData.h"
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
#include "GM_Game/PD_GM_MapManager.h"
#include "GM_Game/PD_GM_GameManager.h"
#include "GM_Game/PD_GM_EnemyManager.h"
#include "PD_PlayersManager.h"
#include "PD_MatchConfigManager.h"

//Includes de prueba
#include "MapInfo/PD_MM_MapInfo.h"

#include "Actors/PD_E_Character.h"

#include "Actors/Accesors/MapManagerAccesor.h"
#include "Actors/Accesors/PlayerManagerAccesor.h"
#include "Actors/Interactuables/PD_E_Interactuable.h"


bool UPD_ClientGameInstance::SuscribeToEvents(int inPlayer, UStructType inType) {
	return true; //de momento recibe todos, siempre es cierto.
}



#pragma region INIT

void UPD_ClientGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Init GameInstance ~> "));
	//InitializeNetworking();
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Orange, FString::Printf(TEXT("Version: 0")));
	//Inicializar Arrays de Skills and Weapons
	LoadSkillActiveDatafromFile();
	LoadSkillPasiveDatafromFile();
	LoadWeaponDataFromFile();

	levelsNameDictionary = LevelsNameDictionary();

	playersManager = new PD_PlayersManager();

	structClientState = new StructClientState();
	structClientState->NETMAPDATA = new FStructMapData();

	ChangeState(EClientState::StartApp);
	//structClientState->enumClientState = EClientState::NoConnection;

	playersManager->MyPlayerInfo = new StructPlayer(); //El constructor del StructPlayer inicializa sus variables
													   /*
													   Construimos el logicCharacter, para luego poder rellenarlo. Lo configuramos como Jugador.
													   */
	playersManager->MyPlayerInfo->logic_Character = new PD_GM_LogicCharacter();
	playersManager->MyPlayerInfo->turnOrders = new FStructTurnOrders();
	playersManager->MyPlayerInfo->logic_Character->SetIsPlayer(true);
	playersManager->MyPlayerInfo->logic_Character->SetTypeCharacter(ECharacterType(0)); //Al ser player. 0 vuelve a indicar que es Jugador.

	switch (GetWorld()->WorldType)
	{		
		case EWorldType::Game:
		{
			playersManager->MyPlayerInfo->ID_Client = FGenericPlatformMisc::GetMachineId().ToString(); //IDENTIFICADOR UNICO POR CLIENTE
			break;
		}
		case EWorldType::PIE:
		{
			playersManager->MyPlayerInfo->ID_Client = FGenericPlatformMisc::GetMachineId().ToString().Append(FString::FromInt(GetWorldContext()->PIEInstance)); //IDENTIFICADOR UNICO POR CLIENTE
			break;
		}
		default:
		{
			playersManager->MyPlayerInfo->ID_Client = FGenericPlatformMisc::GetMachineId().ToString(); //IDENTIFICADOR UNICO POR CLIENTE
			break;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("FGenericPlatformMisc::GetUniqueDeviceId(): %s"), *playersManager->MyPlayerInfo->ID_Client));

	InitializeNetworking();

	networkManager->RegisterObserver(this);

	MatchConfigManager = new PD_MatchConfigManager(this);
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

#pragma endregion



#pragma region HANDLER EVENT


void UPD_ClientGameInstance::HandleEvent(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {
	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::HandleEvent:: Evento recibido%d. Estado: %d"), static_cast<uint8>(inEventType), static_cast<uint8>(structClientState->enumClientState));

	if (inEventType == UStructType::FStructRequestIDClient) {
		//Cuando el Servidor pide el ID Client 
		HandleEvent_RequestIDClient(inDataStruct, inPlayer, inEventType);
	}

	if (inEventType == UStructType::FStructWelcome) {
		//Cuando el Servidor pide el ID Client 
		HandleEvent_Welcome(inDataStruct, inPlayer, inEventType);
	}

	if (inEventType == UStructType::FStructPing) {
		//Cuando el Servidor pide el ID Client 
		HandleEvent_PingReceive(inDataStruct, inPlayer);
	}

	if (inEventType == UStructType::FStructPong) {
		//Cuando el Servidor pide el ID Client 
		HandleEvent_PongReceive(inDataStruct, inPlayer);
	}

	/// IF DE ESTADOS DEL CLIENTE
	if (structClientState->enumClientState == EClientState::StartApp) {

	}
	else if (structClientState->enumClientState == EClientState::Game_NoConnection) {
		//HandleEvent_Welcome(inDataStruct, inPlayer, inEventType);
	}
	else if (structClientState->enumClientState == EClientState::ConfigureMatch) {
		if (inEventType == UStructType::FStructMatchConfig) {

			// Cuando el server envía cambios del ConfigMatch 
			HandleEvent_ConfigMatch(inDataStruct, inPlayer, inEventType);
		}
		else if (inEventType == UStructType::FStructMatchConfigDone) {

			// Cuando el server envía un ConfigMatchDone
			HandleEvent_ConfigMatchDone(inDataStruct, inPlayer, inEventType);
		}
		else {
			if (inEventType == UStructType::FStructOrderMenu) {
				FStructOrderMenu* in = (FStructOrderMenu*)inDataStruct;

				UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::HandleEvent:: ConfigureMatch - NOT MATCHING FStructOrderMenu %d"), in->orderType);

			}
			else
				UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::HandleEvent:: ConfigureMatch - NOT MATCHING "));
		}
	}
	else if (structClientState->enumClientState == EClientState::WaitingMatchConfiguration) {
		if (inEventType == UStructType::FStructMatchConfig) {

			// Cuando el server envía cambios del ConfigMatch 
			HandleEvent_ConfigMatch(inDataStruct, inPlayer, inEventType);
		}
		else if (inEventType == UStructType::FStructMatchConfigDone) {

			// Cuando el server envía un ConfigMatchDone
			HandleEvent_ConfigMatchDone(inDataStruct, inPlayer, inEventType);
		}
		else {
			if (inEventType == UStructType::FStructOrderMenu) {
				FStructOrderMenu* in = (FStructOrderMenu*)inDataStruct;

				UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::HandleEvent:: WaitingMatchConfiguration - NOT MATCHING FStructOrderMenu %d"), in->orderType);

			}
			else
				UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::HandleEvent:: WaitingMatchConfiguration - NOT MATCHING "));
		}
	}
	else if (structClientState->enumClientState == EClientState::Lobby_Tabern) {
		if (inEventType == UStructType::FStructMap) {

			// Cuando el server envia el mapa ... 
			HandleEvent_MapIncoming(inDataStruct, inPlayer, inEventType);

		}
		else if (inEventType == UStructType::FStructMapData) {

			HandleEvent_FStructMapDataIncoming(inDataStruct, inPlayer, inEventType);
			UE_LOG(LogTemp, Warning, TEXT("Init GameInstance ~> Tras el MAPINCOMING"));
		}

		else if (inEventType == UStructType::FStructClientStartMatchOnGM) {

			HandleEvent_LaunchMatchFromServer(inDataStruct, inPlayer, inEventType);
		}
		else {
			if (inEventType == UStructType::FStructOrderMenu) {
				FStructOrderMenu* in = (FStructOrderMenu*)inDataStruct;

				UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::HandleEvent:: Lobby_Tabern - NOT MATCHING FStructOrderMenu %d"), in->orderType);

			}
			else
				UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::HandleEvent:: Lobby_Tabern - NOT MATCHING "));
		}



		/// Aqui no se envía la instanciacion de los personajes !!!!!!!!!!
		/*
		if (inEventType == UStructType::FStructInstatiatePlayers) {
		FStructInstatiatePlayers* InstaPlayers = (FStructInstatiatePlayers*)inDataStruct;
		for (int i = 0; i < InstaPlayers->listInfoPlayerAtClient.Num(); i++)
		{
		playersManager->AddNewPlayer(InstaPlayers->listInfoPlayerAtClient[i]);
		}
		//this->UpdateState();
		}

		*/
	}
	else if (structClientState->enumClientState == EClientState::ReconnectingInGame) {
		//Esto en principio es para simular los paquetes que se envian durante el lobby para la reconexion inGame
		if (inEventType == UStructType::FStructMap) {

			// Cuando el server envia el mapa ... 
			HandleEvent_MapIncoming(inDataStruct, inPlayer, inEventType);
		}

		else if (inEventType == UStructType::FStructClientStartMatchOnGM) {

			HandleEvent_LaunchMatchFromServer(inDataStruct, inPlayer, inEventType);
		}
		else if(inEventType == UStructType::FStructMapData) {

			HandleEvent_FStructMapDataIncoming(inDataStruct, inPlayer, inEventType);
		}
		else if (inEventType == UStructType::FStructCharacter) {

			FStructCharacter* structCharacter = (FStructCharacter*)inDataStruct;
			*(playersManager->GetMyCharacter()->GetTotalStats()) = structCharacter->totalStats;
			*(playersManager->GetMyCharacter()->GetBasicStats()) = structCharacter->basicStats;
			*(playersManager->GetMyCharacter()->GetInitBaseStats()) = structCharacter->initBaseStats;
			*(playersManager->GetMyCharacter()->GetSkills()) = structCharacter->skills;
			*(playersManager->GetMyCharacter()->GetWeapon()) = structCharacter->weapon;
			*(playersManager->GetMyCharacter()->GetSkin()) = structCharacter->skin;
			*(playersManager->GetMyCharacter()->GetCharacterState()) = structCharacter->charState;
			

		}
		else if (inEventType == UStructType::FStructUpdateTurn) {
			UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::HandleEvent:: RECONNECTING_IN_GAME - Update RECONNECT"));
			structClientState->update_turn_Reconnect = *(FStructUpdateTurn*)inDataStruct;
			structClientState->updateTurnReceived = true;
		}
	}
	else if (structClientState->enumClientState == EClientState::Launch_Match) {

		if (inEventType == UStructType::FStructInstatiatePlayers) {
			HandleEvent_AllCharactersInfoIncoming(inDataStruct, inPlayer, inEventType);
		}
		else if (inEventType == UStructType::FStructClientStartMatchOnGM) {
			HandleEvent_StartMatch_GoGameMnager(inDataStruct, inPlayer, inEventType);
		}

		else if (inEventType == UStructType::FStructUpdateTurn) {
			UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::HandleEvent:: LAUNCH_MATCH - Update RECONNECT"));
			structClientState->update_turn_Reconnect = *(FStructUpdateTurn*)inDataStruct;
			structClientState->updateTurnReceived = true;
		}

		else {
			if (inEventType == UStructType::FStructOrderMenu) {
				FStructOrderMenu* in = (FStructOrderMenu*)inDataStruct;

				UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::HandleEvent:: Launch_Match - NOT MATCHING FStructOrderMenu %d"), in->orderType);

			}
			else
				UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::HandleEvent:: Launch_Match - NOT MATCHING "));
		}
	}
	else if (structClientState->enumClientState == EClientState::GameInProcess) {

	}
	else {}



}


#pragma region HANDLERS
void UPD_ClientGameInstance::HandleEvent_RequestIDClient(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_RequestIDClient"));
	if (inEventType == UStructType::FStructRequestIDClient)
	{
		FStructClientID myID_Client;
		myID_Client.ID_Client = playersManager->MyPlayerInfo->ID_Client;
		playersManager->MyPlayerInfo->isConnected = true;
		networkManager->SendNow(&myID_Client, 0);
	}
}

///Eventos para Ping y Pong structs -- Tema de la conexion
void UPD_ClientGameInstance::HandleEvent_PingReceive(FStructGeneric* inDataStruct, int inPlayer) //Desde el server inPlayer le envian un ping 
{
	FStructPong* pong = new FStructPong();
	networkManager->SendNow(pong, inPlayer);
}

void UPD_ClientGameInstance::HandleEvent_PongReceive(FStructGeneric* inDataStruct, int inPlayer) //Desde el server inPlayer le envian un pong (por envio del ping desde el cliente)
{
	playersManager->MyPlayerInfo->pingPong = 2;
}



void UPD_ClientGameInstance::HandleEvent_Welcome(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {
	UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_Welcome"));

	FStructWelcome* structWelcome = (FStructWelcome*)inDataStruct;
	structClientState->clientMaster = structWelcome->isClientMaster;
	structClientState->numPlayer = structWelcome->playerIndex;

	GameState clientGameState = GameState(structWelcome->GameState);
	switch (clientGameState)
	{
		case GameState::ConfigureMatch:
		{
			ChangeState(EClientState::ConfigureMatch);
			break;
		}
		case GameState::WaitingMatchConfiguration:
		{
			ChangeState(EClientState::WaitingMatchConfiguration);
			break;
		}
		case GameState::Lobby_Tabern:
		{
			ChangeState(EClientState::Lobby_Tabern);
			break;
		}
		case GameState::GameInProcess:
		{
			ChangeState(EClientState::ReconnectingInGame);
			break;
		}
		case GameState::ClientCanCreateOrders:
		{
			break;
		}
		case GameState::ClientWaitForServer:
		{
			break;
		}
		case GameState::NoConnectionAllowed:
		{
			//NO CONEXION PERMITIDA 
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NO CONNECTION ALLOWED AT THIS TIME"));
			break;
		}
		default:
			break;
	}




	if (inEventType == UStructType::FStructOrderMenu) {
		UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_Welcome:esta entrando en el oren menu"));

		FStructOrderMenu* menuOrder = (FStructOrderMenu*)inDataStruct;
		UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_Welcome: Menuorder ordertype = %d"), menuOrder->orderType);

		if (MenuOrderType(menuOrder->orderType) == MenuOrderType::Welcome) {
			UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_Welcome: si que lo esta recibiendo"));

			structClientState->clientMaster = menuOrder->isClientMaster;
			structClientState->numPlayer = menuOrder->playerIndex;
			this->UpdateState();
		}
	}
}

void UPD_ClientGameInstance::HandleEvent_ConfigMatch(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {
	UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_ConfigMatch"));

	FStructMatchConfig* StructMatchConfig = (FStructMatchConfig*)inDataStruct;

	// Obtener la información y actualizar la configuración del match que manda el servidor:
	// StructMatchConfig.id		: id del campo a cambiar
	// StructMatchConfig.value	: nuevo valor del campo id


	switch (StructMatchConfig->id) {
	case 0:
		MatchConfigManager->Set_MissionType(static_cast<MATCHCONFIG_MISSIONTYPE>(StructMatchConfig->intvalue));
		break;
	case 1:
		MatchConfigManager->Set_MapSize(static_cast<MATCHCONFIG_MAPSIZE>(StructMatchConfig->intvalue));
		break;
	case 2:
		MatchConfigManager->Set_Difficulty(static_cast<MATCHCONFIG_DIFFICULTY>(StructMatchConfig->intvalue));
		break;
	}

}
void UPD_ClientGameInstance::HandleEvent_ConfigMatchDone(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {
	UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_ConfigMatchDone"));

	FStructMatchConfigDone* configDone = (FStructMatchConfigDone*)inDataStruct;

	// Si el ConfigMatchDone viene del server, vamonos al lobby
	if (configDone->from == 0) {
		structClientState->ConfigMatchDone = true;
		this->UpdateState();//Cambio de estado al ConfigureCharacter
	}
}

void UPD_ClientGameInstance::HandleEvent_MapIncoming(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {
	UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_MapIncoming"));
	FStructMap* mapStruct = (FStructMap*)inDataStruct;
	structClientState->mapString = mapStruct->stringMap;


	//UE_LOG(LogTemp, Log, TEXT("map=%s"), (mapStruct->stringMap));
	//this->UpdateState();
}

void UPD_ClientGameInstance::HandleEvent_FStructMapDataIncoming(FStructGeneric * inDataStruct, int inPlayer, UStructType inEventType)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_FStructMapDataIncoming"));
	UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_FStructMapDataIncoming %s"), *((FStructMapData*)inDataStruct)->PARSER_VERSION);

	*structClientState->NETMAPDATA = *(FStructMapData*)inDataStruct;
	UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_FStructMapDataIncoming : Final"));

}

void UPD_ClientGameInstance::HandleEvent_LaunchMatchFromServer(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {
	UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_LaunchMatchFromServer"));
	
	

	structClientState->ConfigAllCharactersDone = true;
	this->UpdateState();
	
}


void UPD_ClientGameInstance::HandleEvent_AllCharactersInfoIncoming(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {

	FStructInstatiatePlayers* InstaPlayers = (FStructInstatiatePlayers*)inDataStruct;
	UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_AllCharactersInfoIncoming: Num players:%d, %d"), InstaPlayers->listInfoPlayerAtClient.Num(), InstaPlayers->listInfoPlayerAtClient[0].logicPosition.positionX);
	//Seteamos el id del propio character del cliente para luego poder diferenciarlo
	playersManager->GetMyCharacter()->SetIDCharacter(InstaPlayers->idClientCharacter);

	for (int i = 0; i < InstaPlayers->listInfoPlayerAtClient.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_AllCharactersInfoIncoming: Num player:%s "), *InstaPlayers->listInfoPlayerAtClient[i].ID_character);

		playersManager->AddNewPlayer(InstaPlayers->listInfoPlayerAtClient[i]);
	}
	structClientState->AllCharactersIncoming = true;
	this->UpdateState();
}

void UPD_ClientGameInstance::HandleEvent_StartMatch_GoGameMnager(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {

	this->UpdateState();
}

#pragma endregion


#pragma endregion








#pragma region UPDATE & CHANGE & ON BEGIN STATE

void UPD_ClientGameInstance::ChangeState(EClientState newState) {

	switch (newState)
	{
	case EClientState::StartApp:
		UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::ChangeState to StartApp"));
		break;
	case EClientState::Game_NoConnection:
		UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::ChangeState to NoConnection"));
		break;
	case EClientState::ConfigureMatch:
		UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::ChangeState to ConfigureMatch"));
		break;
	case EClientState::WaitingMatchConfiguration:
		UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::ChangeState to WaitingMatchConfiguration"));
		break;
	case EClientState::Lobby_Tabern:
		UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::ChangeState to Lobby_Tabern"));
		break;
	case EClientState::ReconnectingInGame:
		UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::ChangeState to ReconnectingInGame"));
		break;
	case EClientState::Launch_Match:
		UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::ChangeState to Launch_Match"));
		break;
	case EClientState::GameInProcess:
		UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::ChangeState to GameInProcess"));
		break;

	default:
		break;
	}

	structClientState->enumClientState = newState;
	OnBeginState();
}


void UPD_ClientGameInstance::OnBeginState() {

	if (structClientState->enumClientState == EClientState::StartApp) {

		ChangeState(EClientState::Game_NoConnection);
	}
	else if (structClientState->enumClientState == EClientState::Game_NoConnection) {

	}
	else if (structClientState->enumClientState == EClientState::ConfigureMatch) {
		this->LoadMap(levelsNameDictionary.GetMapName(2));
	}
	else if (structClientState->enumClientState == EClientState::WaitingMatchConfiguration) {
		this->LoadMap(levelsNameDictionary.GetMapName(2));
	}
	else if (structClientState->enumClientState == EClientState::Lobby_Tabern) {
		this->LoadMap(levelsNameDictionary.GetMapName(3));
	}
	else if (structClientState->enumClientState == EClientState::ReconnectingInGame) {
		structClientState->reconnected = true;
	}
	else if (structClientState->enumClientState == EClientState::Launch_Match) {
		//mapParser = new PD_MG_MapParser();

		//PD_MG_StaticMap* staticMapRef = new PD_MG_StaticMap();
		//PD_MG_DynamicMap* dynamicMapRef = new PD_MG_DynamicMap();

		//// Parsea el chorizo
		//mapParser->StartParsingFromChorizo(&structClientState->mapString, staticMapRef, dynamicMapRef);
		//mapManager = new PD_GM_MapManager();
		//mapManager->Init(staticMapRef, dynamicMapRef);


		mapParser = new PD_MG_MapParser();

		PD_MG_StaticMap* staticMapRef = new PD_MG_StaticMap();
		PD_MG_DynamicMap* dynamicMapRef = new PD_MG_DynamicMap();

		// Parsea el chorizo
		//mapParser->StartParsingFromFile(&mapPath, staticMapRef, dynamicMapRef);
		//mapParser->StartParsingFromChorizo(&mapPath, staticMapRef, dynamicMapRef);

		mapManager = new PD_GM_MapManager();
		mapManager->Init();
		mapManager->MapInfo->NETMAPDATA = structClientState->NETMAPDATA;
		//UE_LOG(LogTemp, Log, TEXT("PD_MM_MapInfo::ShowMapData - MISSION_TYPE %d "), structClientState->NETMAPDATA->enemyComposition.Num());

		mapManager->MapInfo->ShowMapData();
		mapParser->StartParsingFromFStructMapData(structClientState->NETMAPDATA, mapManager->MapInfo, mapManager->DynamicMapRef);

	}
	else if (structClientState->enumClientState == EClientState::GameInProcess) {

		this->LoadMap(levelsNameDictionary.GetMapName(4));//Mapa de juego
	}
	else {}



}



void UPD_ClientGameInstance::UpdateState() {

	if (structClientState->enumClientState == EClientState::StartApp) {



	}
	else if (structClientState->enumClientState == EClientState::Game_NoConnection) {
		//Transiciones de estados
		if (!structClientState->clientMaster && structClientState->numPlayer != -1) {
			this->ChangeState(EClientState::WaitingMatchConfiguration);
		}
		else if (structClientState->clientMaster && structClientState->numPlayer != -1) {
			this->ChangeState(EClientState::ConfigureMatch);

		}
	}
	else if (structClientState->enumClientState == EClientState::ConfigureMatch) {
		//Transiciones de estados
		if (structClientState->ConfigMatchDone) {
			this->ChangeState(EClientState::Lobby_Tabern);
		}
	}
	else if (structClientState->enumClientState == EClientState::WaitingMatchConfiguration) {
		//Transiciones de estados
		if (structClientState->ConfigMatchDone) {
			this->ChangeState(EClientState::Lobby_Tabern);
		}
	}
	else if (structClientState->enumClientState == EClientState::Lobby_Tabern) {
		//Agregada condicion de que el stringMap este inicializado para pasar de estado
		if (structClientState->ConfigAllCharactersDone /*&& !structClientState->mapString.IsEmpty()*/) {
			this->ChangeState(EClientState::Launch_Match);
		}
//		else if (structClientState->mapString.IsEmpty()) {
//			UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::UpdateState: Intentando hacer LAUNCH MATCH - ERROR: No se encuentra el mapString"));
//		}
	}
	else if (structClientState->enumClientState == EClientState::ReconnectingInGame) {
		//Agregada condicion de que el stringMap este inicializado para pasar de estado
		if (structClientState->ConfigAllCharactersDone /*&& !structClientState->mapString.IsEmpty()*/) {
			this->ChangeState(EClientState::Launch_Match);
		}
//		else if (structClientState->mapString.IsEmpty()) {
//			UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::UpdateState: Intentando hacer LAUNCH MATCH - ERROR: No se encuentra el mapString"))
//		}
	}
	else if (structClientState->enumClientState == EClientState::Launch_Match) {
		
		if (structClientState->AllCharactersIncoming ) {
			this->ChangeState(EClientState::GameInProcess);
		}
		
	}
	else if (structClientState->enumClientState == EClientState::GameInProcess) {

	}
	else {}

}



#pragma endregion


void UPD_ClientGameInstance::LoadMap(FString mapName)
{
	UGameplayStatics::OpenLevel((UObject*)this, FName(*mapName));
}

//Callback cuando el mapa este cargado
void UPD_ClientGameInstance::OnLoadedLevel() {
	//Sin importar el estado hacemos:
	APD_NW_TimerActor* TimerActorSpawned = (APD_NW_TimerActor*)GetWorld()->SpawnActor(APD_NW_TimerActor::StaticClass());
	networkManager->GetSocketManager()->InitTimerActor(TimerActorSpawned);

	if (structClientState->enumClientState == EClientState::Lobby_Tabern) {

	}


	if (structClientState->enumClientState == EClientState::GameInProcess) {
		//Quizas esto es tarea del gameManager.

		// le pasamos al mapManager un instanciador
		AMapInstantiatorActor* InstantiatorActor = (AMapInstantiatorActor*)GetWorld()->SpawnActor(AMapInstantiatorActor::StaticClass());
		MapManagerAccesor = (AMapManagerAccesor*)GetWorld()->SpawnActor(AMapManagerAccesor::StaticClass());
		PlayersManagerAccesor = (APlayerManagerAccesor*)GetWorld()->SpawnActor(APlayerManagerAccesor::StaticClass());

		mapManager->instantiator = InstantiatorActor;
		MapManagerAccesor->mapManager = mapManager;
		PlayersManagerAccesor->playersManager = playersManager;

		//Aqui cedemos el control al GameManager.


		if (structClientState->reconnected) {
			UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::OnLoadedLevel: creando GameManager en modo de reconexion"));

			//Esto hace que en caso de reconexion pasemos directamente a esperar un update
			gameManager = new PD_GM_GameManager(this, mapManager, playersManager, networkManager, EClientGameState::WaitingServer);

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::OnLoadedLevel: creando GameManager en modo normal"));

			gameManager = new PD_GM_GameManager(this, mapManager, playersManager, networkManager);

		}


	}
}

void UPD_ClientGameInstance::SendToServerMatchConfigUpdate(int id, int intvalue, FString FStringvalue)
{
	FStructMatchConfig msg = FStructMatchConfig();

	msg.id = id;
	msg.intvalue = intvalue;
	msg.FStringvalue = FStringvalue;

	networkManager->SendNow(&msg, 0);

}

void UPD_ClientGameInstance::SendToServerEndMatchConfig()
{
	FStructMatchConfigDone msg = FStructMatchConfigDone();
	msg.from = 1;
	networkManager->SendNow(&msg, 0);
}


#pragma region ShutDown


void UPD_ClientGameInstance::Shutdown()
{
	Super::Shutdown();

	if (networkManager) {
		delete networkManager;
	}
	//Aqui habria que hacer muchos deletes 

}


#pragma endregion






/*********************************
****** FUNCIONES BP / UTILIDAD
*********************************/

#pragma region BP FUNCTIONS

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

// Esta funcion YA NO SE USA, se usa el SendToServerEndMatchConfig() para enviar la info
void UPD_ClientGameInstance::GoToLobby()
{
	/*
	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::GoToLobby()."));
	FStructOrderMenu respuesta = FStructOrderMenu();
	respuesta.orderType = static_cast<uint8>(MenuOrderType::GameConfigurationDone);
	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance:: Enviando: 2 - GameConfigurationDone"));
	networkManager->SendNow(&respuesta, 0);

	*/
	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::GoToLobby()."));
	FStructMatchConfigDone configDone = FStructMatchConfigDone();
	configDone.from = 1;
	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance:: Enviando: 2 - GameConfigurationDone"));
	bool successSend = networkManager->SendNow(&configDone, 0);
	if (!successSend) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("No se ha enviado bien la informacion al servidor o error de conexion")));
			UE_LOG(LogTemp, Warning, TEXT("No se ha enviado bien la informacion al servidor o error de conexion"));
			networkManager->SendPingToServer();
	}
}


bool UPD_ClientGameInstance::GetReadyToParty()
{
	// Comprobamos APTotal para comprobar si el usuario ha metido valores o no. Sera distinto de 0 si hubiera un personaje seleccionado.
	//Hasta que vea otra forma de ver si el Jugador ha elegido personaje o no
	if (playersManager->MyPlayerInfo->isSetPlayerCharacter) {

		FStructOrderMenu respuesta = FStructOrderMenu();
		respuesta.orderType = respuesta.orderType = static_cast<uint8>(MenuOrderType::ClientReady);
		UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance:: Enviando: 4 - ClientReady"));
		bool successSend = networkManager->SendNow(&respuesta, 0);
		if (!successSend) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("No se ha enviado bien la informacion al servidor o error de conexion")));
			UE_LOG(LogTemp, Warning, TEXT("No se ha enviado bien la informacion al servidor o error de conexion"));
			networkManager->SendPingToServer();
		}
		playersManager->MyPlayerInfo->readyMenu = !playersManager->MyPlayerInfo->readyMenu;
		return playersManager->MyPlayerInfo->readyMenu;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "ERROR!!  Tienes que ACEPTAR y ENVIAR tu personaje antes del READY !");

	}
	return false;
}

void UPD_ClientGameInstance::FillCharecterStats(int nPOD, int nAGI, int nDES, int nCON, int nPER, int nMAL)
{
	playersManager->MyPlayerInfo->logic_Character->SetBasicStats(nPOD, nAGI, nDES, nCON, nPER, nMAL);
	playersManager->MyPlayerInfo->logic_Character->SetInitBaseStats(100, 20, 5); //HP - DMG
	playersManager->MyPlayerInfo->logic_Character->SetWeapon();
	playersManager->MyPlayerInfo->logic_Character->SetTotalStats();
}

void UPD_ClientGameInstance::GetCharacterBasicStats(int &nPOD, int &nAGI, int &nDES, int &nCON, int &nPER, int &nMAL)
{
	nPOD = playersManager->MyPlayerInfo->logic_Character->GetBasicStats()->POD;
	nAGI = playersManager->MyPlayerInfo->logic_Character->GetBasicStats()->AGI;
	nDES = playersManager->MyPlayerInfo->logic_Character->GetBasicStats()->DES;
	nCON = playersManager->MyPlayerInfo->logic_Character->GetBasicStats()->CON;
	nPER = playersManager->MyPlayerInfo->logic_Character->GetBasicStats()->PER;
	nMAL = playersManager->MyPlayerInfo->logic_Character->GetBasicStats()->MAL;
}

void UPD_ClientGameInstance::GetCharacterTotalStats(int &nAP, int &nCH, int &nSA, int &nHP)
{
	nAP = playersManager->MyPlayerInfo->logic_Character->GetTotalStats()->APTotal;
	nCH = playersManager->MyPlayerInfo->logic_Character->GetTotalStats()->CH;
	nSA = playersManager->MyPlayerInfo->logic_Character->GetTotalStats()->SA;
	nHP = playersManager->MyPlayerInfo->logic_Character->GetTotalStats()->HPTotal;
}

bool UPD_ClientGameInstance::SendCharacterToServer()
{
	if (!playersManager->MyPlayerInfo->logic_Character) {
		return false;
	}


	if (playersManager->MyPlayerInfo->logic_Character->GetTotalStats()->APTotal == 205) //Comprobacion sobre un valor para determinar si hay Status o no
	{
		UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance:: rellenando datos"));
		//FillCharecterStats(100,100,100,100,100,100);
		GenerateRandomChar();
	}

	FStructCharacter structCharacterToSend = FStructCharacter();
	structCharacterToSend.totalStats = *(playersManager->MyPlayerInfo->logic_Character->GetTotalStats());
	structCharacterToSend.initBaseStats = *(playersManager->MyPlayerInfo->logic_Character->GetInitBaseStats());
	structCharacterToSend.skills = *(playersManager->MyPlayerInfo->logic_Character->GetSkills());
	structCharacterToSend.skin = *(playersManager->MyPlayerInfo->logic_Character->GetSkin());
	structCharacterToSend.weapon = *(playersManager->MyPlayerInfo->logic_Character->GetWeapon());
	structCharacterToSend.charState = *(playersManager->MyPlayerInfo->logic_Character->GetCharacterState());

	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance:: Enviando: Character Stats and Data"));
	bool successSend = networkManager->SendNow(&structCharacterToSend, 0);
	if (!successSend) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("No se ha enviado bien la informacion al servidor o error de conexion")));
		UE_LOG(LogTemp, Warning, TEXT("No se ha enviado bien la informacion al servidor o error de conexion"));
		networkManager->SendPingToServer();
	}
	playersManager->MyPlayerInfo->isSetPlayerCharacter = true;
	return true;
}

void UPD_ClientGameInstance::SendToServerMatchConfigUpdateBP(int id, int intvalue, FString FStringvalue)
{
	SendToServerMatchConfigUpdate(id, intvalue, FStringvalue);
}

void UPD_ClientGameInstance::SendToServerEndMatchConfigBP()
{
	SendToServerEndMatchConfig();
}

/*
bool UPD_ClientGameInstance::SendTurnOrderActionsToServer()
{
	if (!playersManager->MyPlayerInfo->turnOrders) {
		return false;
	}

	FStructTurnOrders turnsOrdersToSend = FStructTurnOrders();
	turnsOrdersToSend = *(playersManager->MyPlayerInfo->turnOrders);

	bool sentOk = networkManager->SendNow(&turnsOrdersToSend, 0);

	if (sentOk)  //Si se ha enviado bien el paquete - Vaciar el PlayersInfo->turnOrders y return true
	{
		playersManager->MyPlayerInfo->turnOrders = new FStructTurnOrders();
		gameManager->structGameState->enumGameState = EClientGameState::WaitingServer;
	}

	return sentOk;
}*/

void UPD_ClientGameInstance::SetTypeOfAction(int ntypeAction)
{
	playersManager->MyPlayerInfo->typeOfAction = ntypeAction;
}

uint8 UPD_ClientGameInstance::GetTypeOfAction()
{
	return playersManager->MyPlayerInfo->typeOfAction;
}

void UPD_ClientGameInstance::ChangePhase(bool next)
{
	gameManager->ChangePhase(next);
}

uint8 UPD_ClientGameInstance::GetGameMngStatus()
{
	uint8 GameStatusInt = 10;
	switch (gameManager->structGameState->enumGameState)
	{
	case EClientGameState::Instantiate_Map:
	{
		GameStatusInt = static_cast<uint8>(EClientGameState::Instantiate_Map);
		break;
	}
	case EClientGameState::Start_Match:
	{
		GameStatusInt = static_cast<uint8>(EClientGameState::Start_Match);
		break;
	}
	case EClientGameState::GenerateOrders_Start:
	{
		GameStatusInt = static_cast<uint8>(EClientGameState::GenerateOrders_Start);
		break; }

	case EClientGameState::GenerateOrders_ConsumablePhase:
	{
		GameStatusInt = static_cast<uint8>(EClientGameState::GenerateOrders_ConsumablePhase);
		break; }

	case EClientGameState::GenerateOrders_MovementPhase:
	{
		GameStatusInt = static_cast<uint8>(EClientGameState::GenerateOrders_MovementPhase);
		break; }

	case EClientGameState::GenerateOrders_InteractionPhase:
	{
		GameStatusInt = static_cast<uint8>(EClientGameState::GenerateOrders_InteractionPhase);
		break; }

	case EClientGameState::GenerateOrders_ActionPhase:
	{
		GameStatusInt = static_cast<uint8>(EClientGameState::GenerateOrders_ActionPhase);
		break; }

	case EClientGameState::GenerateOrders_Validate:
	{
		GameStatusInt = static_cast<uint8>(EClientGameState::GenerateOrders_Validate);
		break; }

	case EClientGameState::SendOrdersToServer:
	{
		GameStatusInt = static_cast<uint8>(EClientGameState::SendOrdersToServer);
		break;
	}
	case EClientGameState::WaitingServer:
	{
		GameStatusInt = static_cast<uint8>(EClientGameState::WaitingServer);
		break;
	}
	case EClientGameState::UpdateInfo:
	{
		GameStatusInt = static_cast<uint8>(EClientGameState::UpdateInfo);
		break;
	}
	case EClientGameState::EndOfTurn:
	{
		GameStatusInt = static_cast<uint8>(EClientGameState::EndOfTurn);
		break;
	}
	}

	return GameStatusInt;
}


EClientGameState UPD_ClientGameInstance::GetGameState()
{
	return gameManager->structGameState->enumGameState;
}

uint8 UPD_ClientGameInstance::GetPlayerNumber()
{
	return structClientState->numPlayer;
}




AMapManagerAccesor* UPD_ClientGameInstance::GetMapManagerAccessor(bool& existe) {

	if (MapManagerAccesor != nullptr) {
		existe = true;
		return MapManagerAccesor;
	}
	else {
		existe = false;
		return nullptr;
	}

}

APlayerManagerAccesor* UPD_ClientGameInstance::GetPlayersManagerAccessor(bool& existe) {

	if (PlayersManagerAccesor != nullptr) {
		existe = true;
		return PlayersManagerAccesor;
	}
	else {
		existe = false;
		return nullptr;
	}

}



APD_E_Character*  UPD_ClientGameInstance::GetCharacterPlayerAtPosition(FVector position, bool& existe) {
	PD_MG_LogicPosition lp = mapManager->WorldToLogicPosition(position);
	for (int i = 0; i < playersManager->GetDataPlayers().Num(); i++) {
		if (lp == playersManager->GetDataPlayers()[i]->logic_Character->GetCurrentLogicalPosition()) {
			existe = true;
			return playersManager->GetDataPlayers()[i]->logic_Character->GetCharacterBP();
		}
	}
	existe = false;
	return nullptr;
}


///CREAR PERSONAJE ALEATORIO
void UPD_ClientGameInstance::GenerateRandomChar()
{
	UE_LOG(LogTemp, Warning, TEXT("UPD_ClientGameInstance::GenerateRandomChar"));

	//Weapon - Clase y Tipo
	
	int64 DateInSeconds = FDateTime::Now().ToUnixTimestamp();
	FRandomStream SRand = FRandomStream();
	SRand.Initialize(DateInSeconds);
	int weaponSelected = SRand.RandRange(0, weapons.Num() - 1);
	//int weaponSelected = FMath::RandRange(0, weapons.Num() -1);

	//int weaponSelected = rand() % 9;//weapons.Num();

	FStructWeapon weaponChar = LoadWeaponStructData(weaponSelected);
	UE_LOG(LogTemp, Warning, TEXT("UPD_ClientGameInstance::GenerateRandomChar weapon totales - %d "), weapons.Num());
	UE_LOG(LogTemp, Warning, TEXT("UPD_ClientGameInstance::GenerateRandomChar weapon selected - %d "), weaponSelected);

	//FStructWeapon weaponChar = LoadWeaponStructData(8);
	if (weaponChar.ID_Weapon >= 0)
	{
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->ID_Weapon = weaponChar.ID_Weapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->ClassWeapon = weaponChar.ClassWeapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->TypeWeapon = weaponChar.TypeWeapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->RangeWeapon = weaponChar.RangeWeapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->DMWeapon = weaponChar.DMWeapon;
	}

	UE_LOG(LogTemp, Warning, TEXT("UPD_ClientGameInstance::GenerateRandomChar weapon selected - %d "), playersManager->MyPlayerInfo->logic_Character->GetWeapon()->TypeWeapon);

	//Skin
	switch (playersManager->MyPlayerInfo->logic_Character->GetWeapon()->TypeWeapon)
	{
	case 11:
		playersManager->MyPlayerInfo->logic_Character->GetSkin()->ID_SkinHead = 0;
		break;
	case 12:
		playersManager->MyPlayerInfo->logic_Character->GetSkin()->ID_SkinHead = 0;
		break;
	case 13:
		playersManager->MyPlayerInfo->logic_Character->GetSkin()->ID_SkinHead = 0;
		break;
	case 21:
		playersManager->MyPlayerInfo->logic_Character->GetSkin()->ID_SkinHead = 1;
		break;
	case 22:
		playersManager->MyPlayerInfo->logic_Character->GetSkin()->ID_SkinHead = 1;
		break;
	case 23:
		playersManager->MyPlayerInfo->logic_Character->GetSkin()->ID_SkinHead = 2;
		break;
	case 31:
		playersManager->MyPlayerInfo->logic_Character->GetSkin()->ID_SkinHead = 3;
		break;
	case 32:
		playersManager->MyPlayerInfo->logic_Character->GetSkin()->ID_SkinHead = 3;
		break;
	case 33:
		playersManager->MyPlayerInfo->logic_Character->GetSkin()->ID_SkinHead = 3;
		break;
	default:
		break;
	}

	//playersManager->MyPlayerInfo->logic_Character->GetSkin()->ID_SkinHead = 0;

	playersManager->MyPlayerInfo->logic_Character->GetSkin()->weapon_class = playersManager->MyPlayerInfo->logic_Character->GetWeapon()->ClassWeapon;
	playersManager->MyPlayerInfo->logic_Character->GetSkin()->weapon_type = playersManager->MyPlayerInfo->logic_Character->GetWeapon()->TypeWeapon;

	UE_LOG(LogTemp, Warning, TEXT("UPD_ClientGameInstance::GenerateRandomChar skin selected - %d "), playersManager->MyPlayerInfo->logic_Character->GetSkin()->ID_SkinHead);

	//HABILIDADES
	switch (playersManager->MyPlayerInfo->logic_Character->GetWeapon()->TypeWeapon)
	{
	case  11: //espada y escudo
	{
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1,0));

		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 4));
		break;
	}
	case  12: //dagas
	{
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1, 1));

		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 2));
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 4));
		break;
	}
	case  13: //mandoble
	{
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1, 2));

		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 3));
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 4));
		break;
	}
	case  21: //ballesta y escudo
	{
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1, 3));

		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 5));
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 6));
		break;
	}
	case  22: //pistolas
	{
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1, 4));

		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 5));
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 6));
		break;
	}
	case  23: //arco
	{
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1, 5));

		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 5));
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 6));
		break;
	}
	case  31: //varita y escudo
	{
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1, 6));
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1, 12));

		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 10));
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 7));
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 9));
		break;
	}
	case  32: //cetro y tomo arcano
	{
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1, 7));
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1, 10));

		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 7));
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 9));
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 10));
		break;
	}
	case  33: //baston
	{
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1, 8));
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1, 9));

		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 7));
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 9));
		playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 10));
		break;
	}
	default:
		break;
	}
	
	//STATS
	playersManager->MyPlayerInfo->logic_Character->SetInitBaseStats(200, 20, 10); //volver a cambiar a 5 AP base

	playersManager->MyPlayerInfo->logic_Character->SetBasicStats(5,5 ,5, 5, 6 ,6 );
	if (playersManager->MyPlayerInfo->logic_Character->GetSkin()->weapon_type / 10 == 1 ) //si es a melee
		playersManager->MyPlayerInfo->logic_Character->SetBasicStats(7, 5, 5, 7, 7, 6);

	playersManager->MyPlayerInfo->logic_Character->SetTotalStats();
}

///MELEE CHARACTER
void UPD_ClientGameInstance::GenerateMeleeChar()
{
	///WEAPON
	FStructWeapon weaponChar = LoadWeaponStructData(0);

	if (weaponChar.ID_Weapon >= 0)
	{
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->ID_Weapon = weaponChar.ID_Weapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->ClassWeapon = weaponChar.ClassWeapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->TypeWeapon = weaponChar.TypeWeapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->RangeWeapon = weaponChar.RangeWeapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->DMWeapon = weaponChar.DMWeapon;
	}
	///SKIN
	playersManager->MyPlayerInfo->logic_Character->GetSkin()->ID_SkinHead = 0; //espada (y escudo) ?

	///SKILLS
	//PAS
	playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1, 0));

	//ACT
	playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 2));
	playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 4));
	
	///STATS
	playersManager->MyPlayerInfo->logic_Character->SetInitBaseStats(200, 20, 10); //volver a cambiar a 5 AP base
	playersManager->MyPlayerInfo->logic_Character->SetBasicStats(7, 5, 5, 7, 7, 6);
	playersManager->MyPlayerInfo->logic_Character->SetTotalStats();
}

///RANGO CHARACTER
void UPD_ClientGameInstance::GenerateRangedChar()
{
	///WEAPON
	FStructWeapon weaponChar = LoadWeaponStructData(3);

	if (weaponChar.ID_Weapon >= 0)
	{
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->ID_Weapon = weaponChar.ID_Weapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->ClassWeapon = weaponChar.ClassWeapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->TypeWeapon = weaponChar.TypeWeapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->RangeWeapon = weaponChar.RangeWeapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->DMWeapon = weaponChar.DMWeapon;
	}
	///SKIN
	playersManager->MyPlayerInfo->logic_Character->GetSkin()->ID_SkinHead = 2; //ballesta

	///SKILLS
	//PAS
	playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1, 4));
	playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1, 5));

	//ACT
	playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 5));
	playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 6));

	///STATS
	playersManager->MyPlayerInfo->logic_Character->SetInitBaseStats(200, 20, 10); //volver a cambiar a 5 AP base
	playersManager->MyPlayerInfo->logic_Character->SetBasicStats(5, 5, 5, 5, 6, 6);
	playersManager->MyPlayerInfo->logic_Character->SetTotalStats();
}

void UPD_ClientGameInstance::GeneratedMagicChar()
{
	///WEAPON
	FStructWeapon weaponChar = LoadWeaponStructData(8);

	if (weaponChar.ID_Weapon >= 0)
	{
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->ID_Weapon = weaponChar.ID_Weapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->ClassWeapon = weaponChar.ClassWeapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->TypeWeapon = weaponChar.TypeWeapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->RangeWeapon = weaponChar.RangeWeapon;
		playersManager->MyPlayerInfo->logic_Character->GetWeapon()->DMWeapon = weaponChar.DMWeapon;
	}
	///SKIN
	playersManager->MyPlayerInfo->logic_Character->GetSkin()->ID_SkinHead = 3; //Mago - Baston

	///SKILLS
    //PAS
	playersManager->MyPlayerInfo->logic_Character->GetSkills()->listPasiveSkills.Add(LoadSkillStructData(1, 8));

	//ACT
	playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 7));
	playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 9));
	playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Add(LoadSkillStructData(0, 10));

	///STATS
	playersManager->MyPlayerInfo->logic_Character->SetInitBaseStats(200, 20, 10); //volver a cambiar a 5 AP base
	playersManager->MyPlayerInfo->logic_Character->SetBasicStats(5, 5, 5, 5, 6, 6);
	playersManager->MyPlayerInfo->logic_Character->SetTotalStats();
}



void UPD_ClientGameInstance::GivePlayerStatToHUD(float &HP, int &Score, int &AP, FString &player_id)
{
	//EL HP se tiene que dar en forma de porcentaje de 1 a 0
	float life = 0.0f;

	life = (playersManager->MyPlayerInfo->logic_Character->GetTotalStats()->HPCurrent) / (double)playersManager->MyPlayerInfo->logic_Character->GetTotalStats()->HPTotal;
	HP = life;
	//HP = playersManager->MyPlayerInfo->logic_Character->GetTotalStats()->HPCurrent;
	AP = playersManager->MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent;
	player_id = playersManager->MyPlayerInfo->logic_Character->GetIDCharacter();
	Score = playersManager->MyPlayerInfo->logic_Character->GetTotalStats()->PointsCurrent;
	//playersManager->MyPlayerInfo->logic_Character->GetPoints(); /// ¿Donde esta el score del jugfador?
}

void UPD_ClientGameInstance::GiveAPForTurnPlayer(int &APfinal)
{
	APfinal = playersManager->MyPlayerInfo->logic_Character->GetTotalStats()->APTotal - playersManager->MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent;
}


void UPD_ClientGameInstance::GiveSkillsPickOnTurn(TArray<int> &id_skills, TArray<FString> &name_skills)
{
	for (int i = 0; i < playersManager->MyPlayerInfo->turnOrders->actions.Num(); i++)
	{
		for (int j = 0; j < activeSkills.Num(); j++)
		{
			if (activeSkills[j].ID_Skill == playersManager->MyPlayerInfo->turnOrders->actions[i].id_action)
			{
				id_skills.Add(activeSkills[j].ID_Skill);
				name_skills.Add(activeSkills[j].name_Skill);
			}
		}
	}
}

void UPD_ClientGameInstance::FillEnemiesOnRangeForSkill(int ID_Skill, TArray<FString> &ID_Enemy, TArray<FString> &TypeEnemy)
{
	int skillRange = 0;

	switch (ActiveSkills(ID_Skill))
	{
	case ActiveSkills::BasicAttack:
	{
		skillRange = playersManager->MyPlayerInfo->logic_Character->GetWeapon()->RangeWeapon;
		UE_LOG(LogTemp, Warning, TEXT("UPD_ClientGameInstance::FillEnemiesOnRangeForSkill baisc wth range - %d "), skillRange);
		break;
	}
	default:
		break;
	}

	for (int i = 0; i < playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Num(); i++)
	{
		if (playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills[i].ID_Skill == ID_Skill)
		{
			skillRange = playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills[i].range;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UPD_ClientGameInstance::FillEnemiesOnRangeForSkill Nose ha encontrado skill player con ID - %d "), ID_Skill);
		}
	}

	if (skillRange > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPD_ClientGameInstance::FillEnemiesOnRangeForSkill  skill con rango:  - %d "), skillRange);

		//Si hay rango - la habilidad la posee el jugador - buscar todas las tiles adyacentes a la posicion del jugador despues de la fase de movimiento dentro del rango
		TArray<PD_MG_LogicPosition> tilesInRangeOfSkill = TArray<PD_MG_LogicPosition>();
		PD_MG_LogicPosition futurePosition;
		if (playersManager->MyPlayerInfo->turnOrders->positionsToMove.Num() > 0)
		{
			futurePosition = PD_MG_LogicPosition(playersManager->MyPlayerInfo->turnOrders->positionsToMove.Last().positionX,
				playersManager->MyPlayerInfo->turnOrders->positionsToMove.Last().positionY);
		}
		else {
			futurePosition = playersManager->MyPlayerInfo->logic_Character->GetCurrentLogicalPosition();
		}

		tilesInRangeOfSkill = mapManager->GetAllTilesInRange((float)skillRange, futurePosition);

		if (tilesInRangeOfSkill.Num() > 0) //si hay tiles y se ha hecho bien la funcion de rango
		{
			UE_LOG(LogTemp, Warning, TEXT("UPD_ClientGameInstance::FillEnemiesOnRangeForSkill hay tiles con ese rango ! - %d "), tilesInRangeOfSkill.Num());

			//Coge los enemigos que tengan una CurrentLogicPosition = a las que hemos conseguido anteriormente
			for (int j = 0; j < gameManager->enemyManager->GetEnemies().Num(); j++)
			{
				if (tilesInRangeOfSkill.Contains(gameManager->enemyManager->GetEnemies()[j]->GetCurrentLogicalPosition()))
				{

					FHitResult hit;
					FVector iniPos = mapManager->LogicToWorldPosition(futurePosition);
					iniPos.Z = 50;
					FVector endPos = mapManager->LogicToWorldPosition(gameManager->enemyManager->GetEnemies()[j]->GetCurrentLogicalPosition());
					endPos.Z = 50;

					FCollisionQueryParams paramsRay = FCollisionQueryParams();
					paramsRay.AddIgnoredActor(playersManager->MyPlayerInfo->logic_Character->GetCharacterBP());
					GetWorld()->LineTraceSingleByChannel(hit, iniPos, endPos, ECollisionChannel::ECC_Camera, paramsRay);

					if (hit.GetActor() == gameManager->enemyManager->GetEnemies()[j]->GetCharacterBP()) {
						ID_Enemy.Add(gameManager->enemyManager->GetEnemies()[j]->GetIDCharacter());
						switch (ECharacterType(gameManager->enemyManager->GetEnemies()[j]->GetTypeCharacter()))
						{
						case ECharacterType::OrcBow:
						{
							TypeEnemy.Add("Archer Orc");
							break;
						}
						case ECharacterType::OrcGuns:
						{
							TypeEnemy.Add("Guns Orc");
							break;
						}
						case ECharacterType::OrcMelee:
						{
							TypeEnemy.Add("Melee Orc");
							break;
						}
						default:
							break;
						}
					}

				}
			}
		}
	}
}


void UPD_ClientGameInstance::FillPlayersOnRangeForSkill(int ID_Skill, TArray<FString> &ID_Player, TArray<int> &TypePlayer)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("UPD_ClientGameInstance::FillPlayersOnRangeForSkill::  ID SKILL:%d"), ID_Skill));

	int skillRange = 0;

	for (int i = 0; i < playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Num(); i++)
	{
		if (playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills[i].ID_Skill == ID_Skill)
		{
			skillRange = playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills[i].range;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UPD_ClientGameInstance::FillPlayersOnRangeForSkill Nose ha encontrado skill player con ID - %d "), ID_Skill);
		}
	}

	if (skillRange > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPD_ClientGameInstance::FillEnemiesOnRangeForSkill  skill con rango:  - %d "), skillRange);

		//Si hay rango - la habilidad la posee el jugador - buscar todas las tiles adyacentes a la posicion del jugador despues de la fase de movimiento dentro del rango
		TArray<PD_MG_LogicPosition> tilesInRangeOfSkill = TArray<PD_MG_LogicPosition>();
		if (playersManager->MyPlayerInfo->turnOrders->positionsToMove.Num() > 0)
		{
			tilesInRangeOfSkill = mapManager->GetAllTilesInRange((float)skillRange, PD_MG_LogicPosition(playersManager->MyPlayerInfo->turnOrders->positionsToMove.Last().positionX,
				playersManager->MyPlayerInfo->turnOrders->positionsToMove.Last().positionY));
		}
		else {
			tilesInRangeOfSkill = mapManager->GetAllTilesInRange((float)skillRange, playersManager->MyPlayerInfo->logic_Character->GetCurrentLogicalPosition());
		}

		if (tilesInRangeOfSkill.Num() > 0) //si hay tiles y se ha hecho bien la funcion de rango
		{
			UE_LOG(LogTemp, Warning, TEXT("UPD_ClientGameInstance::FillEnemiesOnRangeForSkill hay tiles con ese rango ! - %d "), tilesInRangeOfSkill.Num());

			//Coge los enemigos que tengan una CurrentLogicPosition = a las que hemos conseguido anteriormente
			for (int j = 0; j < playersManager->GetNumPlayers(); j++)
			{
				PD_MG_LogicPosition posOtherPlayer;

				if (playersManager->GetDataStructPlayer(j)->turnOrders->positionsToMove.Num() > 0)
				{
					posOtherPlayer = PD_MG_LogicPosition(playersManager->GetDataStructPlayer(j)->turnOrders->positionsToMove.Last().positionX,
						playersManager->GetDataStructPlayer(j)->turnOrders->positionsToMove.Last().positionY);
				}
				else {
					posOtherPlayer = playersManager->GetDataStructPlayer(j)->logic_Character->GetCurrentLogicalPosition();
				}
				if ( tilesInRangeOfSkill.Contains(posOtherPlayer))
				{
					ID_Player.Add(playersManager->GetDataStructPlayer(j)->logic_Character->GetIDCharacter());
					
						TypePlayer.Add(playersManager->GetDataStructPlayer(j)->logic_Character->GetWeapon()->TypeWeapon);
				}
			}
		}
	}
}

void UPD_ClientGameInstance::InteractuablesThatYouCanInteractFrom(TArray<APD_E_Interactuable*>& interactuable)
{
	interactuable = TArray<APD_E_Interactuable*>();
	UE_LOG(LogTemp, Log, TEXT("UPD_ClientGameInstance::InteractuablesThatYouCanInteractFrom"));

	for (PD_MM_InteractuableInfo* interInfo : mapManager->MapInfo->interactuableInfoInMap) {

		if (mapManager->MapInfo->interactuableActorByLogicPosition.Contains(interInfo->logpos)) {

			UE_LOG(LogTemp, Log, TEXT("UPD_ClientGameInstance::InteractuablesThatYouCanInteractFrom - interInfo pos(%d,%d), InteractFromThisLogicPositions Num %d"), interInfo->logpos.GetX(), interInfo->logpos.GetY(),
				mapManager->MapInfo->interactuableActorByLogicPosition[interInfo->logpos]->InteractFromThisLogicPositions.Num());
			for (PD_MG_LogicPosition lp : mapManager->MapInfo->interactuableActorByLogicPosition[interInfo->logpos]->InteractFromThisLogicPositions) {
				

				if (playersManager->MyPlayerInfo->turnOrders->positionsToMove.Num() == 0) {

					UE_LOG(LogTemp, Log, TEXT("UPD_ClientGameInstance::InteractuablesThatYouCanInteractFrom - (%d,%d) - (%d,%d)"), lp.GetX(), lp.GetY(),
						playersManager->MyPlayerInfo->logic_Character->GetCurrentLogicalPosition().GetX(),
						playersManager->MyPlayerInfo->logic_Character->GetCurrentLogicalPosition().GetY());

					if (lp == playersManager->MyPlayerInfo->logic_Character->GetCurrentLogicalPosition()) {
						UE_LOG(LogTemp, Log, TEXT("UPD_ClientGameInstance::InteractuablesThatYouCanInteractFrom - added!"));
						interactuable.Add(mapManager->MapInfo->interactuableActorByLogicPosition[interInfo->logpos]);
					}
				}
				else {

					UE_LOG(LogTemp, Log, TEXT("UPD_ClientGameInstance::InteractuablesThatYouCanInteractFrom - (%d,%d) - (%d,%d)"), lp.GetX(), lp.GetY(),
						playersManager->MyPlayerInfo->turnOrders->positionsToMove.Last().positionX,
						playersManager->MyPlayerInfo->turnOrders->positionsToMove.Last().positionY);

					if (lp.GetX() == playersManager->MyPlayerInfo->turnOrders->positionsToMove.Last().positionX &&
						lp.GetY() == playersManager->MyPlayerInfo->turnOrders->positionsToMove.Last().positionY) {
						UE_LOG(LogTemp, Log, TEXT("UPD_ClientGameInstance::InteractuablesThatYouCanInteractFrom - added!"));
						interactuable.Add(mapManager->MapInfo->interactuableActorByLogicPosition[interInfo->logpos]);
					}
				}


				
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("UPD_ClientGameInstance::InteractuablesThatYouCanInteractFrom ENDED %d"), interactuable.Num());
}



FStructSkill UPD_ClientGameInstance::LoadSkillStructData(int type, int indexSkill)
{
	FStructSkill skillSelected = FStructSkill();

	if (type == 0) //Activas
	{
		if (activeSkills.Num() > 0)
		{
			for (int i = 0; i < activeSkills.Num(); i++)
			{
				if (indexSkill == activeSkills[i].ID_Skill)
				{
					skillSelected = activeSkills[i];
				}
			}
		}
	}
	else if (type == 1) //Pasivas
	{
		if (pasiveSkills.Num() > 0)
		{
			for (int i = 0; i < pasiveSkills.Num(); i++)
			{
				if (indexSkill == pasiveSkills[i].ID_Skill)
				{
					skillSelected = pasiveSkills[i];
				}
			}
		}
	}

	return skillSelected;
}




void UPD_ClientGameInstance::SaveCharacterLogicData() {
	//Create an instance of our savegame class
	UPD_SaveCharacterData* SaveGameInstance = Cast<UPD_SaveCharacterData>(UGameplayStatics::CreateSaveGameObject(UPD_SaveCharacterData::StaticClass()));
	//SaveGameInstance->basicStatsArray.Add(*playersManager->MyPlayerInfo->logic_Character->GetBasicStats());
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot"), 0);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Game Saved."));
}

void UPD_ClientGameInstance::LoadCharacterLogicData() {
	//Create an instance of our savegame class
	UPD_SaveCharacterData* SaveGameInstance = Cast<UPD_SaveCharacterData>(UGameplayStatics::CreateSaveGameObject(UPD_SaveCharacterData::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist("MySlot", 0)) {
		SaveGameInstance = Cast<UPD_SaveCharacterData>(UGameplayStatics::LoadGameFromSlot("MySlot", 0));
		//FStructBasicStats* bStats = &SaveGameInstance->basicStatsArray[0];
		//playersManager->MyPlayerInfo->logic_Character->SetBasicStats(bStats->POD, bStats->AGI, bStats->DES, bStats->CON, bStats->PER, bStats->MAL);
		//UE_LOG(LogTemp, Warning, TEXT("PODER CARGADO: %d. AGILIDAD CARGADA: %d"), static_cast<uint8>(bStats->POD), static_cast<uint8>(bStats->AGI));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Game Loaded."));
	}

	else
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("File does not exist."));
}

void UPD_ClientGameInstance::DeleteCharacterLogicData(FString slotName, int slotNumber) {
	if (UGameplayStatics::DoesSaveGameExist(slotName, slotNumber)) {
		UGameplayStatics::DeleteGameInSlot(slotName, slotNumber);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Slot Deleted."));
	}

	else
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("File does not exist. Can not be deleted"));
}




void UPD_ClientGameInstance::LoadWeaponData(TArray<int> &indexWeapons)
{
	indexWeapons = TArray<int>();

		if (weapons.Num() > 0)
		{
			for (int i = 0; i < weapons.Num(); i++)
			{
				indexWeapons.Add(weapons[i].ID_Weapon);
				UE_LOG(LogTemp, Warning, TEXT("ID_weapon : %d"), weapons[i].ID_Weapon);
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Weapons Loaded."));
	
}

void UPD_ClientGameInstance::LoadWeaponSpecificData(int indexWeapon, int &id_weapon, int &classWeapon, int &typeWeapon, int &damage, int &range) //Para cargar TODOS los datos de un arma especifica
{
	if (weapons.Num() > 0)
		{
			for (int i = 0; i < weapons.Num(); i++)
			{
				if (indexWeapon == weapons[i].ID_Weapon)
				{
					id_weapon = indexWeapon;
					classWeapon = weapons[i].ClassWeapon;
					typeWeapon = weapons[i].TypeWeapon;
					damage = weapons[i].DMWeapon;
					range = weapons[i].RangeWeapon;
				}
			}
		}
}

FStructWeapon UPD_ClientGameInstance::LoadWeaponStructData(int indexWeapon)
{
	FStructWeapon weaponSelected = FStructWeapon();

		if (weapons.Num() > 0)
		{
			for (int i = 0; i < weapons.Num(); i++)
			{
				if (indexWeapon == weapons[i].ID_Weapon)
				{
					weaponSelected = weapons[i];
				}
			}
		}

	
	return weaponSelected;

}


//PARA HABILIDADES
//Para Gestionar las Habilidades
void UPD_ClientGameInstance::LoadSkillData(int TypeSkill, TArray<int> &skills, TArray<FString> &nameSkills)
{

	skills = TArray<int>();
	nameSkills = TArray<FString>();

		if (TypeSkill == 0) //ACTIVA
		{
			if (activeSkills.Num() > 0)
			{
				for (int i = 0; i < activeSkills.Num(); i++)
				{
					skills.Add(activeSkills[i].ID_Skill);
					nameSkills.Add(activeSkills[i].name_Skill);
				}
			}
		}
		else if (TypeSkill == 1)
		{
			if (pasiveSkills.Num() > 0)
			{
				for (int i = 0; i < pasiveSkills.Num(); i++)
				{
					skills.Add(pasiveSkills[i].ID_Skill);
					nameSkills.Add(pasiveSkills[i].name_Skill);
				}
			}
		}
	
}


void UPD_ClientGameInstance::LoadSkillSpecificData(int TypeSkill, int id_skill, FString &nameSkill, FString &effectSkill, int &weaponRequired, int &AP, int &CD, int &target, int &range)
{
		if (TypeSkill == 0) //ACTIVA
		{
			if (activeSkills.Num() > 0)
			{
				for (int i = 0; i < activeSkills.Num(); i++)
				{
					if (id_skill == activeSkills[i].ID_Skill)
					{
						nameSkill = activeSkills[id_skill].name_Skill;
						effectSkill = activeSkills[id_skill].description;
						weaponRequired = activeSkills[id_skill].weaponRequired;
						AP = activeSkills[id_skill].AP;
						CD = activeSkills[id_skill].CD;
						target = activeSkills[id_skill].target;
						range = activeSkills[id_skill].range;
					}
				}
			

			}
		}
		else if (TypeSkill == 1)
		{
			if (pasiveSkills.Num() > 0)
			{
				for (int i = 0; i < pasiveSkills.Num(); i++)
				{
					if (id_skill == pasiveSkills[i].ID_Skill)
					{
						nameSkill = pasiveSkills[id_skill].name_Skill;
						effectSkill = pasiveSkills[id_skill].description;
						weaponRequired = pasiveSkills[id_skill].weaponRequired;
						AP = pasiveSkills[id_skill].AP;
						CD = pasiveSkills[id_skill].CD;
						target = pasiveSkills[id_skill].target;
						range = pasiveSkills[id_skill].range;
					}
				}
				
			}
		}
}

#pragma endregion

#pragma region LoadDataFromFile

void UPD_ClientGameInstance::LoadSkillActiveDatafromFile()
{
	FString filepath = "Content/Data/ActiveSkills.csv";
	FString FilePath = FPaths::GameDir() + *filepath;
	FString FileData = "";

	if (FFileHelper::LoadFileToString(FileData, *FilePath))
	{
		TArray<FString> raws;
		FileData.ParseIntoArray(raws, TEXT("\n"), true); //Consigo cada linea del CSV (1 habilidad)

		for (int i = 0; i < raws.Num(); i++)
		{
			TArray<FString> columns;
			raws[i].ParseIntoArray(columns, TEXT(";"), true); //Consigo los campos de cada linea

			//UE_LOG(LogTemp, Warning, TEXT("UPD_ClientGameInstance::LoadSkillActiveDatafromFile::  Fields of Skill :%d "), columns.Num());
			FStructSkill newActiveSkill = FStructSkill();
			for (int j = 0; j < columns.Num(); j++)
			{
				/*
				0 - ID
				1 - Name_ID
				2 - Skill name
				3 - Effect
				4 - Type 
				5 - AP
				6 - CD
				7 - Range
				8 - Target
				9 - Weapon Required
				*/
				if (j == 0) 
					newActiveSkill.ID_Skill = FCString::Atoi(*columns[j]);
				if (j == 2)
					newActiveSkill.name_Skill = columns[j];
				if (j == 3)
					newActiveSkill.description = columns[j];
				if (j == 4)
					newActiveSkill.typeSkill = FCString::Atoi(*columns[j]);
				if (j == 5)
					newActiveSkill.AP = FCString::Atoi(*columns[j]);
				if (j == 6)
					newActiveSkill.CD = FCString::Atoi(*columns[j]);
				if (j == 7)
					newActiveSkill.range = FCString::Atoi(*columns[j]);
				if (j == 8)
					newActiveSkill.target = FCString::Atoi(*columns[j]);
				if (j == 9)
					newActiveSkill.weaponRequired = FCString::Atoi(*columns[j]);
			}

			newActiveSkill.currentCD = 0;
			activeSkills.Add(newActiveSkill);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("UPD_ClientGameInstance::LoadSkillActiveDatafromFile::  Error loading Active Skills! Failed to load file!. Path :%s"), *FilePath));
	}

}

void UPD_ClientGameInstance::LoadSkillPasiveDatafromFile()
{
	FString filepath = "Content/Data/PasiveSkills.csv";
	FString FilePath = FPaths::GameDir() + *filepath;
	FString FileData = "";

	if (FFileHelper::LoadFileToString(FileData, *FilePath))
	{
		TArray<FString> raws;
		FileData.ParseIntoArray(raws, TEXT("\n"), true); //Consigo cada linea del CSV (1 habilidad)

		for (int i = 0; i < raws.Num(); i++)
		{
			TArray<FString> columns;
			raws[i].ParseIntoArray(columns, TEXT(";"), true); //Consigo los campos de cada linea

															  //UE_LOG(LogTemp, Warning, TEXT("UPD_ClientGameInstance::LoadSkillActiveDatafromFile::  Fields of Skill :%d "), columns.Num());
			FStructSkill newPasiveSkill = FStructSkill();
			for (int j = 0; j < columns.Num(); j++)
			{
				/*
				0 - ID
				1 - Name_ID
				2 - Skill name
				3 - Effect
				4 - Type
				5 - AP
				6 - CD
				7 - Range
				8 - Target
				9 - Weapon Required
				*/
				if (j == 0)
					newPasiveSkill.ID_Skill = FCString::Atoi(*columns[j]);
				if (j == 2)
					newPasiveSkill.name_Skill = columns[j];
				if (j == 3)
					newPasiveSkill.description = columns[j];
				if (j == 4)
					newPasiveSkill.typeSkill = FCString::Atoi(*columns[j]);
				if (j == 5)
					newPasiveSkill.AP = FCString::Atoi(*columns[j]);
				if (j == 6)
					newPasiveSkill.CD = FCString::Atoi(*columns[j]);
				if (j == 7)
					newPasiveSkill.range = FCString::Atoi(*columns[j]);
				if (j == 8)
					newPasiveSkill.target = FCString::Atoi(*columns[j]);
				if (j == 9)
					newPasiveSkill.weaponRequired = FCString::Atoi(*columns[j]);
			}

			newPasiveSkill.currentCD = 0;
			pasiveSkills.Add(newPasiveSkill);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("UPD_ClientGameInstance::LoadSkillPasiveDatafromFile::  Error loading Pasive Skills! Failed to load file!. Path :%s"), *FilePath));
	}
}

void UPD_ClientGameInstance::LoadWeaponDataFromFile()
{
	FString filepath = "Content/Data/Weapons.csv";
	FString FilePath = FPaths::GameDir() + *filepath;
	FString FileData = "";

	if (FFileHelper::LoadFileToString(FileData, *FilePath))
	{
		TArray<FString> raws;
		FileData.ParseIntoArray(raws, TEXT("\n"), true); //Consigo cada linea del CSV (1 arma)

		for (int i = 0; i < raws.Num(); i++)
		{
			TArray<FString> columns;
			raws[i].ParseIntoArray(columns, TEXT(";"), true); //Consigo los campos de cada linea

			//UE_LOG(LogTemp, Warning, TEXT("UPD_ClientGameInstance::LoadWeaponDataFromFile::  Fields of weapon :%d "), columns.Num());
			FStructWeapon newWeapon = FStructWeapon();
			for (int j = 0; j < columns.Num(); j++)
			{
				/*
				0 - ID
				1 - Name_ID
				2 - Type
				3 - Class
				4 - DM
				5 - Rango
				*/
				if (j == 0)
					newWeapon.ID_Weapon = FCString::Atoi(*columns[j]);
				if (j == 2)
					newWeapon.TypeWeapon = FCString::Atoi(*columns[j]);
				if (j == 3)
					newWeapon.ClassWeapon = FCString::Atoi(*columns[j]);
				if (j == 4)
					newWeapon.DMWeapon = FCString::Atoi(*columns[j]);
				if (j == 5)
					newWeapon.RangeWeapon = FCString::Atoi(*columns[j]);
			}

			weapons.Add(newWeapon);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("UPD_ClientGameInstance::LoadSkillActiveDatafromFile::  Error loading Active Skills! Failed to load file!. Path :%s"), *FilePath));
	}
}

void UPD_ClientGameInstance::LoadPlayerActiveSkillsForPanel(TArray<int> &ID_Skills, TArray<FString> &name_skills, TArray<FString> &effect_skills)
{
	// añadir Ataque basico a la lista --  lo tienen todos
	ID_Skills.Add(LoadSkillStructData(0, 0).ID_Skill); 
	name_skills.Add(LoadSkillStructData(0,0).name_Skill);
	effect_skills.Add(LoadSkillStructData(0,0).description);

	// añadir defensa a la lista --  lo tienen todos
	ID_Skills.Add(LoadSkillStructData(0, 1).ID_Skill);
	name_skills.Add(LoadSkillStructData(0, 1).name_Skill);
	effect_skills.Add(LoadSkillStructData(0, 1).description);

	if (playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Num() > 0)
	{
		for (int i = 0; i < playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Num(); i++)
		{
			ID_Skills.Add(playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills[i].ID_Skill);
			name_skills.Add(playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills[i].name_Skill);
			effect_skills.Add(playersManager->MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills[i].description);
		}
	}
	
}


#pragma endregion

bool UPD_ClientGameInstance::LogicPositionIsValidToMove(int positionX, int positionY)
{
	bool positionIsValid = false;
	PD_MG_LogicPosition positionToCheck = PD_MG_LogicPosition(positionX, positionY);

	if (playersManager->MyPlayerInfo->turnOrders->positionsToMove.Num() > 0)
	{
		PD_MG_LogicPosition lastPositionOnMovement = PD_MG_LogicPosition(playersManager->MyPlayerInfo->turnOrders->positionsToMove[playersManager->MyPlayerInfo->turnOrders->positionsToMove.Num() - 1].positionX,
			playersManager->MyPlayerInfo->turnOrders->positionsToMove[playersManager->MyPlayerInfo->turnOrders->positionsToMove.Num() - 1].positionY);

		TArray<PD_MG_LogicPosition>  tilesNear = mapManager->Get_LogicPosition_Adyacents_To(lastPositionOnMovement);
		for (int i = 0; i < tilesNear.Num(); i++)
		{
			if (tilesNear[i] == positionToCheck)
				positionIsValid = true;
		}
	}
	else {
		positionIsValid = true;
	}
	return positionIsValid;

}
