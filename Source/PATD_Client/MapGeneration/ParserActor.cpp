// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PATD_Client/MapGeneration/ParserActor.h"
#include "PATD_Client/MapGeneration/PD_MG_StaticMap.h"
#include "PATD_Client/MapGeneration/PD_MG_LogicPosition.h"
#include "PATD_Client/MapGeneration/PD_MG_MapParser.h"
#include "PATD_Client/ElementActors/PD_E_TileActor.h"
#include "PATD_Client/ElementActors/PD_E_WallActor.h"



// Sets default values
AParserActor::AParserActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AParserActor::BeginPlay()
{
	Super::BeginPlay();

	StaticMapRef = new PD_MG_StaticMap();
	//Parsear();


	PD_MG_MapParser* parseMap = new PD_MG_MapParser();

	// Recibir desde el servidor el chorizo del mapa

	FString data;
	parseMap->StartParsingFromFString(&data, StaticMapRef);
	parseMap->InstantiateStaticMap(this);



}
void AParserActor::Parsear(){
	StaticMapRef = new PD_MG_StaticMap();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Starting Dungeon Parser v0.1");
	
	FString CompleteFilePath = FPaths::GameDir() + "Content/DungeonTestingMaps/test1.dungeon";

	FString FileData = "";
	if (FFileHelper::LoadFileToString(FileData, *CompleteFilePath)) {

		// Muesta por pantalla el contenido del fichero pasado al parser
		//GEngine->AddOnScreenDebugMessage(-1, 500000.f, FColor::Red, FileData);



		// Obtenemos el numero de filas y de columnas 
		FString A, B;

		uint32 fil, col;

		FileData.Split("\n", &A, &B, ESearchCase::CaseSensitive, ESearchDir::FromStart);
		fil = FCString::Atoi(*A);

		B.Split("\n", &A, &B, ESearchCase::CaseSensitive, ESearchDir::FromStart);
		col = FCString::Atoi(*A);
		////////////////


		// Obtenemos los chars del fichero de texto (los estáticos)
		TArray<TArray<TCHAR>> map;
		map.Empty(fil);
		
		/*
		for (int i = 0; i < (int)fil; i++) {
			B.Split("\n", &A, &B, ESearchCase::CaseSensitive, ESearchDir::FromStart);
			TArray<TCHAR> myCol = A.GetCharArray();
			map.Add(myCol);
			map[i].Empty(col);
			for (int j = 0; j < (int)col; j++) {
				map[i][j] = myCol[j];
			}
		}
		/**/
		/**/
		int i = 0;
		while (B.Split("\n", &A, &B, ESearchCase::CaseSensitive, ESearchDir::FromStart))
		{
			map.Add(A.GetCharArray());
			//map[i].Empty(col);
			//GEngine->AddOnScreenDebugMessage(-1, 5000000.f, FColor::Red, );
			/*
			for (i = 0; i < map[map.Num() - 1].Num(); i++) {
				//map[map.Num() - 1][i] = A[i];
				GEngine->AddOnScreenDebugMessage(-1, 5000000.f, FColor::Red, &map[map.Num() - 1][i]);
			}
			/**/
		}
		map.Add(B.GetCharArray());
		/*
		FString s = "F: ";
		s.AppendInt(fil);
		s.Append(" C: ");
		s.AppendInt(col);
		GEngine->AddOnScreenDebugMessage(-1, 5000000.f, FColor::Red, s);
		/**/

		//int i = 0;

		for (i = 0; i < (int)fil; i++)
		{
			for (int j = 0; j < (int)col; j++)
			{
				
				//PD_MG_LogicPosition* lp = StaticMapRef->AddNewLogicPosition(i, j);
				//ParserElementByChar(lp, &(map[i][j]));
			}

			/*
			s = "";
			s.AppendInt(i);

			GEngine->AddOnScreenDebugMessage(-1, 5000000.f, FColor::Red, s);
			/**/
		}

		
	}
}

void AParserActor::ParserElementByChar(PD_MG_LogicPosition* logpos, TCHAR* c) 
{
	FString s;
	/**/
	if (/*logpos->GetX() == 19 &&*/ logpos->GetY() == 0 ) {
		
		//GEngine->AddOnScreenDebugMessage(-1, 5000000.f, FColor::Red, c);
	}
	/**/

	switch (*c) {
		// Si es una pared ...
		case 'w':/*
			s = "Wall at ";
			s.AppendInt(logpos->GetX());
			s.Append(", ");
			s.AppendInt(logpos->GetY());

			GEngine->AddOnScreenDebugMessage(-1, 5000000.f, FColor::Red, s);
			/**/
			InstantiateWall(logpos);
			break;

		// Si es una puerta ...
		case 'd': 

			s = "Door at ";
			s.AppendInt(logpos->GetX());
			s.Append(", ");
			s.AppendInt(logpos->GetY());

			//GEngine->AddOnScreenDebugMessage(-1, 5000000.f, FColor::Red, s);

			break;

		// default
		// Si es un tile normal y corriente ...
		default: 
			InstantiateTile(logpos);
			break;
	}
}
/**/
AActor* AParserActor::InstantiateTile(PD_MG_LogicPosition* logpos)
{
	//return GetWorld()->SpawnActor<APD_E_TileActor>(tileActor,FVector(logpos->GetX()*100.0f, logpos->GetY() * 100.0f, 0.f), FRotator(0.0f, 0.f, 0.f), FActorSpawnParameters());

	return GetWorld()->SpawnActor<APD_E_TileActor>(FVector(-1.0f * logpos->GetX()*100.0f, logpos->GetY() * 100.0f, 0.f), FRotator(0.0f, 0.f, 0.f), FActorSpawnParameters());
}


AActor* AParserActor::InstantiateWall(PD_MG_LogicPosition* logpos)
{
	//return GetWorld()->SpawnActor<APD_E_TileActor>(tileActor,FVector(logpos->GetX()*100.0f, logpos->GetY() * 100.0f, 0.f), FRotator(0.0f, 0.f, 0.f), FActorSpawnParameters());

	return GetWorld()->SpawnActor<APD_E_WallActor>(FVector(-1.0f * logpos->GetX()*100.0f, logpos->GetY() * 100.0f, 0.f), FRotator(0.0f, 0.f, 0.f), FActorSpawnParameters());
}
/**/

// Called every frame
void AParserActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}