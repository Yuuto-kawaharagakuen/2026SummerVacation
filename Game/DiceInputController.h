#pragma once
#include "DiceRound.h"

class DiceInputController
{
public:
	void Update(DiceRound& round);

private:
	bool m_prevKeyState[5] = {};
	bool m_prevSpaceState = false;
	bool m_prevRState = false;
};