# Author: Maxim Yurchuk

import copy

class Queue(object):
    def __init__(self):
        self.l = []
        self.current = 0

    def pop(self):
        self.current += 1
        return self.l[self.current-1]

    def push(self, val):
        self.l.append(val)

    def size(self):
        return len(self.l) - self.current

def solve(a):
    keys_count = 0
    key_to_num = dict()
    current_key_mask = 1

    def is_door(key):
        return key.isalpha() and key.upper() == key

    def is_key(key):
        return key.isalpha() and key.lower() == key

    def can_open(conf, door):
        return key_to_num[door.lower()] & conf != 0

    def add_key(conf, new_key):
        return conf | key_to_num[new_key]

    for i in range(n):
        for j in range(m):
            if a[i][j].isalpha():
                c = a[i][j].lower()
                if c not in key_to_num:
                    keys_count += 1
                    key_to_num[c] = current_key_mask
                    current_key_mask *= 2

    exit_pos = None
    q = Queue()

    p = [[None for j in range(m)] for i in range(n)]
    p = copy.deepcopy(p)

    p = [copy.deepcopy(p) for i in range(2 ** keys_count)]

    # generate masks
    for i in range(n):
        for j in range(m):
            if a[i][j] == "@":
                # start
                q.push((0, i, j))
                p[0][i][j] = 0
            if a[i][j] == "%":
                # exit
                exit_pos = (i, j)
            if a[i][j].isalpha():
                c = a[i][j].lower()
                if c not in key_to_num:
                    keys_count += 1
                    key_to_num[c] = current_key_mask
                    current_key_mask *= 2

    while q.size() != 0:
        conf, i, j = q.pop()
        for di in [-1, 0, 1]:
            for dj in [-1, 0, 1]:
                if dj * di != 0:
                    continue
                new_i = i + di
                new_j = j + dj
                new_conf = conf
                current_length = p[conf][i][j]
                new_length = current_length + 1
                item = a[new_i][new_j]
                if item == "." or item == "%":
                    pass
                elif is_door(item) and can_open(conf, item):
                    # can open door
                    pass
                elif is_key(item):
                    # can move and modify conf
                    new_conf = add_key(conf, item)
                else:
                    new_length = None

                if p[new_conf][new_i][new_j] == None and new_length != None:
                    p[new_conf][new_i][new_j] = new_length
                    q.push((new_conf, new_i, new_j))

    answer = 10 ** 9
    for i in range(2 ** keys_count):
        r = p[i][exit_pos[0]][exit_pos[1]]
        if r != None:
            answer = min(r, answer)
    return answer

with open("input.txt") as f:
    a = []
    n, m = [int(x) for x in f.readline().split()]
    for i in range(n):
        a.append(f.readline().strip())
    answer = solve(a)
    print answer
