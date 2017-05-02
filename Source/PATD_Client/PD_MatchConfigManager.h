// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Structs/PD_ClientEnums.h"
#include "PD_MatchConfig.h"
#include "PD_ClientGameInstance.h"
/**
 * 
 */
class PATD_CLIENT_API PD_MatchConfigManager
{
private:
	PD_MatchConfig* ActualMatchConfig;
	UPD_ClientGameInstance *SGI;

public:
	PD_MatchConfigManager(UPD_ClientGameInstance * sgi);
	~PD_MatchConfigManager();

	// estas funciones modifican los valores de la configuracion, Y MANDA EL CAMBIO AL SERVER
	void SetAndSend_MapSize(MATCHCONFIG_MAPSIZE newSize) {
		ActualMatchConfig->SetMapSize(newSize);

		// mandar cambio al server
		SGI->SendToServerMatchConfigUpdate(1, (int)newSize, "");
	}

	void SetAndSend_Difficulty(MATCHCONFIG_DIFFICULTY newDiff) {
		ActualMatchConfig->SetDifficulty(newDiff);

		// mandar cambio al server
		SGI->SendToServerMatchConfigUpdate(2, (int)newDiff, "");
	}

	void SetAndSend_MissionType(MATCHCONFIG_MISSIONTYPE newMission) {
		ActualMatchConfig->SetMissionType(newMission);

		// mandar cambio al server
		SGI->SendToServerMatchConfigUpdate(0, (int)newMission, "");
	}

	

	// estas funciones modifican los valores de la configuracion (no las manda al server de vuelta)
	void Set_MapSize(MATCHCONFIG_MAPSIZE newSize) {
		ActualMatchConfig->SetMapSize(newSize);
	}

	void Set_Difficulty(MATCHCONFIG_DIFFICULTY newDiff) {
		ActualMatchConfig->SetDifficulty(newDiff);
	}

	void Set_MissionType(MATCHCONFIG_MISSIONTYPE newMission) {
		ActualMatchConfig->SetMissionType(newMission);
	}






	// estas funciones devuelven los valores de la configuracion
	MATCHCONFIG_MAPSIZE Get_MapSize() {
		return ActualMatchConfig->GetMapSize();
	}

	MATCHCONFIG_DIFFICULTY Get_Difficulty() {
		return ActualMatchConfig->GetDifficulty();
	}

	MATCHCONFIG_MISSIONTYPE Get_MissionType() {
		return ActualMatchConfig->GetMissionType();
	}

};
