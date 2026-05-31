#pragma once

#include <iostream>
#include <string>
#include <vector>

// These enums make the code safer than using random strings like "done" or "high".
enum class Status {
    Todo,
    InProgress,
    Done
};

enum class Priority {
    Low,
    Medium,
    High
};

enum class SortType {
    Title,
    Deadline,
    Priority,
    Status
};

std::string statusToString(Status status);
std::string priorityToString(Priority priority);

bool intToStatus(int choice, Status& status);
bool intToPriority(int choice, Priority& priority);

// ABSTRACT BASE CLASS
// This class represents the shared idea of a "work item".
// Both Task and Project have id, title, deadline, and can be printed.
class WorkItem {
protected:
    // protected: children can use these fields in their print() methods,
    // but outside code cannot directly change them.
    int id;
    std::string title;
    std::string deadline;

public:
    WorkItem(int id, const std::string& title, const std::string& deadline);
    virtual ~WorkItem() = default;

    int getId() const;
    const std::string& getTitle() const;
    const std::string& getDeadline() const;

    void setTitle(const std::string& newTitle);
    void setDeadline(const std::string& newDeadline);

    // POLYMORPHISM + ABSTRACTION:
    // WorkItem cannot be created directly because print() is pure virtual.
    // Task and Project must implement their own version of print().
    virtual void print(std::ostream& os) const = 0;
};

// operator<< works with any WorkItem.
// Because print() is virtual, it calls Task::print or Project::print automatically.
std::ostream& operator<<(std::ostream& os, const WorkItem& item);

// INHERITANCE:
// Task is a specific type of WorkItem.
class Task : public WorkItem {
private:
    // ENCAPSULATION:
    // Task details are private and are accessed/changed through methods.
    std::string description;
    Priority priority;
    Status status;

public:
    Task(int id, const std::string& title, const std::string& deadline,
         const std::string& description, Priority priority, Status status);

    const std::string& getDescription() const;
    Priority getPriority() const;
    Status getStatus() const;

    void setDescription(const std::string& newDescription);
    void setPriority(Priority newPriority);
    void setStatus(Status newStatus);

    void print(std::ostream& os) const override;
};

// INHERITANCE:
// Project is also a WorkItem, but it additionally contains tasks.
class Project : public WorkItem {
private:
    // ENCAPSULATION:
    // The vector is private, so tasks cannot be changed directly from outside.
    // Outside code must use addTask(), deleteTask(), filterByStatus(), etc.
    std::vector<Task> tasks;

public:
    Project(int id, const std::string& title, const std::string& deadline);

    void addTask(const Task& task);

    Task* findTaskById(int taskId);
    const Task* findTaskById(int taskId) const;

    bool deleteTask(int taskId);

    std::vector<const Task*> filterByStatus(Status status) const;
    std::vector<const Task*> filterByPriority(Priority priority) const;

    void sortTasks(SortType sortType);

    const std::vector<Task>& getTasks() const;

    void print(std::ostream& os) const override;
};

// TaskManager controls all projects and creates unique IDs.
class TaskManager {
private:
    std::vector<Project> projects;
    int nextProjectId;
    int nextTaskId;

    Project* findProjectById(int projectId);
    const Project* findProjectById(int projectId) const;

public:
    TaskManager();

    // Feature 1: Project CRUD grouped together.
    int addProject(const std::string& title, const std::string& deadline);
    bool updateProject(int projectId, const std::string& newTitle, const std::string& newDeadline);
    bool deleteProject(int projectId);

    // Feature 2: Task CRUD grouped together.
    bool addTask(int projectId, const std::string& title,
                 const std::string& deadline, const std::string& description,
                 Priority priority, Status status);
    bool updateTask(int taskId, const std::string& newTitle, const std::string& newDeadline,
                    const std::string& newDescription, Priority newPriority, Status newStatus);
    bool deleteTask(int taskId);

    // Features 3-9.
    void displayAll() const;
    void filterByStatus(Status status) const;
    void filterByPriority(Priority priority) const;
    void sortTasks(SortType sortType);
    void search(const std::string& keyword) const;

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
};
