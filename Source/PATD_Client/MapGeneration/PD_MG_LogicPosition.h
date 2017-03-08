// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "math.h"
/**
 * 
 */
class PATD_CLIENT_API PD_MG_LogicPosition
{
private:
	uint32 _x;
	uint32 _y;

public:

	PD_MG_LogicPosition();
	PD_MG_LogicPosition(uint32 x, uint32 y);
	~PD_MG_LogicPosition();

	uint32 GetX() { return _x; }
	void SetX(uint32 x) { _x = x; }
	uint32 GetY() { return _y; }
	void SetY(uint32 y) { _y = y; }

	FORCEINLINE bool operator==(const PD_MG_LogicPosition& other) const
	{
		return this->_x == other._x && this->_y == other._y;
	}

	friend uint32 GetTypeHash(const PD_MG_LogicPosition& logpos)
	{
		return (logpos._x << 16) + logpos._y;
	}
	
	
	/*
	static FVector* LogicToWorldPosition(PD_MG_LogicPosition pos) {
	return new FVector(-1.0f * pos.GetX()*100.0f, pos.GetY() * 100.0f, 0.f);
	}
	

	Se pone el bool isCharacter para determina si hay que subir su Z en 100 unidades, y que asi no caiga cuando empiece el mapa
	en tiempo de ejecucion.
	*/


	const FVector ToWorldPosition(bool isCharacter) {
		if (isCharacter)
			return FVector(-1.0f * GetX()*100.0f, GetY() * 100.0f, 100.f);
		else
			return FVector(-1.0f * GetX()*100.0f, GetY() * 100.0f, 0.f);
	}

	const FVector ToWorldPosition() {
		return FVector(-1.0f * GetX()*100.0f, GetY() * 100.0f, 0.f);
	}



	TArray<PD_MG_LogicPosition> GetAdjacents(TArray<PD_MG_LogicPosition> list) {

		TArray<PD_MG_LogicPosition> res = TArray<PD_MG_LogicPosition>();

		for (int i = 0; i < list.Num(); i++) {
			if (this->ManhattanDistance(list[i]) == 1) {
				res.Add(list[i]);
			}
		}
		return res;
	}

	int ManhattanDistance(PD_MG_LogicPosition other) {
		int deltaX = other.GetX() - this->GetX();
		int deltaY = other.GetY() - this->GetY();
		return  abs(deltaX) + abs(deltaY);
	}

};
