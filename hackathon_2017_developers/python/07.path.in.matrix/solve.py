# Author: Maxim Yurchuk

a = open("input.txt").read()

a = [[int(item) for item in row.split()] for row in a.split("\n")]

# skip size
a = a[1:]

n = len(a)
m = len(a[0])

b = [[None for j in range(m)] for i in range(n)]

b[0][0] = a[0][0]

for i in range(1, n):
    b[i][0] = b[i-1][0] + a[i][0]

for i in range(1, m):
    b[0][i] = b[0][i-1] + a[0][i]

for i in range(1, n):
    for j in range(1, m):
        b[i][j] = a[i][j] + min(b[i][j-1], b[i-1][j])

print b[n-1][m-1]
