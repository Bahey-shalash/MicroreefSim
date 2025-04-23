
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
 * Authors: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#include "Scavenger.h"

int Scavenger::nbsca = 0;

std::set<unsigned int> Scavenger::targetIDs;

Scavenger::Scavenger(const S2d& position, unsigned int initialAge, double radius,
                     Statut_sca status, int targetCoralId_)
    : CircularLifeform(position, radius, initialAge),
      status(status),
      targetCoralId(targetCoralId_) {
    ++nbsca;
    // ad targetID to the set if it is not -1
    if (targetCoralId != -1) {
        targetIDs.insert(targetCoralId);
    }
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

void Scavenger::incrementNbScavengers() {
    ++nbsca;
}

void Scavenger::decrementNbScavengers() {
    --nbsca;
}

void Scavenger::move(const S2d& newPosition) {
    setPosition(newPosition);
}

void Scavenger::set_targetCoralId(unsigned int newtargetCoralId) {
    targetCoralId = newtargetCoralId;
}

void Scavenger::clear_targetIDs() {
    targetIDs.clear();
}

void Scavenger::addTargetID(unsigned int targetID) {
    targetIDs.insert(targetID);
}

void Scavenger::removeTargetID(unsigned int targetID) {
    targetIDs.erase(targetID);
}

std::set<unsigned int> Scavenger::getTargetIDs() {
    return targetIDs;
}

void Scavenger::printTargetIDs() {
    for (auto it = targetIDs.begin(); it != targetIDs.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void Scavenger::setStatus(Statut_sca newStatus) {
    status = newStatus;
}

void Scavenger::increse_radius(double delta) {
    setRadius(radius + delta);
}