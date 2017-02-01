// Fill out your copyright notice in the Description page of Project Settings.

#include "SR_Serializer/PD_SR_UStruct.h"
#pragma once

//forward declarations
class PD_NW_SocketManager;
class PD_NW_EventManager;
class PD_NW_iEventObserver;
class PD_SR_SerializerManager;
/**
*
*/




class PATD_CLIENT_API PD_NW_NetworkManager
{
private:
	//PD_NW_CompressionManager* compressionManager;
	PD_NW_SocketManager* socketManager;
	PD_NW_EventManager* eventManager;
	PD_SR_SerializerManager* serializerManager;

	TArray<FStructGenericList*> sendGenericStruct;
	//	TArray<FStructGenericList>* reciveGenericStruct;







public:
	PD_NW_NetworkManager();
	~PD_NW_NetworkManager();

	//Getters de Managers internos
	//PD_NW_CompressionManager* GetCompressionManager();
	PD_NW_SocketManager* GetSocketManager();
	PD_NW_EventManager* GetEventManager();
	PD_SR_SerializerManager* GetSerializerManager();

	/*Para enviar, hace una copia del struct generico, asi lo que pase con el en la capa de aplicacion no nos afecta
	despues tiene reservado espacio para el struct-array, que se va limpiando segun se envia
	Finalmente al llamar a serializar nos devuelve una copia serializada en array<uint8>
	que se borra tras enviar
	*/

	//Funciones publicas para enviar
	bool SendNow(FStructGenericoHito2* st, int player = -1);

	/*bool SendNow(FStructGeneric* st,int player=-1);
	bool SendNow(int player=-1);
	bool AddToSendList(FStructGeneric* st, int player=-1);*/
	//Funcion para conectar (se usa en el cliente) Devuelve el "player" que seria el servidor
	//que deberia ser 0 si solo tenemos un servidor. Es el que usaremos en las funciones de enviar y recibir.
	//-1 para error. Encapsula una llamada al socketmanager y crea el buffer para esta conexion.
	int ConnectTo(FString ip, int port);

	//Funciones publicas para recibir (Registrar observer);
	bool RegisterObserver(PD_NW_iEventObserver* observer);
	bool UnregisterObserver(PD_NW_iEventObserver* observer);




	//Punto de entrada de los eventos del socketManager
	//Podemos hacerlas friend para ponerlas privadas y que no se vean. El socketManager necesita llamarlas.

	/*Para recibir, reserva memoria en el socket manager para array<uint8>
	y despues hace 1 copia en el serializer para el struct-lista.
	Las dos reservas se deletean dentro de la llamada de HandleNewSocketData.
	Si se quiere guardar los datos del struct hay que hacerlo en la llamada del evento.
	(haciendo otra copia, ya a cargo de la capa de aplicacion)

	(Si quisieramos podemos hacer que el struct generico se copie especifico para el evento siempre, evitando la necesidad de antes)
	*/
	void HandleNewSocketData(TArray<uint8>* data, int socketIndex);
	void HandleNewConnectionSocketListener(int player);

};
