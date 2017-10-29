// Author: Dmitriy Khodyrev

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

std::vector<uint64_t> GetPowersOf10(const uint64_t pMax)
{
	std::vector<uint64_t> powers;
	uint64_t p = 0;
	uint64_t v = 1;
	while (p <= pMax)
	{
		powers.push_back(v);
		p += 1;
		v *= 10;
	}
	return powers;
}

uint64_t Pow10(const uint64_t p)
{
	static const std::vector<uint64_t> Powers = GetPowersOf10(19);
	return Powers[(size_t)p];
}

// Sum [1..N]
uint64_t Sum(const uint64_t n)
{
	return n * (n + 1) / 2;
}

// Sum [10^K .. 10^(K+1)-1]
uint64_t GetDigitsSumFrom10PowKToNext10Pow(const uint64_t k)
{
	// 0..9
	if (k == 0)
		return Sum(9);

	const uint64_t sumForFirstDigits = Sum(9) * Pow10(k);
	const uint64_t sumForOtherDigits = Sum(9) * Pow10(k - 1) * k * 9;

	return sumForFirstDigits + sumForOtherDigits;
}

// Sum [1 .. 10^K-1]
uint64_t GetDigitsSumFrom1To10PowK(const uint64_t k)
{
	if (k == 0)
		return 0;

	// 1 .. 9
	uint64_t sum = Sum(9);

	// 10 .. 10^K-1
	for (uint64_t p = 1; p < k; p++)
	{
		uint64_t d = GetDigitsSumFrom10PowKToNext10Pow(p);
		sum += d;
	}
	return sum;
}

std::vector<uint64_t> SplitDigits(uint64_t n)
{
	std::vector<uint64_t> digits;
	while (n != 0)
	{
		digits.push_back(n % 10);
		n /= 10;
	}
	std::reverse(digits.begin(), digits.end());
	return digits;
}

uint64_t GetDigitsSumFrom1ToN(const uint64_t n)
{
	if (n < 10)
		return Sum(n);

	uint64_t maxPow = 0;
	while (Pow10(maxPow + 1) <= n)
	{
		maxPow += 1;
	}

	const std::vector<uint64_t> digits = SplitDigits(n);

	uint64_t sum = 0;
	uint64_t prefixSum = 0;
	for (size_t i = 0; i < digits.size(); i++)
	{
		const uint64_t digit = digits[i];
		if (digit == 0)
			continue;

		const uint64_t suffixLength = digits.size() - i - 1;

		// Calculate the sum of:
		//  [prefix] | 0         | [00..99]
		//  [prefix] | 1         | [00..99]
		//  [prefix] | digit - 1 | [00..99]
		// It's:
		//  s1       | s2          | s3
		const uint64_t s1 = prefixSum * digit * Pow10(suffixLength);
		const uint64_t s2 = Sum(digit - 1) * Pow10(suffixLength);
		const uint64_t s3 = digit * GetDigitsSumFrom1To10PowK(suffixLength);

		sum += s1;
		sum += s2;
		sum += s3;
		prefixSum += digit;
	}

	sum += prefixSum;

	return sum;
}

int main()
{
	uint64_t n;
	std::cin >> n;

	const uint64_t sum = GetDigitsSumFrom1ToN(n);
	std::cout << sum << std::endl;
}
