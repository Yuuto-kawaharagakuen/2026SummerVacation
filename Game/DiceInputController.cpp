#include "stdafx.h"
#include "DiceInputController.h"
#include <windows.h>

namespace
{
	bool IsKeyDown(int vkCode)
	{
		return (GetAsyncKeyState(vkCode) & 0x8000) != 0;
	}

	void PrintState(const DiceRound& round)
	{
		const auto& dice = round.GetDice();
		const auto& mask = round.GetKeepMask();
		wchar_t buf[128];
		swprintf_s(buf, L"ダイス:[%d,%d,%d,%d,%d] キープ:[%d,%d,%d,%d,%d] 残り%d回\n",
			dice[0], dice[1], dice[2], dice[3], dice[4],
			mask[0], mask[1], mask[2], mask[3], mask[4],
			round.GetRollsRemaining());
		OutputDebugStringW(buf);
	}
}

void DiceInputController::Update(DiceRound& round)
{
	// 数字キー1〜5でキープ切り替え(押した瞬間のみ反応、押しっぱなし対策)
	const int keyCodes[5] = { '1', '2', '3', '4', '5' };
	for (int i = 0; i < 5; i++)
	{
		bool down = IsKeyDown(keyCodes[i]);
		if (down && !m_prevKeyState[i])
		{  
			if (round.GetRollState()==enRollState::Settled) {
				round.ToggleKeep(i);
				PrintState(round);
			}	
		}
		m_prevKeyState[i] = down;
	}

	// スペースキーで振る(残り回数がある時だけ)
	bool spaceDown = IsKeyDown(VK_SPACE);
	if (spaceDown && !m_prevSpaceState && round.CanRoll())
	{
		round.RollDice();
		PrintState(round);
	}
	m_prevSpaceState = spaceDown;

	// Rキーで手番リセット(テスト用の便利機能)
	bool rDown = IsKeyDown('R');
	if (rDown && !m_prevRState)
	{
		round.StartNewRound();
		OutputDebugStringW(L"---- 手番リセット ----\n");
		PrintState(round);
	}
	m_prevRState = rDown;
}