# Database

A simple C++ database implementation with an interactive REPL (Read-Eval-Print Loop).

## Features

- Interactive command-line interface
- Built with C++17 standard

## Prerequisites

- GCC compiler (g++)
- Windows (build.bat provided)

## Building

```bash
build.bat
```

This will compile the project and generate `build/database.exe`.

## Running

```bash
build/database.exe
```

Or use the run task:
```bash
npm run build
```

## Project Structure

```
├── src/
│   ├── main.cpp       - Entry point
│   ├── repl.cpp       - REPL implementation
│   └── repl.hpp       - REPL header
├── build/             - Output directory
├── build.bat          - Build script
└── Makefile           - Build configuration
```

## Development

The project uses:
- C++17 standard
- GCC compiler with `-Wall` flag for strict warnings
- Debug symbols enabled (`-g`)
