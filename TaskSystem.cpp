#include "TaskSystem.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

// ---------- Helper functions ----------

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

bool intToStatus(int choice, Status& status) {
    switch (choice) {
        case 1:
            status = Status::Todo;
            return true;
        case 2:
            status = Status::InProgress;
            return true;
        case 3:
            status = Status::Done;
            return true;
        default:
            return false;
    }
}

bool intToPriority(int choice, Priority& priority) {
    switch (choice) {
        case 1:
            priority = Priority::Low;
            return true;
        case 2:
            priority = Priority::Medium;
            return true;
        case 3:
            priority = Priority::High;
            return true;
        default:
            return false;
    }
}

static int priorityValue(Priority priority) {
    switch (priority) {
        case Priority::Low:
            return 1;
        case Priority::Medium:
            return 2;
        case Priority::High:
            return 3;
    }

    return 0;
}

static int statusValue(Status status) {
    switch (status) {
        case Status::Todo:
            return 1;
        case Status::InProgress:
            return 2;
        case Status::Done:
            return 3;
    }

    return 0;
}

static std::string toLower(std::string text) {
    for (char& ch : text) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }

    return text;
}

static std::vector<std::string> splitLine(const std::string& line, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string part;

    while (std::getline(ss, part, delimiter)) {
        parts.push_back(part);
    }

    return parts;
}

// ---------- WorkItem ----------

WorkItem::WorkItem(int id, const std::string& title, const std::string& deadline)
    : id(id), title(title), deadline(deadline) {}

int WorkItem::getId() const {
    return id;
}

const std::string& WorkItem::getTitle() const {
    return title;
}

const std::string& WorkItem::getDeadline() const {
    return deadline;
}

void WorkItem::setTitle(const std::string& newTitle) {
    title = newTitle;
}

void WorkItem::setDeadline(const std::string& newDeadline) {
    deadline = newDeadline;
}

std::ostream& operator<<(std::ostream& os, const WorkItem& item) {
    item.print(os);
    return os;
}

// ---------- Task ----------

Task::Task(int id, const std::string& title, const std::string& deadline,
           const std::string& description, Priority priority, Status status)
    : WorkItem(id, title, deadline),
      description(description),
      priority(priority),
      status(status) {}

const std::string& Task::getDescription() const {
    return description;
}

Priority Task::getPriority() const {
    return priority;
}

Status Task::getStatus() const {
    return status;
}

void Task::setDescription(const std::string& newDescription) {
    description = newDescription;
}

void Task::setPriority(Priority newPriority) {
    priority = newPriority;
}

void Task::setStatus(Status newStatus) {
    status = newStatus;
}

void Task::print(std::ostream& os) const {
    os << "  [Task #" << id << "] " << title
       << " | deadline: " << deadline
       << " | " << description
       << " | priority: " << priorityToString(priority)
       << " | status: " << statusToString(status);
}

// ---------- Project ----------

Project::Project(int id, const std::string& title, const std::string& deadline)
    : WorkItem(id, title, deadline) {}

void Project::addTask(const Task& task) {
    tasks.push_back(task);
}

Task* Project::findTaskById(int taskId) {
    for (Task& task : tasks) {
        if (task.getId() == taskId) {
            return &task;
        }
    }

    return nullptr;
}

const Task* Project::findTaskById(int taskId) const {
    for (const Task& task : tasks) {
        if (task.getId() == taskId) {
            return &task;
        }
    }

    return nullptr;
}

bool Project::deleteTask(int taskId) {
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->getId() == taskId) {
            tasks.erase(it);
            return true;
        }
    }

    return false;
}

std::vector<const Task*> Project::filterByStatus(Status status) const {
    std::vector<const Task*> result;

    for (const Task& task : tasks) {
        if (task.getStatus() == status) {
            result.push_back(&task);
        }
    }

    return result;
}

std::vector<const Task*> Project::filterByPriority(Priority priority) const {
    std::vector<const Task*> result;

    for (const Task& task : tasks) {
        if (task.getPriority() == priority) {
            result.push_back(&task);
        }
    }

    return result;
}

void Project::sortTasks(SortType sortType) {
    switch (sortType) {
        case SortType::Title:
            std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
                return a.getTitle() < b.getTitle();
            });
            break;

        case SortType::Deadline:
            std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
                return a.getDeadline() < b.getDeadline();
            });
            break;

        case SortType::Priority:
            std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
                return priorityValue(a.getPriority()) > priorityValue(b.getPriority());
            });
            break;

        case SortType::Status:
            std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
                return statusValue(a.getStatus()) < statusValue(b.getStatus());
            });
            break;
    }
}

const std::vector<Task>& Project::getTasks() const {
    return tasks;
}

