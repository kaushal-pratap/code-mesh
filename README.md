# Code Mesh

Code Mesh is a C code analysis and visualization tool built around Clang's AST.

The goal of Code Mesh is to parse C source code, understand its structure and relationships, transform that information into a structured representation, and visualize it through an interactive frontend.

## Vision

Code Mesh aims to make the structure of a C program easier to understand by turning source code into a visual representation of its underlying structure.

The project is intended to eventually analyse and visualize:

- Source files and directories
- Variables and declarations
- Functions and function calls
- Arrays and matrices
- Structs and data structures
- Expressions
- Dependencies and relationships between code elements

The overall pipeline is:

```text
C Source Code
      │
      ▼
   Clang AST
      │
      ▼
 AST Traversal
      │
      ▼
   Analysis
      │
      ▼
 Code Mesh Model
      │
      ▼
    JSON
      │
      ▼
 Interactive Frontend
```

## Current Development

The project is currently focused on building the AST analysis engine using Clang's `libclang` C API.

Current work includes:

- Traversing the C AST
- Identifying variable declarations
- Inspecting C types
- Detecting 2D arrays and matrices
- Extracting matrix dimensions
- Detecting matrix initializers
- Traversing initializer expressions
- Evaluating initializer values
- Extracting initialized matrix elements

For example, Code Mesh can analyse a declaration such as:

```c
double matrix[3][4] = {
    {1.0, 2.2, 3.8, 4.2},
    {5.0, 6.0, 7.0, 8.0},
    {9.0, 10.0, 11.0, 12.0}
};
```

and determine that it represents a `3 × 4` matrix and inspect the values contained in its initializer.

## Architecture

The project is being developed with a separation of responsibilities between the AST traversal, analysis logic, data model, and frontend.

```text
                C Source
                   │
                   ▼
              libclang
                   │
                   ▼
                  AST
                   │
                   ▼
            AST Traversal
                   │
        ┌──────────┼──────────┐
        ▼          ▼          ▼
     Matrix     Function    Other
    Analyzer    Analyzer    Analyzers
        │          │          │
        └──────────┼──────────┘
                   ▼
             Code Mesh Model
                   │
                   ▼
                 JSON
                   │
                   ▼
               Frontend
```

The AST traversal is responsible for navigating the tree, while individual analyzers interpret specific parts of the source code.

The resulting information is represented independently of Clang so that the frontend does not need to understand the underlying AST.

## Project Structure

```text
Code Mesh/
│
├── include/
│   ├── exporter/
│   ├── matrix/
│   └── scanner/
│
├── src/
│   ├── exporter/
│   ├── matrix/
│   └── scanner/
│
├── test/
│
├── web/
│
├── Makefile
├── render.js
└── run.sh
```

The structure is evolving as the analysis engine develops.

## Technologies

- C
- Clang / LLVM
- libclang C API
- Make
- JavaScript
- HTML
- CSS

## Development Status

🚧 **Work in Progress**

Code Mesh is currently in the early development stage. The AST analysis engine is being built incrementally, beginning with C arrays and matrices and gradually expanding toward broader source-code analysis and visualization.

## Author

**Kaushal Pratap**

GitHub: https://github.com/kaushal-pratap
