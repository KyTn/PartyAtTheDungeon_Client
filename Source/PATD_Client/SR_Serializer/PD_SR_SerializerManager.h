// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
*
*/
#include "SR_Serializer/PD_SR_UStruct.h"

class PATD_CLIENT_API PD_SR_SerializerManager
{
public:
	PD_SR_SerializerManager();
	~PD_SR_SerializerManager();

	//El serializer crea nuevo espacio en memoria y copia el que le llega.
	//No hace delete de la estructura vieja.
	//Hay posibilidad de mejorar eso?


	/*Realmente los booleanos podrian ser lo que devuelvan y que se les pase por parametro el struct
	y el tArray.
	*/
	//Esta funcion la llama el socketManager cuando tiene un nuevo paquete de datos
	FStructGenericList* DeserializeData(TArray<uint8>* data, bool &correct);//El bool no lo veo necesario, total si fstruct es nulo no lo habra hecho bien


																			//Esta funcion comunica con la capa superior para serializar datos
	TArray<uint8>* SerializeData(FStructGenericList* generycstructs, bool &correct);



	FStructGeneric* DeserializeData(TArray<uint8>* data);

	TArray<uint8>* SerializeData(FStructGeneric* generycstructs);




};
