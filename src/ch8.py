#!/usr/bin/env python

import fileinput
import math
import sys

class Table:
    def __init__(self):
        self.moves = 0
        self.buffer = [None] * 9
    def idx(self, x, y):
        return (x + y*3)
    def get(self, x, y):
        return self.buffer[self.idx(x, y)]
    def set(self, x, y, val):
        self.buffer[self.idx(x, y)] = val
    def empty(self, x, y):
        return self.get(x, y) == ''
    def swap(self, idx1, idx2):
        if (not self.adjacent(idx1, idx2)):
            return False
        aux = self.buffer[idx2]
        self.buffer[idx2] = self.buffer[idx1]
        self.buffer[idx1] = aux
        self.moves += 1
        return True
    def print(self):
        for y in range(3):
            line = ''
            for x in range(3):
                line += self.get(x, y) + ', '
            print (line[:-2])
    def print_comp(self, target):
        for y in range(3):
            line = ''
            for x in range(3):
                line += self.get(x, y) + ', '
            line = line[:-2]
            line = line + '   '
            for x in range(3):
                line += target.get(x, y) + ', '
            print(line[:-2])

    def adjacent(self, idx1, idx2):
        return self.dist(idx1, idx2) == 1
    def equals(self, target):
        for idx in range(9):
            if (self.buffer[idx] != target.buffer[idx]):
                return False
        return True
    def dist(self, idx1, idx2):
        x1 = math.floor(idx1 % 3)
        x2 = math.floor(idx2 % 3)
        y1 = math.floor(idx1 / 3)
        y2 = math.floor(idx2 / 3)
        return abs(x1-x2) + abs(y1-y2)
    def find(self, value):
        for idx in range(9):
            if self.buffer[idx] == value:
                return idx
        return -1
    def score(self, idx, target):
        score = self.dist(idx, target.find(self.buffer[idx]))
        sn = 0
        for idx2 in range(9):
            if self.adjacent(idx, idx2):
                sn += self.dist(idx2, target.find(self.buffer[idx2]))
        return score * sn

def process(source, target):
    # Solo mover dos celdas adyacentes (no diagonal) en un movimiento
    
    limit = 10
    while (not source.equals(target)):
        # calculate scores
        scores = [0] * 9
        maxscore = -100
        idx_maxscore = -1
        for idx in range(9):
            scores[idx] = source.score(idx, target)
            if scores[idx] > maxscore:
                maxscore = scores[idx]
                idx_maxscore = idx

        # swap the candidate with the best scored neighbor
        candscore = 100
        idx_c = -1
        for idx in range(9):
            if source.adjacent(idx_maxscore, idx):
                if scores[idx] > 0 and scores[idx] < candscore:
                    candscore = scores[idx]
                    idx_c = idx

        print('Idx max score: {} with score {} - Idx candidate {} with score {}'.format(idx_maxscore, maxscore, idx_c, candscore))
        source.print_comp(target)

        source.swap(idx_maxscore, idx_c)
        limit -= 1
        if limit < 0:
            break

    return source.moves
    #return math.ceil(sumdist/2) + (frozen % 2)
    #return maxdist*maxdist + int((frozen % 2) == 0)

lines = list(fileinput.input())
t = int(lines[0])

for i in range(t):
    source = Table()
    target = Table()

    offset = 2+8*i
    for table in [source, target]:
        for y in range(3):
            names = lines[offset].split(',')
            for x in range(3):
                name = names[x].strip()
                table.set(x, y, name)
            offset += 1
        offset += 1

    #print('-- Source', '-'*20)
    #source.print()
    #print('-- Target', '-'*20)
    #target.print()
    #print('-'*30)

    print(process(source, target))
