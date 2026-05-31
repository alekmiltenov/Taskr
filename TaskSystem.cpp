#include "TaskSystem.h"

#include <cctype>
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
