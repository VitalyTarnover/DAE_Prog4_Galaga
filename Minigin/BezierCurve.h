#pragma once
#include "glm/vec2.hpp"

class BezierCurve
{
public: 
	BezierCurve(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);
	glm::vec2 CalculatePointOnCurve (float t) const;
	
private:
	glm::vec2 m_StartPoint; //P0
	glm::vec2 m_StartControlPoint; //P1
	glm::vec2 m_EndControlPoint; //P2
	glm::vec2 m_EndPoint; //P3

};

