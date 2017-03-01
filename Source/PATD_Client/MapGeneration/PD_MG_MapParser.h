// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"


#pragma region Class_References

class PD_MG_StaticMap;
class PD_MG_DynamicMap;
class PD_GM_EnemyManager;
#pragma endregion 


class PATD_CLIENT_API PD_MG_MapParser
{
private:
	PD_MG_StaticMap* Parsing_v_0_1(TArray<FString> fileReaded, PD_MG_StaticMap* staticMapRef, PD_MG_DynamicMap* DynamicMapRef, PD_GM_EnemyManager* enemyMan);

	uint32 ReadRawMap(TArray<FString> fileReaded, uint32 firstIndex, PD_MG_StaticMap* staticMapRef);

	uint32 ReadEnemiesMap(TArray<FString> fileReaded, uint32 nextIndexRead, PD_MG_DynamicMap* dynamicMapRef, PD_GM_EnemyManager* enemyMan);

	uint32 ReadInteraciveObjectMap(TArray<FString> fileReaded, uint32 nextIndexRead, PD_MG_DynamicMap* dynamicMapRef);

public:
	PD_MG_MapParser();
	~PD_MG_MapParser();

	PD_MG_StaticMap* StartParsingFromFile(FString* filepath);
	
	PD_MG_StaticMap* StartParsingFromFile(FString* filepath, PD_MG_StaticMap*  staticMapRef, PD_MG_DynamicMap* DynamicMapRef, PD_GM_EnemyManager* enemyMan);

	PD_MG_StaticMap* StartParsingFromChorizo(FString* filepath, PD_MG_StaticMap*  staticMapRef, PD_MG_DynamicMap* DynamicMapRef, PD_GM_EnemyManager* enemyMan);
};
