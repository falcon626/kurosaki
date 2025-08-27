#pragma once

class MyPolygon : public KdGameObject
{
public:
	MyPolygon();
	~MyPolygon()= default;

	void DrawUnLit()override;
	void Update()override;

private:
	std::shared_ptr<KdSquarePolygon> m_spPol;

};