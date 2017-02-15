// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "LevelsNameDictionary.h"

LevelsNameDictionary::LevelsNameDictionary()
{
}

LevelsNameDictionary::~LevelsNameDictionary()
{
}

FString LevelsNameDictionary::GetMapName(int lvl, bool inEditor)
{
	FString mapName = "";
	switch (lvl)
	{
	case 1:
		if (!inEditor)
			mapName = name_LVL_1;
		else
			mapName = name_LVL_1_inEditor;
		break;
	case 2:
		if (!inEditor)
			mapName = name_LVL_2;
		else
			mapName = name_LVL_2_inEditor;
		break;
	case 3:
		if (!inEditor)
			mapName = name_LVL_3;
		else
			mapName = name_LVL_3_inEditor;
		break;
	case 4:
		if (!inEditor)
			mapName = name_LVL_4;
		else
			mapName = name_LVL_4_inEditor;
		break;
	}
	return mapName;
}