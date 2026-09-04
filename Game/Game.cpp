#include "stdafx.h"
#include "Game.h"
#include "ScoreCategory.h"
#include "DiceRound.h"
#include "CpuPlayer.h"
#include "Dice.h"
#include "ScoreSelectController.h"
namespace
{
	void PrintDice(const wchar_t* label, const DiceValues& dice)
	{
		wchar_t buf[128];
		swprintf_s(buf, L"%ls : [%d,%d,%d,%d,%d]\n", label, dice[0], dice[1], dice[2], dice[3], dice[4]);
		OutputDebugStringW(buf);
	}
	DiceValues RollRandomDice(const DiceValues& current, const std::array<bool, 5>& keepMask)
	{
		DiceValues result = current;
		for (int i = 0; i < 5; i++)
		{
			if (!keepMask[i])
			{
				result[i] = (rand() % 6) + 1; // 1〜6のランダム値
			}
		}
		return result;
	}
}

void Game::StartCpuTurn()
{
	m_cpuPhase = enCpuPhase::ThinkingFirstRoll;
	m_cpuWaitFrames = kCpuThinkFrames;
}

void Game::UpdateCpuTurn()
{
	if (m_isDiceRolling) return; // 物理演算中は共通ロジックに任せる

	if (m_cpuPhase == enCpuPhase::ThinkingFirstRoll || m_cpuPhase == enCpuPhase::ThinkingReroll)
	{
		m_cpuWaitFrames--;
		if (m_cpuWaitFrames <= 0)
		{
			if (m_cpuPhase == enCpuPhase::ThinkingFirstRoll)
			{
				m_round.StartNewRound(); // 内部でRolling状態になる
			}
			else
			{
				m_round.RollDice();
			}
			m_cpuPhase = enCpuPhase::Idle;
			// 実際の物理ロールは、Update()内の既存のRolling検知ブロックが自動でやってくれる
		}
		return;
	}

	if (m_cpuPhase == enCpuPhase::Idle && m_round.GetRollState() == enRollState::Settled)
	{
		CpuDecideAndAct();
	}
}

void Game::CpuDecideAndAct()
{
	if (m_round.GetRollsRemaining() > 0)
	{
		// キープ判断
		auto mask = m_cpuPlayer.DecideKeepMask(m_round.GetDice(), m_board, m_cpuFilled);
		auto currentMask = m_round.GetKeepMask();
		for (int i = 0; i < kDiceNum; i++)
		{
			if (mask[i] != currentMask[i])
			{
				m_round.ToggleKeep(i);
				if (mask[i])
				{
					m_dices[i].SnapToHeldSlot(GetHeldSlotPosition(i));
				}
				else
				{
					m_dices[i].ReturnToPhysics();
				}
			}
		}

		wchar_t keepBuf[64];
		swprintf_s(keepBuf, L"CPUキープ判断 : [%d,%d,%d,%d,%d]\n",
			mask[0], mask[1], mask[2], mask[3], mask[4]);
		OutputDebugStringW(keepBuf);

		// 少し待ってから振る
		m_cpuPhase = enCpuPhase::ThinkingReroll;
		m_cpuWaitFrames = kCpuThinkFrames;
	}
	else
	{
		// 役を決定
		int categoryIndex = m_cpuPlayer.DecideCategoryToFill(m_round.GetDice(), m_board, m_cpuFilled);
		if (categoryIndex >= 0)
		{
			int score = m_board[categoryIndex].calcScore(m_round.GetDice());
			m_cpuScores[categoryIndex] = score;
			m_cpuFilled[categoryIndex] = true;

			wchar_t buf[128];
			swprintf_s(buf, L"CPU役確定: %ls / 得点: %d\n", m_board[categoryIndex].name.c_str(), score);
			OutputDebugStringW(buf);
		}

		// キープ中のダイスを全部物理に戻す
		const auto& keepMask = m_round.GetKeepMask();
		for (int i = 0; i < kDiceNum; i++)
		{
			if (keepMask[i]) m_dices[i].ReturnToPhysics();
		}

		SwitchTurn(); // プレイヤーのターンへ
	}
}
void Game::SwitchTurn()
{
	if (m_currentTurn == enTurnOwner::Player)
	{
		m_currentTurn = enTurnOwner::Cpu;
		OutputDebugStringW(L"---- CPUのターン ----\n");
		//m_turnText.SetText(L"CPUのターン"); 
		StartCpuTurn();
	}
	else
	{
		m_currentTurn = enTurnOwner::Player;
		OutputDebugStringW(L"---- プレイヤーのターン ----\n");
		//m_turnText.SetText(L"あなたのターン"); 
		m_round.StartNewRound();
		for (int i = 0; i < kDiceNum; i++)
		{
			m_dices[i].Roll();
		}
		m_isDiceRolling = true;
	}
}


