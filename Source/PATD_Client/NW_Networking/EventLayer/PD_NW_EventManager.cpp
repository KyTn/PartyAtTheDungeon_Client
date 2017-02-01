// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_NW_EventManager.h"

//Includes of forward declaration

#include "PD_NW_iEventObserver.h"


PD_NW_EventManager::PD_NW_EventManager()
{
}

PD_NW_EventManager::~PD_NW_EventManager()
{
}

bool PD_NW_EventManager::RegisterObserver(PD_NW_iEventObserver* observer) {
	//quizas comprobar que la configuracion sea valida?
	observerList.Add(observer);
	return true;
}

bool  PD_NW_EventManager::UnregisterObserver(PD_NW_iEventObserver* observer) {
	int num = observerList.Remove(observer);
	return num>0;
}

/*
void PD_NW_EventManager::GenerateEvent(FStructGeneric* st, int player) {
for (int i = 0; i < observerList.Num(); i++)
{
if (!observerList[i]) {
UE_LOG(LogTemp, Warning, TEXT("ObserverNull %d"), i);
}
else {
UE_LOG(LogTemp, Warning, TEXT("Generando evento %d "), i);
UStructType obsType = observerList[i]->getType();
int obsPlayer = observerList[i]->getPlayer();
UE_LOG(LogTemp, Warning, TEXT("Parada structtype %d "), st->structType);


UStructType eventType = UStructType(st->structType);
int eventPlayer = player;
UE_LOG(LogTemp, Warning, TEXT("Parada 2 "), i);
//Comprobamos que el observador quiere recibir eventos del jugador y el tipo del que viene.
if ((obsType == UStructType::AllStructs || obsType == eventType)
&& (obsPlayer == -1 || obsPlayer == eventPlayer)) {
UE_LOG(LogTemp, Warning, TEXT("Parada 3 dentro if "), i);
//Llamar a la funcion virtual de la interfaz. Aqui cada uno pondra su codigo.
observerList[i]->handleEvent(st, eventPlayer, eventType);
UE_LOG(LogTemp, Warning, TEXT("Parada 4 dentro if "), i);
}
}
}
}*/

void PD_NW_EventManager::GenerateEvent(FStructGenericoHito2* st, int player) {
	for (int i = 0; i < observerList.Num(); i++)
	{
		if (!observerList[i]) {

		}
		else {

			UStructType obsType = observerList[i]->getType();
			int obsPlayer = observerList[i]->getPlayer();
			UStructType eventType = UStructType::FStructOrderMenu;
			int eventPlayer = player;
			//Comprobamos que el observador quiere recibir eventos del jugador y el tipo del que viene.
			if ((obsType == UStructType::AllStructs || obsType == eventType)
				&& (obsPlayer == -1 || obsPlayer == eventPlayer)) {

				//Llamar a la funcion virtual de la interfaz. Aqui cada uno pondra su codigo.
				observerList[i]->handleEvent(st, eventPlayer, eventType);

			}
		}
	}
}