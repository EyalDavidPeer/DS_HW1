#include "Herd.h"

Herd::Herd(int herdId) : m_herdId(herdId), m_n_herd(0) {}

int Herd::getId() const { return m_herdId; }

void Herd::insertHorse(const std::weak_ptr<Horse> &horse) {
    auto horseShared = horse.lock();
    if (!horseShared) { return; }

    int horseId = horseShared->getId();
    m_horses.insert(horse, horseId);
    m_n_herd++;
}

int Herd::getNumberOfHorses() {
    return m_n_herd;
}

SortedLinkedList<std::weak_ptr<Horse>> &Herd::getHorses() {
    return m_horses;
}

bool Herd::isEmpty() {
    return m_n_herd == 0;
}

void Herd::removeHorse(const std::weak_ptr<Horse> &horse) {
    auto horseShared = horse.lock();
    if (!horseShared) {
        return;
    }

    int horseId = horseShared->getId();
    if (!m_horses.search(horseId)) {
        return;
    }

    if (auto leaderHorse = horseShared->getLeadingHorse().lock()) {
        leaderHorse->removeFollower(horse);
    }

    horseShared->clearFollowers();
    horseShared->setLeaderToNull();
    m_horses.remove(horseId);
    m_n_herd--;


}