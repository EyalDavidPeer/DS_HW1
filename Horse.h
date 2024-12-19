#pragma once

#include <utility>

#include "memory"
#include "Node.h"
#include "LinkedList.h"
//#include "SortedLinkedList.h"

#define NOTINHERD -1

class Horse {
    int m_horseId;
    int m_herdId;
    int m_speed;
    int m_herdPos;
    std::weak_ptr<Horse> m_leadingHorse;
    SortedLinkedList<std::weak_ptr<Horse>> m_followers;

public:
    Horse(int horseId, int speed) : m_horseId(horseId), m_herdId(-1),
                                    m_speed(speed), m_herdPos(NOTINHERD) {}


    int getId() const { return m_horseId; }

    void setHerdId(int herdId) {
        m_herdId = herdId;
    }

    SortedLinkedList<std::weak_ptr<Horse>> getFollowers() {
        return m_followers;
    }

    std::weak_ptr<Horse> getLeadingHorse() {
        return m_leadingHorse;
    }

    void addFollower(std::weak_ptr<Horse> follower) {
        m_followers.insert(follower, follower.lock()->getId());
    }

    void setLeaderHorse(std::weak_ptr<Horse> leader) {
        m_leadingHorse = leader;
    }

    int getHerdId() const { return m_herdId; }

    bool isInHerd() {
        return m_herdId != NOTINHERD;
    }

    int getSpeed() const { return m_speed; }

    void clearFollowers() {
        m_followers.clear();  // Assuming clear works in O(1) for your LinkedList structure
    }

    void setLeaderToNull() {
        m_leadingHorse.reset();
    }

    // Remove follower in O(1)
    void removeFollower(std::weak_ptr<Horse> follower) {
//        if(follower.lock()->getId() == 386407){
//
//        }
        m_followers.remove(
                follower.lock()->getId());
    }

//TODO: Reduce the duplications
    bool operator<(const Horse &other) const {
        return m_horseId < other.m_horseId;
    }

    bool operator>(const Horse &other) const {
        return m_horseId > other.m_horseId;
    }

    bool operator==(const Horse &other) const {
        return m_horseId == other.m_horseId;
    }
};