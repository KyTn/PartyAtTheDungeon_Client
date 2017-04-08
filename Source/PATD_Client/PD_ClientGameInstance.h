// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


//Include de herencia (interfaz)
#include "NW_NetWorking/EventLayer/PD_NW_iEventObserver.h"
//forward declarations
//Include de enums (no se pueden hacer forward declaration)
#include "Structs/PD_ClientEnums.h"
struct StructClientState;
struct StructPlayer;
class PD_NW_NetworkManager;
class PD_MG_MapParser;
class AParserActor;
class PD_GM_LogicCharacter;
class PD_MG_StaticMap;
class AMapInstantiatorActor;
class PD_GM_MapManager;
class PD_GM_GameManager;
class PD_PlayersManager; 
class APD_E_Character;

class AMapManagerAccesor;
#include "LevelsNameDictionary.h"

//Include de unreal
#include "Engine/GameInstance.h"
#include "PD_ClientGameInstance.generated.h"


/**
 * 
 */



UCLASS()															//Interfaz observer para reaccionar a eventos del netmanager
class PATD_CLIENT_API UPD_ClientGameInstance : public UGameInstance, public PD_NW_iEventObserver
{
	GENERATED_BODY()
	
	void InitializeNetworking();


public:

	//Managers
	PD_NW_NetworkManager* networkManager;
	PD_GM_MapManager* mapManager;
	PD_GM_GameManager* gameManager;
	PD_PlayersManager* playersManager;



#pragma region ACCESSORS

	AMapManagerAccesor * MapManagerAccesor;


#pragma endregion



	//Para tener los nombres de los niveles - diferenciar ejecución en editor o en ejecutable
	LevelsNameDictionary levelsNameDictionary;

	PD_MG_MapParser* mapParser;
	AParserActor* parserActor;

	//Overwrites

	///CONSTANTES
	const int32 defaultServerPort = 8890;
	FString serverAddressToConnect = "127.0.0.1"; //Por defecto

	
#pragma region State Machine GI

	//Overwrites - GameInstance
	virtual void Init();
	virtual void Shutdown();
	//Overwrites - iEventObserver
	virtual	bool SuscribeToEvents(int inPlayer, UStructType inType);
	virtual void HandleEvent(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType); //Aqui controlamos la mayoria de entradas
																								   //Carga de mapa

#pragma region State Machine - Handlers
	void HandleEvent_Welcome(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType);
	void HandleEvent_ConfigMatch(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType);
	void HandleEvent_ConfigMatchDone(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType);
	void HandleEvent_MapIncoming(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType);
	void HandleEvent_LaunchMatchFromServer(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType);
	void HandleEvent_AllCharactersInfoIncoming(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType);
	void HandleEvent_StartMatch_GoGameMnager(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType);

#pragma endregion

	//Funciones de configuracion de la maquina
	//Transiciones
	void UpdateState();
	//Acciones al empezar el estado
	void OnBeginState();
	//Funciones auxiliares
	//Control directo del estado, llama a OnBeginState
	void ChangeState(EClientState newState);
	//void InitState();

#pragma endregion
	
	void LoadMap(FString mapName);
	//Callback cuando el mapa este cargado (Lo llama el estado GameStateInitializer en su beginPlay)
	void OnLoadedLevel();

	//Struct con el estado del client
	StructClientState* structClientState;

	
	
#pragma region Blueprint Functions 

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
	void SetServerAddressToConnect(FString ip);

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
	bool GetIsGameMaster();

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		bool GetReadyToParty();

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		void GoToLobby();

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		void FillCharecterStats(int nPOD, int nAGI, int nDES, int nCON, int nPER, int nMAL);

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		void GetCharacterBasicStats(int &nPOD, int &nAGI, int &nDES, int &nCON, int &nPER, int &nMAL);
	
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		void GetCharacterTotalStats(int &nAP, int &nCH, int &nSA, int &nHP, int &nRAN, int &nDMG);
	//Se pasa por referencia el parametro "&" para que Unreal Engine lo detecte como parametro de salida
	//si pones const siempre lo va a detectar como parametro de entrada. Puedes entonces poner un mismo parametro como entrada y salida asi.

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		bool SendCharacterToServer();


	/// Funciones para crear Ordenes en cada Fase

	//Function para crear una orden de consumible para enviar
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		bool CreateConsumableOrder(int id_consumable);

	//Function para crear una orden de movimiento para enviar
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		bool CreateMovementOrder(int positionX, int positionY);
	//Function para quitar una orden de movimiento para enviar
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		bool DeleteLastMovementOrder(int positionX, int positionY);

	//Function para crear una orden de interactuable para enviar
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		bool CreateInteractableOrder (int id_interactable);

	//Function para crear una orden de Accion sobre baldosa para enviar
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		bool CreateActionToPosition (int id_action, TArray<FVector> positions);
	//Function para crear una orden de Accion sobre baldosa para enviar
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		bool CreateActionToCharacter(int id_action, TArray<FString> id_character);

	//Function para enviar el paquete con la orden entera al servidor
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		bool SendTurnOrderActionsToServer();

	//Function para cambiar el tipo de accion sobre una tile
	/*
	0 - Nada
	1 - Movimiento
	2 - Ataque
	*/
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		void SetTypeOfAction(int ntypeAction);
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		uint8 GetTypeOfAction();

	//Para saber el estado del GameManager desde la UI (o el propio Juego)
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		uint8 GetGameMngStatus();

	// Para saber el estado actual del GameManager
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		EClientGameState GetGameState();

	//Para saber la phase actual del GameManager
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		EClientGamePhase GetGamePhase();

	//Para saber el numero del jugador -> Caracter Visual a nivel de Interfaz
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		uint8 GetPlayerNumber();

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		void SaveCharacterLogicData();

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		void LoadCharacterLogicData();

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		void DeleteCharacterLogicData(FString slotName, int slotNumber);

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		APD_E_Character* GetCharacterPlayerAtPosition(FVector position, bool& existe);

	UFUNCTION(BlueprintCallable, Category = "GetAccessors")
		AMapManagerAccesor* GetMapManagerAccessor(bool& existe);

#pragma endregion


};



