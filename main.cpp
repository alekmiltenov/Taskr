#include "TaskSystem.h"

#include <iostream>
#include <limits>
#include <string>

// main.cpp contains only the console interface.
// The real project logic is in TaskSystem.h / TaskSystem.cpp.

static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int readInt(const std::string& message) {
    int value;

    while (true) {
        std::cout << message;
        std::cin >> value;

        if (std::cin) {
            clearInput();
            return value;
        }

        std::cout << "Invalid number. Try again.\n";
        clearInput();
    }
}

static std::string readLine(const std::string& message) {
    std::string value;
    std::cout << message;
    std::getline(std::cin, value);
    return value;
}

static Priority readPriority() {
    Priority priority;

    while (true) {
        std::cout << "Priority:\n";
        std::cout << "1. Low\n";
        std::cout << "2. Medium\n";
        std::cout << "3. High\n";

        int choice = readInt("Choose: ");

        if (intToPriority(choice, priority)) {
            return priority;
        }

        std::cout << "Invalid priority.\n";
    }
}

static Status readStatus() {
    Status status;

    while (true) {
        std::cout << "Status:\n";
        std::cout << "1. Todo\n";
        std::cout << "2. In Progress\n";
        std::cout << "3. Done\n";

        int choice = readInt("Choose: ");

        if (intToStatus(choice, status)) {
            return status;
        }

        std::cout << "Invalid status.\n";
    }
}

static SortType readSortType() {
    while (true) {
        std::cout << "Sort by:\n";
        std::cout << "1. Title\n";
        std::cout << "2. Deadline\n";
        std::cout << "3. Priority\n";
        std::cout << "4. Status\n";

        int choice = readInt("Choose: ");

        switch (choice) {
            case 1:
                return SortType::Title;
            case 2:
                return SortType::Deadline;
            case 3:
                return SortType::Priority;
            case 4:
                return SortType::Status;
            default:
                std::cout << "Invalid sort option.\n";
        }
    }
}

static void projectCrudMenu(TaskManager& manager) {
    while (true) {
        std::cout << "\n--- Project management ---\n";
        std::cout << "1. Add project\n";
        std::cout << "2. Update project\n";
        std::cout << "3. Delete project\n";
        std::cout << "0. Back\n";

        int choice = readInt("Choose: ");

        if (choice == 0) {
            return;
        }

        if (choice == 1) {
            std::string title = readLine("Project title: ");
            std::string deadline = readLine("Deadline: ");

            int id = manager.addProject(title, deadline);
            std::cout << "Project added with ID #" << id << ".\n";
        } else if (choice == 2) {
            int id = readInt("Project ID: ");
            std::string title = readLine("New title: ");
            std::string deadline = readLine("New deadline: ");

            if (manager.updateProject(id, title, deadline)) {
                std::cout << "Project updated.\n";
            } else {
                std::cout << "Project not found.\n";
            }
        } else if (choice == 3) {
            int id = readInt("Project ID: ");

            if (manager.deleteProject(id)) {
                std::cout << "Project deleted.\n";
            } else {
                std::cout << "Project not found.\n";
            }
        } else {
            std::cout << "Invalid choice.\n";
        }
    }
}

static void taskCrudMenu(TaskManager& manager) {
    while (true) {
        std::cout << "\n--- Task management ---\n";
        std::cout << "1. Add task\n";
        std::cout << "2. Update task\n";
        std::cout << "3. Delete task\n";
        std::cout << "0. Back\n";

        int choice = readInt("Choose: ");

        if (choice == 0) {
            return;
        }

        if (choice == 1) {
            int projectId = readInt("Project ID: ");
            std::string title = readLine("Task title: ");
            std::string deadline = readLine("Deadline: ");
            std::string description = readLine("Description: ");
            Priority priority = readPriority();
            Status status = readStatus();

            if (manager.addTask(projectId, title, deadline, description, priority, status)) {
                std::cout << "Task added.\n";
            } else {
                std::cout << "Project not found.\n";
            }
        } else if (choice == 2) {
            int taskId = readInt("Task ID: ");
            std::string title = readLine("New title: ");
            std::string deadline = readLine("New deadline: ");
            std::string description = readLine("New description: ");
            Priority priority = readPriority();
            Status status = readStatus();

            if (manager.updateTask(taskId, title, deadline, description, priority, status)) {
                std::cout << "Task updated.\n";
            } else {
                std::cout << "Task not found.\n";
            }
        } else if (choice == 3) {
            int taskId = readInt("Task ID: ");

            if (manager.deleteTask(taskId)) {
                std::cout << "Task deleted.\n";
            } else {
                std::cout << "Task not found.\n";
            }
        } else {
            std::cout << "Invalid choice.\n";
        }
    }
}
