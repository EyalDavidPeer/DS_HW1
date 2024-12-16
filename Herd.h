#pragma once

#include <utility>

#include "Horse.h"
#include "memory"

class Herd {
    int m_herdId;
    SortedLinkedList<std::weak_ptr<Horse>> m_horses;
    int n_herd;
public:
    explicit Herd(int herdId) : m_herdId(herdId), n_herd(0) {}

    int getId() const { return m_herdId; }


    void insertHorse(std::weak_ptr<Horse> horse) {
        auto horseShared = horse.lock();
        if (!horseShared) { return; }

        int horseId = horseShared->getId(); // Replace getId() with the appropriate method
        m_horses.insert(horse, horseId);
        n_herd++;
    }

    int getNumberOfHorses() {
        return n_herd;
    }

    SortedLinkedList<std::weak_ptr<Horse>> getHorses() {
        return m_horses;
    }

    bool isEmpty() {
        return n_herd == 0;
    }

    void removeHorse(std::weak_ptr<Horse> horse) {
        auto horseShared = horse.lock();
        if (!horseShared) {
            return;
        }

        int horseId = horseShared->getId();
        m_horses.remove(horseId);
        n_herd--;

        // Set the horse's leader and followers to nullptr (weak_ptr will expire)
        if (auto leaderHorse = horseShared->getLeadingHorse().lock()) {
            // Update the leader's followers list if necessary
            leaderHorse->removeFollower(horse);  // O(1) operation
        }

        // Clear all followers' references in O(1)
        horseShared->clearFollowers();  // O(1) operation

        // Optionally, set the horse's leader to nullptr as well
        horseShared->setLeaderToNull();  // O(1) operation
        horseShared.reset();

    }

    //TODO: Reduce the duplications
    bool operator<(const Herd &other) const {
        return m_herdId < other.m_herdId;
    }

    bool operator>(const Herd &other) const {
        return m_herdId > other.m_herdId;
    }

    bool operator==(const Herd &other) const {
        return m_herdId == other.m_herdId;
    }

    bool operator<(const int &other) const {
        return m_herdId < other;
    }

    bool operator>(const int &other) const {
        return m_herdId > other;
    }

    bool operator==(const int &other) const {
        return m_herdId == other;
    }
};