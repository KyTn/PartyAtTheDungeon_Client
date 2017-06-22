// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"

#include "PD_ClientGameInstance.h"

#include "PD_GM_GameManager.h"
#include "PD_GM_MapManager.h"
#include "PD_GM_EnemyManager.h"
#include "PD_PlayersManager.h"
#include "NW_Networking/PD_NW_NetworkManager.h"
#include "Structs/PD_ClientStructs.h" 
#include "Structs/PD_ClientEnums.h" 
#include "Actors/Interactuables/PD_E_Interactuable.h"
#include "Actors/Interactuables/PD_E_Door.h"

//Includes de uso de objetos
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "LogicCharacter/PD_GM_LogicCharacter.h"
#include "Actors/PD_GenericController.h"
#include "Actors/Players/PD_CharacterController.h"

PD_GM_GameManager::PD_GM_GameManager(UPD_ClientGameInstance* gameInstance, PD_GM_MapManager* inMapManager, PD_PlayersManager* inPlayerManager, PD_NW_NetworkManager* networkManager, EClientGameState stateAfterInstantiate_Map= EClientGameState::Start_Match)
{
	UE_LOG(LogTemp, Log, TEXT("Constructor Game Manager"));

	_GAMEINSTANCE = gameInstance;
	playersManager = inPlayerManager;
	mapManager =  inMapManager;
	mapManager->_GAMEMANAGER = this;
	enemyManager = new PD_GM_EnemyManager();
	this->networkManager = networkManager;
	this->networkManager->RegisterObserver(this);
	structGameState = new StructGameState();
	structGameState->stateAfterInstantiate_Map = stateAfterInstantiate_Map;
	InitState();
	

}

PD_GM_GameManager::~PD_GM_GameManager()
{
}


bool PD_GM_GameManager::SuscribeToEvents(int inPlayer, UStructType inType) {
	if (inType == UStructType::FStructTurnOrders) return true; //Suscrito a TurnOrders
	else return true; //Hasta que quede definido todo lo que tiene que recibir, recibe todos.
}

// Inicializa la maquina de estados.
void PD_GM_GameManager::InitState() {
	UE_LOG(LogTemp, Log, TEXT("InitState Game Manager"));
	
	ChangeState(EClientGameState::Instantiate_Map);
}

#pragma region GM STATE MACHINE 

// Esta funcion cambia el estado actual al pasado por parametros y llama al OnBeginState del mismo.
void PD_GM_GameManager::ChangeState(EClientGameState newState) {
	structGameState->enumGameState = newState;
	OnBeginState();
}

// Cambia el estado entre fases, teniendo en cuenta si se quiere avanzar o retroceder
void PD_GM_GameManager::ChangePhase(bool next = true) {
	if (structGameState->enumGameState == EClientGameState::GenerateOrders_ConsumablePhase) {
		ChangeState(EClientGameState::GenerateOrders_MovementPhase);
	}

	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_MovementPhase) {
		if (next)	//Siguiente estado
			ChangeState(EClientGameState::GenerateOrders_InteractionPhase);
		else	//Estado anterior
			ChangeState(EClientGameState::GenerateOrders_ConsumablePhase);
	}

	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_InteractionPhase) {
		if (next)
			ChangeState(EClientGameState::GenerateOrders_ActionPhase);
		else
			ChangeState(EClientGameState::GenerateOrders_MovementPhase);
	}

	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_ActionPhase) {
		if (next)
			ChangeState(EClientGameState::GenerateOrders_Validate);
		else
			ChangeState(EClientGameState::GenerateOrders_InteractionPhase);
	}

	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_Validate) {
		if (next)
			ChangeState(EClientGameState::SendOrdersToServer);
		else
			ChangeState(EClientGameState::GenerateOrders_ActionPhase);
	}
}

