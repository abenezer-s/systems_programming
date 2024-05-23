# HeapManager

## Overview

This repository contains two implementations of a heap manager written in C. Both implementations aim to manage memory efficiently using dynamic memory allocation strategies, but they use different approaches:

1. **Heap Manager 1 (`heapmgr1.c`)**:  
   This implementation uses a **single doubly-linked list** to manage memory chunks, each with a **header** and **footer** to track their size and allocation status.

2. **Heap Manager 2 (`heapmgr2.c`)**:  
   This implementation uses **multiple doubly-linked lists**, to manage memory more efficiently, particularly for different block sizes.

The repository also includes a test program that exercises the `HeapMgr_malloc()` and `HeapMgr_free()` functions. This test can be compiled with either of the heap manager implementations, and it runs several memory allocation and deallocation patterns.

## Structure

- `heapmgr1.c`, `heapmgr1.h`: First heap manager implementation (single doubly-linked list).
- `heapmgr2.c`, `heapmgr2.h`: Second heap manager implementation (multiple alias bins).
- `testheapmgr.c`: Test program that can be compiled with either `heapmgr1.c` or `heapmgr2.c`.
- `Makefile`: Build system for compiling the program.
- `chunk.c`, `chunk.h`: Utility functions and structures used for managing memory chunks.
- `README.md`: This file.

## Compilation

To compile the program with either heap manager implementation, specify which implemntation you want to test.

### Example:

```bash
# To compile with heapmgr1.c (default):
make testheapmgr1

# To compile with heapmgr2.c:
make testheapmgr2
```

This will generate an executable called `testheapmgr1` or `testheamgr2` which you can run with various command-line arguments to test the heap manager.

## Usage

### Running Tests 

The test program tests the `HeapMgr_malloc()` and `HeapMgr_free()` functions with different allocation patterns. The program accepts three command-line arguments:

1. `argv[1]`: **Test Type**
   - `LifoFixed`: LIFO (Last In First Out) with fixed-size chunks.
   - `FifoFixed`: FIFO (First In First Out) with fixed-size chunks.
   - `LifoRandom`: LIFO with random-size chunks.
   - `FifoRandom`: FIFO with random-size chunks.
   - `RandomFixed`: Random order with fixed-size chunks.
   - `RandomRandom`: Random order with random-size chunks.
   - `Worst`: Worst-case scenario for a single linked list implementation.

2. `argv[2]`: **Number of Allocations**
   - This is the number of times `HeapMgr_malloc()` and `HeapMgr_free()` will be called. The number cannot exceed `1000000`.

3. `argv[3]`: **Maximum Chunk Size**
   - This specifies the maximum size for each memory chunk allocated.

### Example:

```bash
# Run a LIFO test with fixed-size chunks, 10,000 malloc/free calls, and chunk size of 12,000 bytes:
./testheapmgr1 LifoFixed 10000 12000

# Run a random order test with random-size chunks, 500 malloc/free calls, and chunk size of 256 bytes:
./testheapmgr2 RandomRandom 500 256
```

### Notes:
- If `NDEBUG` is **not defined** during compilation (e.g., during a debug build), the program will also initialize and check the contents of each allocated memory chunk to verify integrity.
- After the test is complete, the program will output the heap memory usage and the CPU time consumed to `stdout`.

### Test Patterns

The `testheapmgr` program allows you to test different patterns of memory allocation and deallocation:

1. **LifoFixed**: Allocates and frees memory in a LIFO order with chunks of fixed size.
2. **FifoFixed**: Allocates and frees memory in a FIFO order with chunks of fixed size.
3. **LifoRandom**: Allocates and frees memory in a LIFO order with chunks of random sizes.
4. **FifoRandom**: Allocates and frees memory in a FIFO order with chunks of random sizes.
5. **RandomFixed**: Allocates and frees memory in a random order with chunks of fixed size.
6. **RandomRandom**: Allocates and frees memory in a random order with chunks of random sizes.
7. **Worst**: Tests the worst-case performance for a single linked list implementation (mostly relevant for `heapmgr1.c`).

## File Descriptions

### `heapmgr1.c`:
This file implements a simple heap manager using a **single doubly-linked list**. Each chunk of memory is represented by a structure that includes both a **header** and a **footer** to store metadata like the size of the chunk and its allocation status.

### `heapmgr2.c`:
This file implements a more sophisticated heap manager using **multiple doubly-linked lists** (bins) to categorize chunks by size. This approach is intended to reduce fragmentation and improve allocation/deallocation efficiency, especially when dealing with varying chunk sizes.

### `testheapmgr.c`:
The test program that exercises the heap manager. It runs a series of tests based on the test pattern specified in `argv[1]` and uses the number of malloc/free calls and chunk size specified in `argv[2]` and `argv[3]`.

### `chunk.c` & `chunk.h`:
These files define the structures and helper functions related to memory chunks. A chunk is a contiguous block of memory managed by the heap manager, and it contains metadata such as size and pointers to neighboring chunks.

## Learnings

In this section, I summarize the key concepts and techniques I learned during this project:

### Debugging using GDB

I learned how to effectively use the GNU Debugger (**GDB**) to debug C programs. Key techniques include:
- **Setting breakpoints** to pause the execution of the program at specific lines or functions.
- **Stepping through code** line by line to observe variable values and program flow.
- **Inspecting memory and variables** using commands like `print` and `info locals` to view the current state of variables and data structures.
- **Backtrace** to trace the function calls leading to a crash.

GDB proved invaluable in tracking down segmentation faults and memory issues during development.

### Problem Solving

Through this project, I honed my problem-solving skills by:
- Breaking complex problems into smaller, more manageable sub-problems.
- Approaching each problem methodically by first understanding the requirements and then designing a solution step-by-step.
- Iterating on solutions and debugging code, identifying edge cases, and optimizing performance.

### Data Structures

I gained a deeper understanding of data structures, especially:
- **Linked Lists**: Implementing and manipulating multiple and single doubly-linked lists to manage memory chunks in the heap manager.

### Memory Management

This project involved extensive work with memory allocation and deallocation:
### Memory Management

This project involved extensive work with memory allocation and deallocation:
- **Memory Allocation and Deallocation**: I learned the intricacies of `malloc()` and `free()` implementations. This involved understanding how the heap manager allocates and frees memory, manages chunk sizes, and handles memory fragmentation.
- **Memory Fragmentation**: I explored how memory fragmentation can occur over time and how different heap management strategies (like using doubly-linked lists and alias bins) mitigate this.
- **Memory Debugging**: By using tools like `GDB` and `valgrind`, I learned how to detect and debug memory leaks, invalid memory accesses, and incorrect memory deallocations.
