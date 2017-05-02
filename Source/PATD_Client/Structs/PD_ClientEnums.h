#pragma once
/*
template <typename T>
class EnumParser
{
	std::map <FString, T> enumMap;
public:
	EnumParser() {};

	T ParseSomeEnum(const FString &value)
	{
		std::map <FString, T>::const_iterator iValue = enumMap.find(value);
		if (iValue == enumMap.end())
			throw runtime_error("");
		return iValue->second;
	}
};
*/


//enum class EServerGameState { Instantiate_Map, Start_Match, WaitingPlayerOrders, ExecutingPlayersLogic, ExecutingPlayersVisualization, ExecutingEnemiesLogic, ExecutingEnemiesVisualization, EndOfTurn };
UENUM(BlueprintType)
enum class EClientGameState { Instantiate_Map = 00, Start_Match = 10, 
	GenerateOrders_Start = 20, GenerateOrders_ConsumablePhase = 21, GenerateOrders_MovementPhase = 22, GenerateOrders_InteractionPhase = 23, GenerateOrders_ActionPhase = 24, GenerateOrders_Validate = 25, 
	SendOrdersToServer = 30, WaitingServer = 40, UpdateInfo = 50, EndOfTurn = 60};



//enum class EServerState { StartApp, WaitingMasterClient, WaitingGameConfiguration, Lobby_Tabern, Launch_Match, GameInProcess, Podium, OnExit };

enum class EServerState { WaitingClientMaster, WaitingGameConfiguration, WaitingReady, GameInProcess };
enum class InteractionStates { Ready, Working, Finish };
enum class EActionPhase { Move = 0, Attack = 1, EndPhase = 2 };



enum class EClientState { StartApp, Game_NoConnection, ConfigureMatch, WaitingMatchConfiguration, Lobby_Tabern, ReconnectingInGame, Launch_Match, GameInProcess };
enum class ActiveSkills { Punch = 1, Fireball = 2, JustInTheEye = 3, ForTheKing = 4, LookingForCuenca = 5 };
enum class PasiveSkills { NotInMyGuard = 1, TheGoodBier = 2, KingInTheNorth = 3, HelloWorld = 4, YouShallNotPass = 5 };

//Lista que define TODOS las posibles acciones (ataques basicos, habilidades, etc) que puede hacer un character (enemigo o jugador)
UENUM(BlueprintType)
enum class ActionsByCharacter 
{
	BasicAttack = 1
};

enum class ECharacterType { NoCharacter, Player, Archer, Zombie };
/*
EnumParser<ECharacterType>::EnumParser()
{
	enumMap["NoCharacter"] = NoCharacter;
	enumMap["Player"] = Player;
	enumMap["Archer"] = Archer;
	enumMap["Zombie"] = Zombie;
}
*/

#pragma region MATCH CONFIG

// Cuando se envía por un FStructMatchConfig, cada ID se correspone al comentario que hay encima de cada uno


// ID 0
UENUM(BlueprintType)        //"BlueprintType" is essential to include
enum class MATCHCONFIG_MISSIONTYPE : uint8
{
	DefeatBoss     UMETA(DisplayName = "DefeatBoss"),
	DefeatAll    UMETA(DisplayName = "DefeatAll"),
	RecoverTreasure     UMETA(DisplayName = "RecoverTreasure")
};

// ID 1
enum class MATCHCONFIG_MAPSIZE {
	SMALL_SIZE, NORMAL_SIZE, LARGE_SIZE
};

// ID 1
enum class MATCHCONFIG_DIFFICULTY {
	EASY_DIFFICULTY, NORMAL_DIFFICULTY, DIFFICULT_DIFFICULTY
};

#pragma endregion