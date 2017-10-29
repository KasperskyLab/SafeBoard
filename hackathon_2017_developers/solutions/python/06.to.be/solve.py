# Author: Maxim Yurchuk

def bits_to_bytes(bits):
    bytes = []

    bits_count = 0
    byte = 0
    for bit in bits:
        bits_count += 1
        byte = (byte << 1) + bit
        if bits_count == 8:
            bytes.append(chr(byte))
            bits_count = 0
            byte = 0
    return "".join(bytes)


with open("hamlet.txt") as f:
    s = f.read()

bits = []

for c in s:
    if c == ',':
        bits.append(0)
    elif c == ';':
        bits.append(1)

with open('data1.zip', 'wb') as f:
    f.write(bits_to_bytes(bits))

bits = []

i = 0
while i < len(s):
    c = s[i]
    c1 = 'q'
    if i != len(s) - 1:
        c1 = s[i+1]
    if c == ' ' and c1 != ' ':
        bits.append(0)
    elif c == ' ' and c1 == ' ':
        bits.append(1)
        i += 1
    i += 1

with open('data2.png', 'wb') as f:
    f.write(bits_to_bytes(bits))

bits = []

words = []
word = ""

s += '!'

for c in s:
    if c.isalpha():
        word += c
    else:
        if word:
            words.append(word)
        word = ""

i = 0
while i < len(words):
    word = words[i]
    if word.lower() == "she":
        bits.append(0)
    elif word.lower() == "he":
        bits.append(1)
    i += 1

with open('data3.txt', 'wb') as f:
    f.write(bits_to_bytes(bits))

