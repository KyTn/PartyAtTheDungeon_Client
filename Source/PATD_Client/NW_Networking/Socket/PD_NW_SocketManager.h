// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//forward declarations
class PD_NW_Socket;
class APD_NW_ServerActor;

/**
 * 
 */
class PATD_CLIENT_API PD_NW_SocketManager
{
private:

	TArray<PD_NW_Socket*> socketArray;

	PD_NW_Socket* listenerSocket;

	APD_NW_ServerActor* myServerActor;

	bool isServer;

	//Functions ======


	///Comunicacion con la capa superior 

	//Recepcion

	/*Funcion que llama la funcion del timer cuando un socket recibe data.
	* Deberia llamar a los objetos registrados como pendientes de ese socket (por el int).
	O ser abstracta y que se implemente en cada implementacion del socketManager (sin usar objetos registrados)
	*/
	void SocketHasReceivedData(TArray<uint8> data, int socketIndex);

	/*Funcion que llama la funcion del timer cuando tiene una conexion nueva.
	//Deberia llamar a los objetos registrados como pendientes de nueva conexion.
	O ser abstracta y que se implemente en cada implementacion del socketManager (sin usar objetos registrados)
	*/
	void ListenerNewConexion(PD_NW_Socket* newSocket);




public:
	///Inicializacion
	PD_NW_SocketManager();
	~PD_NW_SocketManager();


	///Funciones Get y Set de los Atributos
	void SetIsServer(bool InIsServer);
	bool GetIsServer();
	void SetServerActor(APD_NW_ServerActor* InmyServerActor);


	///FUNCIONES 
	//Inicializa el socketManager y lo pone a funcionar.
	void Init(APD_NW_ServerActor* InmyServerActor);

	///* SERVIDOR */
	//Inicializa el SocketManager como Server
	void InitSocketManager_ServerMode();
	//Esta es la funcion que repetira el timer. Deberia recopilar los datos recibidos en los sockets e ir llamando a las funciones de salida
	void TimerRefreshFunction();


	///* CLIENTE */
	//Inicializa el SocketManager como Clinete
	void InitSocketManager_ClientMode();
	void ListenerDataReceived_ClientMode();

	//Esta funcion inicia el timer. Se llamara desde el init, pero tambien individualmente al cambiar de mapa.
	void InitTimer();
	void DeleteTimer();

	bool InitListener(int port);
	bool CloseListener();


	///Gestion de registro a eventos.
	bool RegisterToSocket(int socketIndex/*,PD_NW_iSocketManagerObserver*/);




	///Comunicacion con la capa superior 

	//Envio
	bool SendInfoTo(int isocket, FString data);
};
