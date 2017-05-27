// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_GM_EnemyManager.h"

#include "Structs/PD_ClientStructs.h"//Para todos los structs
#include "GM_Game/LogicCharacter/PD_GM_LogicCharacter.h"

PD_GM_EnemyManager::PD_GM_EnemyManager()
{
}

PD_GM_EnemyManager::~PD_GM_EnemyManager()
{
}

void PD_GM_EnemyManager::AddEnemy(PD_GM_LogicCharacter* enemy) {
	enemies.Add(enemy);

}

void PD_GM_EnemyManager::DeleteEnemy(PD_GM_LogicCharacter* enemy) {
	enemies.Remove(enemy);
	enemy->~PD_GM_LogicCharacter();
}

TArray<PD_GM_LogicCharacter*> PD_GM_EnemyManager::GetEnemies() {
	return enemies;
}


FStructTurnOrders* PD_GM_EnemyManager::GetTurnOrders(int indexEnemy) {
	return listTurnOrders[indexEnemy];
}


//Devuelve el numero de acciones de la lista mas larga
int PD_GM_EnemyManager::GetMaxLenghtActions(EActionPhase phase) {
	int indexplayer = this->GetEnemyMaxLenghtActions(phase);
	if (indexplayer != -1) {
		if (phase == EActionPhase::Move) {
			return this->listTurnOrders[indexplayer]->positionsToMove.Num();
		}
		else if (phase == EActionPhase::Attack) {
			return this->listTurnOrders[indexplayer]->actions.Num();
		}
	}
	return 0;


}
//Devuelve el index del jugador con la lista de acciones mas larga
int PD_GM_EnemyManager::GetEnemyMaxLenghtActions(EActionPhase phase) {
	int numTicks = 0;
	int indexPlayer = -1;

	for (int i = 0; this->GetEnemies().Num(); i++) {
		TArray<FStructOrderAction> listActions;
		if (phase == EActionPhase::Move) {
			//listActions = this->listTurnOrders[i]->positionsToMove;
		}
		else if (phase == EActionPhase::Attack) {
			//listActions = this->listTurnOrders[i]->actions;
		}


		if (numTicks < (listActions.Num())) {
			numTicks = listActions.Num();
			indexPlayer = i;
		};
	}
	return numTicks;
}

PD_GM_LogicCharacter* PD_GM_EnemyManager::GetCharacterByID(FString id) {
	for (int i = 0; i < enemies.Num(); i++) {
		if (enemies[i]->GetIDCharacter() == id) {
			return enemies[i];
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("PlayerManager:: GetCharacterByID ERROR: No se ha encontrado character con id %s"), *id);
	return nullptr;
}