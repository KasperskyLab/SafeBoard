# Author: Maxim Yurchuk

SUM_SIZE = 5
N = 20

def rotate(grid):
    new = [[0 for x in range(N)] for y in range(N)]
    for i in range(N):
        for j in range(N):
            new[i][j] = grid[j][N - i - 1]
    return new


def get_max_left(grid):
    result = 0
    for row in grid:
        for pos in range(N-SUM_SIZE):
            result = max(result, sum(row[pos:pos+SUM_SIZE]))
    return result


def get_max_diag(grid):
    result = 0
    for i in range(N-SUM_SIZE):
        for j in range(N-SUM_SIZE):
            result = max(result, sum([grid[i+k][j+k] for k in range(SUM_SIZE)]))

    return result

grid = open("input.txt").read()
grid = grid.split("\n")
grid = [[int(val) for val in row.split(" ")] for row in grid]

result = 0

for i in range(2):
    grid = rotate(grid)
    result = max(result, get_max_left(grid))
    result = max(result, get_max_diag(grid))

print result



