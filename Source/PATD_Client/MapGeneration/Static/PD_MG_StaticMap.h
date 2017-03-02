// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MapGeneration/PD_MG_LogicPosition.h"

/**
 * 
 */

 /// CLASS REFERENCES
class PD_MG_LogicPosition;

UENUM(BlueprintType)        //"BlueprintType" is essential to include
enum class TypeOfMission : uint8
{
	DefeatBoss     UMETA(DisplayName = "DefeatBoss"),
	DefeatAll    UMETA(DisplayName = "DefeatAll"),
	RecoverTreasure     UMETA(DisplayName = "RecoverTreasure")
};


class PATD_CLIENT_API PD_MG_StaticMap
{
private:

	uint32 _Height, _Width;
	TypeOfMission _typeMission;

	TMap<PD_MG_LogicPosition, TCHAR> _xymap;


	TArray<PD_MG_LogicPosition> _LogicPositionsRefs;

	//Agregado para el hito2 MCG
	FString mapString;

public:
	PD_MG_StaticMap();
	~PD_MG_StaticMap();

#pragma region GET_SET

	TypeOfMission GetTypeMission() { return _typeMission; }
	void SetTypeMission(TypeOfMission val) { _typeMission = val; }

	uint32 GetHeight() { return _Height; }
	uint32 GetWidth() { return _Width; }

	void SetHeight(uint32 val) { _Height = val; }
	void SetWidth(uint32 val) { _Width = val; }

	TMap<PD_MG_LogicPosition, TCHAR> GetXYMap() { return _xymap; }
	TArray<PD_MG_LogicPosition> GetLogicPositions() { return _LogicPositionsRefs; }

	//Agregado para el hito2 MCG
	FString GetMapString();
	void SetMapString(FString mapStringIn);

#pragma endregion


	PD_MG_LogicPosition AddNewLogicPosition(uint32 x, uint32 y, TCHAR c);

	bool Clear();
};
