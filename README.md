# Calculating edit distance between two sequences using 4 Russians method

## Build project

```
make
```

This will generate two files

```
main
generator
```
## Run project
### generator
By running generator you will get output file containing two random sequences, each string in a single line. When running generator you can specify the length of the generated strings (default value 100).
generator usage:

```
./generator [inputFilename, firstSequenceLength, secondSequenceLength]
```

default filename is input.txt, default sequence length is 100. If only a single (first) sequence length is provided,
both sequence length will have the same length.

Example runs:
```
./generator
./generator twoSequences.txt
./generator input 1000 12000
```
First example will generate a file input.txt with two sequences of length 1000, each in its own line.
Second example will generate same thing, but in file twoSequences.txt
Third example will generate two sequences of length 1000 and 12000, and store it to file 'input'

### main
After generating the strings, you can run the main file to get the edit distance between those two sequences.
main takes 3 arguments; block size, input file name (containing two sequences) and output file name (in MAF format).

```
./main block_size input_file.txt output.maf
```
help message is generated on run with insufficient or incorrect parameters.

