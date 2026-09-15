#include "stdafx.h"
#include "Explanation.h"
#include "Game.h"
#include "Title.h"

Explanation::Explanation()
{
	m_slideSprites[0].Init("Assets/sprite/explanation_1.dds", 1920.0f, 1080.0f);
	m_slideSprites[1].Init("Assets/sprite/explanation_2.dds", 1920.0f, 1080.0f);
	m_slideSprites[2].Init("Assets/sprite/explanation_3.dds", 1920.0f, 1080.0f);

	for (int i = 0; i < kNumSlides; i++)
	{
		m_slideSprites[i].SetPosition({ 0.0f, 0.0f, 0.0f });
	}
	m_prevSpaceState = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
	m_prevBackState = (GetAsyncKeyState(VK_BACK) & 0x8000) != 0;
}

Explanation::~Explanation()
{
}

void Explanation::Update()
{
	bool spaceState = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
	bool backState = (GetAsyncKeyState(VK_BACK) & 0x8000) != 0;

	if (spaceState && !m_prevSpaceState)
	{
		if (m_currentSlide < kNumSlides - 1)
		{
			m_currentSlide++;
		}
		else
		{
			// 最後のスライドでSPACE → ゲーム本編へ
			NewGO<Game>(0, "game");
			DeleteGO(this);
			return;
		}
	}

	if (backState && !m_prevBackState)
	{
		if (m_currentSlide > 0)
		{
			m_currentSlide--;
		}
		else
		{
			// 最初のスライドでBACKSPACE → タイトルへ
			NewGO<Title>(0, "title");
			DeleteGO(this);
			return;
		}
	}

	m_prevSpaceState = spaceState;
	m_prevBackState = backState;
}

void Explanation::Render(RenderContext& rc)
{
	m_slideSprites[m_currentSlide].Draw(rc);
}