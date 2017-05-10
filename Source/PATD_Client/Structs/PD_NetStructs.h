// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SR_Serializer/PD_SR_SerializerStructs.h"

#include "PD_NetStructs.generated.h"


//=================================
///Structs NO serializables -- (No se envian ellos mismos)
//=================================


USTRUCT(BlueprintType)
struct FStructLogicPosition {

	GENERATED_BODY()

		UPROPERTY()
		uint32 positionX;
	UPROPERTY()
		uint32 positionY;

	FStructLogicPosition() {

	}

};

//Struct que se usa para crear un target en una accion (ataque o habilidad)
USTRUCT()
struct FStructTargetToAction {

	GENERATED_BODY()

		UPROPERTY()
		uint32 id_action; //puede ser ataque basico, habilidad, defensa
	UPROPERTY()
		TArray<FStructLogicPosition> positions;
	UPROPERTY()
		TArray<FString> id_character;

	FStructTargetToAction() {
		positions = TArray<FStructLogicPosition>();
		id_character = TArray<FString>();
	}

};

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
	UPROPERTY()
		FStructLogicPosition targetLogicPosition;
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
- Stats básicos - Los que elige el jugador
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
	UPROPERTY()
		uint8 APBase;

	//Constructor
	FStructInitBaseStats()
	{
	}
};


USTRUCT()
struct FStructSkill
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 ID_Skill;
	UPROPERTY()
		uint8 typeSkill; // 0 activa , 1 pasiva

						 //Localizacion?
	UPROPERTY()
		FString name_Skill;
	UPROPERTY()
		FString description;

	UPROPERTY()
		uint8 weaponRequired;
	UPROPERTY()
		uint8 AP;
	UPROPERTY()
		uint8 CD;
	UPROPERTY()
		uint8 currentCD;
	UPROPERTY()
		int8 range; //0 el propio personaje, -1 toda la sala
	UPROPERTY()
		uint8 target; //0 jugador, 1 aliado, 2 enemigos

	FStructSkill() {

	}
};

//Habilidades - Activas y Pasivas
USTRUCT()
struct FStructCharacterSkills
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 max_ActSkills;
	UPROPERTY()
		TArray<FStructSkill> listActiveSkills;

	UPROPERTY()
		uint8 max_PasSkills;
	UPROPERTY()
		TArray<FStructSkill> listPasiveSkills;
	//Constructor
	FStructCharacterSkills()
	{

	}
};


//Weapons 
USTRUCT()
struct FStructWeapon
{
	GENERATED_BODY()
		UPROPERTY()
		uint8 ID_Weapon;
	UPROPERTY()
		uint8 TypeWeapon; //espadon, dagas, ballesta ....
	UPROPERTY()
		uint8 ClassWeapon; //melee, rango , magia
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

USTRUCT()
struct FStructSkin
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 ID_SkinHead;

		UPROPERTY()
		uint8 weapon_type;

		UPROPERTY()
		uint8 weapon_class;
		//Constructor
		FStructSkin()
	{
	}
};

//STATS totales del Jugador - Vida - Daño Total - Rango Total
USTRUCT()
struct FStructTotalStats
{
	GENERATED_BODY()

		//stats generales del jugador
		UPROPERTY()
		uint8 HPTotal;
	UPROPERTY()
		uint8 APTotal;
	UPROPERTY()
		uint8 HPCurrent;
	UPROPERTY()
		uint8 APCurrent;

	//stats principales - siendo el valor de estas el BONUS que da cada stat (no los puntos dados - diferencia respecto al FStructBasicStats
	UPROPERTY()
		float PODBonus;
	UPROPERTY()
		int8 AGIBonus;
	UPROPERTY()
		int8 DESBonus;
	UPROPERTY()
		float CONBonus;
	UPROPERTY()
		float PERBonus;
	UPROPERTY()
		float MALBonus;

	//stats secundarios - AP - SAL - CH (dependen de los BONUS de los stats principales
	UPROPERTY()
		int8 AP;
	UPROPERTY()
		int8 CH;
	UPROPERTY()
		int8 SA;


	//Constructor
	FStructTotalStats()
	{
	}
};

USTRUCT()
struct FStructCharacterState //Para estados secundarios y activas sobre el personaje
{
	GENERATED_BODY()

		UPROPERTY()
		TMap<int, int> activeEffectsOnCharacter; //id_skill, duration

		UPROPERTY()
		TMap<int, int> alteredCharacterState; //id_alteredState , duration
	//Constructor
	FStructCharacterState()
	{
		activeEffectsOnCharacter = TMap<int, int>();
		alteredCharacterState = TMap<int, int>();
	}
};

USTRUCT()
struct FStructPlayerInfoAtClient
{
	GENERATED_BODY()

