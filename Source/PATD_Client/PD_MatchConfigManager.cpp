// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_MatchConfigManager.h"
#include "PD_ClientGameInstance.h"

PD_MatchConfigManager::PD_MatchConfigManager(UPD_ClientGameInstance* sgi)
{
	ActualMatchConfig = new PD_MatchConfig();
	SGI = sgi;
}

PD_MatchConfigManager::~PD_MatchConfigManager()
{
}
