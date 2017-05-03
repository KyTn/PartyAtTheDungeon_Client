// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Structs/PD_ClientEnums.h"
/**
 * 
 */
class PATD_CLIENT_API PD_MatchConfig
{
public:
	PD_MatchConfig();
	~PD_MatchConfig();

	MATCHCONFIG_MAPSIZE mapSize;
	MATCHCONFIG_DIFFICULTY difficulty;
	MATCHCONFIG_MISSIONTYPE missionType;



	void ResetConfig() {
		mapSize = MATCHCONFIG_MAPSIZE::NORMAL_SIZE;
		difficulty = MATCHCONFIG_DIFFICULTY::NORMAL_DIFFICULTY;
		missionType = MATCHCONFIG_MISSIONTYPE::DefeatBoss;
	}

	void SetMapSize(MATCHCONFIG_MAPSIZE newSize) { mapSize = newSize; }
	void SetDifficulty(MATCHCONFIG_DIFFICULTY newDifficulty) { difficulty = newDifficulty; }
	void SetMissionType(MATCHCONFIG_MISSIONTYPE newMissionType) { missionType = newMissionType; }

	MATCHCONFIG_MAPSIZE GetMapSize() { return mapSize; }
	MATCHCONFIG_DIFFICULTY GetDifficulty() { return difficulty; }
	MATCHCONFIG_MISSIONTYPE GetMissionType() { return missionType; }
};
