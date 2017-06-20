// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "Structs/PD_NetStructs.h"
#include "PD_SaveCharacterData.generated.h"

/**
 * 
 */
UCLASS()
class PATD_CLIENT_API UPD_SaveCharacterData : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "Basic")
	TArray<FStructCharacter> charactersOfPlayer;

	UPROPERTY(EditAnywhere, Category = "Basic")
	TArray<FStructSkill> activeSkills;

	UPROPERTY(EditAnywhere, Category = "Basic")
	TArray<FStructSkill> pasiveSkills;

	UPROPERTY(EditAnywhere, Category = "Basic")
	TArray<FStructWeapon> weapons;

	UPROPERTY(EditAnywhere, Category = "IPString")
	FString IPSaveAlready;

	UPD_SaveCharacterData();

	UFUNCTION(BlueprintCallable, Category = "LoadSaveIP")
		FString GetIPSaveAlready();
};
