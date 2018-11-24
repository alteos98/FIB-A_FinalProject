from __future__ import print_function

import string
import sys
from random import shuffle

n_permutations = int(sys.argv[1])
output_folder = sys.argv[2]

input_data = sys.stdin.read().lower().translate(None, string.punctuation)
words = input_data.split()

print('number of words:', len(words))

for i in range(n_permutations):
    shuffle(words)
    with open('%s/doc_%d.txt' % (output_folder, i), 'w') as f:
        f.write(' '.join(words))

print('done!')
