#pragma once

#include "Level3DRender/LevelRender.h"
#include "DiceRound.h"
#include "DiceInputController.h"
#include"Dice.h"
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

	MeshCollider m_trayCollider;
	RigidBody m_trayRigidBody;
};

