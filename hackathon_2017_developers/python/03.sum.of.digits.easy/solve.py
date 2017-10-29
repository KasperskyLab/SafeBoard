# Author: Maxim Yurchuk

n = int(open("input.txt").read())

numbers_list = range(1, n+1)
result_string = "".join([str(i) for i in numbers_list])

digits_sum = 0
for digit in result_string:
    digits_sum += int(digit)

print digits_sum


