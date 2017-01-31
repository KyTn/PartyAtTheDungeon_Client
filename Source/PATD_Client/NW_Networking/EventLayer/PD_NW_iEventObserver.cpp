// Fill out your copyright notice in the Description page of Project Settings.

#include "PATD_Client.h"
#include "PD_NW_iEventObserver.h"

PD_NW_iEventObserver::PD_NW_iEventObserver()
{
}

PD_NW_iEventObserver::~PD_NW_iEventObserver()
{
}


void PD_NW_iEventObserver::setUpObserver(int inPlayer, UStructType inType) {
	//Comprobacion aqui de que es correcto el setup? P.ej: No vale player<-1 (-1 para todos)
	player = inPlayer;
	eventType = inType;
}

int PD_NW_iEventObserver::getPlayer() {
	return player;
}
UStructType PD_NW_iEventObserver::getType() {
	return eventType;
}