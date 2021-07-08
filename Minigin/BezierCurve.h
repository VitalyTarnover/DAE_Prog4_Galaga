#pragma once
#include "glm/vec2.hpp"

struct BezierCurve
{
	glm::vec2 m_StartPoint; //P0
	glm::vec2 m_StartControlPoint; //P1
	glm::vec2 m_EndControlPoint; //P2
	glm::vec2 m_EndPoint; //P3

	glm::vec2 CalculatePointOnCurve(float t)
	{
		float tSqr = t * t;
		float tCube = tSqr * t;
		float u = 1.f - t;
		float uSqr = u * u;
		float uCube = uSqr * u;

		//Cubic Bezier curve formula: B(t) = ( (1 - t)^3 * P0 ) + ( 3 * (1 - t)^2 * t * P1 ) + ( 3 * (1 - t) *  t^2 * P2 ) + ( t^3 * P3 )
		return (uCube * m_StartPoint) + (3 * uSqr * t * m_StartControlPoint) + (3 * u * tSqr * m_EndControlPoint) + (tCube * m_EndPoint);
	}

};

