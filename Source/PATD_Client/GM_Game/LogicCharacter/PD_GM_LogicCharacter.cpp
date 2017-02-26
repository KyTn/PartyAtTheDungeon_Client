// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_GM_LogicCharacter.h"


#include <math.h>       /* ceil */

//Includes of forward declaration
#include "Structs/PD_ClientStructs.h" //Para todos los structs y enums
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "Actors/MyCharacterParent.h"
#include "Actors/PD_GenericController.h"

PD_GM_LogicCharacter::PD_GM_LogicCharacter()
{
}

PD_GM_LogicCharacter::~PD_GM_LogicCharacter()
{
}

/* ==============
METODOS PRINCIPALES PARA INTERACTUAR CON EL MAPA
 =============== */


TArray<FStructOrderAction>* PD_GM_LogicCharacter::FindPathToMove(PD_MG_LogicPosition desirePosition)
{
	/*
		- OBJETIVO: Sirve para conseguir el listado de ordenes de movimiento que se tienen que enviar desde 
	 el cliente al servidor para ir a la posición marcada por el jugador donde se quiere mover.
		- PROCESO: 
			1. recibe como parametro LogicPosition (X e Y) que identifica a la casilla
			2. llama a la funcion del Algoritmo A* con esta posicion y la referencia al MapManager
			3. El algoritmo devuelve un array completo con el camino a seguir en orden.
		- SE LLAMA DESDE: El Cliente (atraves de interfaz) , o la IA para conseguir el camino hasta la posicion elegida en la fase de movimiento
	*/
	
	TArray<FStructOrderAction>* listTileToMove = new TArray<FStructOrderAction>();



	return listTileToMove;
}



PD_MG_LogicPosition* PD_GM_LogicCharacter::MoveToLogicPosition(uint8 tick, TArray<FStructOrderAction> listMove)
{
	/*
		- OBJETIVO: Simula el movimiento del personaje siguiendo una lista de movimientos dada (el camino calculado hasta una
		posicion casilla a casilla)
		- PROCESO:
			1. Recibe como parametro un array con la lista de movimientos casilla a casilla y el indice del array (tick) de 
			la casilla a comprobar la simulacion
			2. Simula donde va casilla por casilla comprobando que es posible el movimiento
			3. Actualizar la variable movingLogicPosition
			3. Al acabar de simular devuelve un LogicPosition con la posicion final tras la simulacion
		- SE LLAMA DESDE: Desde el GameManager cuando sea el turno de simular el movimiento. Se pasa el parametro tick, para asegurar
		de que todos los actors que se mueven lo hacen a la vez.
		**Se devuelve un LogicPosition para que el GamenManager pueda ver si hay conflictos dados las posiciones logicas de ese tick de todos los personajes 
		que se mueven y actue en consecuencia. 
		- Tras el choque habría que cambiar esa movingLogicalPosition
	*/
	PD_MG_LogicPosition* targetPosition;
	targetPosition = movingLogicalPosition;

	return targetPosition;
}


bool PD_GM_LogicCharacter::MoveToPhysicalPosition(PD_MG_LogicPosition targetPosition)
{
	/*
		- OBJETIVO: Mueve al personaje hasta la posicion fisica establecida anteriormente en simulacion
		- PROCESO:
			1. Recibe la posicion logica a la que se tiene que mover el personaje durante la Fase de Movimiento
			2. Con el MapManager convierte la posicion logica en fisica
			3.  Actualiza  las variables currentLogicalPosition a la recibida por parametro
			4.Con el PlayerManager coge su personaje y llama a la funcion MoveTo() de su controlador
		- SE LLAMA DESDE: Desde el GameManager cuando sea el turno de visualizar el movimiento. 
		Devuelve true si todo ha ido bien, si algo ha fallado, devuelve false (normalmente a raiz de que la funcion move del controlador
		devuelva tambien false
	*/
	
	return true;
}


