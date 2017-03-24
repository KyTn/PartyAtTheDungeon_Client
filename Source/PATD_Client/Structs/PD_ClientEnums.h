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
enum class EClientGameState { Instantiate_Map = 0, Start_Match = 1, GenerateOrders = 2, SendOrdersToServer = 3, WaitingServer = 4, UpdateInfo = 5, EndOfTurn = 6};

UENUM(BlueprintType)
enum class EClientGamePhase { ConsumablePhase = 0, MovementPhase = 1, InteractionPhase = 2, ActionPhase = 3 };


//enum class EServerState { StartApp, WaitingMasterClient, WaitingGameConfiguration, Lobby_Tabern, Launch_Match, GameInProcess, Podium, OnExit };

enum class EServerState { WaitingClientMaster, WaitingGameConfiguration, WaitingReady, GameInProcess };
enum class InteractionStates { Ready, Working, Finish };
enum class EActionPhase { Move = 0, Attack = 1, EndPhase = 2 };



enum class EClientState { StartApp, Game_NoConnection, ConfigureMatch, WaitingMatchConfiguration, Lobby_Tabern, Launch_Match, GameInProcess };
enum class ActiveSkills { Punch = 1, Fireball = 2, JustInTheEye = 3, ForTheKing = 4, LookingForCuenca = 5 };
enum class PasiveSkills { NotInMyGuard = 1, TheGoodBier = 2, KingInTheNorth = 3, HelloWorld = 4, YouShallNotPass = 5 };


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
