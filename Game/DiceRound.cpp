#include "stdafx.h"
#include "DiceRound.h"

namespace
{
	int RollOne() { return rand() % 6 + 1; }
}

void DiceRound::StartNewRound()
{
	m_keepMask = {};
	m_rollsRemaining = kMaxRolls;
	RollDice(); // 手番開始時に自動で1投目
}

void DiceRound::RollDice()
{
	if (!CanRoll()) return;

	for (int i = 0; i < 5; i++)
	{
		if (!m_keepMask[i]) m_dice[i] = RollOne();
	}
	m_rollsRemaining--;
}

void DiceRound::ToggleKeep(int diceIndex)
{
	if (diceIndex < 0 || diceIndex >= 5) return;
	m_keepMask[diceIndex] = !m_keepMask[diceIndex];
}