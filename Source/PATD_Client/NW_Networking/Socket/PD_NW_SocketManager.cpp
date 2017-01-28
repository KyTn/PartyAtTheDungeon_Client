// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_NW_SocketManager.h"

//Includes of forward declaration
#include "PD_NW_Socket.h" 
#include "Networking.h"
#include "NW_NetWorking/PD_NW_ServerActor.h"

//
#include <string>

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
//
void PD_NW_SocketManager::Init(APD_NW_ServerActor* InmyServerActor, FString ip, int port)
{
	UE_LOG(LogTemp, Warning, TEXT("INICIANDO SOCKET MANAGER! "));
	//Inicializacion actor

	SetServerActor(InmyServerActor);
	if (isServer)
	{
		InitSocketManager_ServerMode(port);
	}
	else
	{
		InitSocketManager_ClientMode(ip, port);
	}






}

void PD_NW_SocketManager::InitSocketManager_ServerMode(int port)

{
	UE_LOG(LogTemp, Warning, TEXT("Socket MANAGER como SERVIDOR!! "));
	myServerActor->InitTimerActor();

	//Inicializacion listener
	this->InitListener(port);


}

void PD_NW_SocketManager::InitSocketManager_ClientMode(FString ip, int port)
{
	UE_LOG(LogTemp, Warning, TEXT("Socket MANAGER como CLIENTE!! "));


	//1.Crear el Socket que va a comunicar con el Servidor
	//2.Comprobar que hay comunicacion
	//3.Cuando hay comunicacion, Guadar dicho socket en el Array de Socket del SocketManager

	if (this->CreateDataSocket(ip, port) == -1) {
		//ERROR!!
	}
	//Y si no conecta aqui porque es demasiado pronto y no esta el server up?

	//4.Llamar al Timer del ServerActor para que empiece a escuhar para futuras comunicaciones con dicho socket.

	myServerActor->InitTimerActor();

}

//hay posibilidad de que esta funcion falle? quizas debe devolver void
bool PD_NW_SocketManager::InitListener(int port) {


	if (listenerSocket) { //Esto es necesario?
						  //cerrar conexion del listener
						  //deletear
		delete listenerSocket;
	}
	listenerSocket = new PD_NW_Socket();
	listenerSocket->InitAsListener(port);

	return true;

}


int PD_NW_SocketManager::CreateDataSocket(FString ip, int port) {
	PD_NW_Socket* pdSocket = new PD_NW_Socket();
	pdSocket->InitAsDataSocket();
	bool connected = pdSocket->ConnectTo(ip, port);
	if (connected) {
		return socketArray.Add(pdSocket);
	}
	else {
		//Error!
		delete pdSocket;
		return -1;
	}

}

bool PD_NW_SocketManager::SendInfoTo(int indexSocket, TArray<uint8>* data) {
	return socketArray[indexSocket]->SendData(data);
}



void PD_NW_SocketManager::HandleNewSocketData(TArray<uint8>* data, int socketIndex) {


	UE_LOG(LogTemp, Error, TEXT("HandleNewSocketData (socketIndex: %d)"), socketIndex);


	//Esto no va aqui, esto ya seria llamar al serializer o a los objetos suscritos

	//Create a string from a byte array!
	//
	std::string cstr(reinterpret_cast<const char*>(data->GetData()), data->Num());

	FString string = FString(cstr.c_str());

	UE_LOG(LogTemp, Error, TEXT("HandleNewSocketData Read: %s"), *string);

};

void PD_NW_SocketManager::HandleNewListenerConnection(PD_NW_Socket* newSocket) {

	int socketIndex = socketArray.Add(newSocket);

}


void PD_NW_SocketManager::TimerRefreshFunction() {


	UE_LOG(LogTemp, Error, TEXT("Timer Working "));

	if (listenerSocket) {
		PD_NW_Socket* newSocket = listenerSocket->ReceiveNewConnection();
		if (newSocket) {
			HandleNewListenerConnection(newSocket);
		}


	}

	for (int i = 0; i < socketArray.Num(); i++) {

		//Preguntar si hay data y en caso de haberla llamar a la funcion void socketHasReceivedData(TArray<uint8> data, int socketIndex);
		TArray<uint8>* inData = socketArray[i]->ReceiveData();
		if (inData) {
			HandleNewSocketData(inData, i);
		}

	}
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