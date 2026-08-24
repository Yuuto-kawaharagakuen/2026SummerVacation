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
	ModelRender m_trayModelRender;
	DiceRound m_playerRound;
	DiceInputController m_inputController;
	Vector3 m_pos;

	MeshCollider m_trayCollider;
	RigidBody m_trayRigidBody;

	BoxCollider m_floorCollider;
	RigidBody m_floorRigidBody;
    Dice m_testDice;
	bool m_hasPrintedResult = false;
};

