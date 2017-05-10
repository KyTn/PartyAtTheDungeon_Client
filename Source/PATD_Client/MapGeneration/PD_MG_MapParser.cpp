// Fill out your copyright notice in the Description page of Project Settings.'


#include "PATD_Client.h"
#include "MapGeneration/PD_MG_MapParser.h"
#include "MapGeneration/Static/PD_MG_StaticMap.h"
#include "MapGeneration/Dynamic/PD_MG_DynamicMap.h"
#include "Structs/PD_ClientEnums.h"
#include "GM_Game/LogicCharacter/PD_GM_LogicCharacter.h"
#include "GM_Game/PD_GM_EnemyManager.h"

//For UE4 Profiler ~ Stat
DECLARE_CYCLE_STAT(TEXT("MapParser ~ StartParsingFromChorizo"), STAT_StartParsingFromChorizo, STATGROUP_MapParser);

PD_MG_MapParser::PD_MG_MapParser()
{
}

PD_MG_MapParser::~PD_MG_MapParser()
{
}


#pragma region Parse From File

PD_MG_StaticMap* PD_MG_MapParser::StartParsingFromFile(FString* filepath)
{
	return StartParsingFromFile(filepath, new PD_MG_StaticMap(), new PD_MG_DynamicMap());
}

PD_MG_StaticMap* PD_MG_MapParser::StartParsingFromFile(FString* filepath, PD_MG_StaticMap* staticMapRef, PD_MG_DynamicMap* dynamicMapRef)
{
	staticMapRef->Clear();
	dynamicMapRef->Clear();
	// Leamos el fichero
	//Estamos metiendo el FPaths::GameDir() aqui y en el parserActor, asi que solo hay que ponerlo en uno de los sitios
	FString FilePath = FPaths::GameDir() + *filepath;
	FString FileData = "";

	if (FFileHelper::LoadFileToString(FileData, *FilePath))
	{
		//Agregado para el hito2 MCG
		//UE_LOG(LogTemp, Warning, TEXT("PD_MG_MapParser::StartParsingFromFile::  Llamando a SetMapString . Path :%s  Mapa: %s"), *FilePath, *FileData);
		staticMapRef->SetMapString(FileData);

		// Enviar a los clientes el mapa leido ... 


		// Obtenemos la version del parser que se debe usar. 

		TArray<FString> fileSplitted;
		FileData.ParseIntoArray(fileSplitted, TEXT("\n"), true);

		if (fileSplitted[0].Contains("v0.1")) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Using parser version " + fileSplitted[0]);

			Parsing_v_0_1(fileSplitted, staticMapRef, dynamicMapRef);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No parser version registered! Searching for " + fileSplitted[0]);
		}

	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("PD_MG_MapParser::StartParsingFromFile::  Error loading map! Failed to load file!. Path :%s "), *FilePath);

	}


	return staticMapRef;
}


PD_MG_StaticMap* PD_MG_MapParser::StartParsingFromChorizo(FString* chorizo, PD_MG_StaticMap*  staticMapRef, PD_MG_DynamicMap* dynamicMapRef) {

	SCOPE_CYCLE_COUNTER(STAT_StartParsingFromChorizo);

	staticMapRef->Clear();
	dynamicMapRef->Clear();
	//Agregado para el hito2 MCG
	//UE_LOG(LogTemp, Warning, TEXT("PD_MG_MapParser::StartParsingFromChorizo::  Llamando a SetMapString. Mapa: %s"), *chorizo);
	staticMapRef->SetMapString(*chorizo);

	// Obtenemos la version del parser que se debe usar. 

	TArray<FString> fileSplitted;
	chorizo->ParseIntoArray(fileSplitted, TEXT("\n"), true);

	if (fileSplitted.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("PD_MG_MapParser::StartParsingFromChorizo::  Error loading map! chorizo=%s "), chorizo);

	}

	if (fileSplitted[0].Contains("v0.1")) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Using parser version " + fileSplitted[0]);

		Parsing_v_0_1(fileSplitted, staticMapRef, dynamicMapRef);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No parser version registered! Searching for " + fileSplitted[0]);
	}




	return staticMapRef;
}



#pragma endregion


#pragma region VERSION OF PARSERS

PD_MG_StaticMap* PD_MG_MapParser::Parsing_v_0_1(TArray<FString> fileReaded, PD_MG_StaticMap* staticMapRef, PD_MG_DynamicMap* dynamicMapRef)
{
	//if (!fileReaded[0].Equals("v0.1")) { return staticMapRef; }

	uint32 nextIndexRead = 0;
	uint32 mapIndex = 0;
	// Cual es el tipo de misión para este mapa?
	if (fileReaded[1].Contains("defeatBoss")) {
		staticMapRef->SetTypeMission(MATCHCONFIG_MISSIONTYPE::DefeatBoss);
	}
	else if (fileReaded[1].Contains("defeatAll")) {
		staticMapRef->SetTypeMission(MATCHCONFIG_MISSIONTYPE::DefeatAll);
	}
	else if (fileReaded[1].Contains("recoverTreasure")) {
		staticMapRef->SetTypeMission(MATCHCONFIG_MISSIONTYPE::RecoverTreasure);
	}

	// Tamaño de la matriz
	staticMapRef->SetHeight((uint32)FCString::Atoi(*(fileReaded[2])));
	dynamicMapRef->SetHeight((uint32)FCString::Atoi(*(fileReaded[2])));
	staticMapRef->SetWidth((uint32)FCString::Atoi(*(fileReaded[3])));
	dynamicMapRef->SetWidth((uint32)FCString::Atoi(*(fileReaded[3])));

	nextIndexRead = 4;
	mapIndex = nextIndexRead;
	// Cargamos los caractares planos sin referencias de comportamiento
	nextIndexRead = ReadRawMap(fileReaded, nextIndexRead, staticMapRef);

	//Cargamos los enemigos que hay en el map, y los instanciamos en el enemyManager
	nextIndexRead = ReadEnemiesMap(fileReaded, nextIndexRead, dynamicMapRef);

	//Cargamos los objetos interactuables en el mapa
	//nextIndexRead = ReadInteractiveObjectMap(fileReaded, nextIndexRead, dynamicMapRef);


	return staticMapRef;
}

