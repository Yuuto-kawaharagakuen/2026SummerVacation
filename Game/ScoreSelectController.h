#pragma once
#include "ScoreCategory.h"
#include <vector>

class ScoreSelectController
{
public:
	// 選択中の役インデックスを取得(ハイライト表示用)
	int GetSelectedIndex() const { return m_selectedIndex; }

	// 役が確定したらtrueを返す。
	bool Update(
		const std::vector<ScoreCategory>& board,
		const std::vector<bool>& filled,
		int& outConfirmedIndex);

private:
	int m_selectedIndex = 0;
	bool m_prevUpState = false;
	bool m_prevDownState = false;
	bool m_prevEnterState = false;
};

