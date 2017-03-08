// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_NW_SocketManager.h"

//Includes of forward declaration
#include "PD_NW_Socket.h" 
#include "Networking.h"
#include "NW_NetWorking/PD_NW_TimerActor.h"
#include "NW_NetWorking/PD_NW_NetworkManager.h"
//Includes de prueba


//============================================
//*** CONSTRUCTOR Y DESTRUCTOR DE LA CLASE **
//============================================
PD_NW_SocketManager::PD_NW_SocketManager()
{
	socketArray = TArray<PD_NW_Socket*>();//Creo que no hace falta esta inicializacion.

	listenerSocket = nullptr;
}

PD_NW_SocketManager::~PD_NW_SocketManager()
{
	delete listenerSocket;
	//los deletes del socketArray los hace el propio array
}



/*
 FUNCIONES 
//
*/
void PD_NW_SocketManager::Init(APD_NW_TimerActor* InmyTimerActor, FString ip, int port)
{
	UE_LOG(LogTemp, Warning, TEXT("INICIANDO SOCKET MANAGER! "));
	//Inicializacion actor
	InitTimerActor(InmyTimerActor);

	if (isServer)
	{
		InitSocketManager_ServerMode(ip, port);
	}
	else
	{
		InitSocketManager_ClientMode("", port);
	}

}



void PD_NW_SocketManager::InitSocketManager_ServerMode(FString ip, int port)
{
	UE_LOG(LogTemp, Warning, TEXT("Socket MANAGER como SERVIDOR!! "));

	//Inicializacion listener
	if (InitListener(ip, port))
	{
		//Cuando se ha creado el Socket Listener, puedes llamar al Actor para que empiece el Timer de Escuchar.
		// Se llama en el init para todos.
		//myServerActor->InitTimerActor();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No se ha podido crear el Socket Listener del Servidor! "));
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *StateString());
}

void PD_NW_SocketManager::InitSocketManager_ClientMode(FString ip, int port)
{
	UE_LOG(LogTemp, Warning, TEXT("Socket MANAGER como CLIENTE!! "));

	//1.Crear el Socket que va a comunicar con el Servidor
	//2.Comprobar que hay comunicacion
	//3.Cuando hay comunicacion, Guadar dicho socket en el Array de Socket del SocketManager

	//Esto ya no se hace aqui, sino llamando a la funcion de conectar del networkmanager.
	/*
	if (ConnectDataSocket(ip, port) == -1) {
	//ERROR!!
	UE_LOG(LogTemp, Error, TEXT("No se ha podido crear el Socket Cliente! "));
	return;
	}*/

	//Y si no conecta aqui porque es demasiado pronto y no esta el server up? 
	///R : Si no esta el Server UP, esta funcion se volveria a llamar con un boton de "refresh"

	//4.Llamar al Timer del ServerActor para que empiece a escuhar para futuras comunicaciones con dicho socket.
	// Se llama en el init para todos.
	//myServerActor->InitTimerActor();

}

//hay posibilidad de que esta funcion falle? quizas debe devolver void 
///R: Asi sabemos si se ha creado, antes de proceder a que el ServerActor empiece a escuhcar por ese puerto y genere errores
bool PD_NW_SocketManager::InitListener(FString ip, int port) {

	/*if (listenerSocket) { //Esto es necesario?
	//cerrar conexion del listener
	//deletear
	delete listenerSocket;
	}*/

	listenerSocket = new PD_NW_Socket();
	listenerSocket->InitAsListener(ip, port);

	return true;

}

void PD_NW_SocketManager::InitTimerActor(APD_NW_TimerActor* InmyTimerActor)
{
	UE_LOG(LogTemp, Warning, TEXT("InitTimerActor"));

	myTimerActor = InmyTimerActor;
	myTimerActor->SetSocketManager(this);

	GetTimerActor()->InitTimerActor();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *StateString());
}

int PD_NW_SocketManager::ConnectDataSocket(FString ip, int port) {
	PD_NW_Socket* pdSocket = new PD_NW_Socket();
	pdSocket->InitAsDataSocket();
	bool connected = pdSocket->ConnectTo(ip, port);
	int out;
	if (connected) {
		out = socketArray.Add(pdSocket);
	}
	else {
		//Error!
		delete pdSocket;
		out = -1;
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *StateString());
	return out;
}

