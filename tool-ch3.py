

import fileinput

mat = [[]]

for line in fileinput.input():
    p = line.split()
    x = int(p[0])
    y = int(p[1])
    v = float(p[2])
    while len(mat) <= x:
        mat.append([])
    while len(mat[x]) <= y:
        mat[x].append([])
    mat[x][y] = v

for x in range(len(mat)):
    line = ''
    for y in range(len(mat[x])):
        line += '{:5.2f} '.format(mat[x][y])
    print(line)
