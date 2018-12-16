# Genera documents de p paraules DIFERENTS
import sys
import random

usage = """
Usage:
python3 generate_document.py nwords nsamewords output1 output2
"""

if len(sys.argv) != 5:
    print(usage)
    sys.exit(1)

nwords = int(sys.argv[1])
nsamewords = int(sys.argv[2])
output1 = sys.argv[3]
output2 = sys.argv[4]


with open('listado-general.txt') as f:
    words = f.read().strip().split()

common = random.sample(words, nsamewords)

doc1 = [w for w in common]
doc2 = [w for w in common]

for w in common:
    words.remove(w)

doc1_words = random.sample(words, nwords - nsamewords)
doc1 += doc1_words

for w in doc1_words:
    words.remove(w)

doc2 += random.sample(words, nwords - nsamewords)


with open(output1, 'w') as f:
    f.write(' '.join(doc1))

with open(output2, 'w') as f:
    f.write(' '.join(doc2))
