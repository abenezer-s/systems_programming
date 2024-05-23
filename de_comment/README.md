# de_comment

The `de_comment` program is a simple C utility designed to extract the content of comments from C source code. It checks for properly closed comments and outputs the content of the comment if it is correctly closed. If a comment is not properly closed, the program will output the line containing the unclosed comment.

## Features

- **Extracts comments**: The program scans a given C source file and extracts comments enclosed in `/* */`.
- **Handles unclosed comments**: If a comment is not properly closed, the program outputs the line containing the unclosed comment.
- **Simple and fast**: The program focuses on basic comment extraction and checks for common issues like missing closing delimiters.

## How It Works

- The program reads the input source file line by line.
- It looks for comment blocks starting with `/*` and ending with `*/`.
- If a comment block is properly closed, the content of the comment (excluding the delimiters) is printed.
- If a comment block is found that is not properly closed (i.e., it doesn't end with `*/`), the program prints the line number containing the unclosed comment for the user to review.

## Compilation and Usage

### Compilation

To compile the program, use the following command:

```bash
gcc -o de_comment de_comment.c main.c
```

This will create an executable named `de_comment`.

### Usage

To run the program, pass the C source file you want to analyze as an argument:

```bash
./de_comment < sample.c
```
To run the program and direct errors to an error file, pass the C source file and another file you want the error redirected to, as an argument:

```bash
./de_comment < sample.c 2> error
```
