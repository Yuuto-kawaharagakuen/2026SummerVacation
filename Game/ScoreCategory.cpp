#include "stdafx.h"
#include "ScoreCategory.h"
#include <algorithm>

namespace
{
	int SumAll(const DiceValues& d)
	{
		int s = 0;
		for (int v : d) s += v;
		return s;
	}

	int CountValue(const DiceValues& d, int value)
	{
		int c = 0;
		for (int v : d) if (v == value) c++;
		return c;
	}

	// インデックス1~6を使う(0は未使用)
	std::array<int, 7> CountByValue(const DiceValues& d)
	{
		std::array<int, 7> counts{};
		for (int v : d) counts[v]++;
		return counts;
	}

	bool HasNOfAKind(const DiceValues& d, int n)
	{
		auto counts = CountByValue(d);
		for (int i = 1; i <= 6; i++)
		{
			if (counts[i] >= n) return true;
		}
		return false;
	}

	bool IsFullHouse(const DiceValues& d)
	{
		auto counts = CountByValue(d);
		bool has3 = false, has2 = false;
		for (int i = 1; i <= 6; i++)
		{
			if (counts[i] == 3) has3 = true;
			if (counts[i] == 2) has2 = true;
		}
		return has3 && has2;
	}

	// length個以上連続した目があるか
	bool IsStraight(const DiceValues& d, int length)
	{
		std::array<bool, 7> present{};
		for (int v : d) present[v] = true;

		int run = 0;
		for (int i = 1; i <= 6; i++)
		{
			if (present[i])
			{
				run++;
				if (run >= length) return true;
			}
			else
			{
				run = 0;
			}
		}
		return false;
	}

	bool IsRainbow(const DiceValues& d)
	{
		auto counts = CountByValue(d);
		for (int i = 1; i <= 6; i++)
		{
			if (counts[i] > 1) return false;
		}
		return true; // 5個とも別の目
	}
}

ScoreCategory MakeNumberCategory(enScoreCategoryType type, const wchar_t* name, int targetValue)
{
	ScoreCategory c;
	c.type = type;
	c.group = enCategoryGroup::Number;   
	c.sortOrder = targetValue;
	c.name = name;
	c.calcScore = [targetValue](const DiceValues& d) { return CountValue(d, targetValue) * targetValue; };
	return c;
}

ScoreCategory MakeOddSum()
{
	ScoreCategory c;
	c.type = enScoreCategoryType::OddSum;
	c.group = enCategoryGroup::SumRule;   
	c.sortOrder = 20;
	c.name = L"オッド・サム";
	c.calcScore = [](const DiceValues& d) { int s = SumAll(d); return (s % 2 != 0) ? s : 0; };
	return c;
}

ScoreCategory MakeEvenSum()
{
	ScoreCategory c;
	c.type = enScoreCategoryType::EvenSum;
	c.group = enCategoryGroup::SumRule;   
	c.sortOrder = 21;
	c.name = L"イーブン・サム";
	c.calcScore = [](const DiceValues& d) { int s = SumAll(d); return (s % 2 == 0) ? s : 0; };
	return c;
}

ScoreCategory MakeFiveMultiple()
{
	ScoreCategory c;
	c.type = enScoreCategoryType::FiveMultiple;
	c.group = enCategoryGroup::SumRule;  
	c.sortOrder = 23;
	c.name = L"マルチプル5";
	c.calcScore = [](const DiceValues& d) { int s = SumAll(d); return (s % 5 == 0) ? s + 5 : 0; };
	return c;
}

ScoreCategory MakeThreeMultiple()
{
	ScoreCategory c;
	c.type = enScoreCategoryType::ThreeMultiple;
	c.group = enCategoryGroup::SumRule;
	c.sortOrder = 22;
	c.name = L"マルチプル3";
	c.calcScore = [](const DiceValues& d) { int s = SumAll(d); return (s % 3 == 0) ? s + 3 : 0; };
	return c;
}

ScoreCategory MakeLow()
{
	ScoreCategory c;
	c.type = enScoreCategoryType::Low;
	c.group = enCategoryGroup::SumRule; 
	c.sortOrder = 24;
	c.name = L"ロー";
	c.calcScore = [](const DiceValues& d) { int s = SumAll(d); return (s <= 10) ? s * 3 : 0; };
	return c;
}

ScoreCategory MakeHigh()
{
	ScoreCategory c;
	c.type = enScoreCategoryType::High;
	c.group = enCategoryGroup::SumRule;   
	c.sortOrder = 25;
	c.name = L"ハイ";
	c.calcScore = [](const DiceValues& d) { int s = SumAll(d); return (s >= 25) ? s : 0; };
	return c;
}

ScoreCategory MakeRainbow()
{
	ScoreCategory c;
	c.type = enScoreCategoryType::Rainbow;
	c.group = enCategoryGroup::Shape; 
	c.sortOrder = 10;
	c.name = L"レインボー";
	c.calcScore = [](const DiceValues& d) { return IsRainbow(d) ? 20 : 0; };
	return c;
}

