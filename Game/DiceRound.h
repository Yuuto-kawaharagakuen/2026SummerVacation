#pragma once
#include "ScoreCategory.h"
#include <array>

class DiceRound
{
public:
	static const int kMaxRolls = 3;

	void StartNewRound();              // 手番開始：キープ全解除+1投目を振る
	void RollDice();                   // キープしてないダイスだけ振る(残り回数を1消費)
	void ToggleKeep(int diceIndex);    // 指定ダイスのキープ状態を反転

	const DiceValues& GetDice() const { return m_dice; }
	const std::array<bool, 5>& GetKeepMask() const { return m_keepMask; }
	int GetRollsRemaining() const { return m_rollsRemaining; }
	bool CanRoll() const { return m_rollsRemaining > 0; }

private:
	DiceValues m_dice{};
	std::array<bool, 5> m_keepMask{};
	int m_rollsRemaining = kMaxRolls;
};