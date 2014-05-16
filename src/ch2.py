#!/usr/bin/env python

import fileinput

class F1Elem:
    kind = '#'
    def __init__(self, kind):
        self.kind = kind
    def horizontal(self):
        if self.kind == '#' or self.kind == '-':
            return True
        return False
    def vertical(self):
        if self.kind == '|':
            return True
        return False
    def curve(self):
        if self.kind == '/' or self.kind == "\\":
            return True
        return False
    def modifier(self, vertical):
        if self.kind == '\\':
            return 1 * (-1 if vertical == True else 1)
        elif self.kind == '/':
            return -1 * (-1 if vertical == True else 1)
        return 0
    def visual(self, orientation):
        if self.curve():
            return self.kind
        if self.kind in ['#']:
            return self.kind
        if orientation in [1,3]:
            return '-'
        elif orientation in [2,4]:
            return '|'
        else:
            return self.kind

class F1Map:
    elems = [] # Vector of elements
    orientation = 1 # 1 right, 2 down, 3 left, 4 up
    width = 0
    lines = []
    currx = 0
    curry = 0
    offsetx = 0
    offsety = 0
    log = []
    def orient(self, elem):
        if elem.curve():
            self.orientation += elem.modifier(self.orientation in [2,4])
        if self.orientation < 1:
            self.orientation = 4
            self.log.append('A')
        if self.orientation > 4:
            self.orientation = 1
            self.log.append('B')
    def set(self, x, y, visual):
        x = x + self.offsetx
        y = y + self.offsety
        while y < 0:
            self.offsety += 1
            self.lines.insert(0, ' ' * self.width)
            y += 1
        while x < 0:
            self.offsetx += 1
            for ll in range(len(self.lines)):
                self.lines[ll] = ' ' + self.lines[ll]
            x += 1
        while y >= len(self.lines):
            self.lines.append(' ' * self.width)
        while x >= len(self.lines[y]):
            self.lines[y] += ' '
        if x >= self.width:
            self.width = x
            for line in self.lines:
                while len(line) < self.width:
                    line += ' '
        line = list(self.lines[y])
        self.log.append('C: {} - {}'.format(x, len(line)))
        line[x] = visual
        self.lines[y] = "".join(line)
    def printMap(self):
        for elem in self.elems:
            visual = elem.visual(self.orientation)
            if self.orientation == 1: # right
                self.currx += 1
                self.set(self.currx, self.curry, visual)
            elif self.orientation == 3: # left
                self.currx -= 1
                self.set(self.currx, self.curry, visual)
            elif self.orientation == 2: # down
                self.curry += 1
                self.set(self.currx, self.curry, visual)
            elif self.orientation == 4: # up
                self.curry -= 1
                self.set(self.currx, self.curry, visual)
            self.orient(elem)
        
        for line in self.lines:
            print(line)

        #print('-' * 40)
        #for line in self.log:
        #    print(line)

# Initialize map list
maps = []

def processMapInput(line):
    f1map = F1Map()
    maps.append(f1map)
    for i in range(len(line)):
        e = line[i]
        f1map.elems.append(F1Elem(e))

# Process input

n = 0
for line in fileinput.input():
    processMapInput(line)
    n += 1

for i in range(len(maps)):
    maps[i].printMap()
    #print('-' * 40)

