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
	/*//Esta funcion la llama el socketManager cuando tiene un nuevo paquete de datos
	FStructGenericList* DeserializeData(TArray<uint8>* data, bool &correct);//El bool no lo veo necesario, total si fstruct es nulo no lo habra hecho bien


	//Esta funcion comunica con la capa superior para serializar datos
	TArray<uint8>* SerializeData(FStructGenericList* generycstructs, bool &correct);



	FStructGeneric* DeserializeData(TArray<uint8>* data);

	TArray<uint8>* SerializeData(FStructGeneric* generycstructs);
	*/

	//template <typename T>
	//T* Des(T*);

	//TArray<uint8>* SerializeData(FStructGenericoHito2* genericstruct);



	TArray<uint8>* SerializeData(FStructGeneric* structGeneric, UStructType type) {
		switch (type) {
		case UStructType::FStructMap: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::SerializeData:: Serializando FStructMap"));
			FStructMap* structSpecialization = (FStructMap*)structGeneric;
			return SerializeDataTemplate<FStructMap>(structSpecialization);

		}break;

		case UStructType::FStructGenericoHito2: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::SerializeData:: Serializando FStructGenericoHito2"));
			FStructGenericoHito2* structSpecialization = (FStructGenericoHito2*)structGeneric;
			return SerializeDataTemplate<FStructGenericoHito2>(structSpecialization);

		}break;


		default:
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::SerializeData:: Tipo de ustruct no reconocido"));
			break;
			/*case UStructType::FStructMap:
			FStructMap* structSpecialization = (FStructMap*)structGeneric;
			return SerializeDataTemplate<FStructMap>(structSpecialization);
			break;*/
		}

		return nullptr;

	}

	FStructGeneric*  DeserializeData(TArray<uint8>* data, UStructType type) {
		switch (type) {


		case UStructType::FStructMap: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::DeserializeData:: Deserializando FStructMap"));
			return DeserializeDataTemplate<FStructMap>(data);

		}break;

		case UStructType::FStructGenericoHito2: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::DeserializeData:: Deserializando FStructGenericoHito2"));
			return DeserializeDataTemplate<FStructGenericoHito2>(data);

		}break;


		default:
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::DeserializeData:: Tipo de ustruct no reconocido"));
			break;
		}

		return nullptr;

	}




	template<typename T>
	TArray<uint8>* SerializeDataTemplate(T* genericstruct) {
		TArray<uint8>* data = new TArray<uint8>();
		UStruct* FMyStruct = T::StaticStruct();
		FMemoryWriter ArWriter(*data);
		FMyStruct->SerializeBin(ArWriter, genericstruct);


		return data;
	}
	/*
	template<typename T>
	TArray<uint8>* SerializeData(T* genericstruct);
	*/

	//FStructGenericoHito2* DeserializeData(TArray<uint8>* data);

	template<typename T>
	T* DeserializeDataTemplate(TArray<uint8>* data) {
		T* generyStructs = new T();


		UStruct* FMyStruct = T::StaticStruct();


		FMemoryReader ArReader(*data);
		FMyStruct->SerializeBin(ArReader, generyStructs);


		return generyStructs;
	}

};
