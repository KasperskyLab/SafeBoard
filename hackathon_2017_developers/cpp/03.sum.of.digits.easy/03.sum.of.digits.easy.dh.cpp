// Author: Dmitriy Khodyrev

#include <iostream>
#include <iomanip>

uint32_t GetSum(uint32_t x)
{
	uint32_t s = 0;
	while (x != 0)
	{
		s += x % 10;
		x /= 10;
	}
	return s;
}

int main()
{
	uint32_t n;
	std::cin >> n;

	uint32_t ans = 0;
	for (uint32_t x = 1; x <= n; x++)
	{
		ans += GetSum(x);
	}

	std::cout << ans << std::endl;
}
