// Author: Dmitriy Khodyrev

#include <cstdint>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

bool GetSum(const std::vector<std::vector<int32_t>>& v, size_t x, size_t y, size_t dx, size_t dy, size_t d, int32_t& s)
{
	s = 0;

	for (size_t i = 0; i < d; i++)
	{
		if (x >= v.size())
			return false;
		if (y >= v[0].size())
			return false;

		s += v[x][y];
		x += dx;
		y += dy;
	}

	return true;
}

int main()
{
	size_t n, m;
	std::cin >> n >> m;

	std::vector<std::vector<int32_t>> v(n, std::vector<int32_t>(m));
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			std::cin >> v[i][j];
		}
	}

	int32_t ans = 0;
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			const size_t dx[4] = {1, 0, 1, (size_t)(-1)};
			const size_t dy[4] = {0, 1, 1, 1};

			for (size_t t = 0; t < 4; t++)
			{
				int32_t s;
				if (GetSum(v, i, j, dx[t], dy[t], 5, s))
				{
					ans = std::max(ans, s);
				}
			}
		}
	}

	std::cout << ans << std::endl;
}
