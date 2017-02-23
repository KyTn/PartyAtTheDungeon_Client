#pragma once
//Hacemos (#include "Structs/PD_ServerStructs.h") en el .cpp de las clases para usar los structs.
//Si los necesitamos en el .h los hacemos con forward declaration.
#include "PD_NetStructs.h"
#include "PD_ClientEnums.h"



//Usado por: GameInstance -ServerManager-
struct StructClientState {
	EClientState enumClientState;
	bool clientMaster;
	int numPlayer;
};

