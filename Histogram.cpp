#include "Histogram.h"

Histogram::Histogram(int n) : n(n) {
    m_hist = new int[n]();
}

Histogram::~Histogram() {
    delete[] m_hist;
}

int &Histogram::operator[](int index) {
    if (index < 0 || index >= n) {
        throw std::out_of_range("Index out of bounds");
    }
    return m_hist[index];
}

const int &Histogram::operator[](int index) const {
    if (index < 0 || index >= n) {
        throw std::out_of_range("Index out of bounds");
    }
    return m_hist[index];
}

Histogram::Histogram(const Histogram &other) : n(other.n) {
    m_hist = new int[n];
    std::copy(other.m_hist, other.m_hist + n, m_hist);
}

Histogram &Histogram::operator=(const Histogram &other) {
    if (this != &other) {
        int *new_hist = new int[other.n];
        std::copy(other.m_hist, other.m_hist + other.n, new_hist);
        delete[] m_hist;
        m_hist = new_hist;
        n = other.n;
    }
    return *this;
}