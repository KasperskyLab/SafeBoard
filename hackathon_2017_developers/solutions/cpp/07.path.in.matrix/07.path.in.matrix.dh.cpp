// Author: Dmitriy Khodyrev

#include <cstdint>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

int main()
{
	size_t n;
	std::cin >> n;

	std::vector<std::vector<uint32_t>> m(n, std::vector<uint32_t>(n));
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			std::cin >> m[i][j];
		}
	}

	std::vector<std::vector<uint32_t>> p(n, std::vector<uint32_t>(n));
	p[0][0] = m[0][0];

	for (size_t d = 1; d < 2 * n - 1; d++)
	{
		size_t x = d < n ? d : n - 1;
		size_t y = d < n ? 0 : d - n + 1;

		for (size_t i = 0; (i < n) && (x < n) && (y < n); i++)
		{
			uint32_t& ans = p[x][y];
			bool isAssigned = false;

			if (x > 0)
			{
				const uint32_t c = m[x][y] + p[x - 1][y];
				if (!isAssigned || ans > c)
				{
					ans = c;
					isAssigned = true;
				}
			}

			if (y > 0)
			{
				const uint32_t c = m[x][y] + p[x][y - 1];
				if (!isAssigned || ans > c)
				{
					ans = c;
					isAssigned = true;
				}
			}

			x -= 1;
			y += 1;
		}
	}

	std::cout << p[n - 1][n - 1] << std::endl;
}
