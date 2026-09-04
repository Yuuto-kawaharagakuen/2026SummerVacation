#pragma once
#include "ScoreCategory.h"
#include <vector>

class ScoreBoardView
{
public:
	static const int kMaxCategories = 14;

	void Init(const std::vector<ScoreCategory>& board);
	void Update(const std::vector<ScoreCategory>& board,
		const std::vector<bool>& playerFilled, const std::vector<int>& playerScores, const DiceValues& playerCurrentDice,
		const std::vector<bool>& cpuFilled, const std::vector<int>& cpuScores,
		int selectedIndex, bool isPlayerTurn);
	void Render(RenderContext& rc);

private:
	FontRender m_nameTexts[kMaxCategories];
	FontRender m_playerScoreTexts[kMaxCategories];  
	FontRender m_cpuScoreTexts[kMaxCategories];
	FontRender m_playerHeaderText; 
	FontRender m_cpuHeaderText;    
	int m_numCategories = 0;
};