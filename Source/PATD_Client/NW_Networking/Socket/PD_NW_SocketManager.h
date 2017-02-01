// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//forward declarations
class PD_NW_Socket;
class APD_NW_ClientActor;

class PD_NW_NetworkManager;
/**
*

*/
class PATD_CLIENT_API PD_NW_SocketManager
{
private:
	///CONSTANTES

	///VARIABLES
	TArray<PD_NW_Socket*> socketArray;

	PD_NW_Socket* listenerSocket;


	APD_NW_ClientActor* myClientActor;

	bool isServer;


	//Referencia al NerworkManager (Solo para los handlers, a falta de hacer un sistema de observadores)
	PD_NW_NetworkManager* networkManager;

	//Functions ======

	//Esta funcion inicia el timer. Se llamara desde el init, pero tambien individualmente al cambiar de mapa.
	void InitTimer();
	void DeleteTimer();



	///Comunicacion con la capa superior 

	//Recepcion

	/*Funcion que llama la funcion del timer cuando un socket recibe data.
	* Deberia llamar a los objetos registrados como pendientes de ese socket (por el int).
	O ser abstracta y que se implemente en cada implementacion del socketManager (sin usar objetos registrados)
	*/
	void HandleNewSocketData(TArray<uint8>* data, int socketIndex);

	/*Funcion que llama la funcion del timer cuando tiene una conexion nueva.
	//Deberia llamar a los objetos registrados como pendientes de nueva conexion.
	O ser abstracta y que se implemente en cada implementacion del socketManager (sin usar objetos registrados)
	*/
	void HandleNewListenerConnection(PD_NW_Socket* newSocket);
	//Aqui seria donde podriamos comprobar que la ip de la nueva conexion es una que tengamos guardada, para hacer un reconnect entre los sockets (el nuevo y el viejo)
	//Aqui el fsocket ya viene instanciado

	//TO-DO Aqui habria que hacer el control de maximas conexiones, y en caso de no aceptar mas, notificarlo a la conexion con el listener, o cerrar el newSocket.

	//Se deberia aceptar la conexion y despues enviar el mensaje a los objetos registrados? o al reves?
	//O separar esto en dos pasos?

	//Lo de la gestion de reconexiones va aqui (aunque puede llamar a otra clase)



public:
	///Inicializacion
	PD_NW_SocketManager();
	~PD_NW_SocketManager();

	///Utilidades
	FString StateString();

	///Funciones Get y Set de los Atributos
	void SetIsServer(bool InIsServer);
	bool GetIsServer();
	void SetClientActor(APD_NW_ClientActor* InmyClientActor);
	APD_NW_ClientActor* GetClientActor();
	PD_NW_NetworkManager* GetNetworkManager();
	void SetNetworkManager(PD_NW_NetworkManager* networkManagerIn);



	///FUNCIONES 
	//Inicializa el socketManager e inicia el timer. IP se usa solo en modo cliente.
	void Init(APD_NW_ClientActor* InmyClientActor, FString ip, int port);
	//Inicializa el ClientActor para ser llamado desde cualquier Mapa
	void InitClientActor(APD_NW_ClientActor* InmyClientActor);

	///* SERVIDOR */
	//Inicializa el SocketManager como Server
	void InitSocketManager_ServerMode(int port);
	//Esta es la funcion que repetira el timer. Deberia recopilar los datos recibidos en los sockets e ir llamando a las funciones de salida
	void TimerRefreshFunction();


	///* CLIENTE */
	//Inicializa el SocketManager como Clinete
	void InitSocketManager_ClientMode(FString ip, int port);


	//Funcion publica para poder añadir sockets (conectados) manualmente. Devuelve la posicion donde se crea el socket.
	//Solo guarda el socket en el array si se llega a conectar.
	int ConnectDataSocket(FString ip, int port);



	bool InitListener(int port);
	bool CloseListener();


	///Gestion de registro a eventos.
	bool RegisterToSocket(int socketIndex/*,PD_NW_iSocketManagerObserver*/);


	///Comunicacion con la capa superior 

	//Envio
	bool SendInfoTo(int indexSocket, TArray<uint8>* data);


};
