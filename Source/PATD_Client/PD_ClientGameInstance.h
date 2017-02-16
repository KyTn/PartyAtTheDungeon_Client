// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


//Include de herencia (interfaz)
#include "NW_NetWorking/EventLayer/PD_NW_iEventObserver.h"

//Include de unreal
#include "Engine/GameInstance.h"
#include "LevelsNameDictionary.h"
#include "PD_ClientGameInstance.generated.h"




//forward declarations
class PD_NW_NetworkManager;

/**
 * 
 */



UCLASS()															//Interfaz observer para reaccionar a eventos del netmanager
class PATD_CLIENT_API UPD_ClientGameInstance : public UGameInstance, public PD_NW_iEventObserver
{
	GENERATED_BODY()
	
	//Networking
		//PD_NW_SocketManager* socketManager;
		PD_NW_NetworkManager* networkManager;

	//Functions ====
	//Funcion para inicializar entre otros el socketManager. 
	void InitializeNetworking();

	//Funcion para volver a poner todo adecuadamente despues de un travel.
	void InitializeAfterTravel();

public:


	//Para tener los nombres de los niveles - diferenciar ejecución en editor o en ejecutable
	LevelsNameDictionary levelsNameDictionary;

	FString mapString;
	bool isGameMaster = false;
	FString numPlayer;
	FString serverAddressToConnect = "127.0.0.1"; //Por defecto
	//Overwrites

	///CONSTANTES
	const int32 defaultServerPort = 8890;

	//Funcion que llama al inicializar 
	//Existe ya UWorld aqui, y GetTimerManager()??

	virtual void Init();
	

	//Funciones del eventObserver, para determinar que eventos recibimos y para procesarlos
	virtual	bool SuscribeToEvents(int inPlayer, UStructType inType);
	virtual void HandleEvent(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType);


	//void Shutdown();
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
	void SetServerAddressToConnect(FString ip);

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
	bool GetIsGameMaster();

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		void GetReadyToParty();

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		void GoToLobby();
	//PD_NW_SocketManager* GetSocketManager();
	void LoadMap(FString mapName);

	void InitGameMap();
	void InitClientActoWhenLoadMap();
	
};



