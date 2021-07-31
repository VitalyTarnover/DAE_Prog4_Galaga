#pragma once
#include "BaseComponent.h"

class Texture2DComponent final : public BaseComponent
{
public:
	Texture2DComponent(const std::string& filename, float scale = 1.f);

	std::shared_ptr<dae::Texture2D> GetTexture2D() const { return m_pTexture2D; };
	void Render() override;
	void SetVisible(bool visible);

	bool GetVisible() { return m_Visible; }
	float GetScale() { return m_Scale; }
private:
	bool m_Visible;
	std::shared_ptr<dae::Texture2D> m_pTexture2D{};
	//glm::vec3 m_Position = {};
	float m_Scale;

};