bool PD_GM_LogicCharacter::ActionTo(PD_MG_LogicPosition targetPosition, uint32 action)
{
	/*
		- OBJETIVO: Para realizar una accion sobre una casilla. Las acciones son Ataques, Habilidades o Interactuar con objeto
		- PROCESO:
			1. Recibe como parametros la posicion logica de la casilla y un ID de la accion que va a realizar
			(identifica la habilidad, ataque basico o interaccion)
			2. Con el MapManager consigue la referencia sobre lo que hay en la posicion logica dada -
			el actor(Interactuable) o el character(enemigo o personaje) dependiendo de que accion va arealizar.
				Se comprueba de que efectivamente hay algo en la casilla si no return false;
			3. Dependiendo del Enum dado, se sabe si es un ataque o una accion
				3.1 Si es un Accion sobre Jugador o Enemigo
					3.1.0 Comprobar que el rango a la casilla es el adecuado
					3.1.1 Consigue la referencia al enemigo (character) - a traves del Enemy/Player Manager
					3.1.2 Consigue la referencia a su CharacterLogic - a traves del Enemy/Player Manager
					3.1.4 Ejecutar el método Action() de su propio Controllador- Si la accion fallase return false- Sino continua ejecuion.
					3.1.3 Haz que ejecute el metodo UpdateHPCurrent() -- del characterLogic
				3.2 Si es un Accion sobre Interactuable
					3.2.0 Comprobar que el rango de la casilla es el adecuado
					3.2.1 Conseguir la referencia a ese interactuable
					3.2.1 Conseguir el controlador de ese interactuable
					3.2.2 ejecutar el metodo Interacturar() de ese interactuable 
		- SE LLAMA DESDE: El GameManager durante la Fase de Accion
	*/
	return true;
}
void PD_GM_LogicCharacter::ConsumeItem(uint32 idItem)
{
	/*
	- OBJETIVO: Consume un item durante la fase de Consumibles
	- PROCESO:
	1. Recibe un ID que identifica el tipo de Item a consumir
	2. Con el MapManager convierte la posicion logica en fisica
	3.  Actualiza  las variables currentLogicalPosition a la recibida por parametro
	4.Con el PlayerManager coge su personaje y llama a la funcion MoveTo() de su controlador
	- SE LLAMA DESDE: Desde el GameManager cuando sea el turno de visualizar el movimiento.
	Devuelve true si todo ha ido bien, si algo ha fallado, devuelve false (normalmente a raiz de que la funcion move del controlador
	devuelva tambien false
	*/
}

void PD_GM_LogicCharacter::UpdateHPCurrent(float receivedDamage)
{
	/*
		- OBJETIVO: Recibir el daño de un ataque, actualizar la vida actual y comprobar si ha muerto o no.
		NOTA: Si es negativo sera un ataque, si es positivo será una curacion
		- PROCESO:
			1. Pasar el float a un int, debido a que la vida es tipo int, aunque  el daño por las formulas llegara en float
			2. Actualizar el valor del FStructTotalStats HPCurrent (Si es curacion nunca puede pasar de HPTotal
			3. Comprobar si HPCurrent <= 0. Si es asi, actualizar la variable isDead
		- SE LLAMA DESDE: Cualquier Actor - Personaje o Enemigo que dañe a otro
	*/
}


/* ===============
METODOS GET Y SET PARA STRUCTS DE STATS y DATOS
 ================ */
//GET
FStructBasicStats* PD_GM_LogicCharacter::GetBasicStats() { return basicStats; }
FStructInitBaseStats* PD_GM_LogicCharacter::GetInitBaseStats() { return initBaseStats; }
FStructSkills* PD_GM_LogicCharacter::GetSkills() { return skills; }
FStructWeapon* PD_GM_LogicCharacter::GetWeapon() { return weapon; }
FStructSkin* PD_GM_LogicCharacter::GetSkin( ) { return skin; }
FStructTotalStats* PD_GM_LogicCharacter::GetTotalStats( ) { return totalStats; }

bool PD_GM_LogicCharacter::GetIsPlayer() { return isPlayer; }
bool PD_GM_LogicCharacter::GetIsDead() { return isDead; }
uint8 PD_GM_LogicCharacter::GetIDCharacter() { return ID_character; }
APD_GenericController* PD_GM_LogicCharacter::GetController() { return controller; }
AMyCharacterParent* PD_GM_LogicCharacter::GetCharacterParent() { return characterParent; }
TSubclassOf<class AMyCharacterParent> PD_GM_LogicCharacter::GetCharacterBP() { return character_Player_BP; }
PD_MG_LogicPosition* PD_GM_LogicCharacter::GetCurrentLogicalPosition() { return currentLogicalPosition; }
PD_MG_LogicPosition* PD_GM_LogicCharacter::GetMovingLogicalPosition() { return movingLogicalPosition; }

