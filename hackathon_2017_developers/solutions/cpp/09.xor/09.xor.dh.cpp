// Author: Dmitriy Khodyrev

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

uint32_t FuncFast(const std::vector<uint32_t>& a)
{
	if (a.size() % 2 == 0)
		return 0;

	uint32_t ans = 0;
	for (size_t i = 0; i < a.size(); i += 2)
	{
		ans ^= a[i];
	}

	return ans;
}

uint32_t GetAnsUsingRandom(std::vector<uint32_t> a, uint32_t& iterations)
{
	srand(0);

	uint32_t bestAns = FuncFast(a);
	size_t lastIterationWithUpdate = 1;

	const size_t MaxIterations = 100000000;
	for (size_t i = 1; i <= MaxIterations; i++)
	{
		std::random_shuffle(a.begin(), a.end());
		uint32_t ans = FuncFast(a);
		if (bestAns < ans)
		{
			bestAns = ans;
			lastIterationWithUpdate = i;
		}
	}

	iterations = lastIterationWithUpdate;
	return bestAns;
}

int main()
{
	size_t n;
	std::cin >> n;

	std::vector<uint32_t> a(n);
	for (uint32_t& v: a)
	{
		std::cin >> v;
	}

	uint32_t iterations = 0;
	const uint32_t ans = GetAnsUsingRandom(a, iterations);
	std::cout << ans << std::endl;
	std::cout << "Iterations elapsed: " << iterations << std::endl;
}
