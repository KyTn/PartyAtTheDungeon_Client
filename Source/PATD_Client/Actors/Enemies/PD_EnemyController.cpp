// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_EnemyController.h"



bool APD_EnemyController::MoveTo(float x, float y)
{
	FVector currentPosition = GetPawn()->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("MyCharacter's Location is %s"), *currentPosition.ToString());


	FVector newPosition = FVector(x, y, 0);
	UE_LOG(LogTemp, Warning, TEXT("MyCharacter's new Location is %s"), *newPosition.ToString());

	MoveToLocation(newPosition, -1.0f, true, false, false, true, 0, true);
	return true;
}

bool APD_EnemyController::ActionTo(float x, float y, uint8 id_action)
{
	return true;

}

bool APD_EnemyController::Animate(uint8 typeAnimation)
{
	return true;
}
