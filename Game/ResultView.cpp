#include "stdafx.h"
#include "ResultView.h"

namespace
{
	const float kStartY = 400.0f;
	const float kLineHeight = 40.0f;
	const float kNameX = -350.0f;          
	const float kPlayerScoreX = -60.0f;    
	const float kCpuScoreX = 60.0f;
}

void ResultView::Init(const std::vector<ScoreCategory>& board)
{
	m_numCategories = (int)board.size();

	for (int i = 0; i < m_numCategories; i++)
	{
		float y = kStartY - kLineHeight * (float)i;

		m_nameTexts[i].SetText(board[i].name.c_str());
		m_nameTexts[i].SetPosition({ kNameX, y, 0.0f });
		m_nameTexts[i].SetScale(1.0f);
		m_nameTexts[i].SetColor(g_vec4Black);

		m_playerScoreTexts[i].SetPosition({ kPlayerScoreX, y, 0.0f });
		m_playerScoreTexts[i].SetScale(1.0f);
		m_playerScoreTexts[i].SetColor(g_vec4Black);

		m_cpuScoreTexts[i].SetPosition({ kCpuScoreX, y, 0.0f });
		m_cpuScoreTexts[i].SetScale(1.0f);
		m_cpuScoreTexts[i].SetColor(Vector4(0.8f, 0.0f, 0.0f, 1.0f));
	}
	float headerY = kStartY + kLineHeight;
	m_playerHeaderText.SetText(L"あなた");
	m_playerHeaderText.SetPosition({ kPlayerScoreX - 60.0f, headerY, 0.0f }); // ScoreBoardViewと同じ調整
	m_playerHeaderText.SetScale(1.0f);
	m_playerHeaderText.SetColor(g_vec4Black);

	m_cpuHeaderText.SetText(L"CPU");
	m_cpuHeaderText.SetPosition({ kCpuScoreX, headerY, 0.0f });
	m_cpuHeaderText.SetScale(1.0f);
	m_cpuHeaderText.SetColor(g_vec4Black);

	float totalY = kStartY - kLineHeight * (float)m_numCategories - kLineHeight;

	m_totalLabelText.SetText(L"合計"); 
	m_totalLabelText.SetPosition({ kNameX, totalY, 0.0f });
	m_totalLabelText.SetScale(1.2f);
	m_totalLabelText.SetColor(g_vec4Black);

	m_playerTotalText.SetPosition({ kPlayerScoreX, totalY, 0.0f });
	m_playerTotalText.SetScale(1.2f);
	m_playerTotalText.SetColor(g_vec4Black);

	m_cpuTotalText.SetPosition({ kCpuScoreX, totalY, 0.0f });
	m_cpuTotalText.SetScale(1.2f);
	m_cpuTotalText.SetColor(Vector4(0.8f, 0.0f, 0.0f, 1.0f));

	m_resultMessageText.SetPosition({ kNameX, totalY - kLineHeight * 1.5f, 0.0f });
	m_resultMessageText.SetScale(2.0f);
	m_resultMessageText.SetColor(g_vec4Black);
}

void ResultView::Update(const std::vector<ScoreCategory>& board,
	const std::vector<int>& playerScores, const std::vector<int>& cpuScores)
{
	int playerTotal = 0;
	int cpuTotal = 0;

	for (int i = 0; i < m_numCategories; i++)
	{
		wchar_t playerBuf[16];
		swprintf_s(playerBuf, L" %d", playerScores[i]);
		m_playerScoreTexts[i].SetText(playerBuf);

		wchar_t cpuBuf[16];
		swprintf_s(cpuBuf, L" %d", cpuScores[i]);
		m_cpuScoreTexts[i].SetText(cpuBuf);

		playerTotal += playerScores[i];
		cpuTotal += cpuScores[i];
	}

	wchar_t playerTotalBuf[32];
	swprintf_s(playerTotalBuf, L" %d", playerTotal);
	m_playerTotalText.SetText(playerTotalBuf);

	wchar_t cpuTotalBuf[32];
	swprintf_s(cpuTotalBuf, L" %d", cpuTotal);
	m_cpuTotalText.SetText(cpuTotalBuf);

	if (playerTotal > cpuTotal)
	{
		m_resultMessageText.SetText(L"あなたの勝ち！");
	}
	else if (playerTotal < cpuTotal)
	{
		m_resultMessageText.SetText(L"CPUの勝ち");
	}
	else
	{
		m_resultMessageText.SetText(L"引き分け");
	}
}

void ResultView::Render(RenderContext& rc)
{
	for (int i = 0; i < m_numCategories; i++)
	{
		m_nameTexts[i].Draw(rc);
		m_playerScoreTexts[i].Draw(rc);
		m_cpuScoreTexts[i].Draw(rc);
	}
	m_playerHeaderText.Draw(rc);
	m_cpuHeaderText.Draw(rc);
	m_totalLabelText.Draw(rc);
	m_playerTotalText.Draw(rc);
	m_cpuTotalText.Draw(rc);
	m_resultMessageText.Draw(rc);
}