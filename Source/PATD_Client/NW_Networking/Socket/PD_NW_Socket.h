// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//forward declarations
class PD_NW_SocketManager;

/**
*
*/

class PATD_CLIENT_API PD_NW_Socket
{

	FSocket* socket;

private:



public:
	//Inicializacion
	PD_NW_Socket();
	~PD_NW_Socket();

	void InitAsListener(FString ip, int port);
	void InitAsDataSocket();

	void SetFSocket(FSocket* inSocket);
	FSocket* GetFSocket();
	bool ConnectTo(FString ip, int port);
	//Enviar datos
	bool SendData(TArray<uint8>* sendData);

	//Recibir datos
	//Deberia devolver un array de datos recibidos, pero y si hay mas de un paquete pending? Ahora mismo hay un bucle pero solo guardaria el ultimo.
	TArray<TArray<uint8>*>  ReceiveData();

	//Funciones del Listener
	PD_NW_Socket* ReceiveNewConnection();



};


