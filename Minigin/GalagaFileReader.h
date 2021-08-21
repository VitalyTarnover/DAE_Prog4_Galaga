#pragma once
class GalagaFileReader final
{
public:
	GalagaFileReader() {};
	void ReadLevelInfo(const std::string& filePath);

	const std::vector<std::vector<int>>& GetBeeInfo() const { return m_BeeInfo; };
	const std::vector<std::vector<int>>& GetBFInfo() const { return m_BFInfo; };
	const std::vector<std::vector<int>>& GetBirdInfo() const { return m_BirdInfo; };
private:
	std::vector<std::vector<int>> m_BeeInfo{};
	std::vector<std::vector<int>> m_BFInfo{};
	std::vector<std::vector<int>> m_BirdInfo{};
};

