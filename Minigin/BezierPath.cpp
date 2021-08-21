#include "MiniginPCH.h"
#include "BezierPath.h"


void BezierPath::AddCurve(BezierCurve& newCurve, int numberOfSamples) 
{
	m_Curves.push_back(std::make_pair(newCurve, numberOfSamples));
	//m_NumberOfSamples.push_back(samples);
}

void BezierPath::Sample(std::vector<glm::vec2>& sampledPath, int index) const
{
	//loop through curves vector and sample them by the amount of corresponding value in samples vector

	if (index == -1)//all of them
	{
		for (size_t i = 0; i < m_Curves.size(); ++i)
		{
			for (float t = 0; t <= 1.f; t += (1.f / m_Curves[i].second))
			{
				sampledPath.push_back(m_Curves[i].first.CalculatePointOnCurve(t));
			}
		}
	}
	else if (index >= 0 && index < int(m_Curves.size()))//specific one
	{
		for (float t = 0; t <= 1.f; t += (1.f / m_Curves[index].second))
		{
			sampledPath.push_back(m_Curves[index].first.CalculatePointOnCurve(t));
		}
	}
	else return;//error

	

}
