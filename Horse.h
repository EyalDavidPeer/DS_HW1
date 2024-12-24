#pragma once

#include "LinkedList.h"

#define INITVALUE -1

class Horse {
    int m_horseId;
    int m_herdId;
    int m_speed;
    int m_herdPos;
    bool visited;
    std::weak_ptr<Horse> m_leadingHorse;

public:
    Horse(int horseId, int speed);

    int getId() const;

    void setHerdId(int herdId);

    std::weak_ptr<Horse> getLeadingHorse();

    void setLeaderHorse(std::weak_ptr<Horse> leader);

    int getHerdId() const;

    bool isInHerd();

    int getSpeed() const;

    int getHerdPosition() const;

    void setHerdPosition(int inorderPosition);

    bool getVisited() const;

    void setVisited(bool value);

    void setLeaderToNull();

};