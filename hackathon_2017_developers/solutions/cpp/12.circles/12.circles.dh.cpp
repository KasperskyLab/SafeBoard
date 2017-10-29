// Author: Dmitriy Khodyrev

#include <cstdint>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <cmath>

void RaiseContractViolation()
{
	volatile uint32_t x = 0;
	uint32_t z = *reinterpret_cast<uint32_t*>(x);
	z;
}

#define ASSERT(expr) { if (!(expr)) { RaiseContractViolation(); } }

const double Eps = 1.0e-8;

struct Point
{
	double x;
	double y;
};

struct Circle
{
	double r;
	double x;
	double y;
};

double Sqr(const double v)
{
	return v * v;
}

double DistanceSqr(const Point& p1, const Point& p2)
{
	return Sqr(p1.x - p2.x) + Sqr(p1.y - p2.y);
}

bool HasIntersection(Circle c1, Circle c2, Point& p1, Point& p2)
{
	const double dx = c1.x;
	const double dy = c1.y;

	c2.x -= c1.x;
	c2.y -= c1.y;
	c1.x = 0;
	c1.y = 0;

	const double a = -2.0 * c2.x;
	const double b = -2.0 * c2.y;
	const double c = Sqr(c2.x) + Sqr(c2.y) + Sqr(c1.r) - Sqr(c2.r);

	const double x0 = -a * c / (Sqr(a) + Sqr(b));
	const double y0 = -b * c / (Sqr(a) + Sqr(b));

	double z = Sqr(c) - Sqr(c1.r) * (Sqr(a) + Sqr(b));
	if (z > Eps)
	{
		// no intersection
		return false;
	}

	if (std::abs(z) <= Eps)
	{
		// one point, touch is not important
		return false;
	}

	const double d = Sqr(c1.r) - Sqr(c) / (Sqr(a) + Sqr(b));
	const double m = std::sqrt(d / (Sqr(a) + Sqr(b)));
	const double ax = x0 + b * m;
	const double bx = x0 - b * m;
	const double ay = y0 - a * m;
	const double by = y0 + a * m;

	p1.x = ax + dx;
	p1.y = ay + dy;
	p2.x = bx + dx;
	p2.y = by + dy;
	return true;
}

class Function
{
public:
	Function(const Circle& c, const bool upPart):
		m_cx(c.x), m_cy(c.y), m_r(c.r), m_upPart(upPart)
	{
	}

	bool HasValue(const double x) const
	{
		if (x + Eps < m_cx - m_r)
			return false;
		if (x - Eps > m_cx + m_r)
			return false;
		return true;
	}

	double GetValue(const double x) const
	{
		ASSERT(HasValue(x));
		double d = std::sqrt(Sqr(m_r) - Sqr(x - m_cx));
		if (m_upPart)
			return m_cy + d;
		return m_cy - d;
	}

	double GetAntiDerivative(const double x) const
	{
		ASSERT(HasValue(x));
		const double root = std::sqrt(Sqr(m_r) - (Sqr(x - m_cx)));
		double s1 = 0.5 * (x - m_cx) * root;
		double s2 = 0.5 * Sqr(m_r) * atan((m_cx - x) / root);

		if (m_upPart)
			return m_cy * x + (s1 - s2);
		return m_cy * x - (s1 - s2);
	}

public:
	const double m_cx;
	const double m_cy;
	const double m_r;
	const bool m_upPart;
};

double IntegrateDifference(const double xMin, const double xMax, const Function& f1, const Function& f2)
{
	const double s2 = f2.GetAntiDerivative(xMax) - f2.GetAntiDerivative(xMin);
	const double s1 = f1.GetAntiDerivative(xMax) - f1.GetAntiDerivative(xMin);
	const double s = s2 - s1;
	return s;
}

bool IsPointInside(const Circle& c, const Point& p)
{
	return DistanceSqr({c.x, c.y}, p) + Eps < Sqr(c.r);
}

