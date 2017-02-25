// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "PD_GameStateInitializer.generated.h"

/**
 * 
 */
UCLASS()
class PATD_CLIENT_API APD_GameStateInitializer : public AGameStateBase
{
	GENERATED_BODY()

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;



};
