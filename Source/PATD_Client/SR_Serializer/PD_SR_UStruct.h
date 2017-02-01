// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PD_SR_UStruct.generated.h"
/**
*
*/
/*
Procedimiento para agregar un struct:
Añadir en UstructType el siguiente numero. (,FStruct NombreStruct=17)
Tener en cuenta que no deben repetirse para ningun struct los numeros.

Crear el struct en la zona de //Definicion de structs usuario.

El constructor debe de tener:

NombreStruct()
{
structType=static_cast<int>(UStructType::NombreStruct);
[...]
}


Por claridad es mejor que el nombre del struct coincida con el del enumerado. (aunque no obligatorio).

*/


/*Lista de structs de usuario
NoDefined=-9, para detectar errores de instanciacion.
AllStructs=-1 para suscribirse a eventos para todos.
*/
enum class UStructType { NoDefined = -9, AllStructs = -1, FStructMap = 0, FStructOrderMenu = 1 };


enum class MenuOrderType {};

//Struct Generico
USTRUCT()
struct FStructGeneric
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 structType;

	//Constructor
	FStructGeneric() {

		structType = static_cast<uint8>(UStructType::NoDefined);
	}
};

//Struct lista de genericos.
USTRUCT()
struct FStructGenericList
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<FStructGeneric> list;

	//Constructor
	FStructGenericList() {
	}
};


//=================================
///Definicion de structs usuario
//=================================


USTRUCT()
struct FStructGenericoHito2
{
	GENERATED_BODY()

		UPROPERTY()
		FString stringMap;

	UPROPERTY()
		uint8 orderType;

	//Constructor
	FStructGenericoHito2()
	{
		stringMap = "";
		orderType =-1;
	}

};



USTRUCT()
struct FStructMap
{
	GENERATED_BODY()

		UPROPERTY()
		FString stringMap;

	//Constructor
	FStructMap()
	{
	}

};

USTRUCT()
struct FStructOrderMenu
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 orderType;

	//Constructor
	FStructOrderMenu()
	{
	}

};