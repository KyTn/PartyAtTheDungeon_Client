// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


//Forward declarations
//class PD_NW_iEventObserver;
#include "PD_NW_iEventObserver.h"

//Includes prueba
#include "Structs/PD_NetStructs.h"

/**
*
*/
class PATD_CLIENT_API PD_NW_EventManager
{
private:
	TArray<PD_NW_iEventObserver*> observerList;

public:
	PD_NW_EventManager();
	~PD_NW_EventManager();

	//Funcion de entrada al gestor. Aqui llama la capa de arriba para mandar un evento
	// (a todos los que escuchen) del tipo que sea el Struct.
	void GenerateEvent(FStructGeneric* st, int player);


	bool RegisterObserver(PD_NW_iEventObserver* observer);
	bool UnregisterObserver(PD_NW_iEventObserver* observer);

};


