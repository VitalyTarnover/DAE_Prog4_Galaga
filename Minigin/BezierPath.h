#pragma once
#include "BezierCurve.h"
#include <vector>

class BezierPath final
{
public:
	BezierPath();
	~BezierPath();

	void AddCurve(BezierCurve newCurve, int samples);
	void Sample(std::vector<glm::vec2>* sampledPath, int index = -1);

private:
	std::vector<BezierCurve> m_Curves;
	std::vector<int> m_Samples;
};

