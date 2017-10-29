# Author: Maxim Yurchuk

def get_count(digit, pos, n):
    k = 10 ** (pos + 1)
    full_cycles = n / k
    count = full_cycles * (10 ** pos)
    reminder = n % k
    if reminder > (digit + 1) * (10 ** pos):
        count += 10 ** pos
    else:
        if reminder > (digit) * (10 ** pos):
            count += reminder - (digit) * (10 ** pos)
    return count

n = int(open("input.txt").read())

count = 0

for pos in range(20):
    for digit in range(10):
        count += get_count(digit, pos, n+1) * digit

print count



