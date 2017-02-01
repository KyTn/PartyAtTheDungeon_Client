// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "Engine/GameInstance.h"
#include "PD_ClientGameInstance.generated.h"




//forward declarations
class PD_NW_NetworkManager;

/**
 * 
 */






UCLASS()
class PATD_CLIENT_API UPD_ClientGameInstance : public UGameInstance
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
	bool isGameMaster = false;
	FString numPlayer;
	//Overwrites

	///CONSTANTES
	const int32 defaultServerPort = 8890;

	//Funcion que llama al inicializar 
	//Existe ya UWorld aqui, y GetTimerManager()??

	virtual void Init();

	//void Shutdown();

	//PD_NW_SocketManager* GetSocketManager();
	void LoadMap(FString mapName);

	void InitClientActoWhenLoadMap();
	
};



