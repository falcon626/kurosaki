#pragma once

class Bullet : public KdGameObject
{
public:
	Bullet() = default;
	~Bullet() = default;

	void Init()override;
	void DrawLit()override;
	void DrawBright()override;
	void Update()override;

	void SetParam(const Math::Vector3& pos, const Math::Vector3& dir, const float speed, const int life);

private:
	std::shared_ptr<KdModelData> m_spModel;
	Math::Vector3 m_pos;
	Math::Vector3 m_dir;

	float m_speed = 0.5f;

	int m_lifeSpan = 0; // 寿命
};