# GenomeGraphProject

GenomeGraphProject is a project that constructs a directed graph between sequences of genetic mutations. The goal of the project is to find all minimum supersets for each sequence, which can help track variants to their origins and better mitigate the spread of COVID-19.

## Installation

To compile GenomeGraphProject, use the following command:

```
g++ -O3 -o sequence sequence.cpp
```

## Usage

To run GenomeGraphProject, use the following command:

```
./sequence seqX.txt dirX.txt
```

Replace "X" with the file number for a graph to be constructed for.