bool Game::Start()
{
	srand((unsigned int)time(nullptr));

	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	PhysicsWorld::GetInstance()->SetGravity(Vector3(0.0f, -300.0f, 0.0f));

	m_whiteRender.Init("Assets/SIRO.DDS", 500.0f, 700.0f);
	m_whiteRender.SetPosition({ -650.0f, 125.0f, 0.0f });
	m_whiteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.5f)); // 最後の0.5fがアルファ(不透明度)、0が完全透明、1が不透明

	m_trayModelRender.Init("Assets/modelData/Tray.tkm");
	m_trayModelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));

	m_turnText.SetPosition({ -100.0f, 520.0f, 0.0f }); // 画面上部、位置はお好みで調整
	m_turnText.SetScale(1.5f);
	m_turnText.SetColor(g_vec4Black);
	m_turnText.SetText(L"あなたのターン");

	m_BackGround.Init("Assets/modelData/BackGround.tkm");
	m_BackGround.SetPosition(Vector3(0.0f, -200.0f, 0.0f));
	m_BackGround.SetScale(Vector3(10.0f, 10.0f, 10.0f));
	Matrix upAxisFix;
	upAxisFix.MakeRotationX(Math::DegToRad(-90.0f));
	m_trayCollider.CreateFromModel(m_trayModelRender.GetModel(), upAxisFix);

	RigidBodyInitData trayInfo;
	trayInfo.pos = Vector3(0.0f, 10.0f, 0.0f);
	trayInfo.rot = Quaternion::Identity;
	trayInfo.collider = &m_trayCollider;
	trayInfo.mass = 0.0f;
	m_trayRigidBody.Init(trayInfo);

	g_camera3D->SetPosition(Vector3(-70.0f, 500.0f, 0.0f));
	g_camera3D->SetTarget(Vector3(-70.0f, 0.0f, 0.0f));
	g_camera3D->SetUp(Vector3(0.0f, 0.0f, 1.0f));

	m_heldSlotPositions[0] = Vector3(220.0f, 150.0f, 150.0f);
	m_heldSlotPositions[1] = Vector3(220.0f, 150.0f, 75.0f);
	m_heldSlotPositions[2] = Vector3(220.0f, 150.0f, 0.0f);
	m_heldSlotPositions[3] = Vector3(220.0f, 150.0f, -75.0f);
	m_heldSlotPositions[4] = Vector3(220.0f, 150.0f, -150.0f);

	m_round.StartNewRound(); 
	m_board = BuildMatchScoreboard(); 
	m_playerFilled.assign(m_board.size(), false);
	m_playerScores.assign(m_board.size(), 0);
	m_cpuFilled.assign(m_board.size(), false);   
	m_cpuScores.assign(m_board.size(), 0);       
	m_scoreBoardView.Init(m_board);
	OutputDebugStringW(L"---- プレイヤーのターン開始 ----\n");
	{
		const auto& dice = m_round.GetDice();
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

	skyCube = NewGO<SkyCube>(0);
	skyCube->SetType(enSkyCubeType_Grass);

	g_renderingEngine->SetAmbientByIBLTexture(skyCube->GetTextureFilePath(), 2.0f);

	m_dices[0].SetKinematic(true);
	m_dices[0].DebugSetRotationForCalibration(Quaternion::Identity);
	return true;
}

