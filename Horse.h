#pragma once

#include <utility>

#include "memory"
#include "Node.h"
#include "LinkedList.h"

#define INITVALUE -1

class Horse {
    int m_horseId;
    int m_herdId;
    int m_speed;
    int m_herdPos;
    int m_followCycle;
    std::weak_ptr<Horse> m_leadingHorse;
    SortedLinkedList<std::weak_ptr<Horse>> m_followers;

public:
    Horse(int horseId, int speed);

    int getId() const;

    void setHerdId(int herdId);

    SortedLinkedList<std::weak_ptr<Horse>> getFollowers();

    std::weak_ptr<Horse> getLeadingHorse();

    void addFollower(const std::weak_ptr<Horse> &follower);

    void setLeaderHorse(std::weak_ptr<Horse> leader);

    int getHerdId() const;

    bool isInHerd();

    int getSpeed() const;

    void clearFollowers();

    int getHerdPosition() const;

    void setHerdPosition(int inorderPosition);

    int getFollowCycle() const;

    void setFollowCycle(int followCycle);

    void setLeaderToNull();

    void removeFollower(const std::weak_ptr<Horse> &follower);

};