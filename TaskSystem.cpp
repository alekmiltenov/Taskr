#include <iostream>
#include <string>
#include <vector>

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

std::string statusToString(Status status) {
    switch (status) {
        case Status::Todo:
            return "Todo";
        case Status::InProgress:
            return "In Progress";
        case Status::Done:
            return "Done";
    }

    return "Unknown";
}

std::string priorityToString(Priority priority) {
    switch (priority) {
        case Priority::Low:
            return "Low";
        case Priority::Medium:
            return "Medium";
        case Priority::High:
            return "High";
    }

    return "Unknown";
}

class WorkItem {
protected:
    int id;
    std::string title;
    std::string deadline;

public:
    WorkItem(int id, const std::string& title, const std::string& deadline)
        : id(id), title(title), deadline(deadline) {}

    virtual ~WorkItem() = default;

    int getId() const {
        return id;
    }

    const std::string& getTitle() const {
        return title;
    }

    const std::string& getDeadline() const {
        return deadline;
    }

    virtual void print(std::ostream& os) const = 0;
};

std::ostream& operator<<(std::ostream& os, const WorkItem& item) {
    item.print(os);
    return os;
}

class Task : public WorkItem {
private:
    std::string description;
    Priority priority;
    Status status;

public:
    Task(int id, const std::string& title, const std::string& deadline,
         const std::string& description, Priority priority, Status status)
        : WorkItem(id, title, deadline),
          description(description),
          priority(priority),
          status(status) {}

    Status getStatus() const {
        return status;
    }

    Priority getPriority() const {
        return priority;
    }

    void setStatus(Status newStatus) {
        status = newStatus;
    }

    void print(std::ostream& os) const override {
        os << "  [Task #" << id << "] " << title
           << " | deadline: " << deadline
           << " | " << description
           << " | priority: " << priorityToString(priority)
           << " | status: " << statusToString(status);
    }
};

class Project : public WorkItem {
private:
    std::vector<Task> tasks;

public:
    Project(int id, const std::string& title, const std::string& deadline)
        : WorkItem(id, title, deadline) {}

    void addTask(const Task& task) {
        tasks.push_back(task);
    }

    std::vector<const Task*> filterByStatus(Status status) const {
        std::vector<const Task*> result;

        for (const Task& task : tasks) {
            if (task.getStatus() == status) {
                result.push_back(&task);
            }
        }

        return result;
    }

    std::vector<const Task*> filterByPriority(Priority priority) const {
        std::vector<const Task*> result;

        for (const Task& task : tasks) {
            if (task.getPriority() == priority) {
                result.push_back(&task);
            }
        }

        return result;
    }

    void print(std::ostream& os) const override {
        os << "[Project #" << id << "] " << title
           << " | deadline: " << deadline << "\n";

        if (tasks.empty()) {
            os << "  (no tasks)\n";
            return;
        }

        for (const Task& task : tasks) {
            os << task << "\n";
        }
    }
};

class TaskManager {
private:
    std::vector<Project> projects;
    int nextProjectId = 1;
    int nextTaskId = 1;

    Project* findProjectById(int projectId) {
        for (Project& project : projects) {
            if (project.getId() == projectId) {
                return &project;
            }
        }

        return nullptr;
    }

public:
    void addProject(const std::string& title, const std::string& deadline) {
        projects.emplace_back(nextProjectId, title, deadline);
        nextProjectId++;
    }

    bool addTask(int projectId, const std::string& title,
                 const std::string& deadline, const std::string& description,
                 Priority priority, Status status) {
        Project* project = findProjectById(projectId);

        if (project == nullptr) {
            return false;
        }

        Task task(nextTaskId, title, deadline, description, priority, status);
        project->addTask(task);
        nextTaskId++;

        return true;
    }

    void displayAll() const {
        if (projects.empty()) {
            std::cout << "No projects yet.\n";
            return;
        }

        for (const Project& project : projects) {
            std::cout << project << "\n";
        }
    }

    void filterByStatus(Status status) const {
        bool foundAny = false;

        for (const Project& project : projects) {
            std::vector<const Task*> results = project.filterByStatus(status);

            if (!results.empty()) {
                foundAny = true;
                std::cout << "Project: " << project.getTitle() << "\n";

                for (const Task* task : results) {
                    std::cout << *task << "\n";
                }
            }
        }

        if (!foundAny) {
            std::cout << "No tasks with status: " << statusToString(status) << "\n";
        }
    }

    void filterByPriority(Priority priority) const {
        bool foundAny = false;

        for (const Project& project : projects) {
            std::vector<const Task*> results = project.filterByPriority(priority);

            if (!results.empty()) {
                foundAny = true;
                std::cout << "Project: " << project.getTitle() << "\n";

                for (const Task* task : results) {
                    std::cout << *task << "\n";
                }
            }
        }

        if (!foundAny) {
            std::cout << "No tasks with priority: " << priorityToString(priority) << "\n";
        }
    }
};
