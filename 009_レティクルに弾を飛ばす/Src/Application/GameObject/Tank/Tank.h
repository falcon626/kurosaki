#pragma once

class TPSCamera;

class Tank : public KdGameObject
{
public:
	Tank() = default;
	~Tank() = default;

	void Init()override;
	void GenerateDepthMapFromLight()override;
	void DrawLit()override;

	void Update()override;
	void PostUpdate()override;

	void SetCameraWeakPtr(const std::shared_ptr<TPSCamera>& spCamera) { m_wpCamera = spCamera; }
private:
	std::shared_ptr<KdModelData> m_spModel;
	std::weak_ptr<TPSCamera>    m_wpCamera;

	float m_angle = 0;

	bool m_isAtk = false;

	bool m_isTPS = false;

	Math::Vector3 m_pos;
};