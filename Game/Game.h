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

	enTurnOwner m_currentTurn = enTurnOwner::Player; 
	CpuPlayer m_cpuPlayer;                            
	void RunCpuTurnInstant();                          
	void SwitchTurn();                                  

	SkyCube* skyCube = nullptr;
};