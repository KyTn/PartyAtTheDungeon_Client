// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

DECLARE_STATS_GROUP(TEXT("MapParser"), STATGROUP_MapParser, STATCAT_Advanced);

#pragma region Class_References

class PD_MG_StaticMap;
class PD_MG_DynamicMap;
class PD_GM_EnemyManager;
#pragma endregion 


class PATD_CLIENT_API PD_MG_MapParser
{
private:
	PD_MG_StaticMap* Parsing_v_0_1(TArray<FString> fileReaded, PD_MG_StaticMap* staticMapRef, PD_MG_DynamicMap* DynamicMapRef);

	uint32 ReadRawMap(TArray<FString> fileReaded, uint32 firstIndex, PD_MG_StaticMap* staticMapRef);

	uint32 ReadEnemiesMap(TArray<FString> fileReaded, uint32 nextIndexRead, PD_MG_DynamicMap* dynamicMapRef);

	//uint32 ReadInteraciveObjectMap(TArray<FString> fileReaded, uint32 nextIndexRead, PD_MG_DynamicMap* dynamicMapRef);


	bool Parsing_v_0_2(FStructMapData* NETMAPDATA, PD_MM_MapInfo* MapInfoRef, PD_MG_DynamicMap* DynamicMapRef);

public:
	PD_MG_MapParser();
	~PD_MG_MapParser();

	PD_MG_StaticMap* StartParsingFromFile(FString* filepath);
	PD_MG_StaticMap* StartParsingFromFile(FString* filepath, PD_MG_StaticMap* staticMapRef, PD_MG_DynamicMap* DynamicMapRef);

	PD_MG_StaticMap* StartParsingFromChorizo(FString* filepath, PD_MG_StaticMap* staticMapRef, PD_MG_DynamicMap* DynamicMapRef);


	// parsea un mapa que ha sido enviado desde la red como un paquete de datos, no como un string (file). 
	bool StartParsingFromFStructMapData(FStructMapData* NETMAPDATA, PD_MM_MapInfo * MapInfoRef, PD_MG_DynamicMap* DynamicMapRef);
};
