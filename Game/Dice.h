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
	void SetKinematic(bool isKinematic);
	void SnapToHeldSlot(const Vector3& slotPos); // キープ状態になった時に呼ぶ
	void ReturnToPhysics();                      // キープ解除時に呼ぶ
	const Vector3& GetPosition() const { return m_position; }
	Vector3 GetVelocity() const { return m_rigidBody.GetLinearVelocity(); }
	Vector3 GetAngularVelocity() const { return m_rigidBody.GetAngularVelocity(); }
	void StopCompletely();

private:
	bool m_isHeld = false;        
	Quaternion m_heldDisplayRot;
	ModelRender m_modelRender;
	BoxCollider m_collider;
	RigidBody m_rigidBody;
	Vector3 m_position;
	Quaternion m_rotation;
	int m_settleCheckDelay = 0;
	Vector3 m_returnPos;
	Quaternion m_returnRot;
};