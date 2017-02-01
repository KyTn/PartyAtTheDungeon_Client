// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_NW_NetworkManager.h"

//Includes of forward declaration
#include "NW_NetWorking/Socket/PD_NW_SocketManager.h"
#include "NW_NetWorking/EventLayer/PD_NW_EventManager.h"
#include "SR_Serializer/PD_SR_SerializerManager.h"

/*Todo la investigacion para hacerlo con structs genericos esta comentado
Queda como codigo lo necesario para pasar solamente Mapa y MenuOrder (lo necesario para el hito2)
*/

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


	FStructGenericoHito2* structEventGenericHito2 = serializerManager->DeserializeData(data);
	eventManager->GenerateEvent(structEventGenericHito2, socketIndex);


	/*
	FStructGeneric* structEvent = serializerManager->DeserializeData(data);

	eventManager->GenerateEvent(structEvent, socketIndex);
	*/
	/* Con compresor

	bool serializeOk;
	//No necesitamos pasar por el compresor aqui
	UE_LOG(LogTemp, Warning, TEXT("Handle Paso1 "));

	FStructGenericList* structList=serializerManager->DeserializeData(data, serializeOk);
	UE_LOG(LogTemp, Warning, TEXT("Handle Paso2 "));

	//Aqui ya podemos hacer delete del array de datos pues el serializer crea datos nuevos.
	//delete data;
	UE_LOG(LogTemp, Warning, TEXT("Handle Paso3 %d"), structList->list.Num());

	for (int i = 0; i < structList->list.Num(); i++)
	{
	UE_LOG(LogTemp, Warning, TEXT("Handle Paso4 "));


	FStructGeneric* structEvent = &(structList->list[i]); //Cogemos la referencia. El struct sigue alojado en el array
	UE_LOG(LogTemp, Warning, TEXT("Handle Paso5 "));

	eventManager->GenerateEvent(structEvent, socketIndex);//Generamos el evento con el puntero.

	}*/
	//una vez lanzados todos los eventos debemos borrar los datos creados (aqui se borran los structs que reciben en los eventos,
	//por eso hay que copiarlos si se quieren conservar). 
	//	delete structList;


}
//Cuando hay una nueva conexion creamos un nuevo elemento para el array de structs-array
//Este se borrara al hacer el delete de la clase 

void PD_NW_NetworkManager::HandleNewConnectionSocketListener(int socketIndex) {
	//Con compresor
	//sendGenericStruct.Insert(new FStructGenericList(),socketIndex);

	//Generar el evento de nuevo jugador.

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

bool PD_NW_NetworkManager::SendNow(FStructGenericoHito2* st, int player) {

	
	TArray<uint8>* data=serializerManager->SerializeData(st);
	return socketManager->SendInfoTo(player, data);

}

/*/
//Funciones publicas para enviar
bool PD_NW_NetworkManager::SendNow(FStructGeneric* st,int player) {
//Para hacer broadcast con -1?

TArray<uint8>* data = serializerManager->SerializeData(st);
socketManager->SendInfoTo(player, data);


//Con compresor
/*AddToSendList(st, player);
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
		sendGenericStruct.Insert(new FStructGenericList(), player);

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