// Author: Dmitriy Khodyrev

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <set>
#include <cstdint>

int64_t Pop(std::vector<int64_t>& v)
{
	int64_t r = *v.rbegin();
	v.pop_back();
	return r;
}

int main()
{
	size_t n;
	std::cin >> n;

	std::vector<int64_t> stack;
	for (size_t i = 0; i < n; i++)
	{
		std::string command;
		std::cin >> command;

		if (command == "push")
		{
			int64_t v;
			std::cin >> v;
			stack.push_back(v);
		}
		else if (command == "add")
		{
			int64_t v1 = Pop(stack);
			int64_t v2 = Pop(stack);
			stack.push_back(v1 + v2);
		}
		else if (command == "sub")
		{
			int64_t v1 = Pop(stack);
			int64_t v2 = Pop(stack);
			stack.push_back(v1 - v2);
		}
		else if (command == "mul")
		{
			int64_t v1 = Pop(stack);
			int64_t v2 = Pop(stack);
			stack.push_back(v1 * v2);
		}
		else if (command == "top")
		{
			std::cout << *stack.rbegin() << std::endl;
		}
	}
}
