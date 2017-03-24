// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_GM_GameManager.h"
#include "PD_GM_MapManager.h"
#include "PD_GM_EnemyManager.h"
#include "PD_PlayersManager.h"
#include "NW_Networking/PD_NW_NetworkManager.h"
#include "Structs/PD_ClientStructs.h" 
#include "Structs/PD_ClientEnums.h" 

//Includes de uso de objetos
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "LogicCharacter/PD_GM_LogicCharacter.h"
#include "Actors/PD_GenericController.h"
#include "Actors/Players/PD_CharacterController.h"

PD_GM_GameManager::PD_GM_GameManager(PD_GM_MapManager* inMapManager, PD_PlayersManager* inPlayerManager, PD_NW_NetworkManager* networkManager)
{
	UE_LOG(LogTemp, Log, TEXT("Constructor Game Manager"));

	playersManager = inPlayerManager;
	mapManager =  inMapManager;
	mapManager->_GAMEMANAGER = this;
	enemyManager = new PD_GM_EnemyManager();
	networkManager->RegisterObserver(this);
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
	structGameState = new StructGameState();
	ChangeState(EClientGameState::Instantiate_Map);
}

// Inicializa la maquina de estados.
void PD_GM_GameManager::InitPhase() {
	UE_LOG(LogTemp, Log, TEXT("InitState Game Manager"));
	structGamePhase = new StructGamePhase();
	ChangePhase(EClientGamePhase::ConsumablePhase);
}

#pragma region GM STATE MACHINE 

// Esta funcion cambia el estado actual al pasado por parametros y llama al OnBeginState del mismo.
void PD_GM_GameManager::ChangeState(EClientGameState newState) {
	structGameState->enumGameState = newState;
	OnBeginState();
}

// Dado un paquete de red, actualiza el estado correspondiente y realiza las acciones pertinentes. 
void PD_GM_GameManager::HandleEvent(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {
	//Estos dos eventos van aqui?
	if (structGameState->enumGameState == EClientGameState::Instantiate_Map) {
		// Si se recibe del servidor un Start_Match, ir a ese estado. 
		if (inEventType == UStructType::FStructClientStartMatchOnGM) {
			ChangeState(EClientGameState::Start_Match);
		}
	}
	else if(structGameState->enumGameState == EClientGameState::Start_Match) {
		if (inEventType == UStructType::FStructClientCanGenerateOrders) {
			InitPhase(); //Inicializa la maquina de estados de fase de juego (primero consumablePhase)
			ChangeState(EClientGameState::GenerateOrders);
		 }
	}	 
	else if(structGameState->enumGameState == EClientGameState::GenerateOrders) {
		 
	}	 
	else if(structGameState->enumGameState == EClientGameState::SendOrdersToServer) {
		 
	}	 
	else if(structGameState->enumGameState == EClientGameState::WaitingServer) {
		if (inEventType == UStructType::FStructUpdateTurn) {
			FStructUpdateTurn* updateTurn = (FStructUpdateTurn*)inDataStruct;
			structGameState->update_turn = *updateTurn;

			ChangeState(EClientGameState::UpdateInfo);
		}
	}	 
	else if(structGameState->enumGameState == EClientGameState::UpdateInfo) {
		 
	}	 
	else if(structGameState->enumGameState == EClientGameState::EndOfTurn) {

	}


}

// Actualiza el estado actual
void PD_GM_GameManager::UpdateState() {

	if (structGameState->enumGameState == EClientGameState::Instantiate_Map) {

	}
	else if (structGameState->enumGameState == EClientGameState::Start_Match) {

	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders) {
		
	}
	else if (structGameState->enumGameState == EClientGameState::SendOrdersToServer) {
		ChangeState(EClientGameState::WaitingServer);
	}
	else if (structGameState->enumGameState == EClientGameState::WaitingServer) {

	}
	else if (structGameState->enumGameState == EClientGameState::UpdateInfo) {
		ChangeState(EClientGameState::EndOfTurn);
	}
	else if (structGameState->enumGameState == EClientGameState::EndOfTurn) {
		ChangeState(EClientGameState::GenerateOrders);
	}
	else { //Caso indeterminado
		UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::UpdateState: WARNING: estado sin update"));
	}
}

// Funcion que se llama una sola vez al entrar en un estado. Llamado desde ChangeState. 
void PD_GM_GameManager::OnBeginState() {


	if (structGameState->enumGameState == EClientGameState::Instantiate_Map) {
		mapManager->InstantiateMap();
	}
	else if (structGameState->enumGameState == EClientGameState::Start_Match) {
		
	}
	else if (structGameState->enumGameState == EClientGameState::GenerateOrders) {
		//ChangeState(EClientGameState::SendOrdersToServer); //Cambiar a cuando se le da a aceptar todas las ordenes.
	}
	else if (structGameState->enumGameState == EClientGameState::SendOrdersToServer) {
		// 
	}
	else if (structGameState->enumGameState == EClientGameState::WaitingServer) {
		
	}
	else if (structGameState->enumGameState == EClientGameState::UpdateInfo) {
		UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: UpdateInfo"));

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
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: ERROR: No se identifica el character de player con id %s"), *updateCharacter.ID_character);
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: Updateando Enemigos"));
		
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
	else if (structGameState->enumGameState == EClientGameState::EndOfTurn) {

	}
	else //Caso indeterminado
	{
		//UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: WARNING: estado sin inicializacion"));
	}

	UpdateState();
}

#pragma endregion

#pragma region GM PHASE MACHINE 

// Esta funcion cambia el estado actual al pasado por parametros y llama al OnBeginState del mismo.
void PD_GM_GameManager::ChangePhase(EClientGamePhase newPhase) 
{
	structGamePhase->enumGamePhase = newPhase;
	OnBeginPhase();
}

void PD_GM_GameManager::UpdatePhase() 
{
	if (structGamePhase->enumGamePhase == EClientGamePhase::ConsumablePhase) 
	{
		ChangePhase(EClientGamePhase::MovementPhase);
	}
	else if (structGamePhase->enumGamePhase == EClientGamePhase::MovementPhase) 
	{
		ChangePhase(EClientGamePhase::InteractionPhase);
	}
	else if (structGamePhase->enumGamePhase == EClientGamePhase::InteractionPhase)
	{
		ChangePhase(EClientGamePhase::ActionPhase);
	}
	else if (structGamePhase->enumGamePhase == EClientGamePhase::ActionPhase)
	{
		/*
		Cuando actualizas el estado de Accion es para enviar todo al servidor.
		*/
		ChangeState(EClientGameState::SendOrdersToServer); 
	}
	else //Caso indeterminado
	{
		UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::UpdatePhase: WARNING: estado sin inicializacion"));
	}

}

void PD_GM_GameManager::OnBeginPhase() 
{
	if (structGamePhase->enumGamePhase == EClientGamePhase::ConsumablePhase)
	{
		//Poner un Consumable Widget
	}
	else if (structGamePhase->enumGamePhase == EClientGamePhase::MovementPhase)
	{
		//Poner un Movement Widget

	}
	else if (structGamePhase->enumGamePhase == EClientGamePhase::InteractionPhase)
	{
		//Poner un Interaction Widget

	}
	else if (structGamePhase->enumGamePhase == EClientGamePhase::ActionPhase)
	{
		//Poner un Action Widget

	}
	else //Caso indeterminado
	{
		UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::UpdatePhase: WARNING: estado sin inicializacion"));
	}

}
#pragma endregion
