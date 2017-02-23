// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


//Include de herencia (interfaz)
#include "NW_NetWorking/EventLayer/PD_NW_iEventObserver.h"
//forward declarations
//Include de enums (no se pueden hacer forward declaration)
#include "Structs/PD_ClientEnums.h"
struct StructClientState;
class PD_NW_NetworkManager;
class AParserActor;
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
	PD_NW_NetworkManager* networkManager;
	AParserActor* parserActor;


	//Para tener los nombres de los niveles - diferenciar ejecución en editor o en ejecutable
	LevelsNameDictionary levelsNameDictionary;

	//Overwrites

	///CONSTANTES
	const int32 defaultServerPort = 8890;
	FString serverAddressToConnect = "127.0.0.1"; //Por defecto

	//Funcion que llama al inicializar 
	//Existe ya UWorld aqui, y GetTimerManager()??

	//Overwrites - GameInstance
	virtual void Init();
	virtual void Shutdown();
	//Overwrites - iEventObserver
	virtual	bool SuscribeToEvents(int inPlayer, UStructType inType);
	virtual void HandleEvent(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType); //Aqui controlamos la mayoria de entradas.

																								   //Carga de mapa
	void LoadMap(FString mapName);
	//Callback cuando el mapa este cargado (Lo llama el estado GameStateInitializer en su beginPlay)
	void OnMapFinishLoad();

	//=========
	//Funciones de gestion del estado (maquina de estados)
	//=========

	//Struct con el estado del client
	StructClientState* structClientState;

	//Funciones de configuracion de la maquina
	//Transiciones
	void UpdateState();
	//Acciones al empezar el estado
	void OnBeginState();
	//Funciones auxiliares
	//Control directo del estado, llama a OnBeginState
	void ChangeState(EClientState newState);
	//void InitState();


	UFUNCTION(BlueprintCallable, Category = "GameInstance")
	void SetServerAddressToConnect(FString ip);

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
	bool GetIsGameMaster();

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		void GetReadyToParty();

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		void GoToLobby();

	void InitGameMap();
	void InitClientActoWhenLoadMap();
	
};



