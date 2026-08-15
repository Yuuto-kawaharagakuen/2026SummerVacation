#pragma once
#include "ScoreCategory.h"
#include <vector>
#include <array>

class CpuPlayer
{
public:
	// 現在のダイスと未使用の役一覧から、キープするダイスを決める(true=キープ)
	std::array<bool, 5> DecideKeepMask(
		const DiceValues& currentDice,
		const std::vector<ScoreCategory>& board,
		const std::vector<bool>& filled);

	// 最終的なダイスで、どの役に入れるかを決める(board内のインデックスを返す)
	int DecideCategoryToFill(
		const DiceValues& finalDice,
		const std::vector<ScoreCategory>& board,
		const std::vector<bool>& filled);
};
