#include "stdafx.h"
#include "Game.h"
#include "ScoreCategory.h"
#include "DiceRound.h"
#include "CpuPlayer.h"

namespace
{
	void PrintDice(const wchar_t* label, const DiceValues& dice)
	{
		wchar_t buf[128];
		swprintf_s(buf, L"%ls : [%d,%d,%d,%d,%d]\n", label, dice[0], dice[1], dice[2], dice[3], dice[4]);
		OutputDebugStringW(buf);
	}

	void RunCpuTurn()
	{
		OutputDebugStringW(L"==== CPUのターン開始 ====\n");

		auto board = BuildMatchScoreboard();
		std::vector<bool> filled(board.size(), false);

		CpuPlayer cpu;
		DiceRound round;
		round.StartNewRound();
		PrintDice(L"1投目", round.GetDice());

		while (round.GetRollsRemaining() > 0)
		{
			auto mask = cpu.DecideKeepMask(round.GetDice(), board, filled);

			wchar_t keepBuf[64];
			swprintf_s(keepBuf, L"キープ判断 : [%d,%d,%d,%d,%d]\n",
				mask[0], mask[1], mask[2], mask[3], mask[4]);
			OutputDebugStringW(keepBuf);

			for (int i = 0; i < 5; i++)
			{
				if (mask[i] != round.GetKeepMask()[i]) round.ToggleKeep(i);
			}
			round.RollDice();

			wchar_t rollLabel[32];
			swprintf_s(rollLabel, L"振り直し後(残り%d回)", round.GetRollsRemaining());
			PrintDice(rollLabel, round.GetDice());
		}

		int categoryIndex = cpu.DecideCategoryToFill(round.GetDice(), board, filled);
		if (categoryIndex >= 0)
		{
			int score = board[categoryIndex].calcScore(round.GetDice());
			wchar_t resultBuf[128];
			swprintf_s(resultBuf, L"選んだ役 : %ls / 得点 : %d\n",
				board[categoryIndex].name.c_str(), score);
			OutputDebugStringW(resultBuf);
		}

		OutputDebugStringW(L"==== CPUのターン終了 ====\n\n");
	}
}

bool Game::Start()
{
	srand((unsigned int)time(nullptr));

	m_trayModelRender.Init("Assets/modelData/Tray.tkm");
	m_trayModelRender.SetPosition(Vector3(0.0f, -20.0f, 0.0f));
	
	m_trayCollider.CreateFromModel(m_trayModelRender.GetModel(),Matrix::Identity); //モデルの座標系に合わせて作成

	RigidBodyInitData trayInfo;
	trayInfo.pos = Vector3(0.0f, 0.0f, 0.0f);
	trayInfo.rot = Quaternion::Identity;
	trayInfo.collider = &m_trayCollider;
	trayInfo.mass = 0.0f;
	m_trayRigidBody.Init(trayInfo);

	g_camera3D->SetPosition(Vector3(-100.0f, 350.0f, 0.0f));
	//g_camera3D->SetTarget(Vector3(0.0f, 0.0f, 0.0f));

	m_playerRound.StartNewRound();
	OutputDebugStringW(L"---- プレイヤーのターン開始 ----\n");
	{
		const auto& dice = m_playerRound.GetDice();
		wchar_t buf[128];
		swprintf_s(buf, L"1投目:[%d,%d,%d,%d,%d]\n", dice[0], dice[1], dice[2], dice[3], dice[4]);
		OutputDebugStringW(buf);
	}

	// ---- ダイス物理検証用 ----
	m_floorCollider.Create(Vector3(500.0f, 10.0f, 500.0f));
	RigidBodyInitData floorInfo;
	floorInfo.pos = Vector3(0.0f, 0.0f, 0.0f);
	floorInfo.rot = Quaternion::Identity;
	floorInfo.collider = &m_floorCollider;
	floorInfo.mass = 0.0f;
	m_floorRigidBody.Init(floorInfo);

	m_testDice.Init(Vector3(0.0f, 100.0f, 0.0f));
	m_testDice.Roll();
	OutputDebugStringW(L"---- テストダイス投擲 ----\n");

	return true;
}

void Game::Update()
{
	m_inputController.Update(m_playerRound);
	m_trayModelRender.Update();

	// ---- ダイス物理検証用 ----
	m_testDice.Update();

	static int frameCount = 0;
	frameCount++;
	if (frameCount % 30 == 0) // 約0.5秒ごと(60fps想定)
	{
		Vector3 pos = m_testDice.GetPosition();
		Vector3 vel = m_testDice.GetVelocity();
		wchar_t buf[128];
		swprintf_s(buf, L"[診断] 座標:(%.1f, %.1f, %.1f) 速度:(%.1f, %.1f, %.1f)\n",
			pos.x, pos.y, pos.z, vel.x, vel.y, vel.z);
		OutputDebugStringW(buf);
	}

	if (!m_hasPrintedResult && m_testDice.IsSettled())
	{
		wchar_t buf[64];
		swprintf_s(buf, L"テストダイス出目: %d\n", m_testDice.GetFaceValue());
		OutputDebugStringW(buf);
		m_hasPrintedResult = true;
	}
}

void Game::Render(RenderContext& rc)
{
	m_trayModelRender.Draw(rc);
}