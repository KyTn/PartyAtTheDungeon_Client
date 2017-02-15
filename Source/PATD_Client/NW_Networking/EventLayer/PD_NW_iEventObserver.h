// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SR_Serializer/PD_SR_UStruct.h"
/**
*
*/

class PATD_CLIENT_API PD_NW_iEventObserver
{
private:

	//De esta manera puede escuchar a un player o a todos y a un tipo o a todos.
	//Para escuchar dos players especificos, dos observers.
	int player; //-1 for all
	UStructType eventType; //UStructType::AllStructs for all


public:
	PD_NW_iEventObserver();
	~PD_NW_iEventObserver();

	//Seters and getters;
	void setUpObserver(int inPlayer, UStructType inType);
	int getPlayer();
	UStructType getType();

	//El struct desaparecera despues de esta llamada. Si se desea guardar hay que copiarlo en la llamada.
	virtual void handleEvent(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) = 0;
	//virtual void handleEvent(FStructGenericoHito2* dataStruct, int inPlayer, UStructType inEventType) = 0;


	//	virtual void handleEvent(T* dataStruct, int inPlayer, UStructType inEventType) = 0;
};
