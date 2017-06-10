// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "GM_Game/PD_GM_MapManager.h"

#include "GameFramework/Actor.h"
#include "MapManagerAccesor.generated.h"



class PD_GM_MapManager;
class APD_E_Character;
class PD_MG_LogicPosition;
UCLASS()
class PATD_CLIENT_API AMapManagerAccesor : public AActor
{
	GENERATED_BODY()

public:	
	TArray<PD_MG_LogicPosition>  tilesNear;
	TArray<PD_MG_LogicPosition>  positionMoves;
	PD_GM_MapManager* mapManager;
	bool lastMoveWasClean;
	// Sets default values for this actor's properties
	AMapManagerAccesor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
		APD_E_Character*  GetCharacterPlayerAtPosition(FVector position, bool& existe);

	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
		bool IsMyCharacterPlayerAtPosition(FVector position);
	
	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
		int GetIdOfCharacterAt(FVector position);

	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
		FVector GetOwnPlayerPosition();

	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
		void TransformFVectorToLogicPosition(FVector positionInWorld, int &logicX, int &logicY);

	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
		bool GetPossibleEnemiesToAttack(TArray<AActor*> &possibleEnemies);

	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
		bool GetIDCharFromEnemy(AActor* enemyToCheck , FString &id_char);

	///FUNCIONES PUBLICADAS PARA EL SISTEMA DE INPUT DEL MOVIMIENTO
	//Colorea y setea como actual la posicion dada y muestra y setea las tiles proximas a las que se puede mover desde ahi
	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor") 
		void ShowAdjenctsTiles(FVector currentPosition);

	//Borra las tiles proximas a las que se puede mover desde la antigua posicion
	/*UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
		void DeleteAdjenctsTiles(FVector currentPosition);*/

	//Añade una casilla al array del movmiento y visualiza los sigueintes movimientos - actualiza el camino recorrido
	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
		void SetCurrentPositionAsPathMovement(FVector currentPosition);

	//elimina la ultima casilla del array de posiciones - visualiza los siguientes movimientos - actualiza el camino
	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
		void DeleteLastTileSelecteInPathMovenebt();

	//actualiza el camino recorrido
	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
		void UpdatePathMovement();

	//Limpia todas las tiles seleccionadas 
	UFUNCTION(BlueprintCallable, Category = "MapManagerAccesor")
		void ClearAllOverTilesInPathMovement();
};
 