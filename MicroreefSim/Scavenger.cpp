
/**
 * File: Scavenger.cpp
 * -------------------
 * Description: Implements the Scavenger class from Scavenger.h. This source file
 * provides the functionality for managing the life cycle and behaviors of scavengers,
 * including their interactions with corals as part of the ecosystem simulation.
 * Methods include those for movement, coral consumption, reproduction, and managing
 * death, along with static methods to manage the population count. The class
 * effectively extends CircularLifeform with additional behaviors critical to
 * scavengers.
 *
 * Author: Bahey Shalash 
 * Version: 1.0
 * Date: 27/04/2024
 */

#include "Scavenger.h"

int Scavenger::nbsca = 0;

Scavenger::Scavenger(const S2d& position, unsigned int initialAge, double radius,
                     Statut_sca status, int targetCoralId_)
    : CircularLifeform(position, radius, initialAge),
      status(status),
      targetCoralId(targetCoralId_) {
    ++nbsca;
}

Scavenger::Scavenger(const Scavenger& other)
    : CircularLifeform(other),
      status(other.status),
      targetCoralId(other.targetCoralId) {
    ++nbsca;
}

Scavenger& Scavenger::operator=(const Scavenger& other) {
    if (this != &other) {
        CircularLifeform::operator=(other);
        status = other.status;
        targetCoralId = other.targetCoralId;
    }
    return *this;
}

Scavenger::~Scavenger() {
    --nbsca;
}

bool Scavenger::operator==(const Scavenger& other) const {
    return CircularLifeform::operator==(other) && status == other.status &&
           targetCoralId == other.targetCoralId;
}

bool Scavenger::operator!=(const Scavenger& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Scavenger& scavenger) {
    if (scavenger.status == MANGE) {
        os << static_cast<const CircularLifeform&>(scavenger) << " "
           << scavenger.status << "  " << scavenger.targetCoralId;
        return os;
    } else {
        os << static_cast<const CircularLifeform&>(scavenger) << " "
           << scavenger.status;
        return os;
    }
    return os;
}
//-----getters-------
int Scavenger::getNbScavengers() {
    return nbsca;
}

Statut_sca Scavenger::getStatus() const {
    return status;
}

int Scavenger::getTargetCoralId() const {
    return targetCoralId;
}
