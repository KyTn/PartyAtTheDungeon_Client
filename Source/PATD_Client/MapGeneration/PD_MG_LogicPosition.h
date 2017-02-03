// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class PATD_CLIENT_API PD_MG_LogicPosition
{
private:
	uint32 _x;
	uint32 _y;

public:

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

	
	friend FORCEINLINE  uint32 GetTypeHash(const PD_MG_LogicPosition& logpos)
	{
		return (logpos._x << 16) + logpos._y;
	}
};
