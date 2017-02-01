// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_SR_SerializerManager.h"

PD_SR_SerializerManager::PD_SR_SerializerManager() {

}
PD_SR_SerializerManager::~PD_SR_SerializerManager() {

}

FStructGenericList* PD_SR_SerializerManager::DeserializeData(TArray<uint8>* data, bool & correct)
{

	UE_LOG(LogTemp, Warning, TEXT("indice data   %d"), data->Num());

	FStructGenericList* generyStructs = new FStructGenericList();
	UE_LOG(LogTemp, Warning, TEXT("indice tras new %d"), generyStructs->list.Num());

	UStruct* FMyStruct = FStructGenericList::StaticStruct();
	UE_LOG(LogTemp, Warning, TEXT("indice tras Ustruct %d"), generyStructs->list.Num());

	FMemoryReader ArReader(*data);
	FMyStruct->SerializeBin(ArReader, &generyStructs);//No estoy seguro si añade las estructuras al array

	UE_LOG(LogTemp, Warning, TEXT("indice tras serializebin %d"), generyStructs->list.Num());

	if (generyStructs->list.Num() == 0) {// si esta vacio no le llegara nada, ¿no?
		correct = false;
		return nullptr;
	}
	else {
		correct = true;
		return generyStructs;
	}
}

TArray<uint8>* PD_SR_SerializerManager::SerializeData(FStructGenericList* generycStructs, bool & correct)
{
	TArray<uint8>* data = new TArray<uint8>();
	UStruct* FMyStruct = FStructGenericList::StaticStruct();

	FMemoryWriter ArWriter(*data);
	FMyStruct->SerializeBin(ArWriter, &generycStructs);
	if (data->Num() == 0) {// si esta vacio no le llegara nada, ¿no?
		correct = false;
		return nullptr;
	}
	else {
		correct = true;
		return data;
	}
}



FStructGeneric* PD_SR_SerializerManager::DeserializeData(TArray<uint8>* data) {
	FStructGeneric* genericStruct = new FStructGeneric(); //aqui va a haber problemas

	UStruct* FMyStruct = FStructGeneric::StaticStruct();
	FMemoryReader ArReader(*data);
	FMyStruct->SerializeBin(ArReader, &genericStruct);
	return genericStruct;

}

TArray<uint8>* PD_SR_SerializerManager::SerializeData(FStructGeneric* genericstruct) {
	TArray<uint8>* data = new TArray<uint8>();
	UStruct* FMyStruct = FStructGeneric::StaticStruct();
	FMemoryWriter ArWriter(*data);
	FMyStruct->SerializeBin(ArWriter, &genericstruct);
	return data;
}