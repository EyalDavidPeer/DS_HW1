#pragma once

#include <iostream>

class Histogram {
    int m_n;
    int *m_hist;
public:
    explicit Histogram(int n);

    ~Histogram();

    int &operator[](int index);

    const int &operator[](int index) const;

    Histogram(const Histogram &other);

    Histogram &operator=(const Histogram &other);

};