		UPROPERTY()
		FStructSkin structSkin;
	UPROPERTY()
		FString ID_character; //Identifica al Jugador que realiza la modificacion
	UPROPERTY()
		uint8 playerNum;
	UPROPERTY()
		FStructLogicPosition logicPosition;


	//Constructor
	FStructPlayerInfoAtClient()
	{

	}
};

//Sirve para actualizar el estado de un character cuando se ha iniciado la partida
//Solo se van a enviar los atributos del character que sean necesarios
USTRUCT()
struct FStructUpdateCharacter
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 HPCurrent;
	UPROPERTY()
		FString ID_character; //Identifica al LogicCharacter que realiza la modificacion
	UPROPERTY()
		FStructLogicPosition currentCharacterPosition;


	//Constructor
	FStructUpdateCharacter()
	{
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
- NoDefined=0, -> para detectar errores de instanciacion.
- AllStructs=1 -> para suscribirse a eventos para todos.
- FStructNewConnection=2 struct que crea el networkmanager (no necesita serializacion) -> para gestionar nuevas conexiones al servidor (NO SE ENVIA POR RED COMO TAL)
- FStructServerToClientWellcome -> Mensaje desde el Server al Cliente para comunicarle que la comunicacion ha sido exitoso y pedirle que envie el Cliente ID
- FStructClientID -> El cliente envia su identificador unico, sirve para la reconexion
*/

enum class UStructType {
	NotDefined = 0, AllStructs = 1, FStructNewConnection = 2, FStructRequestIDClient = 3, FStructClientID = 4, FStructWelcome = 5, FStructPing = 6, FStructPong = 7, FStructLostConnection = 8, FStructMap = 10, FStructOrderMenu = 20, FStructMatchConfig = 21, FStructMatchConfigDone = 22, FStructTurnOrders = 30,
	FStructCharacter = 40, FStructUpdateTurn = 41, FStructClientMapAlreadyInstantiated = 50, FStructClientStartMatchOnGM = 51, FStructClientCanGenerateOrders = 52,
	FStructInstatiatePlayers = 60
};



//=================================
///Structs SERIALIZABLES (heredan de FStructGeneric) (Se envian)
//=================================

//se generan en el propio networkmanager

USTRUCT()
struct FStructNewConnection : public FStructGeneric
{	//No necesita estar en el serializeManager
	//FStructNewConnection: struct que crea el networkmanager(no necesita serializacion)
	GENERATED_BODY()



		//Constructor
		FStructNewConnection()
	{
		structType = static_cast<uint8>(UStructType::FStructNewConnection);
	}

};

USTRUCT()
struct FStructLostConnection : public FStructGeneric
{	//No necesita estar en el serializeManager
	//FStructNewConnection: struct que crea el networkmanager(no necesita serializacion)
	GENERATED_BODY()

		uint8 indexPlayer;

	//Constructor
	FStructLostConnection()
	{
		structType = static_cast<uint8>(UStructType::FStructLostConnection);
	}

};




//COMPROBACION DE RED
USTRUCT()
struct FStructPing : public  FStructGeneric
{
	GENERATED_BODY()

		//Constructor
		FStructPing()
	{
		structType = static_cast<uint8>(UStructType::FStructPing);
	}
};

USTRUCT()
struct FStructPong : public  FStructGeneric
{
	GENERATED_BODY()

		//Constructor
		FStructPong()
	{
		structType = static_cast<uint8>(UStructType::FStructPong);
	}
};



USTRUCT()
struct FStructRequestIDClient : public  FStructGeneric
{
	GENERATED_BODY()

		//Constructor
		FStructRequestIDClient()
	{
		structType = static_cast<uint8>(UStructType::FStructRequestIDClient);
	}
};

USTRUCT()
struct FStructClientID : public  FStructGeneric
{
	GENERATED_BODY()

		UPROPERTY()
		FString ID_Client;

	//Constructor
	FStructClientID()
	{
		structType = static_cast<uint8>(UStructType::FStructClientID);
	}
};


enum class GameState //Solo se envia al cliente
{
	ConfigureMatch = 0, //Menu Principal
	WaitingMatchConfiguration = 1, //Menu principal para NO CLient Master
	Lobby_Tabern = 2, //Seleccion de personajes
	GameInProcess = 4,
	ClientCanCreateOrders = 5,
	ClientWaitForServer = 6,
	NoConnectionAllowed = 7 //nuevo Cliente no se puede conectar - partida empezada
};

USTRUCT()
struct FStructWelcome : public  FStructGeneric
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 GameState;

	//Usadas por welcome
	UPROPERTY()
		uint8 playerIndex;
	UPROPERTY()
		bool isClientMaster = false;


	//Constructor
	FStructWelcome()
	{
		structType = static_cast<uint8>(UStructType::FStructWelcome);
	}
};



