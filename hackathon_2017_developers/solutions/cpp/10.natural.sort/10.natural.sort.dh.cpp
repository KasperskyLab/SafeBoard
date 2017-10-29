// Author: Dmitriy Khodyrev

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>

bool IsDigit(const char c)
{
	return c >= '0' && c <= '9';
}

std::string ExtractNumber(const std::string& s, size_t& p)
{
	std::string res;
	while (p < s.size() && IsDigit(s[p]))
	{
		res += s[p];
		++p;
	}
	return res;
}

void AddLeadingZeros(std::string& na, std::string& nb)
{
	std::string& shorter = na.size() < nb.size() ? na : nb;
	std::string& longer = na.size() < nb.size() ? nb : na;
	const size_t diff = longer.size() - shorter.size();
	shorter = std::string(diff, '0') + shorter;
}

bool IsLess(const std::string& sa, const std::string& sb)
{
	size_t pa = 0;
	size_t pb = 0;
	while (pa < sa.size() && pb < sb.size())
	{
		const char ca = sa[pa];
		const char cb = sb[pb];

		if (!IsDigit(ca) || !IsDigit(cb))
		{
			if (ca == cb)
			{
				++pa;
				++pb;
				continue;
			}

			return ca < cb;
		}

		// IsDigit(ca) && IsDigit(cb)
		std::string na = ExtractNumber(sa, pa);
		std::string nb = ExtractNumber(sb, pb);
		AddLeadingZeros(na, nb);
		if (na == nb)
			continue;

		return na < nb;
	}

	// equal
	if (pa == sa.size() && pb == sb.size())
		return false;

	// less
	if (pa == sa.size())
		return true;

	// greater
	return false;
}

int main()
{
	size_t n, x;
	std::cin >> n >> x;

	std::vector<std::string> lines;

	std::string s;
	std::getline(std::cin, s);

	for (size_t i = 0; i < n; i++)
	{
		std::getline(std::cin, s);
		lines.push_back(s);
	}

	std::sort(lines.begin(), lines.end(), &IsLess);

	std::cout << lines[x - 1] << std::endl;
}
