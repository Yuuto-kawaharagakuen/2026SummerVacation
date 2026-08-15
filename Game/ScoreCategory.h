#pragma once
#include <array>
#include <functional>
#include <string>
#include <vector>

enum class enScoreCategoryType
{
	Ones, Twos, Threes, Fours, Fives, Sixes,
	OddSum, EvenSum, FiveMultiple, ThreeMultiple, Low, High,
	Rainbow, Chance, FullHouse, ThreeCard, FourCard,
	SStraight, LStraight,
	Yacht,
	Num
};

using DiceValues = std::array<int, 5>; // 5個のダイスの出目(1~6)

struct ScoreCategory
{
	enScoreCategoryType type;
	std::wstring name;
	std::function<int(const DiceValues&)> calcScore; // 条件を満たさなければ0を返す
};

// 19種類の役プールを生成(ヨットは含まない)
std::vector<ScoreCategory> BuildFullCategoryPool();
// 試合開始時に13個をランダム抽選し、ヨットを固定追加したスコアボードを作る
std::vector<ScoreCategory> BuildMatchScoreboard();
