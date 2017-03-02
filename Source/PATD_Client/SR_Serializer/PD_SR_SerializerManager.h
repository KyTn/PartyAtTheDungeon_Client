// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
*
*/
#include "Structs/PD_NetStructs.h"



class PATD_CLIENT_API PD_SR_SerializerManager
{
public:
	PD_SR_SerializerManager();
	~PD_SR_SerializerManager();

	//El serializer crea nuevo espacio en memoria y copia el que le llega.
	//No hace delete de la estructura vieja.
	//Hay posibilidad de mejorar eso?

	TArray<uint8>* SerializeData(FStructGeneric* structGeneric, UStructType type) {
		switch (type) {
		case UStructType::FStructMap: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::SerializeData:: Serializando FStructMap"));
			FStructMap* structSpecialization = (FStructMap*)structGeneric;
			return SerializeDataTemplate<FStructMap>(structSpecialization);

		}break;

		case UStructType::FStructOrderMenu: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::SerializeData:: Serializando FStructOrderMenu"));
			FStructOrderMenu* structSpecialization = (FStructOrderMenu*)structGeneric;
			return SerializeDataTemplate<FStructOrderMenu>(structSpecialization);

		}break;

		case UStructType::FStructTurnOrders: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::SerializeData:: Serializando FStructTurnOrders"));
			FStructTurnOrders* structSpecialization = (FStructTurnOrders*)structGeneric;
			return SerializeDataTemplate<FStructTurnOrders>(structSpecialization);

		}break;
		case UStructType::FStructCharacter: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::SerializeData:: Serializando FStructCharacter"));
			FStructCharacter* structSpecialization = (FStructCharacter*)structGeneric;
			return SerializeDataTemplate<FStructCharacter>(structSpecialization);

		}break;
		case UStructType::FStructUpdateCharacter: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::SerializeData:: Serializando FStructUpdateCharacter"));
			FStructUpdateCharacter* structSpecialization = (FStructUpdateCharacter*)structGeneric;
			return SerializeDataTemplate<FStructUpdateCharacter>(structSpecialization);

		}break;
		case UStructType::FStructInstatiatePlayers: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::SerializeData:: Serializando FStructInstatiatePlayers"));
			FStructInstatiatePlayers* structSpecialization = (FStructInstatiatePlayers*)structGeneric;
			return SerializeDataTemplate<FStructInstatiatePlayers>(structSpecialization);

		}break;

		default:
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::SerializeData:: Tipo de ustruct no reconocido"));
			break;

		}

		return nullptr;

	}

	FStructGeneric*  DeserializeData(TArray<uint8>* data, UStructType type) {
		switch (type) {


		case UStructType::FStructMap: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::DeserializeData:: Deserializando FStructMap"));
			return DeserializeDataTemplate<FStructMap>(data);

		}break;

		case UStructType::FStructOrderMenu: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::DeserializeData:: Deserializando FStructOrderMenu"));
			return DeserializeDataTemplate<FStructOrderMenu>(data);

		}break;

		case UStructType::FStructTurnOrders: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::DeserializeData:: Deserializando FStructTurnOrders"));
			return DeserializeDataTemplate<FStructTurnOrders>(data);

		}break;
		case UStructType::FStructCharacter: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::DeserializeData:: Deserializando FStructCharacter"));
			return DeserializeDataTemplate<FStructCharacter>(data);

		}break;
		case UStructType::FStructUpdateCharacter: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::DeserializeData:: Deserializando FStructUpdateCharacter"));
			return DeserializeDataTemplate<FStructUpdateCharacter>(data);

		}break;
		case UStructType::FStructInstatiatePlayers: {
			UE_LOG(LogTemp, Warning, TEXT("SerializerManager::DeserializeData:: Deserializando FStructInstatiatePlayers"));
			return DeserializeDataTemplate<FStructInstatiatePlayers>(data);

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

	template<typename T>
	T* DeserializeDataTemplate(TArray<uint8>* data) {
		T* generyStructs = new T();


		UStruct* FMyStruct = T::StaticStruct();


		FMemoryReader ArReader(*data);
		FMyStruct->SerializeBin(ArReader, generyStructs);


		return generyStructs;
	}

};
