#include "TaskSystem.h"

#include <cctype>
#include <fstream>
#include <sstream>

// ---------- TaskManager helper functions ----------

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
