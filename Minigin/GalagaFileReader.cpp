#include "MiniginPCH.h"
#include "GalagaFileReader.h"
#include <fstream>
#include <iostream>
#include <string>

void GalagaFileReader::ReadLevelInfo(const std::string& filePath)
{
	m_BeeInfo.clear();
	m_BFInfo.clear();
	m_BirdInfo.clear();

	std::ifstream input;
	std::string currentLine;
	std::string stringBuffer;

	std::vector<int> bufferVector;

	int currentMode = 0; //A-1,B-2,C-3

	input.open(filePath, std::ios::in | std::ios::binary);
	if (input.is_open())
	{
		while (std::getline(input, currentLine))
		{
			if (currentLine.size() > 0)
			{
				if (currentLine[0] == 'A')
				{
					currentMode = 1;
					continue;
				}
				else if (currentLine[0] == 'B')
				{
					currentMode = 2;
					continue;
				}
				else if (currentLine[0] == 'C')
				{
					currentMode = 3;
					continue;
				}
				
				for (size_t i = 0; i < currentLine.size(); i++)
				{
					if (currentLine[i] != '/' && currentLine[i] != '\r')
					{
						stringBuffer += currentLine[i];
					}
					else
					{
						bufferVector.push_back(std::stoi(stringBuffer));
						stringBuffer = "";
					}
					
					if (currentLine[i] == '\r' || i == currentLine.size()-1)
					{
						if (stringBuffer != "") bufferVector.push_back(std::stoi(stringBuffer));

						switch (currentMode)
						{
						case 1:
							m_BeeInfo.push_back(bufferVector);
							break;
						case 2:
							m_BFInfo.push_back(bufferVector);
							break;
						case 3:
							m_BirdInfo.push_back(bufferVector);
							break;
						}
						bufferVector.clear();
					}
				}
			}
		}
	}




	

}