#pragma endregion 

//Rellena el diccionario interno del staticMapRef con la info en crudo del mapa, sin referencias de comportamiento. 
uint32 PD_MG_MapParser::ReadRawMap(TArray<FString> fileReaded, uint32 firstIndex, PD_MG_StaticMap* staticMapRef)
{

	for (uint32 i = firstIndex; i < staticMapRef->GetHeight() + firstIndex; i++) {
		TArray<TCHAR> ta = fileReaded[i].GetCharArray();
		for (uint32 j = 0; j < staticMapRef->GetWidth(); j++) {
			staticMapRef->AddNewLogicPosition(i - firstIndex, j, ta[j]);
		}
	}
	return staticMapRef->GetHeight() + firstIndex;
}

//Rellena la parte de los enemigos del DynamicMap pasado por parametros
uint32 PD_MG_MapParser::ReadEnemiesMap(TArray<FString> fileReaded, uint32 firstIndex, PD_MG_DynamicMap* dynamicMapRef) {
	TArray <TCHAR> enemyLine;
	FString f;
	uint32 Enemynum = (uint32)FCString::Atoi(*(fileReaded[firstIndex]));
	uint32 intType, x, y;
	int32 num, j;
	firstIndex++;
	ECharacterType type;
	for (uint32 i = firstIndex; i < Enemynum + firstIndex; i++) {
		/*enemyValue = fileReaded[i].RightChop(2);
		enemyValue.RemoveAt(enemyValue.Len()-1);//Hay que hacer esto, dado que guardaba basura, un "\n", o un "\0" que se comportaba al hacer el cout como "\n"
		dynamicMapRef->AddEnemyDictionary(fileReaded[i].GetCharArray()[0], enemyValue);*/
		enemyLine = fileReaded[i].GetCharArray();
		enemyLine.RemoveAt(enemyLine.Num() - 1);
		//enemyLine.RemoveAt(enemyLine.Num() - 1);

		j = 0; num = 1; intType = 0; x = 0; y = 0;
		while (enemyLine[j] != ':')
		{
			intType = enemyLine[j] - '0' + (intType*num);
			j++;
			num = num * 10;
		}
		type = ECharacterType(intType);
		num = 1; j++;
		while (enemyLine[j] != ',')
		{
			x = enemyLine[j] - '0' + (x*num);
			j++;
			num = num * 10;
		}
		num = 1; j++;
		while (j<enemyLine.Num())
		{
			y = enemyLine[j] - '0' + (y*num);
			j++;
			num = num * 10;
		}
		PD_MG_LogicPosition lp = PD_MG_LogicPosition(x, y);
		switch (type) {//En este switch metemos la IA lógica de cada uno
		case ECharacterType::Archer: {
			FString id = "Arch" + FString::FromInt(i);	
			//enemyMan->AddEnemie(ch);
			dynamicMapRef->AddNewEnemy(lp, type, id);
			break;
		}
		case ECharacterType::Zombie: {
			FString id = "Zomb" + FString::FromInt(i);
			//enemyMan->AddEnemie(ch);
			dynamicMapRef->AddNewEnemy(lp, type, id);
			break;
		}
		}
	}
	return firstIndex + Enemynum + 1;
}






/*uint32 PD_MG_MapParser::ReadInteraciveObjectMap(TArray<FString> fileReaded, uint32 firstIndex, PD_MG_DynamicMap* dynamicMapRef) {
TArray <TCHAR> intObjLine;
FString f;
uint32 intObjNum = (uint32)FCString::Atoi(*(fileReaded[firstIndex]));
uint32 intType, x, y;
int32 num, j;
firstIndex++;
for (uint32 i = firstIndex; i < intObjNum + firstIndex; i++) {
enemyValue = fileReaded[i].RightChop(2);
enemyValue.RemoveAt(enemyValue.Len()-1);//Hay que hacer esto, dado que guardaba basura, un "\n", o un "\0" que se comportaba al hacer el cout como "\n"
dynamicMapRef->AddEnemyDictionary(fileReaded[i].GetCharArray()[0], enemyValue);
intObjLine = fileReaded[i].GetCharArray();
intObjLine.RemoveAt(intObjLine.Num() - 1);
intObjLine.RemoveAt(intObjLine.Num() - 1);

///Falta decidir como se va a definir en el fichero, el parseo y guardarlo en el mapa dinámico
}
return firstIndex + intObjNum + 1;
}*/