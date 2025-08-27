#pragma once

class Ground : public KdGameObject
{
public:
	Ground() = default;
	~Ground() = default;

	void Init()override;
	void GenerateDepthMapFromLight()override;
	void DrawLit()override;

private:
	std::shared_ptr<KdModelData> m_spModelMap;
	std::shared_ptr<KdModelData> m_spModel;
};