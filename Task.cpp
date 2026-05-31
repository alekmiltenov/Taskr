#include "TaskSystem.h"

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
