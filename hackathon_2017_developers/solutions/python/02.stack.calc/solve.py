# Author: Maxim Yurchuk

s = []

m = 0

n = None
for line in open("input.txt"):
    if n == None:
        n = int(line)
        continue
    command = line.split()
    if command[0] == "push":
        s.append(int(command[1]))
    elif command[0] == "add":
        op1 = s.pop(-1)
        op2 = s.pop(-1)
        r = op1 + op2
        m = max(m, abs(r))
        s.append(r)
    elif command[0] == "mul":
        op1 = s.pop(-1)
        op2 = s.pop(-1)
        r = op1 * op2
        m = max(m, abs(r))
        s.append(r)
    elif command[0] == "sub":
        op1 = s.pop(-1)
        op2 = s.pop(-1)
        r = op1 - op2
        m = max(m, abs(r))
        s.append(r)
    elif command[0] == "top":
        op1 = s.pop(-1)
        print "{}".format(op1)