ScoreCategory MakeChance()
{
	ScoreCategory c;
	c.type = enScoreCategoryType::Chance;
	c.group = enCategoryGroup::Chance;
	c.sortOrder = 30;
	c.name = L"チャンス";
	c.calcScore = [](const DiceValues& d) { return SumAll(d); };
	return c;
}

ScoreCategory MakeFullHouse()
{
	ScoreCategory c;
	c.type = enScoreCategoryType::FullHouse;
	c.group = enCategoryGroup::Shape;   
	c.sortOrder = 11;
	c.name = L"フルハウス";
	c.calcScore = [](const DiceValues& d) { return IsFullHouse(d) ? 25 : 0; };
	return c;
}

ScoreCategory MakeThreeCard()
{
	ScoreCategory c;
	c.type = enScoreCategoryType::ThreeCard;
	c.group = enCategoryGroup::Shape; 
	c.sortOrder = 12;
	c.name = L"スリーカード";
	c.calcScore = [](const DiceValues& d) { return HasNOfAKind(d, 3) ? SumAll(d) : 0; };
	return c;
}

ScoreCategory MakeFourCard()
{
	ScoreCategory c;
	c.type = enScoreCategoryType::FourCard;
	c.group = enCategoryGroup::Shape;   
	c.sortOrder = 13;
	c.name = L"フォーカード";
	c.calcScore = [](const DiceValues& d) { return HasNOfAKind(d, 4) ? SumAll(d) : 0; };
	return c;
}

ScoreCategory MakeSStraight()
{
	ScoreCategory c;
	c.type = enScoreCategoryType::SStraight;
	c.group = enCategoryGroup::Shape;   
	c.sortOrder = 14;
	c.name = L"S.ストレート";
	c.calcScore = [](const DiceValues& d) { return IsStraight(d, 4) ? 30 : 0; };
	return c;
}

ScoreCategory MakeLStraight()
{
	ScoreCategory c;
	c.type = enScoreCategoryType::LStraight;
	c.group = enCategoryGroup::Shape;  
	c.sortOrder = 15;
	c.name = L"L.ストレート";
	c.calcScore = [](const DiceValues& d) { return IsStraight(d, 5) ? 40 : 0; };
	return c;
}

ScoreCategory MakeYacht()
{
	ScoreCategory c;
	c.type = enScoreCategoryType::Yacht;
	c.group = enCategoryGroup::Yacht;
	c.sortOrder = 40;
	c.name = L"ヨット";
	c.calcScore = [](const DiceValues& d) { return HasNOfAKind(d, 5) ? 60 : 0; };
	return c;
}

std::vector<ScoreCategory> BuildFullCategoryPool()
{
	std::vector<ScoreCategory> pool;
	pool.push_back(MakeNumberCategory(enScoreCategoryType::Ones, L"1の目", 1));
	pool.push_back(MakeNumberCategory(enScoreCategoryType::Twos, L"2の目", 2));
	pool.push_back(MakeNumberCategory(enScoreCategoryType::Threes, L"3の目", 3));
	pool.push_back(MakeNumberCategory(enScoreCategoryType::Fours, L"4の目", 4));
	pool.push_back(MakeNumberCategory(enScoreCategoryType::Fives, L"5の目", 5));
	pool.push_back(MakeNumberCategory(enScoreCategoryType::Sixes, L"6の目", 6));
	pool.push_back(MakeOddSum());
	pool.push_back(MakeEvenSum());
	pool.push_back(MakeFiveMultiple());
	pool.push_back(MakeThreeMultiple());
	pool.push_back(MakeLow());
	pool.push_back(MakeHigh());
	pool.push_back(MakeRainbow());
	pool.push_back(MakeChance());
	pool.push_back(MakeFullHouse());
	pool.push_back(MakeThreeCard());
	pool.push_back(MakeFourCard());
	pool.push_back(MakeSStraight());
	pool.push_back(MakeLStraight());
	return pool; // 19種類
}

std::vector<ScoreCategory> BuildMatchScoreboard()
{
	auto pool = BuildFullCategoryPool();

	// Fisher-Yatesシャッフル(ランダム13個を選ぶための処理、既存のまま)
	for (size_t i = pool.size() - 1; i > 0; i--)
	{
		size_t j = rand() % (i + 1);
		std::swap(pool[i], pool[j]);
	}

	std::vector<ScoreCategory> board(pool.begin(), pool.begin() + 13);
	board.push_back(MakeYacht());

	// グループ順に並べ替え(見やすさのため)
	std::sort(board.begin(), board.end(), [](const ScoreCategory& a, const ScoreCategory& b) {
		if (a.group != b.group) return (int)a.group < (int)b.group;
		return a.sortOrder < b.sortOrder;   // グループが同じなら sortOrder で比較
		});

	return board;
}