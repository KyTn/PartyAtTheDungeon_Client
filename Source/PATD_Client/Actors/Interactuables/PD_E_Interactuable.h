// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/PD_E_ElementActor.h"
#include "MapGeneration/PD_MG_LogicPosition.h"

#include "PD_E_Interactuable.generated.h"

/**
 * 
 */
UCLASS()
class PATD_CLIENT_API APD_E_Interactuable : public APD_E_ElementActor
{
	GENERATED_BODY()

public:

	APD_E_Interactuable();

	// es el id del interactuable 
	int ID_Interactuable;

	// Posiciones desde el cual un character puede interactuar con este elemento
	TArray<PD_MG_LogicPosition> InteractFromThisLogicPositions;

	// Cuando el interactuable se activa, llama al InteractActivate de los reactores, en funcion de su configuracion
	TArray<APD_E_Interactuable*> ActivateThisReactorsWhenActive;


	bool IsCurrentlyActivated;

#pragma region INTERACTUABLE CONFIG

	// Puede interactuar este interactuable ahora mismo?
	bool CanInteract;
	// Una vez activado, puedes desactivar el interactuable?
	bool CanBeDeactivate;
	// Activar los reactores solo la primera vez que se active este interactuable, o todas las veces?
	bool ActivateReactorsOnlyOnce;
	// Han sido los reactores activados alguna vez?
	bool ReactorsAlreadyActivated;

#pragma endregion

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Se llamará a esta funcion para activar el interactuable
	virtual void InteractToActivate(AActor* interactor, bool overwriteState = false);

	// Se llamará a esta función para desactivar el interactuable
	virtual void InteractToDeactivate(AActor* interactor, bool overwriteState = false);
};
