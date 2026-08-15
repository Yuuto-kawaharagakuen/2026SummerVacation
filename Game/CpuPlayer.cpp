#include "stdafx.h"
#include "CpuPlayer.h"

namespace
{
	int RollOne()
	{
		return rand() % 6 + 1;
	}

	// keepMaskがtrueの位置はそのまま、falseの位置だけ振り直す
	DiceValues SimulateReroll(const DiceValues& current, const std::array<bool, 5>& keepMask)
	{
		DiceValues result = current;
		for (int i = 0; i < 5; i++)
		{
			if (!keepMask[i]) result[i] = RollOne();
		}
		return result;
	}

	// そのダイスの組み合わせで、未使用の役の中の最高得点を返す
	int BestScoreForDice(const DiceValues& dice, const std::vector<ScoreCategory>& board, const std::vector<bool>& filled)
	{
		int best = 0;
		for (size_t i = 0; i < board.size(); i++)
		{
			if (filled[i]) continue;
			int s = board[i].calcScore(dice);
			if (s > best) best = s;
		}
		return best;
	}
}

std::array<bool, 5> CpuPlayer::DecideKeepMask(
	const DiceValues& currentDice,
	const std::vector<ScoreCategory>& board,
	const std::vector<bool>& filled)
{
	const int kSimulationCount = 60; // 1パターンあたりの試行回数

	std::array<bool, 5> bestMask{};
	float bestAvg = -1.0f;

	for (int maskBits = 0; maskBits < 32; maskBits++)
	{
		std::array<bool, 5> mask{};
		for (int i = 0; i < 5; i++) mask[i] = (maskBits & (1 << i)) != 0;

		int total = 0;
		for (int trial = 0; trial < kSimulationCount; trial++)
		{
			DiceValues simulated = SimulateReroll(currentDice, mask);
			total += BestScoreForDice(simulated, board, filled);
		}
		float avg = (float)total / kSimulationCount;

		if (avg > bestAvg)
		{
			bestAvg = avg;
			bestMask = mask;
		}
	}

	return bestMask;
}

int CpuPlayer::DecideCategoryToFill(
	const DiceValues& finalDice,
	const std::vector<ScoreCategory>& board,
	const std::vector<bool>& filled)
{
	int bestIndex = -1;
	int bestScore = -1;

	for (size_t i = 0; i < board.size(); i++)
	{
		if (filled[i]) continue;
		int s = board[i].calcScore(finalDice);
		if (s > bestScore)
		{
			bestScore = s;
			bestIndex = (int)i;
		}
	}

	return bestIndex;
}