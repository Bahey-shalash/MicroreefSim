
/**
 * File: Algae.cpp
 * ----------------
 * Description: Implements the Algae class from Algae.h. This source file provides
 * detailed implementations for managing algae, extending the CircularLifeform class
 * with a fixed radius `r_alg` (defined in constants.h). It handles life cycle
 * management of algae, including creation, copying, and destruction, along with static
 * methods to manage and access the count of all algae instances. These methods are
 * crucial for simulating ecological dynamics, particularly algae population levels.
 *
 * Author: Bahey Shalash
 * Version: 2.0
 * Date: 27/04/2024
 */

#include "Algae.h"

unsigned int Algae::nbAlg = 0;

Algae::Algae(const S2d& position, unsigned int initialAge = 1)
    : CircularLifeform(position, r_alg, initialAge) {
    ++nbAlg;
}

// copy constructor
Algae::Algae(const Algae& other) : CircularLifeform(other) {
    ++nbAlg;
}

// assignment operator
Algae& Algae::operator=(const Algae& other) {
    if (this != &other) {
        CircularLifeform::operator=(other);
    }
    return *this;
}

Algae::~Algae() {
    --nbAlg;
}

bool Algae::operator==(const Algae& other) const {
    return CircularLifeform::operator==(other);
}

bool Algae::operator!=(const Algae& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Algae& algae) {
    os << static_cast<const Lifeform&>(algae);

    return os;
}

unsigned int Algae::getNbAlg() {
    return nbAlg;
}
void Algae::incrementNbAlg() {
    ++nbAlg;
}
void Algae::decrementNbAlg() {
    if (nbAlg > 0) {
        --nbAlg;
    }
}
