#pragma once

class Grass : public KdGameObject
{
public:
	Grass(const Math::Vector3& siz, const float& rot,  const Math::Vector3& pos) {
	
		if (!m_spModel)
		{
			m_spModel = std::make_shared<KdModelData>();
			m_spModel = KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Grass/Grass.gltf");

			m_mWorld =  Math::Matrix::CreateScale(siz) * Math::Matrix::CreateRotationY(rot) * Math::Matrix::CreateTranslation(pos);
		}
	};
	~Grass() {};


	void GenerateDepthMapFromLight()override;
	void DrawLit()override;

private:
	std::shared_ptr<KdModelData> m_spModel;
};