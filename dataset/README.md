# generate_dataset.py - Tool for generating n datasets from one input document.

## what it does
Reads the document, removes all the punctuation, makes all the words lowercase and then generates n permutations of the words read. 

## usage
> python generate\_dataset.py n destination_folder < input\_document.txt 

## example
Generate 20 permutations of the file lorem/input.txt and save them as lorem/doc\_0.txt, lorem/doc\_1.txt, ..., lorem/doc\_19.txt
> python generate\_dataset.py 20 lorem < lorem/input.txt


# generate_document.py - Tool for generating 2 partially different documents 

## what it does
With the dictionary file, it creates two documents of n words, k of them are common in both documents (and the rest different).

## usage
To create two files, each of them with nwords, and ncommonwords common words:
> python3 generate_document.py nwords ncommonwords dest1 dest2


## example
> python3 generate_document.py 50000 25000 spanish/50000/doc_0.txt spanish/50000/doc_1.txt
