// Author: Dmitriy Khodyrev

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

std::string ToString(const uint64_t v)
{
	std::ostringstream ostr;
	ostr << v;
	return ostr.str();
}

std::string GetAns(const uint64_t n, const uint64_t k)
{
	if (k < 10)
	{
		std::string sequence;
		uint64_t v = k;
		while (sequence.size() <= n)
		{
			sequence += ToString(v);
			v += 1;
		}

		sequence.resize((size_t)n);
		return sequence;
	}

	uint64_t valuePow10 = 10;
	uint64_t indexPow10 = 1;
	uint64_t digits = 9;

	while (digits + (indexPow10 + 1) * 9 * valuePow10 < k)
	{
		digits += (indexPow10 + 1) * 9 * valuePow10;
		indexPow10 += 1;
		valuePow10 *= 10;
	}

	uint64_t rem = k - digits - 1;
	const uint64_t firstValue = rem / (indexPow10 + 1) + valuePow10;
	rem %= indexPow10 + 1;

	std::string sequence;
	uint64_t v = firstValue;
	while (sequence.size() <= rem + n)
	{
		sequence += ToString(v);
		v += 1;
	}

	std::string ans = sequence.substr((size_t)rem, (size_t)n);
	return ans;
}

std::string GetAnsBruteForce(const uint64_t n, const uint64_t k)
{
	std::string sequence;
	uint64_t v = 1;
	while (sequence.size() <= n + k)
	{
		sequence += ToString(v);
		v += 1;
	}

	return sequence.substr((size_t)k - 1, (size_t)n);
}

int main()
{
	uint64_t n, k;
	std::cin >> n >> k;

	std::string ans = GetAns(n, k);
	std::cout << ans << std::endl;
}
