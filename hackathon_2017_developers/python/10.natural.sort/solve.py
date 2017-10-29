# Author: Maxim Yurchuk

m = []
for line in open("input.txt"):
    m += [line.strip()]

n, k = [int(x) for x in m[0].split()]
del m[0]


def break_into_tokens(s):
    res = []
    word = ""
    is_last_num = False
    for i in range(0, len(s)):
        if s[i].isdigit():
            word += s[i]
            is_last_num = True
        else:
            if is_last_num:
                res += [int(word)]
                word = ""

            res += [s[i].upper()]
            is_last_num = False

    if is_last_num:
        res += [int(word)]

    res += ["\0", s]
    return tuple(res)


def std_cmp(lhs, rhs):
    if lhs < rhs:
        return -1
    elif lhs > rhs:
        return 1
    else:
        return 0

def std_cmp_plus(lhs, rhs):
    if isinstance(lhs, (int, long, float, complex)):
        if not isinstance(rhs, (int, long, float, complex)):
            return std_cmp(str(lhs), str(rhs))


    if isinstance(rhs, (int, long, float, complex)):
        if not isinstance(lhs, (int, long, float, complex)):
            return std_cmp(str(lhs), str(rhs))

    return std_cmp(lhs, rhs)

def cmp(lhs, rhs):
    i = 0
    while i != len(lhs) and i != len(rhs):
        r = std_cmp_plus(lhs[i], rhs[i])
        if r != 0:
            return r
        i += 1

    return std_cmp(len(lhs), len(rhs))

for i in range(len(m)):
    m[i] = break_into_tokens(m[i])

print sorted(m, cmp=cmp)[k-1][-1]

