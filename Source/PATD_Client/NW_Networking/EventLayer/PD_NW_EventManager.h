// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


//Forward declarations
//class PD_NW_iEventObserver;
#include "PD_NW_iEventObserver.h"

//Includes prueba
#include "SR_Serializer/PD_SR_UStruct.h"

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
	//void GenerateEvent(FStructGeneric* st, int player);
	//void GenerateEvent(FStructGenericoHito2* st, int player);



	void GenerateEvent(FStructGeneric* st, int player);// {
													   /*	for (int i = 0; i < observerList.Num(); i++)
													   {
													   if (!observerList[i]) {

													   }
													   else {

													   UStructType obsType = observerList[i]->getType();
													   int obsPlayer = observerList[i]->getPlayer();
													   UStructType eventType = st->structType;
													   int eventPlayer = player;
													   //Comprobamos que el observador quiere recibir eventos del jugador y el tipo del que viene.
													   if ((obsType == UStructType::AllStructs || obsType == eventType)
													   && (obsPlayer == -1 || obsPlayer == eventPlayer)) {

													   //Llamar a la funcion virtual de la interfaz. Aqui cada uno pondra su codigo.
													   observerList[i]->handleEvent(st, eventPlayer, eventType);

													   }
													   }
													   }
													   }*/
													   //Funciones de registro de un observer. El observer debe de estar instanciado y seteado.
	bool RegisterObserver(PD_NW_iEventObserver* observer);

	bool UnregisterObserver(PD_NW_iEventObserver* observer);

};


