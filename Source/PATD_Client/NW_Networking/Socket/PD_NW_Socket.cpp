// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_NW_Socket.h"
#include "Networking.h"

//Includes of forward declaration
#include "PD_NW_SocketManager.h" 


PD_NW_Socket::PD_NW_Socket()
{
	socket = NULL;
	//	GetWorld()->S
}


PD_NW_Socket::~PD_NW_Socket()
{
	UE_LOG(LogTemp, Error, TEXT("DELETE DE LOS SOCKET "));

	//Esto puede dar error al llamarse alguna vez sin que tenga nada?
	//if (socket) {
	socket->Close();
	delete socket; // con esto se supone que se borra la instancia de la clase (?)
	socket = NULL;
	//}

}

FSocket* PD_NW_Socket::GetFSocket()
{
	return socket;
}


bool PD_NW_Socket::ConnectTo(FString ip, int port) {


	FIPv4Address  instanceFIPv4Addres;
	FIPv4Address::Parse(ip, instanceFIPv4Addres);


	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	//	addr->SetIp(instanceFIPv4Addres.Value);



	addr->SetIp(instanceFIPv4Addres.Value);
	addr->SetPort(port);


	bool connected = socket->Connect(*addr);

	if (connected) {
		UE_LOG(LogTemp, Warning, TEXT("Nivel Socket: Se ha conectado guay con el server "));

	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Nivel Socket:  No se ha podido conectar con el servidor ");
		UE_LOG(LogTemp, Error, TEXT("Nivel Socket: >>> No se ha podido conectar con el servidor "));

	}
	return connected;

	/*TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool validIp;
	addr->SetIp(ip.GetCharArray(), validIp);
	addr->SetPort(port);


	*/


}

bool PD_NW_Socket::SendData(TArray<uint8>* sendData) {
	int32 bytesReceived;


	//Mirar si la el CountBytes funciona adecuadamente o esta metiendo bytes de mas para el array. (este serializando de mas)
	
	bool successful = socket->Send(sendData->GetData(), sendData->Num(), bytesReceived);
	if (successful) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Nivel Socket: Se ha enviado un paquete!");
		UE_LOG(LogTemp, Error, TEXT("Nivel Socket:>>> Se ha enviado un paquete! "));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Nivel Socket:No se enviado nada! ");
		UE_LOG(LogTemp, Error, TEXT("Nivel Socket:>>> No se enviado nada! "));
	}
	return successful;
}

TArray<uint8>* PD_NW_Socket::ReceiveData() {
	//Ahora mismo, al no tener datos para recibir y el que haya un error se devuelve lo mismo, null.
	// ERROR!
	if (!socket)
	{
		UE_LOG(LogTemp, Error, TEXT("Nivel Socket:>>> No hay Socket Creado! "));
		//		return listPackages;
	}

	TArray<uint8>* receivedData = nullptr;
	TArray<uint8>* receivedDataTotal = new TArray<uint8>();

	uint32 Size;

	uint32 packageSize = 0;
	uint32 contByte = 0;

	//el while hace append de todo lo que reciba hasta agotar el buffer de entrada (cuando no haya hasPendingData)

	if (socket->HasPendingData(Size)) //pasamos una primera vez para conseguir el tamaño total de los datos que se estan recibiendo
	{

		//Estamos creando los datos nuevos en el HEAP
		receivedDataTotal->Init(0, FMath::Min(Size, 65507u));


		int32 Read = 0;
		socket->Recv(receivedDataTotal->GetData(), receivedDataTotal->Num(), Read);


		packageSize = ((uint32)((*receivedDataTotal)[0]) << 24) + ((uint32)((*receivedDataTotal)[1]) << 16) + ((uint32)((*receivedDataTotal)[2]) << 8) + ((uint32)(*receivedDataTotal)[3]);
		packageSize += 5;

		contByte = receivedDataTotal->Num();

		int i = 0;

		UE_LOG(LogTemp, Warning, TEXT("Nivel Socket:>>> ReceiveData --- packageSize size Total - %d :"), packageSize);
		UE_LOG(LogTemp, Warning, TEXT("Nivel Socket:>>> ReceiveData --- contByte First Package %d"), contByte);

		while (contByte < packageSize)
		{
			if (socket->HasPendingData(Size)) //pasamos una primera vez para conseguir el tamaño total de los datos que se estan recibiendo
			{
				//Estamos creando los datos nuevos en el HEAP
				receivedData = new TArray<uint8>(); //Aqui creamos reserva de memoria en heap para el array.
				receivedData->Init(0, FMath::Min(Size, 65507u));

				int Read = 0;
				socket->Recv(receivedData->GetData(), receivedData->Num(), Read);

				UE_LOG(LogTemp, Error, TEXT("Nivel Socket:>>> ReceiveData : bucle while numero %d:, size: %d, read: %d"), i, Size, Read);



				// ERROR!
				if (receivedData == nullptr || receivedData->Num() <= 0)
				{
					//	UE_LOG(LogTemp, Error, TEXT(">>>> No se han enviado datos ! "));
					return receivedDataTotal; //No Data Received
				}
				else {
					//	UE_LOG(LogTemp, Warning, TEXT(">>>> Se van a enviar DATOS :) ! "));
				}

				receivedDataTotal->Append(*receivedData);


				contByte = receivedDataTotal->Num();

				i++; //solo para debug
			}

		}

	}

	return receivedDataTotal;

}




