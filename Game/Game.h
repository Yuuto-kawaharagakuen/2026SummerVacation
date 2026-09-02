#pragma once

#include "Level3DRender/LevelRender.h"
#include "DiceRound.h"
#include "DiceInputController.h"
#include"Dice.h"
#include "ScoreCategory.h" 
#include "ScoreBoardView.h" 
#include "ScoreSelectController.h"
class Player;

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
	DiceRound m_playerRound;
	DiceInputController m_inputController;
	Vector3 m_pos;
	Vector3 GetHeldSlotPosition(int diceIndex) const;
	Vector3 CalcWorldPosFromScreenAndHeight(float screenX, float screenY, float worldY) const;
	SpriteRender m_whiteRender;//白色のスコアボード
	MeshCollider m_trayCollider;
	RigidBody m_trayRigidBody;
	bool m_isDiceRolling = false;
	Vector3 m_heldSlotPositions[kDiceNum];
	std::vector<ScoreCategory> m_playerBoard;   // 13ランダム+ヨット固定の役一覧
	std::vector<bool> m_playerFilled;           // 埋まったかどうか
	std::vector<int> m_playerScores;            // 確定した得点(未確定は0でOK)
	ScoreBoardView m_scoreBoardView;            // 見た目の表示担当
	ScoreSelectController m_scoreSelectController;
};

