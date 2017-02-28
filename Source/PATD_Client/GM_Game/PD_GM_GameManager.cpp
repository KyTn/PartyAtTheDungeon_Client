// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_GM_GameManager.h"
#include "PD_GM_MapManager.h"
#include "PD_GM_EnemyManager.h"
#include "PD_PlayersManager.h"
#include "Structs/PD_ClientStructs.h" 
#include "Structs/PD_ClientEnums.h" 



PD_GM_GameManager::PD_GM_GameManager(PD_GM_MapManager* inMapManager, PD_GM_EnemyManager* inEnemyManager, PD_PlayersManager* inPlayerManager)
{

	playerManager = inPlayerManager;
	mapManager =  inMapManager;
	enemyManager = inEnemyManager;

	InitState();
}

PD_GM_GameManager::~PD_GM_GameManager()
{
}


bool PD_GM_GameManager::SuscribeToEvents(int inPlayer, UStructType inType) {
	if (inType == UStructType::FStructTurnOrders) return true; //Suscrito a TurnOrders
	else return false;
}

// Inicializa la maquina de estados.
void PD_GM_GameManager::InitState() {
	structGameState = new StructGameState();
	ChangeState(EGameState::Instantiate_Map);
}

#pragma region GM STATE MACHINE 

// Esta funcion cambia el estado actual al pasado por parametros y llama al OnBeginState del mismo.
void PD_GM_GameManager::ChangeState(EGameState newState) {
	structGameState->enumGameState = newState;
	OnBeginState();
}

// Dado un paquete de red, actualiza el estado correspondiente y realiza las acciones pertinentes. 
void PD_GM_GameManager::HandleEvent(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) {
	if (structGameState->enumGameState == EGameState::Instantiate_Map) {
		// Si se recibe del servidor un Start_Match, ir a ese estado. 
		if (inEventType == UStructType::FStructStartMatchOnGM) {
			ChangeState(EGameState::Start_Match);
		}
	}
	else if(structGameState->enumGameState == EGameState::Start_Match) {
		if (inEventType == UStructType::FStructClientCanGenerateOrders) {
			ChangeState(EGameState::GenerateOrders);
		 }
	}	 
	else if(structGameState->enumGameState == EGameState::GenerateOrders) {
		 
	}	 
	else if(structGameState->enumGameState == EGameState::SendOrdersToServer) {
		 
	}	 
	else if(structGameState->enumGameState == EGameState::WaitingServer) {
		 
	}	 
	else if(structGameState->enumGameState == EGameState::UpdateInfo) {
		 
	}	 
	else if(structGameState->enumGameState == EGameState::EndOfTurn) {

	}


}

// Actualiza el estado actual
void PD_GM_GameManager::UpdateState() {

	if (structGameState->enumGameState == EGameState::Instantiate_Map) {

	}
	else if (structGameState->enumGameState == EGameState::Start_Match) {

	}
	else if (structGameState->enumGameState == EGameState::GenerateOrders) {

	}
	else if (structGameState->enumGameState == EGameState::SendOrdersToServer) {

	}
	else if (structGameState->enumGameState == EGameState::WaitingServer) {

	}
	else if (structGameState->enumGameState == EGameState::UpdateInfo) {

	}
	else if (structGameState->enumGameState == EGameState::EndOfTurn) {

	}
	else { //Caso indeterminado
		UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::UpdateState: WARNING: estado sin update"));
	}
}

// Funcion que se llama una sola vez al entrar en un estado. Llamado desde ChangeState. 
void PD_GM_GameManager::OnBeginState() {


	if (structGameState->enumGameState == EGameState::Instantiate_Map) {
		mapManager->InstantiateMap();
	}
	else if (structGameState->enumGameState == EGameState::Start_Match) {

	}
	else if (structGameState->enumGameState == EGameState::GenerateOrders) {

		// Load UI


	}
	else if (structGameState->enumGameState == EGameState::SendOrdersToServer) {

	}
	else if (structGameState->enumGameState == EGameState::WaitingServer) {

	}
	else if (structGameState->enumGameState == EGameState::UpdateInfo) {

	}
	else if (structGameState->enumGameState == EGameState::EndOfTurn) {

	}
	else //Caso indeterminado
	{
		//UE_LOG(LogTemp, Warning, TEXT("PD_GM_GameManager::OnBeginState: WARNING: estado sin inicializacion"));
	}

}

#pragma endregion


