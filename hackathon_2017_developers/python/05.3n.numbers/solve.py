# Author: Maxim Yurchuk

n = int(open("input.txt").read())

count = 0

for i in range(10 ** (n - 1), 10 ** n):
    for j in range(1, 10 ** n):
        k = 1
        while k < 10 ** n:
            val = (10 ** n) ** 2 * i + (10 ** n) * j + k

            if val % k + val % j + val % i == 0:
                count += 1
                k += 1
            else:
                # some optimizations...
                nothing_to_do_distance = max(j - val % j, i - val % i)
                if nothing_to_do_distance == 0:
                    nothing_to_do_distance = 1
                k += nothing_to_do_distance

print count

