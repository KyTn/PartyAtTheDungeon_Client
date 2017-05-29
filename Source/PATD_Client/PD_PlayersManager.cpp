// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_PlayersManager.h"

#include "Structs/PD_ClientStructs.h"//Para todos los structs
#include "GM_Game/LogicCharacter/PD_GM_LogicCharacter.h"
#include "PD_ClientGameInstance.h"
#include "Actors/PD_E_Character.h"
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



	UE_LOG(LogTemp, Log, TEXT("PD_PlayersManager: CreateMovementOrder: APCurrent:%d "), MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent);
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

bool PD_PlayersManager::DeleteLastMovementOrder()
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
	target.id_character = id_character;

	//Calcular los AP restantes que le quedan despues de realizar la accion
	if (MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent > 0)
	{
		for (int i = 0; i < id_character.Num(); i++)
		{
			target.id_character.Add(id_character[i]);
		}

		int APleft = 0;
		switch (ActiveSkills(id_action))
		{
			case ActiveSkills::BasicAttack:
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT(" Add Basic Attack to Actions"));

				APleft = 1;
				break;
			}
			case ActiveSkills::Defense:
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT(" Add Defense to Actions"));
				APleft = MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent;
				break;
			}
			case ActiveSkills::Hostion:
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT(" Add Defense to Actions"));
				APleft = MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent;
				break;
			}
			default:
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT(" Add others to Actions"));

				for (int j = 0; j < MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills.Num(); j++)
				{
					if (MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills[j].ID_Skill == id_action)
					{
						APleft = APleft + MyPlayerInfo->logic_Character->GetSkills()->listActiveSkills[j].AP;
					}
				}
				break;
			}
		}
		
		UE_LOG(LogTemp, Log, TEXT("PD_PlayersManager: CreateActionToCharacter: APCurrent BEFORE action:%d "), MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent);

		if (MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent - APleft >= 0)
		{
			MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent = MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent - APleft;

			UE_LOG(LogTemp, Log, TEXT("PD_PlayersManager: CreateActionToCharacter: APCurrent AFTER action :%d "), MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent);

			MyPlayerInfo->turnOrders->actions.Add(target);
			return true;
		}
		else  //si es un numero negativo, nos pasamos del AP, no se puede realizar la accion,
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("YOU DO NOT HAVE ENOUGH AP FOR THAT ACTION!"));

			return false;
		
		}
		
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("YOUR ACTION POINTS (AP) ARE EMPTY !."));

		return false;
	}
	
	return true;

}


bool PD_PlayersManager::ResetAll() {
	bool b = true;
	/*
	b = b && ResetConsumables();
	b = b && ResetMovements();
	b = b && ResetInteractuables();
	b = b && ResetActions();
	*/
	delete MyPlayerInfo->turnOrders;
	MyPlayerInfo->turnOrders = new FStructTurnOrders();
	MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent = MyPlayerInfo->logic_Character->GetTotalStats()->APTotal;
	return true;
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



bool PD_PlayersManager::DeleteActionTurn(int index_action)
{
	//Borra una accion de la lista de habilidades escogidas por el player
	//devuelve los AP de la habilidad borrada

	UE_LOG(LogTemp, Log, TEXT("PD_PlayersManager: DeleteActionTurn:"));

	int id_skill = 0;
	id_skill = MyPlayerInfo->turnOrders->actions[index_action].id_action;
	bool actionDelete = false;
	TArray<FStructSkill> skillsInGI = Cast<UPD_ClientGameInstance>(MyPlayerInfo->logic_Character->GetCharacterBP()->GetGameInstance())->activeSkills;
	for (int i = 0; i < skillsInGI.Num(); i++)
	{
		if (skillsInGI[i].ID_Skill == id_skill)
		{
			UE_LOG(LogTemp, Log, TEXT("PD_PlayersManager: DeleteActionTurn: Se ha encontrado skill para eliminar %d"), id_skill);

			if ( (ActiveSkills(skillsInGI[i].ID_Skill) == ActiveSkills::Defense)  || (ActiveSkills(skillsInGI[i].ID_Skill) == ActiveSkills::Hostion))
			{
				UE_LOG(LogTemp, Log, TEXT("PD_PlayersManager: DeleteActionTurn: Se ha encontrado skill - ES DEFENSA O GASTO TODO AP %d"), id_skill);

					//Para calcular el AP gastado, veo todo lo que se ha hecho y la resta de eso menos el AP total, sera el gastado en esta accion
					int APSpentInOneAction = 0;
					APSpentInOneAction = MyPlayerInfo->turnOrders->positionsToMove.Num();
					for (int j = 0; j < MyPlayerInfo->turnOrders->actions.Num(); j++)
					{
						for (int k = 0; k < skillsInGI.Num(); k++)
						{
							if (MyPlayerInfo->turnOrders->actions[j].id_action == skillsInGI[k].ID_Skill)
							{
								APSpentInOneAction = APSpentInOneAction + skillsInGI[k].AP;
							}
						}
					}
					int ApSpent = MyPlayerInfo->logic_Character->GetTotalStats()->APTotal - APSpentInOneAction;
					MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent = MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent + ApSpent;

					UE_LOG(LogTemp, Log, TEXT("PD_PlayersManager: DeleteActionTurn: Se ha encontrado skill gasto tocho - recuperando AP - %d"), MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent);
					actionDelete = true;
			}
			else 
			{
					MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent = MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent + skillsInGI[i].AP;

					UE_LOG(LogTemp, Log, TEXT("PD_PlayersManager: DeleteActionTurn: Se ha encontrado skill - recuperando AP - %d"), MyPlayerInfo->logic_Character->GetTotalStats()->APCurrent);
					actionDelete = true;
			}
		}
	}

	if (actionDelete)
	{
		UE_LOG(LogTemp, Warning, TEXT("PD_PlayersManager: DeleteActionTurn: Elimiando accion"));
		MyPlayerInfo->turnOrders->actions.RemoveAt(index_action);
	}

	return actionDelete;
}

#pragma endregion