void Project::print(std::ostream& os) const {
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

// ---------- TaskManager ----------

TaskManager::TaskManager()
    : nextProjectId(1), nextTaskId(1) {}

Project* TaskManager::findProjectById(int projectId) {
    for (Project& project : projects) {
        if (project.getId() == projectId) {
            return &project;
        }
    }

    return nullptr;
}

const Project* TaskManager::findProjectById(int projectId) const {
    for (const Project& project : projects) {
        if (project.getId() == projectId) {
            return &project;
        }
    }

    return nullptr;
}

int TaskManager::addProject(const std::string& title, const std::string& deadline) {
    int id = nextProjectId;
    projects.emplace_back(id, title, deadline);
    nextProjectId++;
    return id;
}

bool TaskManager::updateProject(int projectId, const std::string& newTitle, const std::string& newDeadline) {
    Project* project = findProjectById(projectId);

    if (project == nullptr) {
        return false;
    }

    project->setTitle(newTitle);
    project->setDeadline(newDeadline);
    return true;
}

bool TaskManager::deleteProject(int projectId) {
    for (auto it = projects.begin(); it != projects.end(); ++it) {
        if (it->getId() == projectId) {
            projects.erase(it);
            return true;
        }
    }

    return false;
}

bool TaskManager::addTask(int projectId, const std::string& title,
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

bool TaskManager::updateTask(int taskId, const std::string& newTitle, const std::string& newDeadline,
                             const std::string& newDescription, Priority newPriority, Status newStatus) {
    for (Project& project : projects) {
        Task* task = project.findTaskById(taskId);

        if (task != nullptr) {
            task->setTitle(newTitle);
            task->setDeadline(newDeadline);
            task->setDescription(newDescription);
            task->setPriority(newPriority);
            task->setStatus(newStatus);
            return true;
        }
    }

    return false;
}

bool TaskManager::deleteTask(int taskId) {
    for (Project& project : projects) {
        if (project.deleteTask(taskId)) {
            return true;
        }
    }

    return false;
}

void TaskManager::displayAll() const {
    if (projects.empty()) {
        std::cout << "No projects yet.\n";
        return;
    }

    for (const Project& project : projects) {
        std::cout << project << "\n";
    }
}

void TaskManager::filterByStatus(Status status) const {
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

void TaskManager::filterByPriority(Priority priority) const {
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

void TaskManager::sortTasks(SortType sortType) {
    for (Project& project : projects) {
        project.sortTasks(sortType);
    }
}

void TaskManager::search(const std::string& keyword) const {
    std::string loweredKeyword = toLower(keyword);
    bool foundAny = false;

    for (const Project& project : projects) {
        bool projectPrinted = false;

        std::string projectText = project.getTitle() + " " + project.getDeadline();

        if (toLower(projectText).find(loweredKeyword) != std::string::npos) {
            std::cout << project << "\n";
            foundAny = true;
            projectPrinted = true;
        }

        if (!projectPrinted) {
            for (const Task& task : project.getTasks()) {
                std::string taskText = task.getTitle() + " " + task.getDeadline() + " " + task.getDescription();

                if (toLower(taskText).find(loweredKeyword) != std::string::npos) {
                    if (!projectPrinted) {
                        std::cout << "Project: " << project.getTitle() << "\n";
                        projectPrinted = true;
                    }

                    std::cout << task << "\n";
                    foundAny = true;
                }
            }
        }
    }

    if (!foundAny) {
        std::cout << "No results found for: " << keyword << "\n";
    }
}

bool TaskManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    file << "NEXT|" << nextProjectId << "|" << nextTaskId << "\n";

    for (const Project& project : projects) {
        file << "PROJECT|"
             << project.getId() << "|"
             << project.getTitle() << "|"
             << project.getDeadline() << "\n";

        for (const Task& task : project.getTasks()) {
            file << "TASK|"
                 << project.getId() << "|"
                 << task.getId() << "|"
                 << task.getTitle() << "|"
                 << task.getDeadline() << "|"
                 << task.getDescription() << "|"
                 << priorityValue(task.getPriority()) << "|"
                 << statusValue(task.getStatus()) << "\n";
        }
    }

    return true;
}

bool TaskManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    std::vector<Project> loadedProjects;
    int loadedNextProjectId = 1;
    int loadedNextTaskId = 1;

    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::vector<std::string> parts = splitLine(line, '|');

        if (parts.empty()) {
            continue;
        }

        if (parts[0] == "NEXT" && parts.size() == 3) {
            loadedNextProjectId = std::stoi(parts[1]);
            loadedNextTaskId = std::stoi(parts[2]);
        } else if (parts[0] == "PROJECT" && parts.size() == 4) {
            int projectId = std::stoi(parts[1]);
            loadedProjects.emplace_back(projectId, parts[2], parts[3]);
        } else if (parts[0] == "TASK" && parts.size() == 8) {
            int projectId = std::stoi(parts[1]);
            int taskId = std::stoi(parts[2]);

            Priority priority = Priority::Low;
            Status status = Status::Todo;

            int priorityNumber = std::stoi(parts[6]);
            int statusNumber = std::stoi(parts[7]);

            intToPriority(priorityNumber, priority);
            intToStatus(statusNumber, status);

            for (Project& project : loadedProjects) {
                if (project.getId() == projectId) {
                    project.addTask(Task(taskId, parts[3], parts[4], parts[5], priority, status));
                    break;
                }
            }
        }
    }

    projects = loadedProjects;
    nextProjectId = loadedNextProjectId;
    nextTaskId = loadedNextTaskId;

    return true;
}
