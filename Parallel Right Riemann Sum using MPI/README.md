# Repository for Parallel and Concurrent Programming Exercises

This repository is designed to store solutions to various exercises related to parallel and concurrent programming using C.

## Getting Started

To get started, clone this repository to your local machine using:

```
git clone https://github.com/DeanVini/parallel-programing-activities
```

## Installing MPICH on Ubuntu

```
sudo apt install mpich
```

### Compilation Example
```
mpicc mpi_hello.c -o mpihello
```

### Execution Example
```
mpiexec -n 4 ./mpihello
```

