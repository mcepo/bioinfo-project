# Calculating edit distance between two strings using 4 Russians 

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
By running generator you will get input.txt file with two strings, each string in a single line. When running generator you can specifie the length of the generated strings (default value 100).

Example run:
```
./generator 1000
```
Will generate a file input.txt with two strings of length 1000 each in its own line

### main
After generating the strings, you can run the main file to get the edit distance between those two strings. Optionally main takes as an argument filename from which to read the lines (default input.txt)

```
./main 1000
```
