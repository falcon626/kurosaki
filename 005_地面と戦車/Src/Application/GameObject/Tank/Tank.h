#pragma once


class Tank : public KdGameObject
{
public:
	Tank() = default;
	~Tank() = default;

	void Init()override;
	void GenerateDepthMapFromLight()override;
	void DrawLit()override;

	void Update()override;

private:
	std::shared_ptr<KdModelData> m_spModel;

	float m_angle = 0;
};