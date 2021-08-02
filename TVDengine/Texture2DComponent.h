#pragma once
#include "BaseComponent.h"

class Texture2DComponent final : public BaseComponent
{
public:
	Texture2DComponent(const std::string& filePath, float scale = 1.f);

	void Update() override;

	//void Render() override;
	void SetTexture(const std::string& filePath);
	void SetVisible(bool visible);

	bool GetVisible() { return m_Visible; }
	float GetScale() { return m_Scale; }
	std::string GetFilePath() { return m_FilePath; }

private:
	bool m_Visible;
	std::string m_FilePath;
	float m_Scale;

};