//SET
void PD_GM_LogicCharacter::SetBasicStats(int nPOD, int nAGI, int nDES, int nCON, int nPER, int nMAL) 
{
	basicStats->POD = nPOD;
	basicStats->AGI = nAGI;
	basicStats->DES = nDES;
	basicStats->CON = nCON;
	basicStats->PER = nPER;
	basicStats->MAL = nMAL;
}
void PD_GM_LogicCharacter::SetInitBaseStats(int nHP, int nDMG)
{
	initBaseStats->HPBase = nHP;
	initBaseStats->DMGBase = nDMG;
}
void PD_GM_LogicCharacter::SetSkills(TArray<uint8> nActSkills, TArray<uint8> nPasSkills) 
{
	skills->listActiveSkills = nActSkills;
	skills->listPasiveSkills = nPasSkills;
}
void PD_GM_LogicCharacter::SetWapon() 
{

}
void PD_GM_LogicCharacter::SetSkin()
{

}
void PD_GM_LogicCharacter::SetTotalStats() 
{
	//Bonus de STATS Basicos
	totalStats->PODBonus = (0.05 * (basicStats->POD - 5));
	totalStats->AGIBonus = basicStats->AGI - 5;
	totalStats->DESBonus = basicStats->DES - 5;
	totalStats->CONBonus = (0.05 * (basicStats->CON - 5));
	//mele
	if (weapon->TypeWeapon == 0) 
	{
		totalStats->PERBonus = 0.05 * basicStats->PER;
	}
	//rango
	else if (weapon->TypeWeapon == 0) 
	{
		totalStats->PERBonus = (ceil(basicStats->PER / 2)) - 3;
	}
	//distancia
	else if (weapon->TypeWeapon == 0)
	{
		totalStats->PERBonus = (ceil(basicStats->PER / 2)) - 3;
	}
	totalStats->MALBonus = basicStats->MAL * 0.02;

	//Stats Secundarios
	//AP
	if ((basicStats->AGI + basicStats->DES) < 10)
	{
		totalStats->AP = floor((basicStats->AGI + basicStats->DES) / 2) - 5;
	}
	else if ((basicStats->AGI + basicStats->DES) >= 10)
	{
		totalStats->AP = ceil((basicStats->AGI + basicStats->DES) / 2) - 5;
	}
	//Choque
	if ((basicStats->POD + basicStats->MAL) < 10)
	{
		totalStats->CH = floor((basicStats->POD + basicStats->MAL) / 2) - 5;
	}
	else if ((basicStats->POD + basicStats->MAL) >= 10)
	{
		totalStats->CH = ceil((basicStats->POD + basicStats->MAL) / 2) - 5;
	}
	//Salvacion
	if ((basicStats->POD + basicStats->MAL) < 10)
	{
		totalStats->CH = floor((basicStats->POD + basicStats->MAL) / 2) - 5;
	}
	else if ((basicStats->POD + basicStats->MAL) >= 10)
	{
		totalStats->CH = ceil((basicStats->POD + basicStats->MAL) / 2) - 5;
	}

	//Vida total
	//	totalStats->HPTotal = (initBaseStats->HPBase + HPBonus) * (1 + totalStats->CONBonus); - HPBonus es por pasivas u otras fuentes
	totalStats->HPTotal = (initBaseStats->HPBase) * (1 + totalStats->CONBonus);
	totalStats->HPCurrent = totalStats->HPTotal;

	//Damage Total
	//	totalStats->DMGTotal = (initBaseStats->DMGBase) * (1 + totalStats->PODBonus);
	totalStats->DMGTotal = (initBaseStats->DMGBase) * (1 + totalStats->PODBonus);

	/*
	Para el bonus de Vida y Damage, hacer antes un IF para ver si entre la lista de pasivas, tienen algunas que le den bonus extra a eso
	*/
}

void PD_GM_LogicCharacter::SetIsPlayer(bool nIsPlayer){ isPlayer = nIsPlayer; }
void PD_GM_LogicCharacter::SetIsDead(bool nIsDead){ isDead = nIsDead; }
void PD_GM_LogicCharacter::SetIDCharacter(uint8 nID_character){ ID_character = nID_character; }
void PD_GM_LogicCharacter::SetController(APD_GenericController* ncontroller){ controller = ncontroller; }
void PD_GM_LogicCharacter::SetCharacterParent(AMyCharacterParent* ncharacterParent){ characterParent = ncharacterParent; }
void PD_GM_LogicCharacter::SetCharacterBP(TSubclassOf<class AMyCharacterParent> ncharacter_Player_BP){ character_Player_BP = ncharacter_Player_BP; }
void PD_GM_LogicCharacter::SetCurrentLogicalPosition(PD_MG_LogicPosition* ncurrentLogicalPosition){ currentLogicalPosition = ncurrentLogicalPosition; }
void PD_GM_LogicCharacter::SetMovingLogicalPosition(PD_MG_LogicPosition* nmovingLogicalPosition) { movingLogicalPosition = nmovingLogicalPosition; }