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


void PD_NW_EventManager::GenerateEvent(FStructGeneric* st, int player) {
	for (int i = 0; i < observerList.Num(); i++)
	{
		if (!observerList[i]) {
			UE_LOG(LogTemp, Warning, TEXT("ObserverNull %d"), i);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("EventManager:: GenerateEvent::Observador %d , structtype %d "), i, st->structType);
			UStructType obsType = observerList[i]->getType();
			int obsPlayer = observerList[i]->getPlayer();

			//Para mostrar el nombre de un uenum.
			//	const UEnum* enumHelper = FindObject<UEnum>(ANY_PACKAGE, TEXT("UStructType"));
			//	enumHelper->GetDisplayNameText((int)st->structType.GetValue());
			//UE_LOG(LogTemp, Warning, TEXT("Parada structtype %s "), (int)st->structType.GetValue());
			UStructType eventType = UStructType::NoDefined;
			if (st) {





				eventType = UStructType(st->structType);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("EventManager:: GenerateEvent:: struct de evento null"))
			}

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

/*

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
*/