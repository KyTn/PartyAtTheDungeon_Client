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
	PD_NW_Socket();
	PD_NW_Socket(FSocket* socketIn);
	~PD_NW_Socket();


	//Recibir datos
	//Deberia devolver un array de datos recibidos, pero y si hay mas de un paquete pending? Ahora mismo hay un bucle pero solo guardaria el ultimo.
	TArray<uint8>* receiveData();

	void listenerSocket(int port);
};
