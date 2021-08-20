#include "MiniginPCH.h"
#include "BezierCurve.h"

BezierCurve::BezierCurve(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
	:m_StartPoint{p0}
	,m_StartControlPoint{p1}
	,m_EndControlPoint{p2}
	,m_EndPoint{p3}
{
}

glm::vec2 BezierCurve::CalculatePointOnCurve(float t)
{
	float tSqr = t * t;
	float tCube = tSqr * t;

	float u = 1.f - t;
	float uSqr = u * u;
	float uCube = uSqr * u;

	//Cubic Bezier curve formula: B(t) = ( (1 - t)^3 * P0 ) + ( 3 * (1 - t)^2 * t * P1 ) + ( 3 * (1 - t) *  t^2 * P2 ) + ( t^3 * P3 )
	return (uCube * m_StartPoint) + (3 * uSqr * t * m_StartControlPoint) + (3 * u * tSqr * m_EndControlPoint) + (tCube * m_EndPoint);
}
