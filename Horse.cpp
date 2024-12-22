#include "Horse.h"

#include <utility>

Horse::Horse(int horseId, int speed) : m_horseId(horseId), m_herdId(-1),
                                       m_speed(speed), m_herdPos(INITVALUE),
                                       m_followCycle(INITVALUE) {}


int Horse::getId() const { return m_horseId; }

void Horse::setHerdId(int herdId) {
    m_herdId = herdId;
}

SortedLinkedList<std::weak_ptr<Horse>> Horse::getFollowers() {
    return m_followers;
}

std::weak_ptr<Horse> Horse::getLeadingHorse() {
    return m_leadingHorse;
}

void Horse::addFollower(const std::weak_ptr<Horse> &follower) {
    m_followers.insert(follower, follower.lock()->getId());
}

void Horse::setLeaderHorse(std::weak_ptr<Horse> leader) {
    m_leadingHorse = std::move(leader);
}

int Horse::getHerdId() const { return m_herdId; }

bool Horse::isInHerd() {
    return m_herdId != INITVALUE;
}

int Horse::getSpeed() const { return m_speed; }

void Horse::clearFollowers() {
    m_followers.clear();
}

int Horse::getHerdPosition() const { return m_herdPos; }

void Horse::setHerdPosition(int inorderPosition) {
    m_herdPos = inorderPosition;
}

int Horse::getFollowCycle() const { return m_followCycle; }

void Horse::setFollowCycle(int followCycle) {
    m_followCycle = followCycle;
}

void Horse::setLeaderToNull() {
    m_leadingHorse.reset();
}

void Horse::removeFollower(const std::weak_ptr<Horse> &follower) {
    m_followers.remove(
            follower.lock()->getId());
}