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

//Includes de prueba
#include "MapInfo/PD_MM_MapInfo.h"

#include "Actors/PD_E_Character.h"

bool UPD_ClientGameInstance::SuscribeToEvents(int inPlayer, UStructType inType) {
	return true; //de momento recibe todos, siempre es cierto.
}



#pragma region INIT

void UPD_ClientGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Init GameInstance ~> "));
	//InitializeNetworking();

	levelsNameDictionary = LevelsNameDictionary();

	playersManager = new PD_PlayersManager();

	structClientState = new StructClientState();

	ChangeState(EClientState::StartApp);
	//structClientState->enumClientState = EClientState::NoConnection;

	playerInfo = new StructPlayer(); //El constructor del StructPlayer inicializa sus variables
									 /*
									 Construimos el logicCharacter, para luego poder rellenarlo. Lo configuramos como Jugador.
									 */
	playerInfo->logic_Character = new PD_GM_LogicCharacter();
	playerInfo->turnOrders = new FStructTurnOrders();

	playerInfo->logic_Character->SetIsPlayer(true);
	playerInfo->logic_Character->SetTypeCharacter(ECharacterType(0)); //Al ser player. 0 vuelve a indicar que es Jugador.

	InitializeNetworking();

	networkManager->RegisterObserver(this);

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


	if (structClientState->enumClientState == EClientState::StartApp) {

	}
	else if(structClientState->enumClientState == EClientState::Game_NoConnection) {
		HandleEvent_Welcome(inDataStruct, inPlayer, inEventType);
	}	 
	else if(structClientState->enumClientState == EClientState::ConfigureMatch) {
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
	else if(structClientState->enumClientState == EClientState::WaitingMatchConfiguration) {
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
	else if(structClientState->enumClientState == EClientState::Lobby_Tabern) {
		if (inEventType == UStructType::FStructMap) {

			// Cuando el server envia el mapa ... 
			HandleEvent_MapIncoming(inDataStruct, inPlayer, inEventType);
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
	else if (structClientState->enumClientState == EClientState::Launch_Match) {

		if (inEventType == UStructType::FStructInstatiatePlayers) {
			HandleEvent_AllCharactersInfoIncoming(inDataStruct, inPlayer, inEventType);
		}else if (inEventType == UStructType::FStructClientStartMatchOnGM) {
			HandleEvent_StartMatch_GoGameMnager(inDataStruct, inPlayer, inEventType);
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
	else if(structClientState->enumClientState == EClientState::GameInProcess) {

	}
	else { } 



}


#pragma region HANDLERS

void UPD_ClientGameInstance::HandleEvent_Welcome(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {
	UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_Welcome"));

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

void UPD_ClientGameInstance::HandleEvent_LaunchMatchFromServer(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {
	UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_LaunchMatchFromServer"));

	structClientState->ConfigAllCharactersDone = true;
	this->UpdateState();
}


void UPD_ClientGameInstance::HandleEvent_AllCharactersInfoIncoming(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {
	UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::HandleEvent_AllCharactersInfoIncoming"));
	FStructInstatiatePlayers* InstaPlayers = (FStructInstatiatePlayers*)inDataStruct;
	for (int i = 0; i < InstaPlayers->listInfoPlayerAtClient.Num(); i++)
	{
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
		UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::ChangeState to StartApp"));
		break;
	case EClientState::Game_NoConnection:
		UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::ChangeState to NoConnection"));
		break;
	case EClientState::ConfigureMatch:
		UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::ChangeState to ConfigureMatch"));
		break;
	case EClientState::WaitingMatchConfiguration:
		UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::ChangeState to WaitingMatchConfiguration"));
		break;
	case EClientState::Lobby_Tabern:
		UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::ChangeState to Lobby_Tabern"));
		break;
	case EClientState::Launch_Match:
		UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::ChangeState to Launch_Match"));
		break;
	case EClientState::GameInProcess:
		UE_LOG(LogTemp, Warning, TEXT("ServerGameInstance::ChangeState to GameInProcess"));
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
	else if (structClientState->enumClientState == EClientState::Launch_Match) {
		mapParser = new PD_MG_MapParser();

		PD_MG_StaticMap* staticMapRef = new PD_MG_StaticMap();
		PD_MG_DynamicMap* dynamicMapRef = new PD_MG_DynamicMap();

		// Parsea el chorizo
		mapParser->StartParsingFromChorizo(&structClientState->mapString, staticMapRef, dynamicMapRef);
		mapManager = new PD_GM_MapManager();
		mapManager->Init(staticMapRef, dynamicMapRef);
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
		if (structClientState->ConfigAllCharactersDone) {
			this->ChangeState(EClientState::Launch_Match);
		}
	}
	else if (structClientState->enumClientState == EClientState::Launch_Match) {
		if (structClientState->AllCharactersIncoming) {
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
		mapManager->instantiator = InstantiatorActor;

		//Aqui cedemos el control al GameManager.
		gameManager = new PD_GM_GameManager(mapManager, playersManager, networkManager);
		

	}
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
	networkManager->SendNow(&configDone, 0);
}


bool UPD_ClientGameInstance::GetReadyToParty()
{
	// Comprobamos APTotal para comprobar si el usuario ha metido valores o no. Sera distinto de 0 si hubiera un personaje seleccionado.
	//Hasta que vea otra forma de ver si el Jugador ha elegido personaje o no
	if (playerInfo->isSetPlayerCharacter) { 

		FStructOrderMenu respuesta = FStructOrderMenu();
		respuesta.orderType = respuesta.orderType = static_cast<uint8>(MenuOrderType::ClientReady);
		UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance:: Enviando: 4 - ClientReady"));
		networkManager->SendNow(&respuesta, 0);
		playerInfo->readyMenu = !playerInfo->readyMenu;
		return playerInfo->readyMenu;
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "ERROR!!  Tienes que ACEPTAR y ENVIAR tu personaje antes del READY !");

	}
	return false;
}

void UPD_ClientGameInstance::FillCharecterStats(int nPOD, int nAGI, int nDES, int nCON, int nPER, int nMAL) 
{
	playerInfo->logic_Character->SetBasicStats(nPOD, nAGI, nDES, nCON, nPER, nMAL);
	playerInfo->logic_Character->SetInitBaseStats(100, 20,5); //HP - DMG
	playerInfo->logic_Character->SetWapon();
	playerInfo->logic_Character->SetTotalStats();
}

void UPD_ClientGameInstance::GetCharacterBasicStats(int &nPOD, int &nAGI, int &nDES, int &nCON, int &nPER, int &nMAL)
{
	nPOD = playerInfo->logic_Character->GetBasicStats()->POD;
	nAGI = playerInfo->logic_Character->GetBasicStats()->AGI;
	nDES = playerInfo->logic_Character->GetBasicStats()->DES;
	nCON = playerInfo->logic_Character->GetBasicStats()->CON;
	nPER = playerInfo->logic_Character->GetBasicStats()->PER;
	nMAL = playerInfo->logic_Character->GetBasicStats()->MAL;
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
	structCharacterToSend.skills = *(playerInfo->logic_Character->GetSkills());
	structCharacterToSend.skin = *(playerInfo->logic_Character->GetSkin());
	structCharacterToSend.weapon = *(playerInfo->logic_Character->GetWeapon());

	UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance:: Enviando: Character Stats and Data"));
	networkManager->SendNow(&structCharacterToSend, 0);
	
	playerInfo->isSetPlayerCharacter = true;
	return true;
}

bool UPD_ClientGameInstance::CreateMoveOrderToSend(FVector positionTile)
{
	/*
	Recibe por parametros una posicion X e Y en Float.
	Con eso consulta al MapManager por la posición FÍSICA (transformarlo previamente en FVector
	para obtener un LogicPosition.
	- Con ese logic position crear un nuevo FStructLogicPosition y un nuevo FStructOrderAction de tipo Move
	- Guardar este Struct en el Array de PlayerInfo->TurnOders (crear estruct TurnOrders si no estuviera creado)
	*/

	if (playerInfo->turnOrders->listMove.Num() >= 1) //Si hay ya una casilla seleccionada, borrarla y reset de material a esta casilla
	{
		/*
		1. Conseguir la LogicPosition de la casilla
		2. Conseguir la referencia de la tile de la casilla
		3. ejecutar el método ResetToInitMaterial
		4. borrar el structOrdern de la lista
		*/

		PD_MG_LogicPosition tile_pos = PD_MG_LogicPosition(playerInfo->turnOrders->listMove[0].targetLogicPosition.positionX,
			playerInfo->turnOrders->listMove[0].targetLogicPosition.positionY);

		PD_MM_Room* roomSelected = nullptr;
		
		/*if (mapManager->MapInfo->RoomOf(tile_pos, roomSelected))  ----> PREGUNTAR A ANGEL -> FUNCION ROOMOF
		{
			UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::CreateMoveOrderToSend ------> ha encontrado la sala "));
			AActor* tileSelected = nullptr;
			tileSelected = *(roomSelected->tiles.Find(tile_pos));
			
			FVector v = tileSelected->GetActorLocation();
			UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::CreateMoveOrderToSend ------> tile en pos %s"),*v.ToString());

			if (tileSelected)
			{
				UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::CreateMoveOrderToSend ------> ha encontrado la tile"));
				FOutputDeviceNull ar;
				tileSelected->CallFunctionByNameWithArguments(TEXT("ResetToInitMaterial"), ar, NULL, true);
			}
		
		}*/
		
		playerInfo->turnOrders->listMove.RemoveAt(0);
	}

	PD_MG_LogicPosition newLogicPosition = mapManager->WorldToLogicPosition(positionTile);

	FStructLogicPosition LogicPosToMove = FStructLogicPosition();
	LogicPosToMove.positionX = newLogicPosition.GetX();
	LogicPosToMove.positionY = newLogicPosition.GetY();

	FStructOrderAction newOrderMove = FStructOrderAction();
	newOrderMove.targetLogicPosition = LogicPosToMove;
	newOrderMove.orderType = static_cast<uint8>(EOrderAction::Move);
	newOrderMove.targetDirection = 1;
	playerInfo->turnOrders->listMove.Add(newOrderMove);

	SetTypeOfAction(0); //seteas el tipo de accion a 0, para reiniciar las acciones
	return true;
}

bool UPD_ClientGameInstance::CreateActionOrderToSend(FVector positionTile)
{
	/*
	Recibe por parametros una posicion X e Y en Float.
	Con eso consulta al MapManager por la posición FÍSICA (transformarlo previamente en FVector
	para obtener un LogicPosition.
	- Con ese logic position crear un nuevo FStructLogicPosition y un nuevo FStructOrderAction de tipo Atack
	- Guardar este Struct en el Array de PlayerInfo->TurnOders (crear estruct TurnOrders si no estuviera creado)
	*/

	if (playerInfo->turnOrders->listAttack.Num() >= 1) //Si hay ya una casilla seleccionada, borrarla y reset de material a esta casilla
	{
		/*
		1. Conseguir la LogicPosition de la casilla
		2. Conseguir la referencia de la tile de la casilla
		3. ejecutar el método ResetToInitMaterial
		4. borrar el structOrdern de la lista
		*/

		//PD_MG_LogicPosition tile_pos = PD_MG_LogicPosition(playerInfo->turnOrders->[0],
			//playerInfo->turnOrders->listMove[0].targetLogicPosition.positionY);

		//PD_MM_Room* roomSelected = nullptr;

		/*if (mapManager->MapInfo->RoomOf(tile_pos, roomSelected))  ----> PREGUNTAR A ANGEL -> FUNCION ROOMOF
		{
		UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::CreateMoveOrderToSend ------> ha encontrado la sala "));
		AActor* tileSelected = nullptr;
		tileSelected = *(roomSelected->tiles.Find(tile_pos));

		FVector v = tileSelected->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::CreateMoveOrderToSend ------> tile en pos %s"),*v.ToString());

		if (tileSelected)
		{
		UE_LOG(LogTemp, Warning, TEXT("ClientGameInstance::CreateMoveOrderToSend ------> ha encontrado la tile"));
		FOutputDeviceNull ar;
		tileSelected->CallFunctionByNameWithArguments(TEXT("ResetToInitMaterial"), ar, NULL, true);
		}

		}*/

		playerInfo->turnOrders->listAttack.RemoveAt(0);
	}

	PD_MG_LogicPosition newLogicPosition = mapManager->WorldToLogicPosition(positionTile);

	FStructLogicPosition LogicPosToMove = FStructLogicPosition();
	LogicPosToMove.positionX = newLogicPosition.GetX();
	LogicPosToMove.positionY = newLogicPosition.GetY();

	FStructOrderAction newOrderMove = FStructOrderAction();
	newOrderMove.targetLogicPosition = LogicPosToMove;
	newOrderMove.orderType = static_cast<uint8>(EOrderAction::Attack);
	newOrderMove.targetDirection = 1;
	playerInfo->turnOrders->listAttack.Add(newOrderMove);

	SetTypeOfAction(0); //seteas el tipo de accion a 0, para reiniciar las acciones
	return true;
}

bool UPD_ClientGameInstance::SendTurnOrderActionsToServer()
{
	if (!playerInfo->turnOrders) {
		return false;
	}

	FStructTurnOrders turnsOrdersToSend = FStructTurnOrders();
	turnsOrdersToSend = *(playerInfo->turnOrders);

	bool sentOk = networkManager->SendNow(&turnsOrdersToSend, 0);

	if (sentOk)  //Si se ha enviado bien el paquete - Vaciar el PlayersInfo->turnOrders y return true
	{
		playerInfo->turnOrders = new FStructTurnOrders();
		gameManager->structGameState->enumGameState = EClientGameState::WaitingServer;
	}
	
	return sentOk;
}

void UPD_ClientGameInstance::SetTypeOfAction(int ntypeAction)
{
	playerInfo->typeOfAction = ntypeAction;
}

uint8 UPD_ClientGameInstance::GetTypeOfAction()
{
	return playerInfo->typeOfAction;
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
		case EClientGameState::GenerateOrders: 
		{
			GameStatusInt = static_cast<uint8>(EClientGameState::GenerateOrders);
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

EClientGamePhase UPD_ClientGameInstance::GetGamePhase()
{
	return gameManager->structGamePhase->enumGamePhase;
}

uint8 UPD_ClientGameInstance::GetPlayerNumber()
{
	return structClientState->numPlayer;
}

void UPD_ClientGameInstance::SaveCharacterLogicData() {
	//Create an instance of our savegame class
	UPD_SaveCharacterData* SaveGameInstance = Cast<UPD_SaveCharacterData>(UGameplayStatics::CreateSaveGameObject(UPD_SaveCharacterData::StaticClass()));
	SaveGameInstance->basicStatsArray.Add(*playerInfo->logic_Character->GetBasicStats());
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot"), 0);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Game Saved."));
}

void UPD_ClientGameInstance::LoadCharacterLogicData() {
	//Create an instance of our savegame class
	UPD_SaveCharacterData* SaveGameInstance = Cast<UPD_SaveCharacterData>(UGameplayStatics::CreateSaveGameObject(UPD_SaveCharacterData::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist("MySlot", 0)) {
		SaveGameInstance = Cast<UPD_SaveCharacterData>(UGameplayStatics::LoadGameFromSlot("MySlot", 0));
		FStructBasicStats* bStats = &SaveGameInstance->basicStatsArray[0];
		playerInfo->logic_Character->SetBasicStats(bStats->POD, bStats->AGI, bStats->DES, bStats->CON, bStats->PER, bStats->MAL);
		UE_LOG(LogTemp, Warning, TEXT("PODER CARGADO: %d. AGILIDAD CARGADA: %d"), static_cast<uint8>(bStats->POD), static_cast<uint8>(bStats->AGI));
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
#pragma endregion