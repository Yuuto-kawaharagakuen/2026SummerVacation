#pragma once

class Title : public IGameObject
{
public:
	Title();
	~Title();
	void Update() override;
	void Render(RenderContext& rc) override;

private:
	SpriteRender m_titleSprite;
	bool m_prevSpaceState = false;
};

