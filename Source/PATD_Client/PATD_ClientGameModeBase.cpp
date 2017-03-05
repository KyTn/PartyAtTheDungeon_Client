// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PATD_ClientGameModeBase.h"

#include "PD_GameStateInitializer.h"

APATD_ClientGameModeBase::APATD_ClientGameModeBase() {
	//Super();
	GameStateClass = APD_GameStateInitializer::StaticClass();
	
}
