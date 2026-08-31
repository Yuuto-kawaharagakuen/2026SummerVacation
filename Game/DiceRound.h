#pragma once
#include "ScoreCategory.h"
#include <array>

enum class enRollState
{
	Idle,
	Rolling,
	Settled,
};

class DiceRound
{
public:
	static const int kMaxRolls = 3;

	void StartNewRound();
	void RollDice();                              // 振り始める(結果はまだ確定しない)
	void CompleteRoll(const DiceValues& results);  // 物理的に静止した結果を反映
	void ToggleKeep(int diceIndex);

	const DiceValues& GetDice() const { return m_dice; }
	const std::array<bool, 5>& GetKeepMask() const { return m_keepMask; }
	int GetRollsRemaining() const { return m_rollsRemaining; }
	bool CanRoll() const { return m_rollsRemaining > 0 && m_state != enRollState::Rolling; }
	enRollState GetRollState() const { return m_state; }

private:
	DiceValues m_dice{};
	std::array<bool, 5> m_keepMask{};
	int m_rollsRemaining = kMaxRolls;
	enRollState m_state = enRollState::Idle;
};