#include "stdafx.h"
#include "Title.h"
#include "Explanation.h"
#include "Game.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
Title::Title()
{
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/click.wav");
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/bgm.wav");

	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(4);
	m_bgm->Play(true);        
	m_bgm->SetVolume(0.8f);

	m_titleSprite.Init("Assets/sprite/title.dds", 1920.0f, 1080.0f); 
	m_titleSprite.SetPosition({ 0.0f, 0.0f, 0.0f });

	m_prevSpaceState = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
}

Title::~Title()
{
	if (m_bgm != nullptr)
	{
		DeleteGO(m_bgm);
		m_bgm = nullptr;
	}
}

void Title::PlayClickSE()
{
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(1);
	se->Play(false);
}

void Title::Update()
{
	bool spaceState = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
	if (spaceState && !m_prevSpaceState)
	{
		PlayClickSE();
		NewGO<Explanation>(0, "explanation");
		DeleteGO(this);
		return;
	}
	m_prevSpaceState = spaceState;
}

void Title::Render(RenderContext& rc)
{
	m_titleSprite.Draw(rc);
}