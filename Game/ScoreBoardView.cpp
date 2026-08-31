#include "stdafx.h"
#include "ScoreBoardView.h"

namespace
{
	const float kStartY = 400.0f;
	const float kLineHeight = 40.0f;
	const float kNameX = -850.0f;
	const float kScoreX = -600.0f;
}

void ScoreBoardView::Init(const std::vector<ScoreCategory>& board)
{
	m_numCategories = (int)board.size();

	for (int i = 0; i < m_numCategories; i++)
	{
		float y = kStartY - kLineHeight * (float)i;

		m_nameTexts[i].SetText(board[i].name.c_str());
		m_nameTexts[i].SetPosition({ kNameX, y, 0.0f });
		m_nameTexts[i].SetScale(1.0f);
		m_nameTexts[i].SetColor(g_vec4White);

		m_scoreTexts[i].SetPosition({ kScoreX, y, 0.0f });
		m_scoreTexts[i].SetScale(1.0f);
	}
}

void ScoreBoardView::Update(const std::vector<ScoreCategory>& board, const std::vector<bool>& filled,
	const std::vector<int>& finalScores, const DiceValues& currentDice)
{
	for (int i = 0; i < m_numCategories; i++)
	{
		wchar_t buf[16];
		if (filled[i])
		{
			swprintf_s(buf, L"%d", finalScores[i]);
			m_scoreTexts[i].SetColor(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
		}
		else
		{
			int preview = board[i].calcScore(currentDice);
			swprintf_s(buf, L"(%d)", preview);
			m_scoreTexts[i].SetColor(g_vec4Cyan);
		}
		m_scoreTexts[i].SetText(buf);
	}
}

void ScoreBoardView::Render(RenderContext& rc)
{
	for (int i = 0; i < m_numCategories; i++)
	{
		m_nameTexts[i].Draw(rc);
		m_scoreTexts[i].Draw(rc);
	}
}