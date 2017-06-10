// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class PATD_CLIENT_API LevelsNameDictionary
{

	FString name_LVL_1 = "LVL_1_Start_PartyAtDungeon";

	FString name_LVL_2 = "LVL_2_MainMenu";

	FString name_LVL_3 = "LVL_3_SelectChars_Lobby";

	FString name_LVL_4 = "LVL_4_GameMap";

	FString name_LVL_5 = "LVL_5_Podium";

public:
	LevelsNameDictionary();
	~LevelsNameDictionary();

	FString GetMapName(int lvl);

};
