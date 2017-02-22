// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SR_Serializer/PD_SR_SerializerStructs.h"

#include "PD_NetStructs.generated.h"


//=================================
///Structs NO serializables -- (No se envian ellos mismos)
//=================================

enum class EDirections { Up = 1, Down = 2, Left = 3, Right = 4 };

enum class EOrderAction { Move = 1, Attack = 2 };
//Los struct tienen que ser USTRUCT para poder meterlos en un array de otro USTRUCT.
USTRUCT()
struct FStructOrderAction {


	GENERATED_BODY()

		//EOrderAction
		UPROPERTY()
		uint8 orderType;

	//EDirections
	UPROPERTY()
		uint8 targetDirection;

	/*
	LogicPosition targetTile;
	CharacterId targetCharacter;
	etc.
	int subtipo; para numero de consumible, numero de habilidad, etc.
	*/
	FStructOrderAction() {

	}

};


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


/*Lista de structs especiales
NoDefined=0, para detectar errores de instanciacion.
AllStructs=1 para suscribirse a eventos para todos.
FStructNewConnection=2 struct que crea el networkmanager (no necesita serializacion)
*/

enum class UStructType { NotDefined = 0, AllStructs = 1, FStructNewConnection = 2, FStructMap = 10, FStructOrderMenu = 20, FStructTurnOrders = 30 };




//=================================
///Structs SERIALIZABLES (heredan de FStructGeneric) (Se envian)
//=================================


USTRUCT()
struct FStructNewConnection : public FStructGeneric
{
	//FStructNewConnection: struct que crea el networkmanager(no necesita serializacion)
	GENERATED_BODY()



		//Constructor
		FStructNewConnection()
	{
		structType = static_cast<uint8>(UStructType::FStructNewConnection);
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
		structType = static_cast<uint8>(UStructType::FStructMap);
	}

};


//Ordenes. NullOrder significa que el paquete recibido no es de una orden
enum class MenuOrderType {
	NullOrder = -1,
	//Ordenes que recibe el servidor. 
	NewConnection = 0, GoToMainMenu = 1, GoToLobby = 2, GoToMap = 3, ClientReady = 4,
	//Ordenes que recibe el cliente. La 5 cuenta como 6 pero ademas setea ClientMaster.
	SetClientMaster = 5, Welcome = 6, ChangeToMainMenu = 7, ChangeToLobby = 8, ChangeToMap = 9, InvalidConnection = 10
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
		structType = static_cast<uint8>(UStructType::FStructOrderMenu);
	}

};

USTRUCT()
struct FStructTurnOrders : public FStructGeneric
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<FStructOrderAction> listMove;

	UPROPERTY()
		TArray<FStructOrderAction> listAttack;


	FStructTurnOrders() {
		structType = static_cast<uint8>(UStructType::FStructTurnOrders);
	}
};