// Dado un paquete de red, actualiza el estado correspondiente y realiza las acciones pertinentes. 
void PD_GM_GameManager::HandleEvent(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {
	UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::HandleEvent:: Evento recibido%d. Estado: %d "), static_cast<uint8>(inEventType), static_cast<uint8>(structGameState->enumGameState));

	//Estos dos eventos van aqui?
	if (structGameState->enumGameState == EClientGameState::Instantiate_Map) {
		// Si se recibe del servidor un Start_Match, ir a ese estado. 
		if (inEventType == UStructType::FStructClientStartMatchOnGM 
			//Esta condicion se usa en la reconexion, para hacer un flujo en el que despues de Instantiate_Map no vaya a Start_Match
			&& structGameState->stateAfterInstantiate_Map== EClientGameState::Start_Match) {

			UE_LOG(LogTemp, Log, TEXT(" PD_GM_GameManager::HandleEvent - Instiante"));
			ChangeState(EClientGameState::Start_Match);
		}
	}
	else if(structGameState->enumGameState == EClientGameState::Start_Match) {
		UE_LOG(LogTemp, Log, TEXT(" PD_GM_GameManager::HandleEvent"));

		if (inEventType == UStructType::FStructClientCanGenerateOrders) {
		    
			ChangeState(EClientGameState::GenerateOrders_Start);
		 }
	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_Start) {
		
	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_ConsumablePhase) {

	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_MovementPhase) {

	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_InteractionPhase) {

	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_ActionPhase) {

	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_Validate) {

	}
	else if(structGameState->enumGameState == EClientGameState::SendOrdersToServer) {
		 
	}	 
	else if(structGameState->enumGameState == EClientGameState::WaitingServer) {
		if (inEventType == UStructType::FStructUpdateTurn) {
			UE_LOG(LogTemp, Log, TEXT("HandleEvent: FStructUpdateTurn"));
			FStructUpdateTurn* updateTurn = (FStructUpdateTurn*)inDataStruct;
			structGameState->update_turn = *updateTurn;

			ChangeState(EClientGameState::UpdateInfo);
		}
	}	 
	else if(structGameState->enumGameState == EClientGameState::UpdateInfo) {
		 
	}	 
	else if(structGameState->enumGameState == EClientGameState::EndOfTurn) {
		if (inEventType == UStructType::FStructClientCanGenerateOrders) {

			ChangeState(EClientGameState::GenerateOrders_Start);
		}
	}





}


void PD_GM_GameManager::UpdateTurn() {
	UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: UpdateInfo"));

	//Update el Mapa - Instanciar las salas

	for (int id_salas = 0; id_salas < structGameState->update_turn.listOfRoomsInstiantate.Num(); id_salas++)
	{
		mapManager->InstantiateRoomAndAdj(structGameState->update_turn.listOfRoomsInstiantate[id_salas]);
	}
	
	/*for (int id_door = 0; id_door < structGameState->update_turn.listOfDoorOpend.Num(); id_door++)
	{
		APD_E_Door* doorOpend = nullptr;
		doorOpend = mapManager->MapInfo->doorActorByID[structGameState->update_turn.listOfDoorOpend[id_door]];
		if (doorOpend)
		{
			doorOpend->IsDoorOpen = true;
		}
	}*/

	for (int id_interact = 0; id_interact < structGameState->update_turn.listOfInteractuablesActive.Num(); id_interact++)
	{
		switch ((StaticMapElement)structGameState->update_turn.listOfInteractuablesActive[id_interact].ID_Interactable) {

			case StaticMapElement::DOOR: {
				APD_E_Door* doorOpend = nullptr;
				doorOpend = mapManager->MapInfo->doorActorByID[structGameState->update_turn.listOfInteractuablesActive[id_interact].ID_Interactable];
				if (doorOpend)
				{
					doorOpend->IsCurrentlyActivated = structGameState->update_turn.listOfInteractuablesActive[id_interact].isActive;
					
					doorOpend->UpdateState();
				}
				break;
			}
			default:
			{
				APD_E_Interactuable* interactuableActor = nullptr;
				if (mapManager->MapInfo->interactuableActorByID.Contains(structGameState->update_turn.listOfInteractuablesActive[id_interact].ID_Interactable)) {
					interactuableActor = mapManager->MapInfo->interactuableActorByID[structGameState->update_turn.listOfInteractuablesActive[id_interact].ID_Interactable];
					if (interactuableActor)
					{
						interactuableActor->IsCurrentlyActivated = structGameState->update_turn.listOfInteractuablesActive[id_interact].isActive;
						interactuableActor->UpdateState();
						if (interactuableActor->GetIsChest()) {
							mapManager->MapInfo->interactuableActorByID.Remove(structGameState->update_turn.listOfInteractuablesActive[id_interact].ID_Interactable);
							mapManager->MapInfo->interactuableActorByID.Compact();
							mapManager->MapInfo->interactuableActorByID.Shrink();
							PD_MG_LogicPosition lp = *mapManager->MapInfo->interactuableActorByLogicPosition.FindKey(interactuableActor);
							mapManager->MapInfo->interactuableActorByLogicPosition.Remove(lp);
							mapManager->MapInfo->interactuableActorByLogicPosition.Compact();
							mapManager->MapInfo->interactuableActorByLogicPosition.Shrink();
							interactuableActor->Destroy();
							
						}

					}
				}
			}
		}
	}

	mapManager->InstantiateEnemies();

	UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: Updateando Players"));
	for (int iPlayers = 0; iPlayers < structGameState->update_turn.listPlayerCharacters.Num(); iPlayers++) {
		UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: PlayerFor %d"), iPlayers);
		FStructUpdateCharacter updateCharacter = structGameState->update_turn.listPlayerCharacters[iPlayers];
		PD_GM_LogicCharacter* logicCharacter = playersManager->GetCharacterByID(updateCharacter.ID_character);
		//Conversion de Struct a LogicPosition
		PD_MG_LogicPosition logicPosition;
		logicPosition.SetX(updateCharacter.currentCharacterPosition.positionX);
		logicPosition.SetY(updateCharacter.currentCharacterPosition.positionY);

		if (logicCharacter) {
			logicCharacter->MoveAtUpdate(logicPosition);
			logicCharacter->GetTotalStats()->HPCurrent = updateCharacter.HPCurrent;
			logicCharacter->GetTotalStats()->PointsCurrent = updateCharacter.PointsCurrent;
			logicCharacter->GetTotalStats()->APCurrent = logicCharacter->GetTotalStats()->APTotal;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: ERROR: No se identifica el character de player con id %s"), *updateCharacter.ID_character);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: Updateando Enemigos"));
	for (int i = 0; i < enemyManager->GetEnemies().Num(); i++)
	{
		bool found = false;
		for (int j = 0; j < structGameState->update_turn.listEnemyCharacters.Num(); j++)
		{
			if (structGameState->update_turn.listEnemyCharacters[j].ID_character.Equals(enemyManager->GetEnemies()[i]->GetIDCharacter()))
				found = true;
		}
		if (!found)
			enemyManager->DeleteEnemy(enemyManager->GetEnemies()[i]);

	}


	for (int iEnemies = 0; iEnemies < structGameState->update_turn.listEnemyCharacters.Num(); iEnemies++) {

		FStructUpdateCharacter updateCharacter = structGameState->update_turn.listEnemyCharacters[iEnemies];
		UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: EnemyFor %d con id: %s"), iEnemies, *updateCharacter.ID_character);
		PD_GM_LogicCharacter* logicCharacter = enemyManager->GetCharacterByID(updateCharacter.ID_character);
		//Conversion de Struct a LogicPosition
		PD_MG_LogicPosition logicPosition;
		logicPosition.SetX(updateCharacter.currentCharacterPosition.positionX);
		logicPosition.SetY(updateCharacter.currentCharacterPosition.positionY);

		if (logicCharacter) {
			logicCharacter->MoveAtUpdate(logicPosition);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: ERROR: No se identifica el character de enemigo con id %s"), *updateCharacter.ID_character);
		}
	}
}

// Actualiza el estado actual
void PD_GM_GameManager::UpdateState() {

	if (structGameState->enumGameState == EClientGameState::Instantiate_Map) {
		
	}
	else if (structGameState->enumGameState == EClientGameState::Start_Match) {
		

	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_Start) {

	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_ConsumablePhase) {

	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_MovementPhase) {

	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_InteractionPhase) {

	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_ActionPhase) {

	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_Validate) {
		
	}
	else if (structGameState->enumGameState == EClientGameState::SendOrdersToServer) {
		;
	}
	else if (structGameState->enumGameState == EClientGameState::WaitingServer) {

	}
	else if (structGameState->enumGameState == EClientGameState::UpdateInfo) {
		
	}
	else if (structGameState->enumGameState == EClientGameState::EndOfTurn) {
		
	}
	else { //Caso indeterminado
		UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::UpdateState: WARNING: estado sin update"));
	}
}

// Funcion que se llama una sola vez al entrar en un estado. Llamado desde ChangeState. 
void PD_GM_GameManager::OnBeginState() {


	if (structGameState->enumGameState == EClientGameState::Instantiate_Map) {
		UE_LOG(LogTemp, Log, TEXT("Game Manager State: Instantiate_Map"));
		mapManager->InstantiateMap();
		Send_FStructClientMapAlreadyInstantiated();

		//Esta condicion se usa en la reconexion, para hacer un flujo en el que despues de Instantiate_Map no vaya a Start_Match
		if (structGameState->stateAfterInstantiate_Map != EClientGameState::Start_Match) {
			ChangeState(structGameState->stateAfterInstantiate_Map);
		}
	}
	else if (structGameState->enumGameState == EClientGameState::Start_Match) {
		UE_LOG(LogTemp, Log, TEXT("Game Manager State: Start_Match"));
	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_Start) {
		UE_LOG(LogTemp, Log, TEXT("Game Manager State: GenerateOrders_Start"));
		playersManager->ResetAll();
		ChangeState(EClientGameState::GenerateOrders_ConsumablePhase);
	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_ConsumablePhase) {
		UE_LOG(LogTemp, Log, TEXT("Game Manager State: GenerateOrders_ConsumablePhase"));
	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_MovementPhase) {
		UE_LOG(LogTemp, Log, TEXT("Game Manager State: GenerateOrders_MovementPhase"));
	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_InteractionPhase) {
		UE_LOG(LogTemp, Log, TEXT("Game Manager State: GenerateOrders_InteractionPhase"));
	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_ActionPhase) {
		UE_LOG(LogTemp, Log, TEXT("Game Manager State: GenerateOrders_ActionPhase"));
	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders_Validate) {
		UE_LOG(LogTemp, Log, TEXT("Game Manager State: GenerateOrders_ValidateCACAFUTI"));
		//ChangeState(EClientGameState::SendOrdersToServer); No hay que hacerlo aqui, ya que sino no da tiempo a lanzar el PopUp. El PopUp ya se encarga de cambiar de estado.
	}
	else if (structGameState->enumGameState == EClientGameState::SendOrdersToServer) {
		UE_LOG(LogTemp, Log, TEXT("Game Manager State: SendOrdersToServer"));
			if (!playersManager->MyPlayerInfo->turnOrders) {
				UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: SendOrdersToServer: Error, no hay turn orders para enviar"));
			}

			FStructTurnOrders turnsOrdersToSend = FStructTurnOrders();
			turnsOrdersToSend = *(playersManager->MyPlayerInfo->turnOrders);

			bool sentOk = networkManager->SendNow(&turnsOrdersToSend, 0);

			if (sentOk)  //Si se ha enviado bien el paquete - Vaciar el PlayersInfo->turnOrders y return true
			{
				playersManager->MyPlayerInfo->turnOrders = new FStructTurnOrders();
				ChangeState(EClientGameState::WaitingServer);
			}

		
		
	}
	else if (structGameState->enumGameState == EClientGameState::WaitingServer) {
		UE_LOG(LogTemp, Log, TEXT("Game Manager State: WaitingServer"));
		if (_GAMEINSTANCE->structClientState->updateTurnReceived) {
			UE_LOG(LogTemp, Log, TEXT("Game Manager State: WaitingServer: Avanzando por reconexion a UpdateInfo"));
			structGameState->update_turn = _GAMEINSTANCE->structClientState->update_turn_Reconnect;

			ChangeState(EClientGameState::UpdateInfo);
		}
	}
	else if (structGameState->enumGameState == EClientGameState::UpdateInfo) {
		UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: UpdateInfo"));

		UpdateTurn();

		ChangeState(EClientGameState::EndOfTurn);
	}
	else if (structGameState->enumGameState == EClientGameState::EndOfTurn) {
		UE_LOG(LogTemp, Log, TEXT("Game Manager State: EndOfTurn"));
		if  (_GAMEINSTANCE->structClientState->updateTurnReceived) {
			_GAMEINSTANCE->structClientState->updateTurnReceived = false;
			ChangeState(EClientGameState::GenerateOrders_Start);
		}
			
	}
	else //Caso indeterminado
	{
		//UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: WARNING: estado sin inicializacion"));
	}

	//UpdateState();
}



#pragma endregion


#pragma region SEND TO SERVER FUNCTIONS

bool PD_GM_GameManager::Send_FStructClientMapAlreadyInstantiated() {
	FStructClientMapAlreadyInstantiated msg = FStructClientMapAlreadyInstantiated();
	return networkManager->SendNow(&msg, 0);
}


#pragma endregion 

