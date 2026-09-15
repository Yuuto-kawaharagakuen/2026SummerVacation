#pragma once

class Explanation : public IGameObject
{
public:
	Explanation();
	~Explanation();
	void Update() override;
	void Render(RenderContext& rc) override;

private:
	static const int kNumSlides = 3;
	SpriteRender m_slideSprites[kNumSlides];
	int m_currentSlide = 0;
	bool m_prevSpaceState = false;
	bool m_prevBackState = false;
};