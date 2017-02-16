// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SR_Serializer/PD_SR_UStruct.h"
/**
*
*/

class PATD_CLIENT_API PD_NW_iEventObserver
{
private:




public:


	//Debe devolver true cuando quieres recibir el evento.
	virtual	bool SuscribeToEvents(int inPlayer, UStructType inType) = 0;


	//El struct desaparecera despues de esta llamada. Si se desea guardar hay que copiarlo en la llamada.
	virtual void HandleEvent(FStructGeneric* inDataStruct, int inPlayer, UStructType inEventType) = 0;
	//virtual void handleEvent(FStructGenericoHito2* dataStruct, int inPlayer, UStructType inEventType) = 0;


	//	virtual void handleEvent(T* dataStruct, int inPlayer, UStructType inEventType) = 0;
};
