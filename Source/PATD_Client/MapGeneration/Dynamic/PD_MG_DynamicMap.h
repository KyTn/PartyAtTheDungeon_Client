// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "Structs/PD_ClientEnums.h"
/**
 * 
 */

 /// CLASS REFERENCES
class PD_MG_LogicPosition;
class PD_GM_LogicCharacter;

class PATD_CLIENT_API PD_MG_DynamicMap
{

private:

	uint32 _Height, _Width;
	TMap<PD_MG_LogicPosition, TArray<PD_GM_LogicCharacter*>> enemies;
	TArray<PD_MG_LogicPosition*> _LogicPositionsRefs;

public:
	PD_MG_DynamicMap();
	~PD_MG_DynamicMap();

#pragma region GET_SET

	uint32 GetHeight() { return _Height; }
	uint32 GetWidth() { return _Width; }

	void SetHeight(uint32 val) { _Height = val; }
	void SetWidth(uint32 val) { _Width = val; }

	TMap<PD_MG_LogicPosition, TArray<PD_GM_LogicCharacter*>> getEnemies() { return enemies; }
	TArray<PD_MG_LogicPosition*> GetLogicPositions() { return _LogicPositionsRefs; }

#pragma endregion

	//PD_MG_LogicPosition* AddNewLogicPosition(uint32 x, uint32 y, TCHAR c);

	void AddNewEnemy(PD_MG_LogicPosition* lp, PD_GM_LogicCharacter* ch, ECharacterType type);

	void UpdateActor(AActor* actor, PD_MG_LogicPosition* lp);

	bool Clear();
};