#include "Histogram.h"

Histogram::Histogram(int n) : m_n(n) {
    m_hist = new int[n]();
}

Histogram::~Histogram() {
    delete[] m_hist;
}

int &Histogram::operator[](int index) {
    if (index < 0 || index >= m_n) {
        throw std::out_of_range("Index out of bounds");
    }
    return m_hist[index];
}

const int &Histogram::operator[](int index) const {
    if (index < 0 || index >= m_n) {
        throw std::out_of_range("Index out of bounds");
    }
    return m_hist[index];
}

Histogram::Histogram(const Histogram &other) : m_n(other.m_n) {
    m_hist = new int[m_n];
    std::copy(other.m_hist, other.m_hist + m_n, m_hist);
}

Histogram &Histogram::operator=(const Histogram &other) {
    if (this != &other) {
        int *new_hist = new int[other.m_n];
        std::copy(other.m_hist, other.m_hist + other.m_n, new_hist);
        delete[] m_hist;
        m_hist = new_hist;
        m_n = other.m_n;
    }
    return *this;
}