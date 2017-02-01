// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PATD_Client/MapGeneration/PD_MG_StaticMap.h"
#include "PATD_Client/MapGeneration/PD_MG_LogicPosition.h"

PD_MG_StaticMap::PD_MG_StaticMap()
{
	//_rooms = new TArray<PD_MG_Room*>();
	//_LogicPositionsRefs = new TArray<PD_MG_LogicPosition*>();
}

PD_MG_StaticMap::~PD_MG_StaticMap()
{
}


PD_MG_LogicPosition* PD_MG_StaticMap::AddNewLogicPosition(uint32 x, uint32 y, TCHAR c)
{
	PD_MG_LogicPosition* lp = new PD_MG_LogicPosition(x, y);

	_LogicPositionsRefs.Add(lp);
	_xymap.Add(*lp, c);

	/*
	FString s;
	s.AppendInt(x);
	s.Append(",");
	s.AppendInt(y);
	s.Append("-");
	s.AppendChar(c);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Adding LP (" + s + ")");
	/**/
	return lp;
}

bool PD_MG_StaticMap::Clear() { return false; }