PD_NW_Socket* PD_NW_Socket::ReceiveNewConnection() {



	//	UE_LOG(LogTemp, Warning, TEXT("Ha entrado a ReceiveNewConnection ! "));
	//~~~~~~~~~~~~~
	//Ahora mismo, al no tener datos para recibir y el que haya un error se devuelve lo mismo, null.
	// ERROR!
	if (!socket) {
		UE_LOG(LogTemp, Warning, TEXT("Nivel Socket: ReceiveNewConnection no tiene Fsocket ! "));

		return nullptr;
	}
	//~~~~~~~~~~~~~

	//Esto se podria usar si la funcion de getAdress() del FSocket no funciona como pensamos.
	/*TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	//Global cache of current Remote Address
	RemoteAddressForConnection = FIPv4Endpoint(RemoteAddress);
	*/


	bool Pending;
	// handle incoming connections
	/*if (socket == NULL) {
	UE_LOG(LogTemp, Error, TEXT("Socket es null en Pending "));

	}
	else {
	UE_LOG(LogTemp, Error, TEXT("SocketNO NULL en Pending "));

	}*/

	if (socket->HasPendingConnection(Pending) && Pending)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nivel Socket:Ha entrado a HasPendingConnection ! "));
		FSocket* newFSocket;
		PD_NW_Socket* newPD_NW_Socket = nullptr;
		//En principio no necesitamos guardar la direccion aqui. (Accept permite guardarla)
		newFSocket = socket->Accept(TEXT("Data Socket created at Listener"));

		if (newFSocket) {
			UE_LOG(LogTemp, Warning, TEXT("Nivel Socket:Ha entrado a HasPendingConnection ! "));

			newPD_NW_Socket = new PD_NW_Socket();
			newPD_NW_Socket->SetFSocket(newFSocket);

			UE_LOG(LogTemp, Warning, TEXT("Nivel Socket:Ha creado el nuevo socket ! "));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Nivel Socket:Error en ReceiveNewConnection: socket null tras Accept ! "));
		}

		return newPD_NW_Socket;

	}


	return nullptr;
}


void PD_NW_Socket::InitAsListener(FString ip, int port) {
	//Muevo aqui el codigo que antes estaba en el gameinstance (castIP). Por que al fin y al cabo la inicializacion
	//es partiendo de la logica de Unreal.
	UE_LOG(LogTemp, Warning, TEXT("Nivel Socket: InitAsListener: ip:%s port %d "), *ip, port);

	//IP Formatting
	ip.Replace(TEXT(" "), TEXT(""));
	ip.Replace(TEXT(" "), TEXT(""));
	//String Parts
	TArray<FString> Parts;
	ip.ParseIntoArray(Parts, TEXT("."), true);

	TArray <uint8> ipArray;

	//String to Number Parts
	for (int32 i = 0; i < 4; ++i)
	{
		ipArray.Add(FCString::Atoi(*Parts[i]));
	}


	FIPv4Endpoint Endpoint(FIPv4Address(ipArray[0], ipArray[1], ipArray[2], ipArray[3]), port);
	this->socket = FTcpSocketBuilder("Listener Socket").AsReusable().BoundToEndpoint(Endpoint).Listening(8);
}

void PD_NW_Socket::InitAsDataSocket() {
	socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("Data Socket created from init"), false);
}


void PD_NW_Socket::SetFSocket(FSocket* inSocket) {
	socket = inSocket;
}



