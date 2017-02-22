// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PD_SR_UStruct.generated.h"


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

enum class UStructType { NoDefined = -9, AllStructs = -1, FStructMap = 1, FStructOrderMenu = 2, FStructGenericoHito2 = 3 };

//Ordenes. NullOrder significa que el paquete recibido no es de una orden
enum class MenuOrderType {
	NullOrder = -1,
	//Ordenes que recibe el servidor. 
	NewConnection = 0, GoToMainMenu = 1, GoToLobby = 2, GoToMap = 3, ClientReady = 4,
	//Ordenes que recibe el cliente. La 5 cuenta como 6 pero ademas setea ClientMaster.
	SetClientMaster = 5, Welcome = 6, ChangeToMainMenu = 7, ChangeToLobby = 8, ChangeToMap = 9, InvalidConnection = 10
};

//Struct Generico

USTRUCT()
struct FStructGeneric
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 structType; //conservamos el tipo en este struct para que se pueda generar de forma automatica al definir el struct


						  //Constructor
	FStructGeneric() {

		//structType = static_cast<uint8>(UStructType::NoDefined);
	}
};

//Struct Generico
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

		structType = static_cast<uint8>(UStructType::NoDefined);
	}
};


//Struct lista de genericos.
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


//=================================
///Definicion de structs usuario
//=================================


USTRUCT()
struct FStructGenericoHito2 : public FStructGeneric
{
	GENERATED_BODY()

		UPROPERTY()
		FString stringMap;

	UPROPERTY()
		uint8 orderType;

	//Constructor
	FStructGenericoHito2()
	{
		structType = static_cast<uint8>(UStructType::FStructGenericoHito2);
	}

};



USTRUCT()
struct FStructMap : public FStructGeneric
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
struct FStructOrderMenu : public FStructGeneric
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 orderType;

	//Constructor
	FStructOrderMenu()
	{
	}

};