bool PD_NW_SocketManager::SendInfoTo(int indexSocket, TArray<uint8>* data) {

	if (indexSocket == -1) { //support to broadcast
		bool ok = true;
		for (int i = 0; i < socketArray.Num(); i++)
		{
			ok = ok && SendInfoTo(i, data);
		}
		return ok;
	}
	else {
		if (socketArray.IsValidIndex(indexSocket) && socketArray[indexSocket] != nullptr) { //Comprobamos que el indice es valido
			return socketArray[indexSocket]->SendData(data);
		}
		else return false;
	}

}



void PD_NW_SocketManager::HandleNewSocketData(TArray<uint8>* data, int socketIndex) {


	UE_LOG(LogTemp, Error, TEXT("HandleNewSocketData (socketIndex: %d)"), socketIndex);

	networkManager->HandleNewSocketData(data, socketIndex);

};

void PD_NW_SocketManager::HandleNewListenerConnection(PD_NW_Socket* newSocket) {

	UE_LOG(LogTemp, Warning, TEXT("New Listener Connection"));

	int socketIndex = socketArray.Add(newSocket);
	readyPlayersArray.Add(false); //Set siempre a false el ready inicial del cliente

	UE_LOG(LogTemp, Warning, TEXT("%s"), *StateString());

	//Se podria incluso preguntar antes si meterlo o no meterlo.

	networkManager->HandleNewConnectionSocketListener(socketIndex);

}


void PD_NW_SocketManager::TimerRefreshFunction() {

	//UE_LOG(LogTemp, Warning, TEXT("Timer Working "));

	if (listenerSocket) {
		//UE_LOG(LogTemp, Warning, TEXT(">>>> Hay Listener Socket y Funcionando ! "));
		PD_NW_Socket* newSocket = listenerSocket->ReceiveNewConnection();
		if (newSocket) {
			HandleNewListenerConnection(newSocket);
		}
	}

	for (int iSocket = 0; iSocket < socketArray.Num(); iSocket++) {
		//UE_LOG(LogTemp, Warning, TEXT(">>>> Comprobando sockets lista abiertos ! "));
		//Preguntar si hay data y en caso de haberla llamar a la funcion void socketHasReceivedData(TArray<uint8> data, int socketIndex);
		TArray<TArray<uint8>*> listPackages = socketArray[iSocket]->ReceiveData();
		for (int iPackages = 0; iPackages < listPackages.Num(); iPackages++) {

			HandleNewSocketData(listPackages[iPackages], iSocket);

		}

	}
}




/*
 FUNCIONES GET Y SET / APOYO 
*/
void PD_NW_SocketManager::SetIsServer(bool InIsServer)
{
	isServer = InIsServer;

}

bool PD_NW_SocketManager::GetIsServer()
{
	return isServer;

}






APD_NW_TimerActor* PD_NW_SocketManager::GetTimerActor()
{
	return myTimerActor;
}


PD_NW_NetworkManager* PD_NW_SocketManager::GetNetworkManager() {
	return networkManager;
}
void PD_NW_SocketManager::SetNetworkManager(PD_NW_NetworkManager* networkManagerIn) {
	networkManager = networkManagerIn;
}

FString PD_NW_SocketManager::StateString() {
	FString out = "SocketManager state:";
	if (GetTimerActor()->isTimerActive()) {

		out += "=[TimerActor OK]=";

		if (GetTimerActor()->isTimerActive()) {
			out += "=[Timer running]=";
		}
		else {
			out += "=[Timer stopped]=";
		}
	}
	else {
		out += "=[TimerActor missing!]=";
	}

	if (listenerSocket) {
		out += "=[Listener socket: YES]=";
	}
	else {
		out += "=[Listener socket: NO]=";
	}
	for (int i = 0; i < socketArray.Num(); i++) {
		if (socketArray[i]) {
			out += "=[Socket ";
			out.AppendInt(i);
			out += "]=";
		}
		else {
			out += "=[Socket ";
			out.AppendInt(i);
			out += " missing!(nullptr)]=";
		}
	}
	return out;
}