*This project has been created as part of the 42 curriculum by dbinti-m, bpichyal.*

## MINISHELL

## Description

A simplified shell implementation in C, inspired by bash. The goal is to understand how a shell works under the hood - parsing commands, managing processes, handling signals, and dealing with file descriptors.

The shell reads user input, tokenizes it, builds an abstract syntax tree, and executes commands while handling pipes, redirections, and environment variables.

## Instructions

## Compilation

```
make
make bonus
```

## Execution

```
./minishell
./minishell_bonus
```

Run in interactive mode and you'll get a prompt where you can type commands.

## Features

- Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Pipes (`|`) and multiple pipes
- Redirections: `<`, `>`, `>>`, `<<` (heredoc)
- Logical operators: `&&`, `||`
- Parentheses for grouping commands
- Environment variable expansion (`$VAR`, `$?`)
- Single and double quote handling
- Wildcard expansion (`*`)
- Signal handling (Ctrl+C, Ctrl+D, Ctrl+\)

## Examples

```
minishell$ echo "Hello World"
Hello World

minishell$ ls -la | grep minishell | wc -l
3

minishell$ cat < input.txt > output.txt

minishell$ echo $HOME
/home/user

minishell$ (cd /tmp && pwd) && pwd
/tmp
/home/user

minishell$ export VAR="test" && echo $VAR
test
```

## Resources

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [Writing Your Own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)
- [Shell Command Language - POSIX](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)

## AI Usage

Used AI for debugging tricky edge cases, understanding signal handling behavior, and explaining bash behavior for specific test cases.
