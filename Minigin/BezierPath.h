#pragma once
#include "BezierCurve.h"
#include <vector>

class BezierPath final
{
public:
	BezierPath() {};
	~BezierPath() {};

	void AddCurve(BezierCurve& newCurve, int samples);
	void Sample(std::vector<glm::vec2>& sampledPath, int index = -1) const;

private:
	//std::vector<BezierCurve> m_Curves;
	//std::vector<int> m_NumberOfSamples;

	std::vector<std::pair<BezierCurve, int>> m_Curves;
};