bool IsPointInside(const std::vector<Circle>& cs, const Point& p)
{
	for (const Circle& c: cs)
	{
		if (IsPointInside(c, p))
			return true;
	}
	return false;
}

double GetExactSquare(const std::vector<Circle>& c)
{
	std::vector<double> xPoints;
	std::vector<Function> functions;
	for (size_t i = 0; i < c.size(); i++)
	{
		for (size_t j = i + 1; j < c.size(); j++)
		{
			Point p1, p2;
			if (HasIntersection(c[i], c[j], p1, p2))
			{
				xPoints.push_back(p1.x);
				xPoints.push_back(p2.x);
			}
		}
		xPoints.push_back(c[i].x - c[i].r);
		xPoints.push_back(c[i].x + c[i].r);

		functions.push_back(Function(c[i], true));
		functions.push_back(Function(c[i], false));
	}

	std::sort(xPoints.begin(), xPoints.end());

	double s = 0.0;
	for (size_t i = 1; i < xPoints.size(); i++)
	{
		const double x1 = xPoints[i - 1];
		const double x2 = xPoints[i];
		const double x = (x1 + x2) * 0.5;

		// determine, which functions are available in the interval
		std::vector<size_t> indexes;
		for (size_t j = 0; j < functions.size(); j++)
		{
			if (functions[j].HasValue(x))
			{
				const double v = functions[j].GetValue(x);
				indexes.push_back(j);
			}
		}

		// sort functions by it's values in point x
		std::sort(indexes.begin(), indexes.end(), [&functions, &x](const size_t i1, const size_t i2) -> bool
		{
			const double v1 = functions[i1].GetValue(x);
			const double v2 = functions[i2].GetValue(x);
			return v1 < v2;
		});

		// integrate difference between adjacent functions, if it is inside circles
		for (size_t j = 1; j < indexes.size(); j++)
		{
			const Function& f1 = functions[indexes[j - 1]];
			const Function& f2 = functions[indexes[j]];
			ASSERT(f1.HasValue(x));
			ASSERT(f2.HasValue(x));
			const double y = (f1.GetValue(x) + f2.GetValue(x)) * 0.5;
			if (IsPointInside(c, {x, y}))
			{
				const double ds = IntegrateDifference(x1, x2, f1, f2);
				s += ds;
			}
		}
	}

	return s;
}

double GetApproximatedSquare(const std::vector<Circle>& c, const double d)
{
	double xMin = c[0].x;
	double xMax = c[0].x;
	double yMin = c[0].y;
	double yMax = c[0].y;
	for (const Circle& ci: c)
	{
		xMin = std::min(xMin, ci.x - ci.r);
		xMax = std::max(xMax, ci.x + ci.r);
		yMin = std::min(yMin, ci.y - ci.r);
		yMax = std::max(yMax, ci.y + ci.r);
	}

	double x = xMin + d * 0.5;
	double y = yMin + d * 0.5;
	double s = 0;
	while (y < yMax)
	{
		while (x < xMax)
		{
			if (IsPointInside(c, {x, y}))
			{
				s += d * d;
			}

			x += d;
		}
		x = xMin + d * 0.5;
		y += d;
	}

	return s;
}

int main()
{
	size_t n;
	std::cin >> n;

	std::vector<Circle> circles(n);
	for (Circle& c: circles)
	{
		std::cin >> c.r >> c.x >> c.y;
	}

	std::cout << std::fixed;

	const double s = GetExactSquare(circles);
	std::cout << "Exact: " << std::setprecision(12) << s << std::endl;

	std::vector<double> steps = {1.0, 0.5, 0.1, 0.01};
	for (double step: steps)
	{
		const double sApprox = GetApproximatedSquare(circles, step);
		std::cout << "Approximated (d = " << std::setprecision(2) << step << "): " << std::setprecision(12) << sApprox << std::endl;

	}
}
