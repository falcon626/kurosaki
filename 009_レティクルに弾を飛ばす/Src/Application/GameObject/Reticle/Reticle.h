#pragma once

class Reticle :public KdGameObject
{
public:
	Reticle(const std::string& path);
	~Reticle() = default;

	void DrawSprite()override;

private:
	std::shared_ptr<KdTexture> m_spTex;
};