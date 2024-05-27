#pragma once

class Stage :public KdGameObject
{
public:
	Stage() { Init(); }
	~Stage() {}

	void Init() override;
	void DrawLit() override;
private:
	std::shared_ptr<KdModelData> m_model;
};