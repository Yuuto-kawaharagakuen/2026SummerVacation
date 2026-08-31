#include "stdafx.h"
#include "Dice.h"

namespace
{
	const float kDiceHalfSize = 15.0f*0.5f; 
}

void Dice::Init(const Vector3& startPos)
{
	m_modelRender.Init("Assets/modelData/Dice.tkm"); 
	m_collider.Create(Vector3(kDiceHalfSize * 6.0f, kDiceHalfSize * 6.0f, kDiceHalfSize * 6.0f));


	RigidBodyInitData rbInfo;
	rbInfo.pos = startPos;
	rbInfo.rot = Quaternion::Identity;
	rbInfo.collider = &m_collider;
	rbInfo.mass = 1.0f;
	rbInfo.restitution = 0.6f;
	m_rigidBody.Init(rbInfo);

	m_rigidBody.SetFriction(0.6f);
	m_rigidBody.SetCcd(kDiceHalfSize, kDiceHalfSize * 0.9f);
}

void Dice::Roll()
{
	// 現在のXZ位置は維持しつつ、Y=40の高さに戻してから落とす
	Vector3 dropPos = m_position;
	dropPos.y = 200.0f;
	// 落とす瞬間の初期姿勢をランダムにする(常にY軸基準にならないように)
	Quaternion rotX, rotY, rotZ;
	rotX.SetRotationDeg(Vector3::AxisX, (float)(rand() % 360));
	rotY.SetRotationDeg(Vector3::AxisY, (float)(rand() % 360));
	rotZ.SetRotationDeg(Vector3::AxisZ, (float)(rand() % 360));
	Quaternion startRot = rotX * rotY * rotZ; 


	m_rigidBody.SetPositionAndRotation(dropPos, startRot);  
	m_rigidBody.SetLinearVelocity(Vector3::Zero);

	Vector3 angularVel(
		(float)(rand() % 1000 - 500) * 0.02f,
		(float)(rand() % 1000 - 500) * 0.02f,
		(float)(rand() % 1000 - 500) * 0.02f
	);
	m_rigidBody.SetAngularVelocity(angularVel);
	m_settleCheckDelay = 20;
}

void Dice::Update()
{
	m_rigidBody.GetPositionAndRotation(m_position, m_rotation);
	if (m_settleCheckDelay > 0) m_settleCheckDelay--;
	 m_modelRender.SetPosition(m_position);
	 m_modelRender.SetRotation(m_rotation);
	 m_modelRender.SetScale(Vector3(0.5f, 0.5f, 0.5f)); 
	 m_modelRender.Update();
}
Vector3 RotateVectorByQuaternion(const Quaternion& q, const Vector3& v)
{
	// クォータニオンでベクトルを回転(q * v * qの共役)
	float qx = q.x, qy = q.y, qz = q.z, qw = q.w;
	float vx = v.x, vy = v.y, vz = v.z;

	// q * v (vをクォータニオン(vx,vy,vz,0)とみなして掛ける)
	float ix = qw * vx + qy * vz - qz * vy;
	float iy = qw * vy + qz * vx - qx * vz;
	float iz = qw * vz + qx * vy - qy * vx;
	float iw = -qx * vx - qy * vy - qz * vz;

	// 上の結果 * qの共役(-qx, -qy, -qz, qw)
	Vector3 result;
	result.x = ix * qw + iw * (-qx) + iy * (-qz) - iz * (-qy);
	result.y = iy * qw + iw * (-qy) + iz * (-qx) - ix * (-qz);
	result.z = iz * qw + iw * (-qz) + ix * (-qy) - iy * (-qx);
	return result;
}

void Dice::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

bool Dice::IsSettled() const
{

	if (m_settleCheckDelay > 0) return false; 
	Vector3 vel = m_rigidBody.GetLinearVelocity();
	Vector3 angVel = m_rigidBody.GetAngularVelocity();
	return vel.Length() < 1.0f && angVel.Length() < 1.0f;
}

int Dice::GetFaceValue() const
{
	struct FaceDir { Vector3 localDir; int value; };
	static const FaceDir faces[6] = {
		{ Vector3(1,  0, 0), 3 },
		{ Vector3(-1, 0, 0), 4 },
		{ Vector3(0,  1, 0), 1 },
		{ Vector3(0, -1, 0), 6 },
		{ Vector3(0,  0, 1), 2 },
		{ Vector3(0,  0,-1), 5 },
	};

	int bestValue = 1;
	float bestDot = -999.0f;
	for (auto& f : faces)
	{
		Vector3 worldDir = RotateVectorByQuaternion(m_rotation, f.localDir);
		if (worldDir.y > bestDot)
		{
			bestDot = worldDir.y;
			bestValue = f.value;
		}
	}
	return bestValue;
}