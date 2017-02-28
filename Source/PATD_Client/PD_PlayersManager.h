// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

struct StructPlayer;
class PATD_CLIENT_API PD_PlayersManager
{

private:
	TArray<StructPlayer*> dataPlayers;


	bool CheckPlayerIndex(int player);

public:
	PD_PlayersManager();
	~PD_PlayersManager();
	int GetNumPlayers();
	//Funciones de gestion de conexion

	int GetMaxLenghtActions(EActionPhase phase);
	int GetPlayerMaxLenghtActions(EActionPhase phase);
	//Funciones de consulta de mision y objetivos de la partida

	//FStructTurnOrders* getTurnOrders(int player);
	//Funcion de acceso directo al struct 
	StructPlayer* GetDataStructPlayer(int player);
};
