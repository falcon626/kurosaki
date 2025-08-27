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
	std::shared_ptr<KdModelData> m_spModel;

	// 水面表現１Texture用意(法線マップ用)
	std::shared_ptr<KdTexture> m_spTex;
};