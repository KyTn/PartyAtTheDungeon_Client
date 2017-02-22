// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "PD_PLY_Controller.generated.h"

/**
 * 
 */
UCLASS()
class PATD_CLIENT_API APD_PLY_Controller : public AAIController
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;


	void Move(float x, float y);

	void Attack();
	
	
};
