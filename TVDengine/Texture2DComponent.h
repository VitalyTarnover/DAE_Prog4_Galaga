#pragma once
#include "BaseComponent.h"

class Texture2DComponent final : public BaseComponent
{
public:
	Texture2DComponent(const std::string& filePath, float scale = 1.f);

	void Update() override;

	void SetTexture(const std::string& filePath);
	void SetVisible(bool visible);

	bool GetVisible() const { return m_Visible; }
	float GetScale() const { return m_Scale; }
	std::string GetFilePath() const { return m_FilePath; }

private:
	bool m_Visible;
	std::string m_FilePath;
	float m_Scale;

};

