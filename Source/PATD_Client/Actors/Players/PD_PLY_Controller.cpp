// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_PLY_Controller.h"


void APD_PLY_Controller::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APD_PLY_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	FTimerHandle TimerHandleActor;
	//GetWorldTimerManager().SetTimer(TimerHandleActor, this, &AMyAIController::Move, 1.0f, true);

}

void APD_PLY_Controller::Move(float x, float y)
{
	UE_LOG(LogTemp, Warning, TEXT("Init GameMap"));

	FVector currentPosition = GetPawn()->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("MyCharacter's Location is %s"), *currentPosition.ToString());


	FVector newPosition = FVector(x, y, 0);
	UE_LOG(LogTemp, Warning, TEXT("MyCharacter's new Location is %s"), *newPosition.ToString());

	MoveToLocation(newPosition, -1.0f, true, false, false, true, 0, true);
}

void APD_PLY_Controller::Attack()
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "AL ATAQUERRRRRR !!!!!");

}

