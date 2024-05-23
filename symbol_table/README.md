# Symbol Table Implementations

This project provides two different implementations of a **Symbol Table**:

1. **Hash Table-based Symbol Table**: A symbol table implemented using a hash table.
2. **Linked List-based Symbol Table**: A symbol table where symbols are stored in a simple linked list.

### Features

- **Hash Table Implementation**: Provides fast look-up, insertion, and deletion based on the hash of the symbol's name.
- **Linked List Implementation**: Stores symbols in a linear list, ideal for simple or small-scale use cases where the overhead of hashing is not necessary.

---

## Data Structures

### Symbol Table (Common Structure)

Both implementations use a common structure for representing a **symbol**:

- **Symbol**: Contains the symbol's key and value.
  - `key`: The key of the symbol (e.g., a variable name).
  - `value`: The associated value of the symbol (could be a data type, integer, etc.).
  - `next`: Pointer to the next symbol (in case of a linked list or collision handling in hash table).

### Hash Table-based Symbol Table

This implementation uses a **hash table** where each bucket contains a linked list. The hash table resolves collisions by chaining symbols that hash to the same bucket.

## Files

- **`symTable_hash.h`**: Header file that declares the data structures and function prototypes.
- **`symTable_hash.c`**: Contains the implementation of the symbol table for linked list methods.
- **`main.c`**: Example program demonstrating the usage of the symbol table.


### Linked List-based Symbol Table

This implementation uses a **single linked list** to store symbols sequentially. It does not use hashing or collision handling, and each symbol is stored in the order it is inserted.

---

## Files

- **`symTable_list.h`**: Header file that declares the data structures and function prototypes.
- **`symTable_list.c`**: Contains the implementation of the symbol table for linked list methods.
- **`main.c`**: Example program demonstrating the usage of the symbol table.

---

## Compilation and Usage

### Compilation

To compile the program, use the following command:

```bash
gcc -o symTable_list symTable_list.c main.c
gcc -o symTable_hash symTable_hash.c main.c
```

This will create an executable named `symTable_list` and `symTable_hash`.

### Usage
To run the executables run the following commands:
```bash
./symTable_list
./symTable_hash
```

