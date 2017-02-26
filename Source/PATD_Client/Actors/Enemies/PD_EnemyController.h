// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/PD_GenericController.h"
#include "PD_EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class PATD_CLIENT_API APD_EnemyController : public APD_GenericController
{
	GENERATED_BODY()
	
	virtual bool MoveTo(float x, float y) override;
	virtual bool ActionTo(float x, float y, uint8 id_action) override;
	virtual bool Animate(uint8 typeAnimation) override;
	
	
};
