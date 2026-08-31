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

enum class enCategoryGroup   
{
	Number,   // 1の目~6の目
	SumRule,  // オッドサム、イーブンサム、ファイブ倍、スリー倍、ロー、ハイ
	Shape,    // レインボー、フルハウス、スリーカード、フォーカード、S/Lストレート
	Chance,   // チャンス
	Yacht,    // ヨット
};

using DiceValues = std::array<int, 5>;

struct ScoreCategory
{
	enScoreCategoryType type;
	enCategoryGroup group;
	int sortOrder;
	std::wstring name;
	std::function<int(const DiceValues&)> calcScore;
};

std::vector<ScoreCategory> BuildFullCategoryPool();
std::vector<ScoreCategory> BuildMatchScoreboard();