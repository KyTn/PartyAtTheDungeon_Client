// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "math.h"
/**
 * 
 */
class PATD_CLIENT_API PD_MG_LogicPosition
{
private:
	int32 _x;
	int32 _y;

public:

	PD_MG_LogicPosition();
	PD_MG_LogicPosition(uint32 x, uint32 y);
	PD_MG_LogicPosition(uint16 pos);
	~PD_MG_LogicPosition();

	int32 GetX() { return _x; }
	void SetX(int32 x) { _x = x; }
	int32 GetY() { return _y; }
	void SetY(int32 y) { _y = y; }

	FORCEINLINE bool operator==(const PD_MG_LogicPosition& other) const
	{
		return this->_x == other._x && this->_y == other._y;
	}

	FORCEINLINE PD_MG_LogicPosition operator-(const PD_MG_LogicPosition& other) const
	{
		return PD_MG_LogicPosition(this->_x - other._x, this->_y - other._y);
	}

	FORCEINLINE PD_MG_LogicPosition operator+(const PD_MG_LogicPosition& other) const
	{
		return PD_MG_LogicPosition(this->_x + other._x, this->_y + other._y);
	}

	friend uint32 GetTypeHash(const PD_MG_LogicPosition& logpos)
	{
		return (logpos._x << 16) + logpos._y;
	}


	uint16 GetIn16bits(const PD_MG_LogicPosition& logpos)
	{
		return ((uint16)(logpos._x) << 8) + (uint16)(logpos._y);
	}

	uint16 GetIn16bits()
	{
		return ((uint16)(this->_x) << 8) + (uint16)(this->_y);
	}


	void SetIn16bits(uint16 num) {
		this->SetX((uint32)(num & 0x00FF));
		this->SetY((uint32)((num >> 8) & 0x00FF));
	}

	/*
	static FVector* LogicToWorldPosition(PD_MG_LogicPosition pos) {
	return new FVector(-1.0f * pos.GetX()*100.0f, pos.GetY() * 100.0f, 0.f);
	}
	*/
	/*
	Se pone el bool isCharacter para determina si hay que subir su Z en 100 unidades, y que asi no caiga cuando empiece el mapa
	en tiempo de ejecucion.
	*/
	FVector ToWorldPosition(bool isCharacter) {
		if (isCharacter)
			return FVector(-1.0f * GetX()*100.0f, GetY() * 100.0f, 40.f);
		else
			return FVector(-1.0f * GetX()*100.0f, GetY() * 100.0f, 0.f);
	}

	FVector ToWorldPosition() {
		return FVector(-1.0f * GetX()*100.0f, GetY() * 100.0f, 0.f);
	}

	TArray<PD_MG_LogicPosition> GetDiagonalsAndAdjacentsFromList(TArray<PD_MG_LogicPosition> list) {
		TArray<PD_MG_LogicPosition> res = TArray<PD_MG_LogicPosition>();
		for (int i = 0; i < list.Num(); i++) {
			if (this->EuclideanDistance(list[i]) <= 1.7f) {
				res.Add(list[i]);
			}
		}
		return res;

	}


	TArray<PD_MG_LogicPosition> GetAllTilesInRange(float range, TArray<PD_MG_LogicPosition> list) {
		TArray<PD_MG_LogicPosition> res = TArray<PD_MG_LogicPosition>();
		for (int i = 0; i < list.Num(); i++) {
			if (this->EuclideanDistance(list[i]) <= range) {
				res.Add(list[i]);
			}
		}
		return res;

	}


	TArray<PD_MG_LogicPosition> GetAdjacentsFromList(TArray<PD_MG_LogicPosition> list) {

		TArray<PD_MG_LogicPosition> res = TArray<PD_MG_LogicPosition>();

		for (int i = 0; i < list.Num(); i++) {
			if (this->ManhattanDistance(list[i]) == 1) {
				res.Add(list[i]);
			}
		}
		return res;
	}

	TArray<PD_MG_LogicPosition> GenerateAdjacents() {

		TArray<PD_MG_LogicPosition> res = TArray<PD_MG_LogicPosition>();

		res.Add(PD_MG_LogicPosition(this->GetX(), this->GetY() - 1));
		res.Add(PD_MG_LogicPosition(this->GetX(), this->GetY() + 1));
		res.Add(PD_MG_LogicPosition(this->GetX() + 1, this->GetY()));
		res.Add(PD_MG_LogicPosition(this->GetX() - 1, this->GetY()));
		return res;
	}


	int ManhattanDistance(PD_MG_LogicPosition other) {
		int deltaX = other.GetX() - this->GetX();
		int deltaY = other.GetY() - this->GetY();
		return  abs(deltaX) + abs(deltaY);
	}

	float EuclideanDistance(PD_MG_LogicPosition other) {
		int deltaX = this->GetX()*this->GetX() + other.GetX()*other.GetX() - (2 * this->GetX()*other.GetX());
		int deltaY = this->GetY()*this->GetY() + other.GetY()*other.GetY() - (2 * this->GetY()*other.GetY());
		int result = deltaX + deltaY;
		return sqrt(result);
	}

	

};
