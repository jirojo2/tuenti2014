#!/usr/bin/env python

# Tuenti Challenge 2014
# Jose Ignacio Rojo -josi-

# Fuente: http://en.wikipedia.org/wiki/Conway's_Game_of_Life

import fileinput
import copy

class Grid:
    def __init__(self):
        self.width = 8
        self.height = 8
        self.gen = 0
        self.gens = []
        self.v = [False] * (self.width * self.height)
    def idx(self, x, y):
        return (x + y*self.width)
    def get(self, x, y):
        return self.v[self.idx(x, y)]
    def set(self, x, y, val):
        self.v[self.idx(x, y)] = val
    def neighbors(self, x, y):
        # Sums the 8 direct neighbors that are alive
        n = 0
        #print("Neighbors in ({}, {})".format(x, y))
        for j in [y-1,y,y+1]:
            ln = 'y = {} '.format(j)
            for i in [x-1,x,x+1]:
                if (x == i and y == j):
                    ln += 'o'
                    continue
                if (i < 0 or i >= self.width):
                    continue
                if (j < 0 or j >= self.height):
                    continue
                alive = self.get(i, j)
                n += alive
                ln += 'X' if alive else '-'
            #print(ln)
        return n
    def next(self, x, y):
        n = self.neighbors(x, y)
        alive = self.get(x, y)
        # Rules:
        # 1) If alive, and <2 neighbors => die
        # 2) If alive, and 2<=n<=3 => survive
        # 3) If alive, and >3 neighbors => die
        # 4) If dead and there are 3 neighbors => born
        if alive:
            return n in [2, 3]
        else:
            return n == 3
    def print_grid(self):
        for y in range(self.height):
            line = 'y = {} '.format(y)
            for x in range(self.width):
                line += 'X' if self.get(x, y) else '-'
            print(line)
    def hash_gen(self):
        data = ''
        for x in range(self.width):
            for y in range(self.height):
                data += 'X' if self.get(x, y) else '-'
        return data
    def tick(self):
        old = copy.deepcopy(self)
        self.gen += 1
        for x in range(self.width):
            for y in range(self.height):
                self.set(x, y, old.next(x, y))
        self.gens.append(self.hash_gen())
    def test_point(self, x, y):
        print('({}, {}) -> {}'.format(x, y, self.get(x, y)))

# Gen 0: Build the grid
grid = Grid()

x = 0
y = 0
for line in fileinput.input():
    line = line[:-1]
    for c in line:
        #print('({}, {}) = {}'.format(x, y , c == 'X'))
        grid.set(x, y, c == 'X')
        x += 1
    y += 1
    x = 0
grid.gens.append(grid.hash_gen())

#grid.print_grid()

#grid.test_point(3, 2)
#grid.test_point(3, 3)
#grid.test_point(3, 4)

#grid.neighbors(3, 3)
#grid.neighbors(3, 2)
#grid.neighbors(2, 3)

# Loop

start = 0
duration = 0
N = 10
while (N > 0):
    N -= 1
    #print('Generation #{}'.format(grid.gen))
    #grid.print_grid()
    #print('')
    grid.tick()
    nh = grid.gens[-1]
    n = 0
    f = False
    for h in grid.gens[:-1]:
        if (h == nh):
            # Pattern
            start = n
            duration = grid.gen - n
            f = True
            break
        n += 1
    if f:
        #print('Generation #{}'.format(grid.gen))
        #grid.print_grid()
        #print('')
        break

print('{} {}'.format(start, duration))

