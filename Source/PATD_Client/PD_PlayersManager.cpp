// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_PlayersManager.h"

#include "Structs/PD_ClientStructs.h"//Para todos los structs


PD_PlayersManager::PD_PlayersManager()
{
}

PD_PlayersManager::~PD_PlayersManager()
{
}

//Funciones de gestion de conexion

int PD_PlayersManager::GetNumPlayers() {
	return dataPlayers.Num();
}

//Devuelve el numero de acciones de la lista mas larga
/*
int PD_PlayersManager::GetMaxLenghtActions(EActionPhase phase) {
	int indexplayer = this->GetPlayerMaxLenghtActions(phase);
	if (indexplayer != -1) {
		if (phase == EActionPhase::Move) {
			return this->GetDataStructPlayer(indexplayer)->turnOrders->listMove.Num();
		}
		else if (phase == EActionPhase::Attack) {
			return this->GetDataStructPlayer(indexplayer)->turnOrders->listAttack.Num();
		}
	}
	return 0;
}
*/



//Devuelve el index del jugador con la lista de acciones mas larga
/*
int PD_PlayersManager::GetPlayerMaxLenghtActions(EActionPhase phase) {
	int numTicks = 0;
	int indexPlayer = -1;

	for (int i = 0; this->GetNumPlayers(); i++) {
		TArray<FStructOrderAction> listActions;
		if (phase == EActionPhase::Move) {
			listActions = this->GetDataStructPlayer(i)->turnOrders->listMove;
		}
		else if (phase == EActionPhase::Attack) {
			listActions = this->GetDataStructPlayer(i)->turnOrders->listAttack;
		}


		if (numTicks < (listActions.Num())) {
			numTicks = listActions.Num();
			indexPlayer = i;
		};
	}
	return numTicks;
}
*/


void PD_PlayersManager::AddNewPlayer(FStructPlayerInfoAtClient conn) {

}



//Funcion de acceso directo al struct
StructPlayer* PD_PlayersManager::GetDataStructPlayer(int player) {
	CheckPlayerIndex(player);
	return dataPlayers[player];
}




//Funciones private
bool PD_PlayersManager::CheckPlayerIndex(int player) {
	if (dataPlayers.IsValidIndex(player) && dataPlayers[player]) {
		return true;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("PlayerManager:: CheckPlayerState ERROR: El indice de player no es valido"));
		return false;
	}
}

