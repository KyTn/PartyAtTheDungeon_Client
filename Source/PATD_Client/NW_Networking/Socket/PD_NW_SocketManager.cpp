// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_NW_SocketManager.h"

//Includes of forward declaration
#include "PD_NW_Socket.h" 
#include "Networking.h"
#include "NW_NetWorking/PD_NW_ServerActor.h"


/******************************
*** CONSTRUCTOR Y DESTRUCTOR DE LA CLASE **
/******************************/
PD_NW_SocketManager::PD_NW_SocketManager()
{
	socketArray = TArray<PD_NW_Socket*>();
}

PD_NW_SocketManager::~PD_NW_SocketManager()
{
	//delete socketArray;
}



/******************************
*** FUNCIONES **
/******************************/
void PD_NW_SocketManager::Init(APD_NW_ServerActor* InmyServerActor)
{
	UE_LOG(LogTemp, Warning, TEXT("INICIANDO SOCKET MANAGER! "));
	SetServerActor(InmyServerActor);
	if (isServer)
	{
		InitSocketManager_ServerMode();
	}
	else
	{
		InitSocketManager_ClientMode();
	}
	
}

void PD_NW_SocketManager::InitSocketManager_ServerMode()
{
	UE_LOG(LogTemp, Warning, TEXT("Socket MANAGER como SERVIDOR!! "));
	myServerActor->InitTimerActor();
}

void PD_NW_SocketManager::InitSocketManager_ClientMode()
{
	UE_LOG(LogTemp, Warning, TEXT("Socket MANAGER como CLIENTE!! "));
	/*
	1.Crear el Socket que va a comunicar con el Servidor
	2.Comprobar que hay comunicacion
	3.Cuando hay comunicacion, Guadar dicho socket en el Array de Socket del SocketManager 
	4.Llamar al Timer del ServerActor para que empiece a escuhar para futuras comunicaciones con dicho socket.
	*/

}

void PD_NW_SocketManager::SocketHasReceivedData(TArray<uint8> data, int socketIndex) {

	//No necesitamos esto porque la funcion del timer ya tendra un bucle
	/*//Busqueda del socket que lo envia para poder guardar la variable int socketIndex .
	int index = socketArray->IndexOfByKey(socket) != -1; //-1 == INDEX_NONE
	if (index != -1) {
	receivedDataAtSocket(data, index);
	}
	else {
	//ERROR!
	}*/

};

void PD_NW_SocketManager::ListenerNewConexion(PD_NW_Socket* newSocket) {

	//TO-DO Aqui habria que hacer el control de maximas conexiones, y en caso de no aceptar mas, notificarlo a la conexion con el listener, o cerrar el newSocket.

	//Se deberia aceptar la conexion y despues enviar el mensaje a los objetos registrados? o al reves?
	//O separar esto en dos pasos?
	socketArray.Add(newSocket);


}

bool PD_NW_SocketManager::InitListener(int port) {

	PD_NW_Socket ListenSocket;
	ListenSocket.listenerSocket(port);
	ListenerNewConexion(&ListenSocket);
	return true;

}


void PD_NW_SocketManager::TimerRefreshFunction() {
	/*for (int i = 0; i < socketArray.Num(); i++) {
	//Preguntar si hay data y en caso de haberla llamar a la funcion void socketHasReceivedData(TArray<uint8> data, int socketIndex);
	//socketArray[i]->receiveData();
	}*/
	UE_LOG(LogTemp, Error, TEXT("Timer Working "));

}



/******************************
*** FUNCIONES GET Y SET / APOYO **
/******************************/
void PD_NW_SocketManager::SetIsServer(bool InIsServer)
{
	isServer = InIsServer;
}

bool PD_NW_SocketManager::GetIsServer()
{
	return isServer;
}

void PD_NW_SocketManager::SetServerActor(APD_NW_ServerActor* InmyServerActor)
{
	myServerActor = InmyServerActor;
	myServerActor->SetSocketManager(this);
}