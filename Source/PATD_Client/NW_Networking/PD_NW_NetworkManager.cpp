// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_NW_NetworkManager.h"

//Includes of forward declaration
#include "NW_NetWorking/Socket/PD_NW_SocketManager.h"
#include "NW_NetWorking/EventLayer/PD_NW_EventManager.h"
#include "SR_Serializer/PD_SR_SerializerManager.h"


PD_NW_NetworkManager::PD_NW_NetworkManager()
{

	socketManager = new PD_NW_SocketManager();
	eventManager = new PD_NW_EventManager();
	serializerManager = new PD_SR_SerializerManager();
}

PD_NW_NetworkManager::~PD_NW_NetworkManager()
{
	delete socketManager;
	delete eventManager;
}


void PD_NW_NetworkManager::HandleNewSocketData(TArray<uint8>* data, int socketIndex) {


	int sizeAll = data->Num();

	while (sizeAll > 0) {

		/*
		FString s = "[";
		for (int i = 0; i < (*data).Num(); i++) {

		s.AppendInt(i);
		s.AppendChar(':');
		s.AppendInt((*data)[i]);
		s.AppendChar(',');
		}
		s.AppendChar(']');

		UE_LOG(LogTemp, Warning, TEXT("NetworkManager::SendNow:: Recibiendo info: tamaño CON cabecera: %d --> %s"), (*data).Num(), *s);

		*/




		uint32 size = ((*data)[0] << 24) + ((*data)[1] << 16) + ((*data)[2] << 8) + ((*data)[3]);
		uint8 type = (*data)[4];

		TArray<uint8> dataaux = TArray<uint8>();

		//UE_LOG(LogTemp, Warning, TEXT("NetworkManager::SendNow:: Size SIN cabecera : %d"), size);
		dataaux.Init(0, size);
		for (uint32 i = 5; i < 5 + size; i++) {

			//UE_LOG(LogTemp, Warning, TEXT("NetworkManager::SendNow:: Limpiando cabecera ... [%d:%d]"), i, (*data)[i]);

			dataaux[i - 5] = (*data)[i];
			//dataaux.Emplace((*data)[i], (int)i - 5);
			//dataaux.Add((*data)[i]);
		}

		/*
		s = "[";
		for (int i = 0; i < dataaux.Num(); i++) {

		s.AppendInt(i);
		s.AppendChar(':');
		s.AppendInt(dataaux[i]);
		s.AppendChar(',');
		}
		s.AppendChar(']');

		UE_LOG(LogTemp, Warning, TEXT("NetworkManager::SendNow:: Recibiendo info: tamaño SIN cabecera: %d --> %s"), dataaux.Num(), *s);
		*/



		FStructGeneric* genericStruct = serializerManager->DeserializeData(&dataaux, UStructType(type)); //Otra copia?
																										 //genericStruct->structType = type;
		UE_LOG(LogTemp, Warning, TEXT("NetworkManager::HandleNewSocketData:: While: type: %d : type2: %d"), type, genericStruct->structType);

		eventManager->GenerateEvent(genericStruct, socketIndex);





		sizeAll -= (5 + size);


		for (uint32 i = 0; i < 5 + size; i++) {
			(*data).RemoveAt(0);
		}

	}

}


//Cuando hay una nueva conexion creamos un nuevo elemento para el array de structs-array
//Este se borrara al hacer el delete de la clase 

void PD_NW_NetworkManager::HandleNewConnectionSocketListener(int socketIndex) {


	UE_LOG(LogTemp, Warning, TEXT("NetworkManager::HandleNewConnectionSocketListener:: socketIndex %d"), socketIndex);

	//Generar el evento de nuevo jugador. 
	//TODO Transformar a struct de nueva conexion
	FStructNewConnection* structNewConnection = new FStructNewConnection(); //alaaaa otro new

	eventManager->GenerateEvent(structNewConnection, socketIndex);
}


PD_NW_SocketManager* PD_NW_NetworkManager::GetSocketManager() {
	return socketManager;
}
PD_NW_EventManager* PD_NW_NetworkManager::GetEventManager() {
	return eventManager;
}

PD_SR_SerializerManager* PD_NW_NetworkManager::GetSerializerManager() {
	return serializerManager;
}


