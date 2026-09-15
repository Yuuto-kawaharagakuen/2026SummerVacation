#include "stdafx.h"
#include "Title.h"
//#include "Explanation.h"
#include "Game.h"
Title::Title()
{
	m_titleSprite.Init("Assets/sprite/title.dds", 1920.0f, 1080.0f); // 画像サイズは実際のdds解像度に合わせて調整
	m_titleSprite.SetPosition({ 0.0f, 0.0f, 0.0f });

}

Title::~Title()
{
}

void Title::Update()
{
	bool spaceState = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
	if (spaceState && !m_prevSpaceState)
	{
		/*NewGO<Explanation>(0, "explanation");*/
		NewGO<Game>(0, "game");
		DeleteGO(this);
		return;
	}
	m_prevSpaceState = spaceState;
}

void Title::Render(RenderContext& rc)
{
	m_titleSprite.Draw(rc);
}