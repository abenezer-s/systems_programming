# str_module

The `str_module` is a C module that provides a set of string manipulation functions. It includes two implementations—`str_a.c` and `str_p.c`. The difference between them is that `str_p.c` uses pointers for the functions, while `str_a.c` operates with arrays.

## Files

- **str_a.c**: Implementation using array-based strings.
- **str_p.c**: Implementation using pointer-based strings.

## Functions

The module provides the following functions for string manipulation:

### `size_t str_getLength(const char *s);`
Returns the length of the string `s` (excluding the null terminator).

### `char *str_copy(char s1[], const char s2[]);`
Copies the string `s2` to `s1`. Returns a pointer to `s1`.

### `char *str_ncopy(char s1[], const char s2[], size_t n);`
Copies at most `n` characters from the string `s2` to `s1`. Returns a pointer to `s1`.

### `char *str_concat(char *s1, const char *s2);`
Concatenates the string `s2` to the end of `s1`. Returns a pointer to the resulting string `s1`.

### `char *str_nconcat(char *s1, const char *s2, size_t n);`
Concatenates at most `n` characters from `s2` to the end of `s1`. Returns a pointer to the resulting string `s1`.

## Differences Between Implementations

- **`str_a.c`**: Uses array notation for handling strings.
- **`str_p.c`**: Uses pointers for string manipulation, which may offer more flexibility and efficiency in certain cases.

## Usage

To use the module, simply include the corresponding header file and compile the appropriate implementation (`str_a.c` or `str_p.c`) based on your preference for array or pointer-based string handling.

