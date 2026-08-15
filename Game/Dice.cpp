#include "stdafx.h"
#include "Dice.h"

namespace
{
	const float kDiceHalfSize = 15.0f; // ダイスの大きさ(モデル完成後に実寸に合わせて調整)
}

void Dice::Init(const Vector3& startPos)
{
	// モデルはまだ無いので、パスは仮。完成したらここを差し替えるだけでOK。
	// m_modelRender.Init("Assets/modelData/dice.tkm");

	m_collider.Create(Vector3(kDiceHalfSize, kDiceHalfSize, kDiceHalfSize));

	RigidBodyInitData rbInfo;
	rbInfo.pos = startPos;
	rbInfo.rot = Quaternion::Identity;
	rbInfo.collider = &m_collider;
	rbInfo.mass = 1.0f;          // 動く剛体にする
	rbInfo.restitution = 0.3f;   // 少し跳ねる
	m_rigidBody.Init(rbInfo);

	m_rigidBody.SetFriction(0.6f);
}

void Dice::Roll()
{
	Vector3 vel(
		(float)(rand() % 400 - 200),
		400.0f,
		(float)(rand() % 400 - 200)
	);
	m_rigidBody.SetLinearVelocity(vel);

	Vector3 angularVel(
		(float)(rand() % 1000 - 500) * 0.01f,
		(float)(rand() % 1000 - 500) * 0.01f,
		(float)(rand() % 1000 - 500) * 0.01f
	);
	m_rigidBody.SetAngularVelocity(angularVel);
}

void Dice::Update()
{
	m_rigidBody.GetPositionAndRotation(m_position, m_rotation);
	// モデルができたら以下を有効化
	// m_modelRender.SetPosition(m_position);
	// m_modelRender.SetRotation(m_rotation);
	// m_modelRender.Update();
}

void Dice::Render(RenderContext& rc)
{
	// m_modelRender.Draw(rc);
}

bool Dice::IsSettled() const
{
	Vector3 vel = m_rigidBody.GetLinearVelocity();
	return vel.Length() < 1.0f;
}

int Dice::GetFaceValue() const
{
	// ダイスのローカル座標系で、どの軸が何の目に対応するかを定義
	// (1-6, 2-5, 3-4が対になるのが一般的。モデル完成後に実際の面と合わせて調整)
	struct FaceDir { Vector3 localDir; int value; };
	static const FaceDir faces[6] = {
		{ Vector3(0,  1, 0), 6 },
		{ Vector3(0, -1, 0), 1 },
		{ Vector3(1,  0, 0), 5 },
		{ Vector3(-1, 0, 0), 2 },
		{ Vector3(0,  0, 1), 4 },
		{ Vector3(0,  0,-1), 3 },
	};

	int bestValue = 1;
	float bestDot = -999.0f;
	for (auto& f : faces)
	{
		Vector3 worldDir; 
		DirectX::XMStoreFloat3(&worldDir.vec, m_rotation* f.localDir);
		if (worldDir.y > bestDot)
		{
			bestDot = worldDir.y;
			bestValue = f.value;
		}
	}
	return bestValue;
}