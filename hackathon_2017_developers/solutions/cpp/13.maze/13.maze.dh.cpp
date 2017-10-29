// Author: Dmitriy Khodyrev

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <algorithm>

const char Start = '@';
const char Exit = '%';
const char FreeCell = '.';
const char Wall = '#';

bool IsKey(const char c)
{
	return c >= 'a' && c <= 'z';
}

bool IsDoor(const char c)
{
	return c >= 'A' && c <= 'Z';
}

char GetKeyForDoor(const char door)
{
	return 'a' + door - 'A';
}

bool IsPossibleToStep(const char cell, const std::set<char>& availableKeys)
{
	if (cell == Wall)
		return false;

	if (cell == FreeCell)
		return true;

	if (cell == Start)
		return true;

	if (cell == Exit)
		return true;

	if (IsKey(cell))
		return true;

	if (IsDoor(cell))
	{
		const char requiredKey = GetKeyForDoor(cell);
		if (availableKeys.count(requiredKey) != 0)
			return true;

		return false;
	}

	return false;
}

struct Coord
{
	bool operator==(const Coord& rv) const
	{
		return std::tie(x, y) == std::tie(rv.x, rv.y);
	}

	bool operator!=(const Coord& rv) const
	{
		return !this->operator==(rv);
	}

	bool operator<(const Coord& rv) const
	{
		return std::tie(x, y) < std::tie(rv.x, rv.y);
	}

	size_t GetHash() const
	{
		return ((x & 0xFFFF) << 16) | (y & 0xFFFF);
	}

	size_t x;
	size_t y;
};

namespace std
{

template<>
struct hash<Coord>
{
	size_t operator()(const Coord& v) const
	{
		return v.GetHash();
	}
};

} // namespace std

typedef std::unordered_map<Coord, size_t> MapCoordSizeT;
typedef std::unordered_set<Coord> SetCoord;

const size_t dx[] = {(size_t)(-1), 1, 0, 0};
const size_t dy[] = {0, 0, (size_t)(-1), 1};

MapCoordSizeT::const_iterator FindPreviousPoint(const MapCoordSizeT& distances, MapCoordSizeT::const_iterator point)
{
	const size_t distance = point->second;

	for (size_t i = 0; i < 4; i++)
	{
		const size_t nx = point->first.x + dx[i];
		const size_t ny = point->first.y + dy[i];

		MapCoordSizeT::const_iterator p = distances.find({nx, ny});
		if (p == distances.end())
			continue;
		if (p->second != distance - 1)
			continue;

		return p;
	}

	return distances.end();
}

bool TryFindPathPart(const std::vector<std::string>& f, const Coord& begin, const Coord& end, const std::set<char>& availableKeys, std::vector<Coord>& steps)
{
	steps.clear();

	MapCoordSizeT distances;
	SetCoord queue;

	queue.insert(begin);

	size_t currentDistance = 0;
	while (!queue.empty())
	{
		SetCoord newQueue;
		for (const Coord& item: queue)
		{
			distances[item] = currentDistance;

			// Path found
			if (item == end)
				break;

			for (size_t i = 0; i < 4; i++)
			{
				const size_t nx = item.x + dx[i];
				const size_t ny = item.y + dy[i];
				if (nx >= f.size())
					continue;
				if (ny >= f[nx].size())
					continue;

				if (!IsPossibleToStep(f[nx][ny], availableKeys))
					continue;

				const Coord nextPoint = {nx, ny};
				if (distances.count(nextPoint) != 0)
					continue;

				if (newQueue.count(nextPoint) != 0)
					continue;

				newQueue.insert(nextPoint);
			}
		}

		currentDistance += 1;
		queue.swap(newQueue);
	}

	MapCoordSizeT::const_iterator beginPoint = distances.find(begin);

	MapCoordSizeT::const_iterator currentPoint = distances.find(end);
	if (currentPoint == distances.end())
		return false;

	while (currentPoint != beginPoint)
	{
		steps.push_back(currentPoint->first);
		currentPoint = FindPreviousPoint(distances, currentPoint);
	}

	std::reverse(steps.begin(), steps.end());
	return true;
}

