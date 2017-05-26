// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/PD_E_ElementActor.h"
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "Structs/PD_ClientEnums.h"
#include "PD_E_WallActor.generated.h"

UCLASS()
class PATD_CLIENT_API APD_E_WallActor : public APD_E_ElementActor
{
	GENERATED_BODY()

public:

	APD_E_WallActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Type")
		MapSkinType mapSkin_N;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Type")
		MapSkinType mapSkin_S;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Type")
		MapSkinType mapSkin_E;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Type")
		MapSkinType mapSkin_W;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;


	void SetMaterialSkin(PD_MG_LogicPosition lp);

private:
	void ChangeMaterialProperties_MapSkin(PD_MG_LogicPosition lp);


};