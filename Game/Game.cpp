#include "stdafx.h"
#include "Game.h"
#include "ScoreCategory.h"
#include "DiceRound.h"
#include "CpuPlayer.h"
#include "Dice.h"
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

	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

    PhysicsWorld::GetInstance()->SetGravity(Vector3(0.0f, -300.0f, 0.0f));

	m_trayModelRender.Init("Assets/modelData/Tray.tkm");
	m_trayModelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	
	Matrix upAxisFix;
	upAxisFix.MakeRotationX(Math::DegToRad(-90.0f));
	m_trayCollider.CreateFromModel(m_trayModelRender.GetModel(), upAxisFix);

	RigidBodyInitData trayInfo;
	trayInfo.pos = Vector3(0.0f, 10.0f, 0.0f);
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

	for (int i = 0; i < kDiceNum; i++)
	{
		// 重ならないように、少しずつXZをずらして配置
		float offsetX = (i - 2) * 45.0f; 
		float offsetZ = ((i % 2 == 0) ? 1.0f : -1.0f) * 15.0f;
		m_dices[i].Init(Vector3(offsetX, 100.0f, 0.0f));
		m_dices[i].Roll();
	}
	return true;
}

void Game::Update()
{
	m_inputController.Update(m_playerRound);
	m_trayModelRender.Update();

	// プレイヤーが「振る」操作をして Rolling 状態になったら、物理ダイスを実際に転がす
	if (m_playerRound.GetRollState() == enRollState::Rolling && !m_isDiceRolling)
	{
		m_isDiceRolling = true;
		const auto& keepMask = m_playerRound.GetKeepMask();
		for (int i = 0; i < kDiceNum; i++)
		{
			if (!keepMask[i]) m_dices[i].Roll(); // キープしてないダイスだけ振る
		}
	}

	for (int i = 0; i < kDiceNum; i++)
	{
		m_dices[i].Update();
	}

	// 振ってる最中なら、5個全部静止するまで待つ
	if (m_isDiceRolling)
	{
		bool allSettled = true;
		for (int i = 0; i < kDiceNum; i++)
		{
			if (!m_dices[i].IsSettled()) { allSettled = false; break; }
		}
		if (allSettled)
		{
			DiceValues results;
			for (int i = 0; i < kDiceNum; i++)
			{
				results[i] = m_dices[i].GetFaceValue();

			}
			m_playerRound.CompleteRoll(results);
			m_isDiceRolling = false;

			// 確認用: DiceRoundに反映された出目をログ出力
			const auto& finalDice = m_playerRound.GetDice();
			wchar_t buf[128];
			swprintf_s(buf, L"確定した出目:[%d,%d,%d,%d,%d]\n",
				finalDice[0], finalDice[1], finalDice[2], finalDice[3], finalDice[4]);
			OutputDebugStringW(buf);
		}
	}
}

void Game::Render(RenderContext& rc)
{
	m_trayModelRender.Draw(rc);
	for (int i = 0; i < kDiceNum; i++)
	{
		m_dices[i].Render(rc);
	}
}