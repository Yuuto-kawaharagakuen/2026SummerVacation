#include "stdafx.h"
#include "DiceRound.h"

void DiceRound::StartNewRound()
{	
		m_keepMask = {};
		m_rollsRemaining = kMaxRolls;
		m_state = enRollState::Idle; // Rolling にせず、Idle のまま待機
}

void DiceRound::RollDice()
{
	if (!CanRoll()) return;
	m_rollsRemaining--;
	m_state = enRollState::Rolling;
}

void DiceRound::CompleteRoll(const DiceValues& results)
{
	m_dice = results;
	m_state = enRollState::Settled;
}

void DiceRound::ToggleKeep(int diceIndex)
{
	if (diceIndex < 0 || diceIndex >= 5) return;
	m_keepMask[diceIndex] = !m_keepMask[diceIndex];
}