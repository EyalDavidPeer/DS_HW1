#pragma once

#include <utility>

#include "Horse.h"
#include "memory"

class Herd {
    int m_herdId;
    SortedLinkedList<std::weak_ptr<Horse>> m_horses;
    int n_herd;
public:
    explicit Herd(int herdId);

    int getId() const;


    void insertHorse(const std::weak_ptr<Horse> &horse);

    int getNumberOfHorses();

    SortedLinkedList<std::weak_ptr<Horse>> &getHorses();

    bool isEmpty();

    void removeHorse(const std::weak_ptr<Horse> &horse);

};