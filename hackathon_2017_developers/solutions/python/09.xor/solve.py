# Author: Maxim Yurchuk

s = open("input.txt").readlines()[1]
a = ([int (x) for x in s.split()])
n = len(a)

current_max = 0


def get(n, enabled_count, current_xor):
    global current_max
    if enabled_count == n/2 + 1:
        if current_xor > current_max:
            current_max = current_xor
        return
    if n == len(a):
        return

    get(n + 1, enabled_count, current_xor)
    get(n + 1, enabled_count + 1, current_xor ^ a[n])


get(0, 0, 0)
print current_max
