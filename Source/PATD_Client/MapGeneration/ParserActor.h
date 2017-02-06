// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ParserActor.generated.h"

class PD_MG_StaticMap;
class PD_MG_LogicPosition;

UCLASS()
class PATD_CLIENT_API AParserActor : public AActor
{
	GENERATED_BODY()
	
private :
	PD_MG_StaticMap* StaticMapRef;
	
	
public:	
	
	
	// Sets default values for this actor's properties
	AParserActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	PD_MG_StaticMap* getMap() { return StaticMapRef; }

	
	AActor* InstantiateTile(PD_MG_LogicPosition* logpos);
	AActor* InstantiateWall(PD_MG_LogicPosition* logpos);


	//Agregado para el hito2 MCG
	void InitGameMap(FString mapData);
};
