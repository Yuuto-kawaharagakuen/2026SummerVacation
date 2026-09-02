#include "stdafx.h"
#include "ScoreSelectController.h"
#include <windows.h>

namespace
{
	bool IsKeyDown(int vkCode)
	{
		return (GetAsyncKeyState(vkCode) & 0x8000) != 0;
	}
}

bool ScoreSelectController::Update(
	const std::vector<ScoreCategory>& board,
	const std::vector<bool>& filled,
	int& outConfirmedIndex)
{
	int numCategories = (int)board.size();
	if (numCategories == 0) return false;

	// ↓キーで次の役へ(埋まっている役はスキップ)
	bool downState = IsKeyDown(VK_DOWN);
	if (downState && !m_prevDownState)
	{
		do {
			m_selectedIndex = (m_selectedIndex + 1) % numCategories;
		} while (filled[m_selectedIndex]);
	}
	m_prevDownState = downState;

	// ↑キーで前の役へ(埋まっている役はスキップ)
	bool upState = IsKeyDown(VK_UP);
	if (upState && !m_prevUpState)
	{
		do {
			m_selectedIndex = (m_selectedIndex - 1 + numCategories) % numCategories;
		} while (filled[m_selectedIndex]);
	}
	m_prevUpState = upState;

	// Enterキーで確定(未確定の役のみ)
	bool enterState = IsKeyDown(VK_RETURN);
	bool confirmed = false;
	if (enterState && !m_prevEnterState && !filled[m_selectedIndex])
	{
		outConfirmedIndex = m_selectedIndex;
		confirmed = true;
	}
	m_prevEnterState = enterState;

	return confirmed;
}