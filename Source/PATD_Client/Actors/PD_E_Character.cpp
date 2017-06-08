// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_E_Character.h"

#include "GM_Game/LogicCharacter/PD_GM_LogicCharacter.h"

// Sets default values
APD_E_Character::APD_E_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APD_E_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APD_E_Character::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void APD_E_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APD_E_Character::SetLogicCharacter(PD_GM_LogicCharacter* nlogic_character)
{
	logic_character = nlogic_character;
}

PD_GM_LogicCharacter* APD_E_Character::GetLogicCharacter()
{
	return logic_character;
}

void APD_E_Character::GetCharacterID(FString &ID_Char)
{
	FString positionLogic = "PoX: ";
	positionLogic.Append(FString::FromInt(logic_character->GetCurrentLogicalPosition().GetX()));
	positionLogic.Append(" PosY: ");
	positionLogic.Append(FString::FromInt(logic_character->GetCurrentLogicalPosition().GetY()));
	
	ID_Char =  logic_character->GetIDCharacter();
	//ID_Char.Append(positionLogic);
}
