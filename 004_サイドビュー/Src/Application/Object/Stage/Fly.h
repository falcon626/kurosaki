#pragma once

class Fly :public KdGameObject
{
public:
	Fly(){}
	~Fly(){}


	void Init() override;
	void DrawLit() override;
	void DrawBright() override;
private:
	std::shared_ptr<KdModelData> m_model;
};