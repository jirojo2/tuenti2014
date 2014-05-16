#!/usr/bin/env python

import fileinput
import math

class Input:
    x = 0
    y = 0
    def process_line(self, line):
        params = line.split(' ')
        self.x = int(params[0])
        self.y = int(params[1])
        return self
    def gamblers_algorythm(self):
        # Despues de probar y probar, pintar todos los datos y crear una
        # matriz...
        v = round(math.sqrt(self.x*self.x + self.y*self.y), 2)
        if float(v).is_integer():
            return int(v)
        return v

N = 0
n = 0
serie = []
for line in fileinput.input():
    if n == 0:
        N = int(line)
    else:
        serie.append(Input().process_line(line))
    n += 1

for entry in serie:
    print('{}'.format(entry.gamblers_algorythm()))

