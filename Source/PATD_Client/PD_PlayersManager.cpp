// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_PlayersManager.h"

#include "Structs/PD_ClientStructs.h"//Para todos los structs
#include "GM_Game/LogicCharacter/PD_GM_LogicCharacter.h"

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

	if (conn.ID_character.Compare(GetMyCharacter()->GetIDCharacter()) == 0) {
		UE_LOG(LogTemp, Warning, TEXT("PD_GM_MapManager::AddNewPlayer - Adding the client player (id:%s) character at(%d,%d)"), *conn.ID_character, conn.logicPosition.positionX, conn.logicPosition.positionY);
		GetMyCharacter()->SetCurrentLogicalPosition(PD_MG_LogicPosition(conn.logicPosition.positionX, conn.logicPosition.positionY));
		dataPlayers.Add(MyPlayerInfo);
	}
	else {
		PD_GM_LogicCharacter* ch = new PD_GM_LogicCharacter();

		UE_LOG(LogTemp, Warning, TEXT("PD_GM_MapManager::AddNewPlayer - Adding player at(%d,%d)"), conn.logicPosition.positionX, conn.logicPosition.positionY);
		ch->SetCurrentLogicalPosition(PD_MG_LogicPosition(conn.logicPosition.positionX, conn.logicPosition.positionY));
		ch->SetIDCharacter(conn.ID_character);

		StructPlayer* sp = new StructPlayer();
		sp->ID_Player = conn.playerNum;

		sp->logic_Character = ch;
		dataPlayers.Add(sp);
	}
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


PD_GM_LogicCharacter* PD_PlayersManager::GetCharacterByID(FString id) {
	for (int i = 0; i < dataPlayers.Num(); i++) {
		if (dataPlayers[i]->logic_Character->GetIDCharacter() == id) {
			return dataPlayers[i]->logic_Character;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("PlayerManager:: GetCharacterByID ERROR: No se ha encontrado character con id %s"), *id);
	return nullptr;
}




PD_GM_LogicCharacter* PD_PlayersManager::GetMyCharacter() {
	return MyPlayerInfo->logic_Character;
}



#pragma region ORDER FUNCTIONS



bool PD_PlayersManager::CreateConsumableOrder(int id_consumable)
{
	//Comprobar que ese id_consumable esta dentro de los valores del enumerado en cuestion
	MyPlayerInfo->turnOrders->consumablesToConsume.Add(id_consumable);
	return true;
}


bool PD_PlayersManager::CreateMovementOrder(int positionX, int positionY)
{
	FStructLogicPosition tile = FStructLogicPosition();
	tile.positionX = positionX;
	tile.positionY = positionY;

	if (MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent > 0) 
	{
	if ( (MyPlayerInfo->turnOrders->positionsToMove.Num() == 0) ||   (MyPlayerInfo->turnOrders->positionsToMove.Last().positionX != tile.positionX)
			|| (MyPlayerInfo->turnOrders->positionsToMove.Last().positionY != tile.positionY) )
		{
			MyPlayerInfo->turnOrders->positionsToMove.Add(tile);
			MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent--;
			return true;
		}
		else 
		{
			return false;
		}
	}
	else 
	{
		return false;

	}

}

bool PD_PlayersManager::DeleteLastMovementOrder(int positionX, int positionY)
{
	MyPlayerInfo->turnOrders->positionsToMove.Pop();

	return true;

}


bool PD_PlayersManager::CreateInteractableOrder(int id_interactable)
{
	//Comprobar que ese id_consumable esta dentro de los valores del enumerado en cuestion

	MyPlayerInfo->turnOrders->consumablesToConsume.Add(id_interactable);

	return true;
}


bool PD_PlayersManager::CreateActionToPosition(int id_action, TArray<FVector> positions)
{
	FStructTargetToAction target = FStructTargetToAction();
	target.id_action = id_action;

	for (int i = 0; i < positions.Num(); i++)
	{
		FStructLogicPosition tile = FStructLogicPosition();
		tile.positionX = positions[i].X;
		tile.positionY = positions[i].Y;

		target.positions.Add(tile);
	}


	MyPlayerInfo->turnOrders->actions.Add(target);
	return true;


}

bool PD_PlayersManager::CreateActionToCharacter(int id_action, TArray<FString> id_character)
{
	FStructTargetToAction target = FStructTargetToAction();
	target.id_action = id_action;

	if (MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent > 0)
	{
		for (int i = 0; i < id_character.Num(); i++)
		{
			target.id_character.Add(id_character[i]);
		}
		MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent--;
		MyPlayerInfo->turnOrders->actions.Add(target);
		return true;
	}
	else 
	{
		return false;
	}
	
	return true;

}


bool PD_PlayersManager::ResetAll() {
	bool b = true;

	b = b && ResetConsumables();
	b = b && ResetMovements();
	b = b && ResetInteractuables();
	b = b && ResetActions();

	return b;
}
bool PD_PlayersManager::ResetConsumables() {
	MyPlayerInfo->turnOrders->consumablesToConsume.Empty();
	return MyPlayerInfo->turnOrders->consumablesToConsume.Num() == 0;
}
bool PD_PlayersManager::ResetMovements() {

	MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent += MyPlayerInfo->turnOrders->positionsToMove.Num();

	MyPlayerInfo->turnOrders->positionsToMove.Empty();
	return MyPlayerInfo->turnOrders->positionsToMove.Num() == 0;
}
bool PD_PlayersManager::ResetInteractuables() {
	MyPlayerInfo->turnOrders->interactuablesToInteract.Empty();
	return MyPlayerInfo->turnOrders->interactuablesToInteract.Num() == 0;
}
bool PD_PlayersManager::ResetActions() {

	MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent += MyPlayerInfo->turnOrders->actions.Num();

	MyPlayerInfo->turnOrders->actions.Empty();
	return MyPlayerInfo->turnOrders->actions.Num() == 0;
}

#pragma endregion


