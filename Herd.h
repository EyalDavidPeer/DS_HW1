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

        int horseId = horseShared->getId();
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
        if (!m_horses.search(horseId)) {  // Ensure horse is in the list
            return;
        }

        m_horses.remove(horseId);
        n_herd--;
//        if (horseId == 478719) {
//            printf("After removal\n\n");
//            getHorses().printInOrder();
//        }


        // Handle leader-follower relationships safely
        if (auto leaderHorse = horseShared->getLeadingHorse().lock()) {
            leaderHorse->removeFollower(horse);  // O(1) operation
        }

        horseShared->clearFollowers();  // O(1) operation
        horseShared->setLeaderToNull();  // O(1) operation
        horseShared.reset();  // Expire the weak pointer

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