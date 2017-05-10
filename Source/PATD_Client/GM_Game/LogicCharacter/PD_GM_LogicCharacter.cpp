// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_GM_LogicCharacter.h"

//Includes de uso 
#include "Actors/Players/PD_CharacterController.h"
#include "Actors/Enemies/PD_EnemyController.h"

#include <math.h>       /* ceil */

//Includes of forward declaration
#include "Structs/PD_ClientStructs.h" //Para todos los structs y enums
#include "MapGeneration/PD_MG_LogicPosition.h"
#include "GM_Game/PD_GM_MapManager.h"
//#include "Actors/PD_GenericController.h"
#include "Actors/PD_E_Character.h"

PD_GM_LogicCharacter::PD_GM_LogicCharacter()
{
	//Inicializar los Structs
	 basicStats = new FStructBasicStats();
	 initBaseStats = new FStructInitBaseStats();
	 skills = new FStructCharacterSkills();
	 weapon = new FStructWeapon();
	 skin = new FStructSkin();
	 totalStats = new FStructTotalStats();

	 //inicializar las variables
	 isDead = false;

	 
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






bool PD_GM_LogicCharacter:: MoveAtUpdate(PD_MG_LogicPosition targetPosition)
{
	
	this->SetCurrentLogicalPosition( targetPosition);
	if (isPlayer) {
		FVector realPosition = mapMng->LogicToWorldPosition(targetPosition);
		//APD_CharacterController* controllerCharacter = Cast<APD_CharacterController>(controller);
		//controllerCharacter->MoveTo(realPosition.X, realPosition.Y);
		character_Player_BP->SetActorLocation(realPosition);
	}
	else {
		FVector realPosition = mapMng->LogicToWorldPosition(targetPosition);
		//APD_EnemyController* controllerCharacter = Cast<APD_EnemyController>(controller);
		//controllerCharacter->MoveTo(realPosition.X, realPosition.Y);
		character_Player_BP->SetActorLocation(realPosition);
	}
	
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

//Devuelve la tirada que le pidan para resolver los conflictos
int8 PD_GM_LogicCharacter::GetARoll(int8 numOfDice, int32 numOfFaces)
{
	int8 totalSum = 0;
	for (int i = 0; i < numOfDice; i++)
	{
		totalSum = totalSum + FMath::RandRange(1, numOfFaces);
	}
	return totalSum;
}

//Metodo para saber el porcentaje de bonus de la probabilidad de IMPACTAR
float PD_GM_LogicCharacter::ImpactPercentage()
{
	float totalBonusImpact = 1;
	/*
		- OBJETIVO: devuelve el porcentaje de impacto bonus que se añade por pasivas u otras fuentes
		- PROCESO:
			1. Recorrer tu lista de pasivas para encontrar si alguna coincide con una habilidad que 
			afecte a este porcentaje
			2. Si tiene alguna, suma al contador global de porcentaje el valor dado
			3. devolver dicho valor
		- SE LLAMA DESDE: la funcion GetImpactCharacter()
	*/
	return totalBonusImpact;
}


//Metodo para saber el porcentaje de bonus de la probabilidad de EVADIR
float PD_GM_LogicCharacter::EvasionPercentage()
{
	float totalBonusEvasion = 1;
	/*
		- OBJETIVO: devuelve el porcentaje de impacto bonus que se añade por pasivas u otras fuentes
		- PROCESO:
			1. Recorrer tu lista de pasivas para encontrar si alguna coincide con una habilidad que
			afecte a este porcentaje
			2. Si tiene alguna, suma al contador global de porcentaje el valor dado
			3. devolver dicho valor
		- SE LLAMA DESDE: la funcion GetEvasionCharacter()
	*/

	return totalBonusEvasion;
}


//Metodo para saber el valor que determina si ha impactado sobre un enemigo o no
int8 PD_GM_LogicCharacter::GetImpactCharacter()
{
	int8 ImpactTotal;
	/*
		- OBJETIVO: devuelve el valor  de impacto total para saber si da o no da
		- PROCESO:
			1. Conseguir el porcentaje de bonus de Impacto -- llamar a ImpactPercentage()
			2. Conseguir el Bonus de Destreza
			3. Calcular el Impacto total
			4. devolver el valor
		- SE LLAMA DESDE: cualuqier actor que quiera saber su valor de Impacto para saber si da o no da sobre otro actor
	*/

	ImpactTotal = roundf( (GetARoll(3, 6) + totalStats->DESBonus) * (1 + ImpactPercentage()) );

	return ImpactTotal;
}


//Metodo para saber el valor que determina si ha evadido un ataque o no
int8 PD_GM_LogicCharacter::GetEvasionCharacter()
{
	int8 EvasionTotal;
	/*
		- OBJETIVO: devuelve el valor  de evasion total de un personaje
		- PROCESO:
			1. Conseguir el porcentaje de bonus de Evasion -- llamar a EvasionPercentage()
			2. Conseguir el Bonus de Agilidad
			3. Calcular la evasion total
			4. devolver el valor
		- SE LLAMA DESDE: cualuqier actor que quiera saber su valor de Impacto para saber si da o no da sobre otro actor
	*/

	EvasionTotal = roundf( (GetARoll(3, 6) + totalStats->AGIBonus) * (1 + EvasionPercentage()) );

	return EvasionTotal;

}

/* ===============
METODOS GET Y SET PARA STRUCTS DE STATS y DATOS
 ================ */
//GET
FStructBasicStats* PD_GM_LogicCharacter::GetBasicStats() { return basicStats; }
FStructInitBaseStats* PD_GM_LogicCharacter::GetInitBaseStats() { return initBaseStats; }
FStructCharacterSkills* PD_GM_LogicCharacter::GetSkills() { return skills; }
FStructWeapon* PD_GM_LogicCharacter::GetWeapon() { return weapon; }
FStructSkin* PD_GM_LogicCharacter::GetSkin( ) { return skin; }
FStructTotalStats* PD_GM_LogicCharacter::GetTotalStats( ) { return totalStats; }
FStructCharacterState* PD_GM_LogicCharacter::GetCharacterState() { return characterState; }


bool PD_GM_LogicCharacter::GetIsPlayer() { return isPlayer; }
bool PD_GM_LogicCharacter::GetIsDead() { return isDead; }
FString PD_GM_LogicCharacter::GetIDCharacter() { return ID_character; }
ECharacterType PD_GM_LogicCharacter::GetTypeCharacter() { return type_character; }
APD_GenericController* PD_GM_LogicCharacter::GetController() { return controller; }
APD_E_Character* PD_GM_LogicCharacter::GetCharacterBP() { return character_Player_BP; }
PD_MG_LogicPosition PD_GM_LogicCharacter::GetCurrentLogicalPosition() { return currentLogicalPosition; }
PD_MG_LogicPosition PD_GM_LogicCharacter::GetMovingLogicalPosition() { return movingLogicalPosition; }

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
void PD_GM_LogicCharacter::SetInitBaseStats(int nHP, int nDMG, int nAP)
{
	initBaseStats->HPBase = nHP;
	initBaseStats->DMGBase = nDMG;
	initBaseStats->APBase = nAP;
}
void PD_GM_LogicCharacter::SetSkills(TArray<FStructSkill> nActSkills, TArray<FStructSkill> nPasSkills)
{
	skills->listActiveSkills = nActSkills;
	skills->listPasiveSkills = nPasSkills;
}
void PD_GM_LogicCharacter::SetWapon() 
{
	weapon->DMWeapon = 20;
	weapon->ID_Weapon = 0;
	weapon->TypeWeapon = 0;// 0-Melee, 1-Distancia, 2-Magia
	weapon->RangeWeapon = 1;
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
	else if (weapon->TypeWeapon == 1) 
	{
		totalStats->PERBonus = (ceil(basicStats->PER / 2)) - 3;
	}
	//distancia
	else if (weapon->TypeWeapon == 2)
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
	totalStats->APTotal = initBaseStats->APBase + totalStats->AP;
	totalStats->APCurrent = totalStats->APTotal;
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
	if ((basicStats->CON + basicStats->PER) < 10)
	{
		totalStats->SA = floor((basicStats->CON + basicStats->PER) / 2) - 5;
	}
	else if ((basicStats->CON + basicStats->PER) >= 10)
	{
		totalStats->SA = ceil((basicStats->POD + basicStats->PER) / 2) - 5;
	}

	//Vida total
	//	totalStats->HPTotal = (initBaseStats->HPBase + HPBonus) * (1 + totalStats->CONBonus); - HPBonus es por pasivas u otras fuentes
	totalStats->HPTotal = (initBaseStats->HPBase) * (1 + totalStats->CONBonus);
	totalStats->HPCurrent = totalStats->HPTotal;

}

void PD_GM_LogicCharacter::SetIsPlayer(bool nIsPlayer){ isPlayer = nIsPlayer; }
void PD_GM_LogicCharacter::SetIsDead(bool nIsDead){ isDead = nIsDead; }
void PD_GM_LogicCharacter::SetIDCharacter(FString nID_character){ ID_character = nID_character; }
void PD_GM_LogicCharacter::SetTypeCharacter(ECharacterType nID_character) { type_character = nID_character; }
void PD_GM_LogicCharacter::SetController(APD_GenericController* ncontroller){ controller = ncontroller; }
void PD_GM_LogicCharacter::SetCharacterBP(APD_E_Character* ncharacter_Player_BP){ character_Player_BP = ncharacter_Player_BP; }
void PD_GM_LogicCharacter::SetCurrentLogicalPosition(PD_MG_LogicPosition ncurrentLogicalPosition){ currentLogicalPosition = ncurrentLogicalPosition; }
void PD_GM_LogicCharacter::SetMovingLogicalPosition(PD_MG_LogicPosition nmovingLogicalPosition) { movingLogicalPosition = nmovingLogicalPosition; }