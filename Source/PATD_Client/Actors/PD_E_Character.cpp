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

	//UpdateCharLife();
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

#pragma region Function For BP
void APD_E_Character::GetCharacterID(FString &ID_Char)
{
	FString positionLogic = "PoX: ";
	positionLogic.Append(FString::FromInt(logic_character->GetCurrentLogicalPosition().GetX()));
	positionLogic.Append(" PosY: ");
	positionLogic.Append(FString::FromInt(logic_character->GetCurrentLogicalPosition().GetY()));
	
	ID_Char =  logic_character->GetIDCharacter();
	//ID_Char.Append(positionLogic);
}

void APD_E_Character::UpdateCharLife()
{
	stringHP = FString::FromInt(logic_character->GetTotalStats()->HPCurrent).Append(FString("/").Append(FString::FromInt((float)logic_character->GetTotalStats()->HPTotal)));
	percentHP = (float)logic_character->GetTotalStats()->HPCurrent / (float)logic_character->GetTotalStats()->HPTotal;
}

void APD_E_Character::GetInfoCharcaterForWidget(FString &ID_Char, FString &TypeChar, int &numberPlayer)
{
	ID_Char = logic_character->GetIDCharacter();

	if (logic_character->GetIsPlayer())
	{
		//UE_LOG(LogTemp, Warning, TEXT("APD_E_Character::GetInfoCharcaterForWidge weapon - %d "), logic_character->GetSkin()->ID_SkinHead);

		switch (logic_character->GetSkin()->ID_SkinHead)
		{
			case 0:
			{
				TypeChar = "Legionary";
				break;
			}
			case 1:
			{
				TypeChar = "Ranger";
				break;
			}
			case 2:
			{
				TypeChar = "Ranger";
				break;
			}
			case 3:
			{
				TypeChar = "Wizard";
				break;
			}
			default:
			{
				break;
			}
		}
		FString auxNumberplayer = logic_character->GetIDCharacter();
		FString charSplit = "_0";
		FString FStringNumberPlayerLeft = "";
		FString FStringNumberPlayerRight = "";
		auxNumberplayer.Split(charSplit, &FStringNumberPlayerLeft, &FStringNumberPlayerRight);

		int numChar = FCString::Atoi(*FStringNumberPlayerRight);
		numChar++;
		numberPlayer = numChar;
	}
	else
	{
		//Type Char
		switch (ECharacterType(logic_character->GetTypeCharacter()))
		{
			case ECharacterType::OrcBow:
			{
				TypeChar = "Archer Orc";
				break;
			}
			case ECharacterType::OrcGuns:
			{
				TypeChar = "Guns Orc";
				break;
			}
			case ECharacterType::OrcMelee:
			{
				TypeChar = "Melee Orc";
				break;
			}
			case ECharacterType::OrcBoss:
			{
				TypeChar = "Boss Orc";
				break;
			}
			default:
				break;
		}
		//NumberPlayer (For the color)
		numberPlayer = 0;
	}



	//UE_LOG(LogTemp, Warning, TEXT("APD_E_Character::GetInfoCharcaterForWidge - name %s "), *ID_Char);
	//UE_LOG(LogTemp, Warning, TEXT("APD_E_Character::GetInfoCharcaterForWidge - type  %s "), *TypeChar);
	//UE_LOG(LogTemp, Warning, TEXT("APD_E_Character::GetInfoCharcaterForWidge number player - %d "), numberPlayer);

}

#pragma endregion
