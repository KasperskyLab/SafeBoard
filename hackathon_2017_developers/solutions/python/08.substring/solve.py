# Author: Maxim Yurchuk

length, position = [int(x) for x in open("input.txt").read().split()]

current_digits_count = 1
current_number_count = 9
current_pos = 1
current_number = 1

while position > current_number_count * current_digits_count + current_pos:
    current_pos += current_number_count * current_digits_count
    current_digits_count += 1
    current_number_count *= 10
    current_number *= 10

numbers_to_skip = (position - current_pos) / current_digits_count

current_pos += numbers_to_skip * current_digits_count
current_number += numbers_to_skip

substring = ""

for i in range(length):
    substring += str(current_number + i)

print substring[position-current_pos:position - current_pos + length]


