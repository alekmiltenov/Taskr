# Task and Project Manager

A console-based C++ application for managing projects and tasks.
The program allows users to create projects, add tasks to them, track deadlines, priorities, and statuses, and organize tasks through filtering, sorting, searching, saving, and loading.

## Features

* Project management: add, update, and delete projects
* Task management: add, update, and delete tasks
* Display all projects and their tasks
* Filter tasks by status
* Filter tasks by priority
* Sort tasks by title, deadline, priority, or status
* Search projects and tasks by keyword
* Save data to a file
* Load data from a file

## Project Structure

```txt
main.cpp          - console menu and user input
TaskSystem.h     - class declarations and public interfaces
TaskSystem.cpp   - class method implementations and core logic
```

## Compilation

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp TaskSystem.cpp -o task_manager
```

## Running

Windows:

```bash
.\task_manager.exe
```

Linux / Git Bash / WSL:

```bash
./task_manager
```

## Menu Options

```txt
1. Project CRUD
2. Task CRUD
3. Display all
4. Filter by status
5. Filter by priority
6. Sort tasks
7. Search
8. Save to file
9. Load from file
0. Exit
```

## Save and Load

The program can save data to a text file and load it later.

Example filename:

```txt
data.txt
```

The save file uses `|` as a separator, so titles and descriptions should not contain the `|` character.
::: 
