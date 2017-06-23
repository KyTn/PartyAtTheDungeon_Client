// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PD_E_Character.generated.h"

class PD_GM_LogicCharacter;

UCLASS()
class PATD_CLIENT_API APD_E_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APD_E_Character();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Variable Logic Character para tener doble referencia y poder llamar a los metodos logicos con metodos del Character
	PD_GM_LogicCharacter* logic_character;

	void SetLogicCharacter(PD_GM_LogicCharacter* nlogic_character);
	PD_GM_LogicCharacter* GetLogicCharacter();

	void UpdateCharLife();

	UFUNCTION(BlueprintCallable, Category = "DataCharacter")
		void GetCharacterID(FString &ID_Char);

	UFUNCTION(BlueprintCallable, Category = "DataCharacter")
		void GetInfoCharcaterForWidget(FString &ID_Char, FString &TypeChar, int &numberPlayer);

	//Funciones para mantener la interfaz,  la barra de vida actualizada.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InGameStats")
		float percentHP = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InGameStats")
		FString stringHP = "0";
};