USTRUCT()
struct FStructCharacter : public  FStructGeneric //Struct para enviar al Servidor y para guardar el personaje creado
{
	GENERATED_BODY()

		UPROPERTY()
		FString nameOfCharacter;
	UPROPERTY()
		FStructTotalStats totalStats;
	UPROPERTY()
		FStructBasicStats basicStats;
	UPROPERTY()
		FStructInitBaseStats initBaseStats;
	UPROPERTY()
		FStructCharacterSkills skills;
	UPROPERTY()
		FStructWeapon weapon;
	UPROPERTY()
		FStructSkin skin;
	UPROPERTY()
		FStructCharacterState charState;

	//Constructor
	FStructCharacter()
	{
		structType = static_cast<uint8>(UStructType::FStructCharacter);
	}
};




USTRUCT()
struct FStructInstatiatePlayers : public FStructGeneric
{
	GENERATED_BODY()

		UPROPERTY()
		FString idClientCharacter;
	UPROPERTY()
		TArray<FStructPlayerInfoAtClient> listInfoPlayerAtClient;


	//Constructor
	FStructInstatiatePlayers()
	{
		structType = static_cast<uint8>(UStructType::FStructInstatiatePlayers);
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
		bool isClientMaster = false;

	//Constructor
	FStructOrderMenu()
	{
		structType = static_cast<uint8>(UStructType::FStructOrderMenu);
	}

};

/*USTRUCT()
struct FStructTurnOrders : public FStructGeneric
{
GENERATED_BODY()

UPROPERTY()
TArray<FStructOrderAction> listMove;

UPROPERTY()
TArray<FStructOrderAction> listAttack;


FStructTurnOrders() {
listMove = TArray<FStructOrderAction>();
listAttack = TArray<FStructOrderAction>();
structType = static_cast<uint8>(UStructType::FStructTurnOrders);
}
};
*/

USTRUCT()
struct FStructTurnOrders : public FStructGeneric
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<uint32> consumablesToConsume;

	UPROPERTY()
		TArray<FStructLogicPosition> positionsToMove;

	UPROPERTY()
		TArray<uint32> interactuablesToInteract;

	UPROPERTY()
		TArray<FStructTargetToAction> actions;

	FStructTurnOrders() {
		consumablesToConsume = TArray<uint32>();
		positionsToMove = TArray<FStructLogicPosition>();
		interactuablesToInteract = TArray<uint32>();
		actions = TArray<FStructTargetToAction>();

		structType = static_cast<uint8>(UStructType::FStructTurnOrders);
	}
};


USTRUCT()
struct FStructMatchConfig : public FStructGeneric
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 id;

	UPROPERTY()
		uint8 intvalue;
	UPROPERTY()
		FString FStringvalue;



	//Constructor
	FStructMatchConfig()
	{
		structType = static_cast<uint8>(UStructType::FStructMatchConfig);
		id = -1;
		intvalue = 0;
		FStringvalue = "";
	}

};

USTRUCT()
struct FStructMatchConfigDone : public FStructGeneric
{
	GENERATED_BODY()

		UPROPERTY()
		uint8 from; // 0 is server, 1 is masterclient

	FStructMatchConfigDone() {
		structType = static_cast<uint8>(UStructType::FStructMatchConfigDone);
	}
};



USTRUCT()
struct FStructClientMapAlreadyInstantiated : public FStructGeneric
{
	GENERATED_BODY()

		FStructClientMapAlreadyInstantiated() {
		structType = static_cast<uint8>(UStructType::FStructClientMapAlreadyInstantiated);
	}
};


USTRUCT()
struct FStructClientStartMatchOnGM : public FStructGeneric
{
	GENERATED_BODY()

		FStructClientStartMatchOnGM() {
		structType = static_cast<uint8>(UStructType::FStructClientStartMatchOnGM);
	}
};


USTRUCT()
struct FStructClientCanGenerateOrders : public FStructGeneric
{
	GENERATED_BODY()

		FStructClientCanGenerateOrders() {
		structType = static_cast<uint8>(UStructType::FStructClientCanGenerateOrders);
	}
};




//
//Sirve para actualizar el estado de un character cuando se ha iniciado la partida
//Solo se van a enviar los atributos del character que sean necesarios
USTRUCT()
struct FStructUpdateTurn : public  FStructGeneric
{
	GENERATED_BODY()


		UPROPERTY()
		TArray<FStructUpdateCharacter> listPlayerCharacters;

	UPROPERTY()
		TArray<FStructUpdateCharacter> listEnemyCharacters;

	//Constructor
	FStructUpdateTurn()
	{
		structType = static_cast<uint8>(UStructType::FStructUpdateTurn);
	}
};