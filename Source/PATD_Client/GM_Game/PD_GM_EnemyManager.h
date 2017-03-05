// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

 //forward declaration
#include "Structs/PD_ClientEnums.h"
struct FStructTurnOrders;
class PD_GM_LogicCharacter;

class PATD_CLIENT_API PD_GM_EnemyManager
{
private:
	//Quizas podriamos hacer un StructEnemy para guardar informacion asociada que podamos querer de un enemigo. (Las ordenes por ejemplo)
	TArray<PD_GM_LogicCharacter*> enemies;
	TArray<FStructTurnOrders*> listTurnOrders;
public:
	PD_GM_EnemyManager();
	~PD_GM_EnemyManager();

	void AddEnemy(PD_GM_LogicCharacter* enemy);
	TArray<PD_GM_LogicCharacter*> GetEnemies();
	FStructTurnOrders* GetTurnOrders(int indexEnemy);
	int GetEnemyMaxLenghtActions(EActionPhase phase);
	int GetMaxLenghtActions(EActionPhase phase);

	PD_GM_LogicCharacter* GetCharacterByID(FString id);
};

