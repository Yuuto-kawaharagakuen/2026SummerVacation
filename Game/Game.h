#pragma once

#include "Level3DRender/LevelRender.h"
#include "DiceRound.h"
#include "DiceInputController.h"
#include "Dice.h"
#include "ScoreCategory.h" 
#include "ScoreBoardView.h" 
#include "ScoreSelectController.h"
#include "CpuPlayer.h"
class Player;

enum class enTurnOwner
{
	Player,
	Cpu,
};

enum class enCpuPhase
{
	Idle,
	ThinkingFirstRoll,  // ターン開始後、最初のロール前の「考え中」待機
	ThinkingReroll,      // キープ決定後、再ロール前の「考え中」待機
};

class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:

	static const int kDiceNum = 5;
	Dice m_dices[kDiceNum];
	bool m_hasPrintedResult[kDiceNum] = {};

	ModelRender m_trayModelRender;
	ModelRender m_BackGround;
	FontRender m_turnText;
	DiceRound m_round;                          
	DiceInputController m_inputController;
	Vector3 m_pos;
	Vector3 GetHeldSlotPosition(int diceIndex) const;
	Vector3 CalcWorldPosFromScreenAndHeight(float screenX, float screenY, float worldY) const;
	SpriteRender m_whiteRender;
	MeshCollider m_trayCollider;
	RigidBody m_trayRigidBody;
	bool m_isDiceRolling = false;
	Vector3 m_heldSlotPositions[kDiceNum];

	std::vector<ScoreCategory> m_board;        
	std::vector<bool> m_playerFilled;
	std::vector<int> m_playerScores;
	std::vector<bool> m_cpuFilled;               
	std::vector<int> m_cpuScores;                

	ScoreBoardView m_scoreBoardView;
	ScoreSelectController m_scoreSelectController;

	SkyCube* skyCube = nullptr;

	enTurnOwner m_currentTurn = enTurnOwner::Player;
	CpuPlayer m_cpuPlayer;
	void SwitchTurn();
	enCpuPhase m_cpuPhase = enCpuPhase::Idle;
	int m_cpuWaitFrames = 0;
	static const int kCpuThinkFrames = 60; //(60fps想定)
	void StartCpuTurn();
	void UpdateCpuTurn();
	void CpuDecideAndAct();
};