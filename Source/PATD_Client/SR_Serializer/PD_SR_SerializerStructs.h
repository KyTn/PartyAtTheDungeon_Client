// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "PD_SR_SerializerStructs.generated.h"

/**
*
*/


//Struct Generico
USTRUCT()
struct FStructGeneric
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 structType; //conservamos el tipo en este struct para que se pueda generar de forma automatica al definir el struct


						  //Constructor
	FStructGeneric() {

		structType = 0;//static_cast<uint8>(UStructType::NotDefined);
	}
};

//Struct con raw data
USTRUCT()
struct FStructData
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 structType; //El tipo le sera asignado desde el FStructGeneric que se construye

	UPROPERTY()
		TArray<uint8> data;
	//Constructor
	FStructData() {

		structType = 0;//static_cast<uint8>(UStructType::NotDefined);
	}
};


//Struct lista de genericos (en forma de rawdata).
USTRUCT()
struct FStructDataList
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<FStructData> list;

	//Constructor
	FStructDataList() {
	}
};




