#include "Horse.h"

Horse::Horse(int horseId, int speed) : m_horseId(horseId), m_herdId(-1),
                                       m_speed(speed), m_herdPos(INITVALUE),
                                       visited(false) {}

int Horse::getId() const { return m_horseId; }

void Horse::setHerdId(int herdId) {
    m_herdId = herdId;
}

std::weak_ptr<Horse> Horse::getLeadingHorse() {
    return m_leadingHorse;
}

void Horse::setLeaderHorse(std::weak_ptr<Horse> leader) {
    m_leadingHorse = std::move(leader);
}

int Horse::getHerdId() const { return m_herdId; }

bool Horse::isInHerd() {
    return m_herdId != INITVALUE;
}

int Horse::getSpeed() const { return m_speed; }

int Horse::getHerdPosition() const { return m_herdPos; }

void Horse::setHerdPosition(int inorderPosition) {
    m_herdPos = inorderPosition;
}

bool Horse::getVisited() const { return visited; }

void Horse::setVisited(bool value) {
    visited = value;
}

void Horse::setLeaderToNull() {
    m_leadingHorse.reset();
}
