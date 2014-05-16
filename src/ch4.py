#!/usr/bin/env python

# Tuenti Challenge 2014
# Jose Ignacio Rojo -josi-

import fileinput

class DNA:
    ori = ''
    dest = ''
    curr = ''
    valid = []
    mutations = []
    def width(self):
        return len(self.ori)
    def isvalid(self, seq):
        return seq in self.valid
    def can_mutate(self, seq):
        # Compare all characters to 'curr' sequence
        # Only one can be different
        if len(seq) != len(self.curr):
            return False
        diff = 0
        for i in range(len(seq)):
            if seq[i] != self.curr[i]:
                diff += 1
        return diff == 1
    def score(self, seq):
        # Calculates the score towards dest sequence
        score = 0
        for i in range(len(seq)):
            if seq[i] == self.dest[i]:
                score += 1
        return score
    def iscandidate(self, seq, t = 1):
        # Checks if a sequence is a good candidate towards dest sequence
        if seq == self.ori or seq in self.mutations:
            return False
        if t > 0:
            return self.can_mutate(seq) and self.score(seq) > self.score(self.curr)
        if t < 0:
            return self.can_mutate(seq) and self.score(seq) < self.score(self.curr)
        return self.can_mutate(seq) and self.score(seq) == self.score(self.curr)
    def mutate(self, seq):
        if self.can_mutate(seq):
            self.curr = seq
            self.mutations.append(seq)
    def done(self):
        return self.curr == self.dest
    def clone(self):
        cp = DNA()
        cp.ori = self.ori
        cp.valid = self.valid[:]
        cp.mutations = self.mutations[:]
        cp.dest = self.dest
        cp.curr = self.curr
        return cp
    def print_mutations(self):
        sep  = '->' 
        line = self.ori + sep
        for seq in self.mutations:
            line += seq + sep
        print(line[:-2])

def recursive_mutation(dna):
    while not dna.done():
        candidates = []
        # Best candidates first
        for seq in dna.valid:
            if dna.iscandidate(seq):
                candidates.append(seq)
        # Then the candidates that maintains the score
        if len(candidates) == 0:
            for seq in dna.valid:
                if dna.iscandidate(seq, 0):
                    candidates.append(seq)
        # Then the worst ones, that reduce the score
        if len(candidates) == 0:
            for seq in dna.valid:
                if dna.iscandidate(seq, -1):
                    candidates.append(seq)
        #print(" curr: {}, {} mutations, {} candidates".format(dna.curr, len(dna.mutations), len(candidates)))
        if len(candidates) == 0:
            return None
        if len(candidates) > 0:        # TODO
            for seq in candidates: # TODO
                dna_clone = dna.clone()
                dna_clone.mutate(seq)
                dna_clone_final = recursive_mutation(dna_clone)
                if (dna_clone_final is not None):
                    return dna_clone_final
                    break

    return dna

dna = DNA()
n = 0
for line in fileinput.input():
    line = line[:-1]
    if n == 0:
        dna.ori  = line
        dna.curr = line
    elif n == 1:
        dna.dest = line
    else:
        dna.valid.append(line)
    n += 1

# DEBUG input
#print(dna.ori +  '->' + dna.dest + ' [{} valid mutations]'.format(len(dna.valid)))

dna_mutated = recursive_mutation(dna)
if dna_mutated is not None:
    dna_mutated.print_mutations()
else:
    print('No path found')

