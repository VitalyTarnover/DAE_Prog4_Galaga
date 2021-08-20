#include "MiniginPCH.h"
#include "BezierPath.h"


void BezierPath::AddCurve(BezierCurve newCurve, int samples)
{
	m_Curves.push_back(newCurve);
	m_Samples.push_back(samples);

}

void BezierPath::Sample(std::vector<glm::vec2>* sampledPath, int index)
{
	//loop through curves vector and sample them by the amount of corresponding value in samples vector

	if (index == -1)//all of them
	{
		for (size_t i = 0; i < m_Curves.size(); ++i)
		{
			for (float t = 0; t <= 1.f; t += (1.f / m_Samples[i]))
			{
				sampledPath->push_back(m_Curves[i].CalculatePointOnCurve(t));
			}
		}
	}
	else if (index >= 0 && index < int(m_Curves.size()))//specific one
	{
		for (float t = 0; t <= 1.f; t += (1.f / m_Samples[index]))
		{
			sampledPath->push_back(m_Curves[index].CalculatePointOnCurve(t));
		}
	}
	else return;//error

	

}
