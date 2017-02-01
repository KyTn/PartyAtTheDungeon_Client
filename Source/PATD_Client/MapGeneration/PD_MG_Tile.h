// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/// CLASS REFERENCES

class PD_MG_StaticMap;
class PD_MG_Room;
class PD_MG_LogicPosition;


class PATD_CLIENT_API PD_MG_Tile
{
private:
	PD_MG_StaticMap* _SMap;
	PD_MG_Room* _room;
	PD_MG_Tile* _neigthbours[3][3];
	PD_MG_LogicPosition* _LogicPos;


public:
	PD_MG_Tile();
	~PD_MG_Tile();
};
