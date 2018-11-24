# Tool for generating n datasets from one input document.

## what it does
Reads the document, removes all the punctuation, makes all the words lowercase and then generates n permutations of the words read. 

## usage
> python generate\_dataset.py n destination_folder < input\_document.txt 

## example
Generate 20 permutations of the file lorem/input.txt and save them as lorem/doc\_0.txt, lorem/doc\_1.txt, ..., lorem/doc\_19.txt
> python generate\_dataset.py 20 lorem < lorem/input.txt
