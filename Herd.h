#pragma once

#include "Horse.h"

class Herd {
    int m_herdId;
    SortedLinkedList<std::weak_ptr<Horse>> m_horses;
    int m_n_herd;
public:
    explicit Herd(int herdId);

    int getId() const;


    void insertHorse(const std::weak_ptr<Horse> &horse);

    int getNumberOfHorses();

    SortedLinkedList<std::weak_ptr<Horse>> &getHorses();

    bool isEmpty();

    void removeHorse(const std::weak_ptr<Horse> &horse);

};