// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_MG_DynamicMap.h"
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "GM_Game/LogicCharacter/PD_GM_LogicCharacter.h"
#include "Structs/PD_ClientEnums.h"

PD_MG_DynamicMap::PD_MG_DynamicMap()
{
	//_rooms = new TArray<PD_MG_Room*>();
	//_LogicPositionsRefs = new TArray<PD_MG_LogicPosition*>();
}

PD_MG_DynamicMap::~PD_MG_DynamicMap()
{
}

void PD_MG_DynamicMap::AddNewEnemy(PD_MG_LogicPosition* lp, ECharacterType type, FString ID_Character) {

	_LogicPositionsRefs.Add(lp);

	struct StructEnemy  structEnemy = StructEnemy();
	structEnemy.currentPosition = lp;
	structEnemy.ID_Character = ID_Character;
	structEnemy.type_Character = type;

	enemies.Add(*lp, structEnemy);
}

void PD_MG_DynamicMap::UpdateActor(AActor* actor, PD_MG_LogicPosition* lp) {

	/*	TArray<	PD_GM_LogicCharacter*> chs = enemies[*lp];
	PD_GM_IALogicCharacter* ch = chs[0];
	ch->UpdateCharacter(actor);
	chs.Reset();
	chs.Add(ch);
	enemies.Emplace(*lp,chs);
	*/
}

bool PD_MG_DynamicMap::Clear() { return false; }