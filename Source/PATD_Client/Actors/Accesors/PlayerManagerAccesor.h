// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PlayerManagerAccesor.generated.h"


class PD_PlayersManager;

UCLASS()
class PATD_CLIENT_API APlayerManagerAccesor : public AActor
{
	GENERATED_BODY()
	
public:	

	PD_PlayersManager *playersManager;

	// Sets default values for this actor's properties
	APlayerManagerAccesor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	UFUNCTION(BlueprintCallable, Category = "OrderFunctions")
		bool CreateConsumableOrder(int id_consumable);
	UFUNCTION(BlueprintCallable, Category = "OrderFunctions")
		bool CreateMovementOrder(int positionX, int positionY);
	UFUNCTION(BlueprintCallable, Category = "OrderFunctions")
		bool DeleteLastMovementOrder();
	UFUNCTION(BlueprintCallable, Category = "OrderFunctions")
		bool CreateInteractableOrder(int id_interactable);
	UFUNCTION(BlueprintCallable, Category = "OrderFunctions")
		bool CreateActionToPosition(int id_action, TArray<FVector> positions);
	UFUNCTION(BlueprintCallable, Category = "OrderFunctions")
		bool CreateActionToCharacter(int id_action, TArray<FString> id_character);
	

	UFUNCTION(BlueprintCallable, Category = "OrderFunctions")
	bool ResetAll();

	UFUNCTION(BlueprintCallable, Category = "OrderFunctions")
	bool ResetConsumables();

	UFUNCTION(BlueprintCallable, Category = "OrderFunctions")
	bool ResetMovements();

	UFUNCTION(BlueprintCallable, Category = "OrderFunctions")
	bool ResetInteractuables();

	UFUNCTION(BlueprintCallable, Category = "OrderFunctions")
	bool ResetActions();

};
