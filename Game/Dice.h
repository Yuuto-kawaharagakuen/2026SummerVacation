#pragma once

class Dice
{
public:
	void Init(const Vector3& startPos);
	void Update();
	void Render(RenderContext& rc);

	void Roll();                 // ランダムな力・回転を加えて振る
	bool IsSettled() const;      // ほぼ静止したか
	int GetFaceValue() const;    // 静止時の出目(1~6)を取得

private:
	ModelRender m_modelRender;
	BoxCollider m_collider;
	RigidBody m_rigidBody;
	Vector3 m_position;
	Quaternion m_rotation;
};