void Game::Update()
{
	m_whiteRender.Update();

	// Game.cpp の Update() 内、先頭に一時的に追加
	static int debugRotIndex = 0;
	static bool prevNState = false;
	bool nState = (GetAsyncKeyState('N') & 0x8000) != 0;
	if (nState && !prevNState)
	{
		debugRotIndex = (debugRotIndex + 1) % 6;

		Quaternion debugRot;
		switch (debugRotIndex)
		{
		case 0: debugRot = Quaternion::Identity; break;
		case 1: debugRot.SetRotationDeg(Vector3::AxisX, 180.0f); break;
		case 2: debugRot.SetRotationDeg(Vector3::AxisZ, 90.0f); break;
		case 3: debugRot.SetRotationDeg(Vector3::AxisZ, -90.0f); break;
		case 4: debugRot.SetRotationDeg(Vector3::AxisX, -90.0f); break;
		case 5: debugRot.SetRotationDeg(Vector3::AxisX, 90.0f); break;
		}
		m_dices[0].DebugSetRotationForCalibration(debugRot);

		wchar_t debugBuf[64];
		swprintf_s(debugBuf, L"===検証パターン: %d===\n", debugRotIndex);
		OutputDebugStringW(debugBuf);
		m_dices[0].GetFaceValue(); // ★これで6方向のy値が全部ログに出る
	}
	prevNState = nState;






	// キーボード入力(1〜5キー)でToggleKeepが呼ばれる。呼び出し前後でkeepFMaskを比較する。
	auto prevKeepMask = m_round.GetKeepMask();

	m_inputController.Update(m_round);

	auto currentKeepMask = m_round.GetKeepMask();
	for (int i = 0; i < kDiceNum; i++)
	{
		if (currentKeepMask[i] && !prevKeepMask[i])
		{
			Vector3 slotPos = GetHeldSlotPosition(i);
			m_dices[i].SnapToHeldSlot(slotPos);
		}
		else if (!currentKeepMask[i] && prevKeepMask[i])
		{
			m_dices[i].ReturnToPhysics();
		}
	}

	m_trayModelRender.Update();

	// プレイヤーが「振る」操作をして Rolling 状態になったら、物理ダイスを実際に転がす
	if (m_round.GetRollState() == enRollState::Rolling && !m_isDiceRolling)
	{
		m_isDiceRolling = true;
		const auto& keepMask = m_round.GetKeepMask();
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
			m_round.CompleteRoll(results);
			m_isDiceRolling = false;

			for (int i = 0; i < kDiceNum; i++)
			{
				m_dices[i].SetKinematic(true);
			}

			// 確認用: DiceRoundに反映された出目をログ出力
			const auto& finalDice = m_round.GetDice();
			wchar_t buf[128];
			swprintf_s(buf, L"確定した出目:[%d,%d,%d,%d,%d]\n",
				finalDice[0], finalDice[1], finalDice[2], finalDice[3], finalDice[4]);
			OutputDebugStringW(buf);
		}
	}
	if (!m_isDiceRolling && m_round.GetRollState() == enRollState::Settled && m_currentTurn == enTurnOwner::Player) 
	{
		int confirmedIndex = -1;
		bool confirmed = m_scoreSelectController.Update(m_board, m_playerFilled, confirmedIndex); 
		if (confirmed)
		{
			int score = m_board[confirmedIndex].calcScore(m_round.GetDice());
			m_playerScores[confirmedIndex] = score;
			m_playerFilled[confirmedIndex] = true;

			wchar_t buf[128];
			swprintf_s(buf, L"役確定: %ls / 得点: %d\n", m_board[confirmedIndex].name.c_str(), score);
			OutputDebugStringW(buf);

			const auto& keepMaskBeforeReset = m_round.GetKeepMask();
			for (int i = 0; i < kDiceNum; i++)
			{
				if (keepMaskBeforeReset[i])
				{
					m_dices[i].ReturnToPhysics();
				}
			}

			SwitchTurn(); 
		}
	}
	if (m_currentTurn == enTurnOwner::Cpu)
	{
		UpdateCpuTurn();
	}
	// ターン表示の更新(何投目かも含める)
	{
		int currentRollNumber;
		if (m_currentTurn == enTurnOwner::Cpu && m_cpuPhase == enCpuPhase::ThinkingFirstRoll)
		{
			currentRollNumber = 1; // CPUの最初のロール前は、まだ古いm_roundの状態を見ない
		}
		else
		{
			currentRollNumber = DiceRound::kMaxRolls - m_round.GetRollsRemaining();
			if (currentRollNumber > DiceRound::kMaxRolls) currentRollNumber = DiceRound::kMaxRolls; // 念のためクランプ
			if (currentRollNumber < 1) currentRollNumber = 1;
		}

		wchar_t turnBuf[32];
		if (m_currentTurn == enTurnOwner::Player)
		{
			swprintf_s(turnBuf, L"あなたのターン %d/%d", currentRollNumber, DiceRound::kMaxRolls);
		}
		else
		{
			swprintf_s(turnBuf, L"CPUのターン %d/%d", currentRollNumber, DiceRound::kMaxRolls);
		}
		m_turnText.SetText(turnBuf);
	}
	m_scoreBoardView.Update(m_board,
		m_playerFilled, m_playerScores, m_round.GetDice(),
		m_cpuFilled, m_cpuScores,
		m_scoreSelectController.GetSelectedIndex(), m_currentTurn == enTurnOwner::Player);
}

Vector3 Game::GetHeldSlotPosition(int diceIndex) const
{
	return m_heldSlotPositions[diceIndex];
}

void Game::Render(RenderContext& rc)
{
	m_whiteRender.Draw(rc);
	m_scoreBoardView.Render(rc);
	m_trayModelRender.Draw(rc);
	m_BackGround.Draw(rc);
	m_turnText.Draw(rc);
	for (int i = 0; i < kDiceNum; i++)
	{
		m_dices[i].Render(rc);
	}
}