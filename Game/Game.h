#pragma once

#include "Level3DRender/LevelRender.h"
#include "DiceRound.h"
#include "DiceInputController.h"
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
	ModelRender m_modelRender;
	DiceRound m_playerRound;
	DiceInputController m_inputController;
	Vector3 m_pos;
};

