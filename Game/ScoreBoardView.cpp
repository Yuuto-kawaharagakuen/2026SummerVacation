#include "stdafx.h"
#include "ScoreBoardView.h"

namespace
{
	const float kStartY = 400.0f;
	const float kLineHeight = 40.0f;
	const float kNameX = -850.0f;
	const float kPlayerScoreX = -590.0f;
	const float kCpuScoreX = -480.0f;  
	const float kPlayerHeaderX = kPlayerScoreX - 30.0f; 
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
		m_nameTexts[i].SetColor(g_vec4Black);

		m_playerScoreTexts[i].SetPosition({ kPlayerScoreX, y, 0.0f });
		m_playerScoreTexts[i].SetScale(1.0f);

		m_cpuScoreTexts[i].SetPosition({ kCpuScoreX, y, 0.0f }); 
		m_cpuScoreTexts[i].SetScale(1.0f);
		m_cpuScoreTexts[i].SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	
	float headerY = kStartY + kLineHeight; 

	m_playerHeaderText.SetText(L"あなた");
	m_playerHeaderText.SetPosition({ kPlayerHeaderX, headerY, 0.0f });
	m_playerHeaderText.SetScale(1.0f);
	m_playerHeaderText.SetColor(g_vec4Black);

	m_cpuHeaderText.SetText(L"CPU");
	m_cpuHeaderText.SetPosition({ kCpuScoreX, headerY, 0.0f });
	m_cpuHeaderText.SetScale(1.0f);
	m_cpuHeaderText.SetColor(g_vec4Black);
}

void ScoreBoardView::Update(const std::vector<ScoreCategory>& board,
	const std::vector<bool>& playerFilled, const std::vector<int>& playerScores, const DiceValues& playerCurrentDice,
	const std::vector<bool>& cpuFilled, const std::vector<int>& cpuScores,
	int selectedIndex, bool isPlayerTurn)
{
	for (int i = 0; i < m_numCategories; i++)
	{
		// --- あなたの列 ---
		wchar_t playerBuf[16];
		if (playerFilled[i])
		{
			swprintf_s(playerBuf, L" %d", playerScores[i]);
			m_playerScoreTexts[i].SetColor(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
		}
		else if (isPlayerTurn)
		{
			int preview = board[i].calcScore(playerCurrentDice);
			swprintf_s(playerBuf, L"(%d)", preview);
			m_playerScoreTexts[i].SetColor(i == selectedIndex ? g_vec4Yellow : g_vec4Cyan);
		}
		else
		{
			swprintf_s(playerBuf, L" 0"); // ★空文字→" 0"に変更
			m_playerScoreTexts[i].SetColor(Vector4(0.5f, 0.5f, 0.5f, 1.0f)); // ★グレーにしておく(未確定だが自分のターンではないことがわかるように)
		}
		m_playerScoreTexts[i].SetText(playerBuf);

		// --- CPUの列 ---
		wchar_t cpuBuf[16];
		if (cpuFilled[i])
		{
			swprintf_s(cpuBuf, L" %d", cpuScores[i]);
		}
		else
		{
			swprintf_s(cpuBuf, L" 0"); 
		}
		m_cpuScoreTexts[i].SetText(cpuBuf);

		// 役名の色(選択中はあなたのターンの時だけ黄色)
		m_nameTexts[i].SetColor((isPlayerTurn && i == selectedIndex) ? g_vec4Yellow : g_vec4Black); 
	}
}

void ScoreBoardView::Render(RenderContext& rc)
{
	for (int i = 0; i < m_numCategories; i++)
	{
		m_nameTexts[i].Draw(rc);
		m_playerScoreTexts[i].Draw(rc);
		m_cpuScoreTexts[i].Draw(rc);
	}
	m_playerHeaderText.Draw(rc); 
	m_cpuHeaderText.Draw(rc);    
}