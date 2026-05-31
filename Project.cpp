#include "TaskSystem.h"

#include <algorithm>

// ---------- Project helper functions ----------

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
