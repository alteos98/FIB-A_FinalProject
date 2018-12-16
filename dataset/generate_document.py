# Genera documents de p paraules DIFERENTS
import sys
import random

usage = """
Usage:
python3 generate_document.py nwords > outputfile
"""

if len(sys.argv) != 2:
    print(usage)
    sys.exit(1)

nwords = int(sys.argv[1])

with open('listado-general.txt') as f:
    words = f.read().strip().split()

output_document = ' '.join(random.sample(words, nwords))

sys.stdout.write(output_document)
