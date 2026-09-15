#pragma once
#include "ScoreCategory.h"
#include <vector>

class ResultView
{
public:
	static const int kMaxCategories = 14;

	void Init(const std::vector<ScoreCategory>& board);
	void Update(const std::vector<ScoreCategory>& board,
		const std::vector<int>& playerScores, const std::vector<int>& cpuScores);
	void Render(RenderContext& rc);

private:
	FontRender m_nameTexts[kMaxCategories];
	FontRender m_playerScoreTexts[kMaxCategories];
	FontRender m_cpuScoreTexts[kMaxCategories];
	FontRender m_playerTotalText;
	FontRender m_cpuTotalText;
	FontRender m_resultMessageText;
	FontRender m_totalLabelText;
	FontRender m_playerHeaderText; 
	FontRender m_cpuHeaderText;
	int m_numCategories = 0;
	
};
