# Simple-Shell

A minimal custom shell implemented in C, mimicking basic shell functionalities. It allows users to execute commands, navigate directories, and interact with the system in a terminal-like environment.

## Features

1. Display a customizable shell prompt with the username and current directory.
2. Execute standard Linux/UNIX commands.
3. Simple argument parsing and error handling.
4. Process management using fork, execvp, and waitpid.

## Getting Started

1. A Linux/UNIX-based operating system.
2. GCC compiler for building the program.

## Error Handling

1. Displays errors for invalid commands, missing arguments, or system call failures.
2. Ensures safe execution and cleanup of child processes.
