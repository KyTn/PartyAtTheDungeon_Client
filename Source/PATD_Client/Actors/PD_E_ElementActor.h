// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Structs/PD_ClientEnums.h"
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "PD_E_ElementActor.generated.h"

class UPD_ClientGameInstance;

UCLASS()
class PATD_CLIENT_API APD_E_ElementActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APD_E_ElementActor();

	PD_MG_LogicPosition ActualLogicPosition;
	UPD_ClientGameInstance* SGI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Type")
		MapSkinType mapSkin;



	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;


	UFUNCTION(BlueprintCallable, Category = "Element Actor")
		MapSkinType GetMaterialSkin() { return mapSkin; };

	UFUNCTION(BlueprintCallable, Category = "Element Actor")
		void SetMaterialSkin(MapSkinType inMaterialSkin);

private:
	void ChangeMaterialProperties_MapSkin();
};
