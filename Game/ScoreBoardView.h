#pragma once
#include "ScoreCategory.h"
#include <vector>

class ScoreBoardView
{
public:
	static const int kMaxCategories = 14;

	void Init(const std::vector<ScoreCategory>& board);
	void Update(const std::vector<ScoreCategory>& board, const std::vector<bool>& filled,
		const std::vector<int>& finalScores, const DiceValues& currentDice);
	void Render(RenderContext& rc);

private:
	FontRender m_nameTexts[kMaxCategories];   // vector→固定配列に変更
	FontRender m_scoreTexts[kMaxCategories];  // vector→固定配列に変更
	int m_numCategories = 0;                  // 実際に使う数(board.size())
};
