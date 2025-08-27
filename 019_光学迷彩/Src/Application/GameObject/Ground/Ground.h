#pragma once

class Ground : public KdGameObject
{
public:
	Ground() = default;
	~Ground() = default;

	void Init()override;
	void GenerateDepthMapFromLight()override;
	void DrawLit()override;
	void DrawSprite() override;

private:
	std::shared_ptr<KdModelData> m_spModel;
};