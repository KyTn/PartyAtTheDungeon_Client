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
STRUCTS PARA EL CHARACTER LOGIC
 - Stats b�sicos - Los que elige el jugador
 - Stats base - Stats base del personaje - unas constantes
 - Weapons
 - Skills
 - Skin
 - Stats Totales
*/

//Stats Basicos - Poder, Agilidad, Destreza, ...
USTRUCT()
struct FStructBasicStats
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 POD;
		UPROPERTY()
		uint8 AGI;
		UPROPERTY()
		uint8 DES;
		UPROPERTY()
		uint8 CON;
		UPROPERTY()
		uint8 PER;
		UPROPERTY()
		uint8 MAL;
	//Constructor
	FStructBasicStats()
	{
	}
};

//Stats BASE - Vida - Damage
USTRUCT()
struct FStructInitBaseStats
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 HPBase;
		UPROPERTY()
		uint8 DMGBase;


	//Constructor
	FStructInitBaseStats()
	{
	}
};

//Habilidades - Activas y Pasivas
USTRUCT()
struct FStructSkills
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<uint8> listActiveSkills;

		UPROPERTY()
		TArray<uint8> listPasiveSkills;
	//Constructor
	FStructSkills()
	{

	}
};


//Weapons - Compuesto por Damage - Range - BP(skin)
USTRUCT()
struct FStructWeapon
{
	GENERATED_BODY()
		UPROPERTY()
		uint8 ID_Weapon;
		UPROPERTY()
		uint8 TypeWeapon;
		UPROPERTY()
		uint8 DMWeapon;
		UPROPERTY()
		uint8 RangeWeapon;
		//UPROPERTY()
		//TSubclassOf<class WeaponParent> SkinWeapon;

	//Constructor
	FStructWeapon()
	{
	}
};

//Weapons - Compuesto por Damage - Range - BP(skin)
USTRUCT()
struct FStructSkin
{
	GENERATED_BODY()

		//UPROPERTY()
		//enum indicando el cabezon
		//UPROPERTY()
		//enum indicando el cuerpo

		/* !!NOTA: Estos datos se tienen que pasar al GenericCharacter.h  para que elija en el Character_BP elija el mesh adecuado en base a estos datos.
		*/
	//Constructor
	FStructSkin()
	{
	}
};

//STATS totales del Jugador - Vida - Da�o Total - Rango Total
USTRUCT()
struct FStructTotalStats
{
	GENERATED_BODY()

		//stats generales del jugador
		UPROPERTY()
		uint8 HPTotal;
		UPROPERTY()
		uint8 HPCurrent;
		UPROPERTY()
		uint8 RangeTotal;
		UPROPERTY()
		uint8 DMGTotal;

		//stats principales - siendo el valor de estas el BONUS que da cada stat (no los puntos dados - diferencia respecto al FStructBasicStats
		UPROPERTY()
		uint8 PODBonus;
		UPROPERTY()
		uint8 AGIBonus;
		UPROPERTY()
		uint8 DESBonus;
		UPROPERTY()
		uint8 CONBonus;
		UPROPERTY()
		uint8 PERBonus;		
		UPROPERTY()
		uint8 MALBonus;

		//stats secundarios - AP - SAL - CH (dependen de los BONUS de los stats principales
		UPROPERTY()
		uint8 AP;
		UPROPERTY()
		uint8 CH;		
		UPROPERTY()
		uint8 SA;


	//Constructor
	FStructTotalStats()
	{
	}
};




/*
Procedimiento para agregar un struct:
A�adir en UstructType el siguiente numero. (,FStruct NombreStruct=17)
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

enum class UStructType { NotDefined = 0, AllStructs = 1, FStructNewConnection = 2, FStructMap = 10, FStructOrderMenu = 20, FStructTurnOrders = 30, FStructCharacter = 40 };




//=================================
///Structs SERIALIZABLES (heredan de FStructGeneric) (Se envian)
//=================================
USTRUCT()
struct FStructCharacter : public  FStructGeneric
{
	GENERATED_BODY()

		UPROPERTY()
			FStructTotalStats totalStats;
		UPROPERTY()
			FStructBasicStats basicStats;
		UPROPERTY()
			FStructInitBaseStats initBaseStats;
		UPROPERTY()
			FStructSkills skils;
		UPROPERTY()
			FStructWeapon weapon;
		UPROPERTY()
			FStructSkin skin;

	//Constructor
	FStructCharacter()
	{
		structType = static_cast<uint8>(UStructType::FStructCharacter);
	}
};

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
	GameConfigurationDone = 1, ClientReady = 4,
	//Ordenes que recibe el cliente. 
	Welcome = 6, ChangeToLobby = 8, InvalidConnection = 10
};
USTRUCT()
struct FStructOrderMenu : public FStructGeneric
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 orderType;

	//Usadas por welcome
	UPROPERTY()
		uint8 playerIndex;
	UPROPERTY()
		bool isClientMaster;

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





