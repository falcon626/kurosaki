#pragma once

class Player :public KdGameObject
{
public:
	Player(){}
	~Player(){}

	void Init()override;
	void DrawLit()override;
	void Update()override;
	void PostUpdate()override;
private:
	KdSquarePolygon m_polygon;
	Math::Vector3 m_pos;
	float m_gravity = 0;
};