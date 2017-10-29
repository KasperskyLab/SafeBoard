// Author: Dmitriy Khodyrev

#include <cstdint>
#include <iostream>
#include <iomanip>

uint64_t Pow10(const uint64_t p)
{
	uint64_t v = 1;
	for (uint64_t i = 1; i <= p; i++)
	{
		v *= 10;
	}
	return v;
}

bool IsGood(const uint64_t v, const uint64_t p10)
{
	uint64_t r = v;
	const uint64_t d1 = r % p10;
	if ((d1 == 0) || (v % d1 != 0))
		return false;

	r /= p10;
	const uint64_t d2 = r % p10;
	if ((d2 == 0) || (v % d2 != 0))
		return false;

	r /= p10;
	const uint64_t d3 = r;
	if ((d3 == 0) || (v % d3 != 0))
		return false;

	return true;
}

uint64_t GetCount(const size_t n)
{
	const uint64_t minValue = Pow10(3 * n - 1);
	const uint64_t maxValue = Pow10(3 * n);

	const uint64_t p10 = Pow10(n);
	uint64_t c = 0;
	for (uint64_t v = minValue; v < maxValue; v++)
	{
		if (IsGood(v, p10))
		{
			c += 1;
		}
	}

	return c;
}

int main()
{
	size_t n;
	std::cin >> n;

	const uint64_t ans = GetCount(n);
	std::cout << ans << std::endl;
}
