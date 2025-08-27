#pragma once

class Yama : public KdGameObject
{
public:
	Yama() { Init(); }
	~Yama() = default;

	void DrawLit()override;
	void Update()override;

	void OnHit()override;

	void SetPos(const Math::Vector3& _pos) override { m_pos = _pos; }

	void SetTarget(const std::shared_ptr<KdGameObject> _spTarget) { m_wpTarget = _spTarget; }

private:
	void Init()override;

	std::weak_ptr<KdGameObject> m_wpTarget;

	std::shared_ptr<KdModelData> m_spModel;
	Math::Vector3 m_pos;

	bool m_isJump;
	float m_jumpPow;
	float m_jump;
};