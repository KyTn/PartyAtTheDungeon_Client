// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "GM_Game/PD_GM_MapManager.h"

#include "GameFramework/Actor.h"
#include "MapManagerAccesor.generated.h"



class PD_GM_MapManager;
class APD_E_Character;

UCLASS()
class PATD_CLIENT_API AMapManagerAccesor : public AActor
{
	GENERATED_BODY()
	

public:	

	PD_GM_MapManager* mapManager;

	// Sets default values for this actor's properties
	AMapManagerAccesor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
		APD_E_Character*  GetCharacterPlayerAtPosition(FVector position, bool& existe);

	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
		bool IsMyCharacterPlayerAtPosition(FVector position);
	
	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
		int GetIdOfCharacterAt(FVector position);

	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
	FVector GetOwnPlayerPosition();

	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
	void TransformFVectorToLogicPosition(FVector positionInWorld, int &logicX, int &logicY);
};
 