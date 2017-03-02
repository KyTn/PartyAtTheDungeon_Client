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

struct StructEnemy {

	// indica el player que es
	FString ID_Character;
	ECharacterType type_Character;

	PD_MG_LogicPosition currentPosition;

	StructEnemy() {
		ID_Character = "NoCharacter";
		type_Character = ECharacterType::NoCharacter;
		currentPosition = PD_MG_LogicPosition(0, 0);
	}

	StructEnemy(FString id, ECharacterType type, PD_MG_LogicPosition cupos) {
		ID_Character = id;
		type_Character = type;
		currentPosition = cupos;
	}

};


class PATD_CLIENT_API PD_MG_DynamicMap
{
private:

	uint32 _Height, _Width;
	TMap<PD_MG_LogicPosition, StructEnemy> enemies;
	TArray<PD_MG_LogicPosition> _LogicPositionsRefs;

public:
	PD_MG_DynamicMap();
	~PD_MG_DynamicMap();

#pragma region GET_SET

	uint32 GetHeight() { return _Height; }
	uint32 GetWidth() { return _Width; }

	void SetHeight(uint32 val) { _Height = val; }
	void SetWidth(uint32 val) { _Width = val; }

	TMap<PD_MG_LogicPosition, StructEnemy> getEnemies() { return enemies; }
	TArray<PD_MG_LogicPosition> GetLogicPositions() { return _LogicPositionsRefs; }

#pragma endregion

	//PD_MG_LogicPosition* AddNewLogicPosition(uint32 x, uint32 y, TCHAR c);

	void AddNewEnemy(PD_MG_LogicPosition lp, ECharacterType type, FString ID_Character);

	void UpdateActor(AActor* actor, PD_MG_LogicPosition lp);

	bool Clear();
};
