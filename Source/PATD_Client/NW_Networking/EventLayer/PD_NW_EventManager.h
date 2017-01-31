// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SR_Serializer/PD_SR_UStruct.h"
//Forward declarations
class PD_NW_iEventObserver;


//Includes prueba


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

	//Funcion de entrada al gestor. Aqui llama la capa de abajo para mandar un evento
	// (a todos los que escuchen) del tipo que sea el Struct.
	void GenerateEvent(FStructGeneric* st, int player);

	//Funciones de registro de un observer. El observer debe de estar instanciado y seteado.
	bool RegisterObserver(PD_NW_iEventObserver* observer);

	bool UnregisterObserver(PD_NW_iEventObserver* observer);

};


