#pragma once

class Ui : public KdGameObject
{
public:
	Ui()noexcept {}
	~Ui()noexcept override = default;

	void Init()override;
	void Update()override;
	void DrawSprite()override;
private:
	KdTexture m_tex;
	float m_hp = 100;
};