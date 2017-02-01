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
enum class UStructType { NoDefined = -9, AllStructs = -1, FStructMap = 0 };

//Struct Generico
USTRUCT()
struct FStructGeneric
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 structType;

	//Constructor
	FStructGeneric() {

		structType = static_cast<int>(UStructType::NoDefined);
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


/*
USTRUCT()
struct FStructListaPruebas
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<FStructGeneric> list;

	//Constructor
	FStructListaPruebas() {
	}
};*/


//=================================
///Definicion de structs usuario
//=================================

USTRUCT()
struct FStructMap : public FStructGeneric
{
	GENERATED_BODY()

		//Always make USTRUCT variables into UPROPERTY()
		//    any non-UPROPERTY() struct vars are not replicated

		// So to simplify your life for later debugging, always use UPROPERTY()
		UPROPERTY()
		FString stringPrueba;

	UPROPERTY()
		uint8 intPrueba;

	UPROPERTY()
		TArray<FString> arrayPruebaStrings;



	//Constructor
	FStructMap()
	{
		structType = static_cast<int>(UStructType::FStructMap);

		intPrueba = 0;
		arrayPruebaStrings = TArray<FString>();
		arrayPruebaStrings.Add("prueba 0");
		arrayPruebaStrings.Add("Prueba 1 tralalalalalla");
		arrayPruebaStrings.Add("Prueba 2 patatin");

		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor

	}

	FStructMap(int p)
	{
		intPrueba = p;
		//structType = Enum.StrucMap;
		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor

	}
};

