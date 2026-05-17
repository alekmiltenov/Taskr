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
