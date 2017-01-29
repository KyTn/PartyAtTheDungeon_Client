// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PruebaUStruct.generated.h"
//If you want this to appear in BP, make sure to use this instead
//USTRUCT(BlueprintType)
USTRUCT()
struct FpruebaUStruct
{
	GENERATED_BODY()

		//Always make USTRUCT variables into UPROPERTY()
		//    any non-UPROPERTY() struct vars are not replicated

		// So to simplify your life for later debugging, always use UPROPERTY()
		UPROPERTY()
		FString stringPrueba;



	/*//Set
	void SetInt(const int NewValue)
	{
	SampleInt32 = NewValue;
	}

	//Get
	AActor* GetActor()
	{
	return TargetActor;
	}

	//Check
	bool ActorIsValid() const
	{
	if (!TargetActor) return false;
	return TargetActor->IsValidLowLevel();
	}
	*/
	//Constructor
	FpruebaUStruct()
	{
		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor

	}
};