bool TryWalkByControlPoints(const std::vector<std::string>& f, const std::vector<Coord>& controlPoints, std::vector<Coord>& steps)
{
	steps.clear();
	steps.push_back(controlPoints[0]);

	std::set<char> availableKeys;
	for (size_t i = 1; i < controlPoints.size(); i++)
	{
		const Coord& begin = controlPoints[i - 1];
		const Coord& end = controlPoints[i];

		const char beginCell = f[begin.x][begin.y];
		if (IsKey(beginCell))
		{
			availableKeys.insert(beginCell);
		}

		std::vector<Coord> stepsPart;
		if (!TryFindPathPart(f, begin, end, availableKeys, stepsPart))
			return false;

		steps.insert(steps.end(), stepsPart.begin(), stepsPart.end());
	}

	return true;
}

// 0 -------> Y
// |
// |
// |
// v
// X
char GetDirectionByCoord(const Coord& before, const Coord& after)
{
	if (after.x == before.x)
		return after.y < before.y ? 'L' : 'R';

	return after.x < before.x ? 'U' : 'D';
}

std::string ConvertPathPointsToDirections(const std::vector<Coord>& points)
{
	std::string directions;
	for (size_t i = 1; i < points.size(); i++)
	{
		directions += GetDirectionByCoord(points[i - 1], points[i]);
	}
	return directions;
}

bool TryFindExit(const std::vector<std::string>& f, std::vector<Coord>& fastestPath, std::string& fastestPathAsDirections)
{
	fastestPath.clear();
	fastestPathAsDirections.clear();

	const Coord UnknownPosition = {f.size(), f[0].size()};

	std::string keys;
	std::map<char, Coord> keysPositions;
	Coord startPosition = UnknownPosition;
	Coord exitPosition = UnknownPosition;

	for (size_t i = 0; i < f.size(); i++)
	{
		for (size_t j = 0; j < f[i].size(); j++)
		{
			const char c = f[i][j];
			const Coord pos = {i, j};
			if (c == Start)
			{
				startPosition = pos;
			}
			else if (c == Exit)
			{
				exitPosition = pos;
			}
			else if (IsKey(c))
			{
				keys += c;
				keysPositions[c] = pos;
			}
		}
	}

	bool isExitFound = false;

	// Enumerate all possible keys combinations
	const uint32_t maskLimit = 1UL << keys.size();
	for (uint32_t mask = 0; mask < maskLimit; mask++)
	{
		std::string keysSequence;
		for (size_t i = 0; i < keys.size(); i++)
		{
			if ((mask & (1UL << i)) == 0)
				continue;

			keysSequence.push_back(keys[i]);
		}

		// Enumerate all possible permutations of the current combination
		std::sort(keysSequence.begin(), keysSequence.end());
		do
		{
			std::vector<Coord> controlPoints;
			controlPoints.push_back(startPosition);
			for (char c: keysSequence)
			{
				controlPoints.push_back(keysPositions[c]);
			}
			controlPoints.push_back(exitPosition);

			std::vector<Coord> foundPath;
			if (TryWalkByControlPoints(f, controlPoints, foundPath))
			{
				if (!isExitFound || fastestPath.size() > foundPath.size())
				{
					isExitFound = true;
					fastestPath = foundPath;
					fastestPathAsDirections = ConvertPathPointsToDirections(foundPath);
				}
			}
		} while (std::next_permutation(keysSequence.begin(), keysSequence.end()));
	}

	return isExitFound;
}

int main()
{
	while (std::cin)
	{
		size_t n, m;
		std::cin >> n >> m;
		if (!std::cin)
			continue;

		std::cout << std::endl;

		std::vector<std::string> field(n);
		for (std::string& s: field)
		{
			std::cin >> s;
		}

		std::vector<Coord> fastestPath;
		std::string fastestPathAsDirections;
		if (TryFindExit(field, fastestPath, fastestPathAsDirections))
		{
			std::cout << "Exit found, path length: " << (fastestPath.size() - 1) << std::endl;
			std::cout << fastestPathAsDirections << std::endl;
		}
		else
		{
			std::cout << "Exit not found" << std::endl;
		}
	}
}
