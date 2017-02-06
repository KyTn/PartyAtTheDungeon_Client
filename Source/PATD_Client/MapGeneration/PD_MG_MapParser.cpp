// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PATD_Client/MapGeneration/PD_MG_MapParser.h"
#include "PATD_Client/MapGeneration/PD_MG_StaticMap.h"
#include "PATD_Client/MapGeneration/ParserActor.h"

PD_MG_MapParser::PD_MG_MapParser()
{
}

PD_MG_MapParser::~PD_MG_MapParser()
{
}


PD_MG_StaticMap* PD_MG_MapParser::StartParsingFromFString(FString* data) {
	return StartParsingFromFString(data, new PD_MG_StaticMap());
}


PD_MG_StaticMap* PD_MG_MapParser::StartParsingFromFString(FString* data, PD_MG_StaticMap* StaticMapRef) {
	StaticMapRef->Clear();

	// Obtenemos la version del parser que se debe usar. 

	TArray<FString> fileSplitted;

	UE_LOG(LogTemp, Warning, TEXT("StartParsingFromFString %s"), data);

	data->ParseIntoArray(fileSplitted, TEXT("\n"), true);

	if (fileSplitted[0].Contains("v0.1")) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Using parser version " + fileSplitted[0]);

		Parsing_v_0_1(fileSplitted, StaticMapRef);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No parser version registered! Searching for " + fileSplitted[0]);
	}


	return StaticMapRef;
}



PD_MG_StaticMap* PD_MG_MapParser::Parsing_v_0_1(TArray<FString> fileReaded, PD_MG_StaticMap* staticMapRef)
{
	//if (!fileReaded[0].Equals("v0.1")) { return staticMapRef; }

	uint32 nextIndexRead = 0;

	// Cual es el tipo de misión para este mapa?
	if (fileReaded[1].Contains("defeatBoss")) {
		staticMapRef->SetTypeMission(TypeOfMission::DefeatBoss);
	}
	else if (fileReaded[1].Contains("defeatAll")) {
		staticMapRef->SetTypeMission(TypeOfMission::DefeatAll);
	}
	else if (fileReaded[1].Contains("recoverTreasure")) {
		staticMapRef->SetTypeMission(TypeOfMission::RecoverTreasure);
	}

	// Tamaño de la matriz
	staticMapRef->SetHeight((uint32)FCString::Atoi(*(fileReaded[2])));
	staticMapRef->SetWidth((uint32)FCString::Atoi(*(fileReaded[3])));

	nextIndexRead = 4;

	// Cargamos los caractares planos sin referencias de comportamiento
	nextIndexRead = ReadRawMap(fileReaded, nextIndexRead, staticMapRef);
	
	//InstantiateStaticMap(staticMapRef);



	return staticMapRef;
}

///Rellena el diccionario interno del staticMapRef con la info en crudo del mapa, sin referencias de comportamiento. 
uint32 PD_MG_MapParser::ReadRawMap(TArray<FString> fileReaded, uint32 firstIndex, PD_MG_StaticMap* staticMapRef)
{

	for (uint32 i = firstIndex; i < staticMapRef->GetHeight() + firstIndex; i++) {
		TArray<TCHAR> ta = fileReaded[i].GetCharArray();
		for (uint32 j = 0; j < staticMapRef->GetWidth(); j++) {
			staticMapRef->AddNewLogicPosition(i- firstIndex, j, ta[j]);
		}
	}

	return staticMapRef->GetHeight() + firstIndex;
	// Test
	/*
	FString s;
	s = "";
	s.AppendInt(staticMapRef->GetLogicPositions().Num());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Testing " + s);
	for (int i = firstIndex; i < staticMapRef->GetLogicPositions().Num(); i++) {
	
		
		FString s;
		TMap<PD_MG_LogicPosition, TCHAR> m = staticMapRef->GetXYMap();
		TArray<PD_MG_LogicPosition*> lps = staticMapRef->GetLogicPositions();
		PD_MG_LogicPosition* lp = lps[i];
		s.AppendInt(lp->GetX());
		s.Append(",");
		s.AppendInt(lp->GetY());
		s.Append("-");
		s.AppendChar(m[*lp]);
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, s);
		
	}
	*/
}

void PD_MG_MapParser::InstantiateStaticMap(AParserActor* parserActor) {
	
	for (int i = 0; i < parserActor->getMap()->GetLogicPositions().Num(); i++) {

		
		switch (parserActor->getMap()->GetXYMap()[*parserActor->getMap()->GetLogicPositions()[i]]) {
		case 'w':
			parserActor->InstantiateWall(parserActor->getMap()->GetLogicPositions()[i]);
			break;

		case 'd':
			

		default: 

			parserActor->InstantiateTile(parserActor->getMap()->GetLogicPositions()[i]);
			break;

		}
		
	}
}
