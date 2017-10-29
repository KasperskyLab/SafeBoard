// Author: Dmitriy Khodyrev

#define STAGE 1 // 1 .. 3

#include <cstdint>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>

class BitCollector
{
public:
	void Add(uint8_t bit)
	{
		m_currentBit <<= 1;
		m_currentBit |= bit;
		m_bitsCount += 1;

		if (m_bitsCount == 8)
		{
			m_data.push_back(m_currentBit);
			m_currentBit = 0;
			m_bitsCount = 0;
		}
	}

	const std::vector<uint8_t>& GetResult() const
	{
		return m_data;
	}

private:
	size_t m_bitsCount = 0;
	uint8_t m_currentBit = 0;
	std::vector<uint8_t> m_data;
};

int main()
{
	BitCollector bits;
	std::string s;
	while (std::getline(std::cin, s))
	{
#if STAGE == 1
		for (char c: s)
		{
			if (c == ',')
			{
				bits.Add(0);
			}
			else if (c == ';')
			{
				bits.Add(1);
			}
		}
#elif STAGE == 2
		size_t i = 0;
		while (i < s.size())
		{
			if (s[i] == ' ')
			{
				if (i + 1 < s.size() && s[i + 1] == ' ')
				{
					bits.Add(1);
					i += 2;
				}
				else
				{
					bits.Add(0);
					i += 1;
				}
				continue;
			}

			i += 1;
		}
#elif STAGE == 3
		std::transform(s.begin(), s.end(), s.begin(), [](char c) -> char
			{
				if (c >= 'A' && c <= 'Z')
					return c - 'A' + 'a';
				if (std::string(" -;:!?'.,\"").find(c) != std::string::npos)
					return ' ';
				return c;
			});

		std::istringstream ist(s);
		std::string w;
		while (ist >> w)
		{
			if (w == "he")
			{
				bits.Add(1);
			}
			else if (w == "she")
			{
				bits.Add(0);
			}
		}
#endif
	}

	const std::vector<uint8_t>& data = bits.GetResult();

#if STAGE == 1
	std::ofstream ofs("output.6.1.zip", std::ios_base::binary);
#elif STAGE == 2
	std::ofstream ofs("output.6.2.png", std::ios_base::binary);
#elif STAGE == 3
	std::ofstream ofs("output.6.3.txt", std::ios_base::binary);
#endif
	ofs.write(reinterpret_cast<const char*>(data.data()), data.size());
	ofs.flush();
}
