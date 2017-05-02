// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "MapGeneration/PD_MG_LogicPosition.h"




PD_MG_LogicPosition::PD_MG_LogicPosition() {
	_x = 0;
	_y = 0;
}
PD_MG_LogicPosition::PD_MG_LogicPosition(int32 x, int32 y)
{
	_x = x;
	_y = y;
}

PD_MG_LogicPosition::~PD_MG_LogicPosition()
{
}



