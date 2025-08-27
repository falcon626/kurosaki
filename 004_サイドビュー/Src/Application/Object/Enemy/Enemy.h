#pragma once

class Enemy : public KdGameObject
{
public:
	Enemy(){}
	~Enemy(){}

	void Init()override;
	void GenerateDepthMapFromLight()override;
	void DrawLit()override;
	void Update()override;
	void PostUpdate()override;
	void SetPos(Math::Vector3 pos) { m_pos = pos; }
	void OnHit()override;
private:
	KdSquarePolygon m_polygon;
	Math::Vector3 m_pos;
	float m_gravity = 0;
};