bool PD_NW_NetworkManager::SendNow(FStructGeneric* structGeneric, int player) {

	//Falta por hacer que se pueda enviar y recibir una lista (lo del compresor)


	//UE_LOG(LogTemp, Warning, TEXT("NetworkManager::SendNow:: Enviando data a player %d. Struct de tipo %d"),player, structGeneric->structType);

	TArray<uint8>* dataIn = serializerManager->SerializeData(structGeneric, UStructType(structGeneric->structType));
	/*
	FString s = "[";
	for (int i = 0; i < dataIn->Num(); i++) {

		s.AppendInt(i);
		s.AppendChar(':');
		s.AppendInt((*dataIn)[i]);
		s.AppendChar(',');
	}
	s.AppendChar(']');
	*/
	//UE_LOG(LogTemp, Warning, TEXT("NetworkManager::SendNow:: Enviando data a player %d. Struct de tipo %d - tamaño precabecera: %d"), player, structGeneric->structType, dataIn->Num());


	// size + dataIn 


	if (dataIn) {

		uint32 size = dataIn->Num();
		uint8 mask = 255;
		uint8 aux = size >> 24;
		uint8 aux2 = (size >> 16) & mask;
		uint8 aux3 = (size >> 8) & mask;
		uint8 aux4 = size & mask;
		dataIn->Insert(aux, 0);
		dataIn->Insert(aux2, 1);
		dataIn->Insert(aux3, 2);
		dataIn->Insert(aux4, 3);
		dataIn->Insert(structGeneric->structType, 4);
		/*
		s = "[";
		for (int i = 0; i < dataIn->Num(); i++) {

			s.AppendInt(i);
			s.AppendChar(':');
			s.AppendInt((*dataIn)[i]);
			s.AppendChar(',');
		}
		s.AppendChar(']');

		UE_LOG(LogTemp, Warning, TEXT("NetworkManager::SendNow:: Enviando data a player %d. Struct de tipo %d - tamaño POSTcabecera: %d --> %s"), player, structGeneric->structType, dataIn->Num(), *s);
		*/


		//Construyendo el dataStruct
		//FStructData dataStruct;
		//dataStruct.data = *dataIn; //Hace una copia o simplemente se asigna??
		//dataStruct.structType = structGeneric->structType;

		//TArray<uint8>* dataSend = serializerManager->SerializeDataTemplate<FStructData>(&dataStruct);
		//Enviar dataStruct, en realidad deberiamos meter este dataStruct en el struct de lista y enviar ese.

		return socketManager->SendInfoTo(player, dataIn);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NetworkManager::SendNow:: data recibida del serializador null"));

		return false;
	}

}

/*
//Funciones publicas para enviar
bool PD_NW_NetworkManager::SendNow(FStructGeneric* st,int player) {
//Para hacer broadcast con -1?

TArray<uint8>* data = serializerManager->SerializeData(st);
socketManager->SendInfoTo(player, data);


//Con compresor
AddToSendList(st, player);
SendNow(player);

*/
/*
return true;
}*/
//Con compresor
/*
bool PD_NW_NetworkManager::SendNow(int player) {//serializamos y enviamos los datos de un jugador, tras esto deberian borrarse¿?



bool serializeOk;
//Para enviar todo lo pendiente con -1?
if (!sendGenericStruct.IsValidIndex(player) || !sendGenericStruct[player]) {
return false; //Hay un error por que el FStructGenericList deberia estar creado
}

FStructGenericList* structPlayer = sendGenericStruct[player];
TArray<uint8>* data = serializerManager->SerializeData(structPlayer, serializeOk); //Esto nos devuelve una copia
structPlayer->list.Empty(); //Aqui tenemos nuestro struct-array limpio. Empty llamara a los destructores de los structs internos.
//Deberia ser como hacer delete structPlayer y volverlo a crear.

socketManager->SendInfoTo(player, data);
//delete data; //Aqui ya podemos borrar los datos serializados que hemos creado.

//	structPlayer->list.Empty();
//	sendGenericStruct->Insert(*structPlayer, player); Vaciar la posicion de ese jugador en el servidor del buffer de enviar datos
return serializeOk;
}

bool PD_NW_NetworkManager::AddToSendList(FStructGeneric* st, int player) {//Actualiza los datos del buffer de salida de ese cliente
if (!sendGenericStruct.IsValidIndex(player) || !sendGenericStruct[player]) {
return false; //Hay un error por que el FStructGenericList deberia estar creado
}

FStructGenericList* structPlayer = sendGenericStruct[player];

//Necesitamos hacer una copia para poder borrar estos structs sin miedo a que se borren en la capa de aplicacion.
FStructGeneric* structCopy = new FStructGeneric(*st);


structPlayer->list.Add(*structCopy);//Aqui hara una copia del FStruct? Creo que no. Por eso los copiamos.

//sendGenericStruct->Insert(*structPlayer,player);
//compressionManager.AddToSendList(st, player);
return true;
}

*/

int  PD_NW_NetworkManager::ConnectTo(FString ip, int port) {
	int player = socketManager->ConnectDataSocket(ip, port);
	if (player == -1) {
		//ERROR!!
		UE_LOG(LogTemp, Error, TEXT("No se ha podido crear el Socket Cliente! "));

	}
	else {
		//Insertamos el buffer igual que para un nuevo jugador que venga del listener.
		sendGenericStruct.Insert(new FStructDataList(), player);

	}
	return player;
}


//Funciones publicas para recibir (Registrar observer);
bool PD_NW_NetworkManager::RegisterObserver(PD_NW_iEventObserver* observer) {
	return eventManager->RegisterObserver(observer);
}

bool PD_NW_NetworkManager::UnregisterObserver(PD_NW_iEventObserver* observer) {
	return eventManager->UnregisterObserver(observer);
}