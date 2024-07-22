/**
 * File: Coral.cpp
 * ----------------
 * Description: Implements the Coral class from Coral.h. This source file includes the
 * logic for constructing coral entities, managing their life cycle, and maintaining a
 * unique ID system. The methods provided allow for dynamic interaction with coral
 * attributes such as segment addition, status updates, and rotational behavior
 * adjustments. Additional functionalities include maintaining a global set of IDs to
 * ensure uniqueness among all coral instances within the simulation.
 *
 * Author: Bahey Shalash 
 * Version: 1.0
 * Date: 27/04/2024
 */

#include "Coral.h"

unsigned int Coral::nbCoral = 0;
std::set<int> Coral::uniqueIDs;

Coral::Coral(const S2d& base_, unsigned int initialAge, int ID, Statut_cor statut,
             Dir_rot_cor direction_rotation, Statut_dev statut_dev, unsigned int nbseg,
             double firstAngle, double firstLength)
    : SegmentLifeform(base_, initialAge, {Segment(base_, firstAngle, firstLength)}),
      ID(ID),
      statut(statut),
      direction_rotation(direction_rotation),
      statut_dev(statut_dev),
      nbseg(nbseg) {
    ++nbCoral;
}
Coral::Coral(const Coral& other)
    : SegmentLifeform(other),
      ID(other.ID),
      statut(other.statut),
      direction_rotation(other.direction_rotation),
      statut_dev(other.statut_dev),
      nbseg(other.nbseg) {
    ++nbCoral;
}

Coral& Coral::operator=(const Coral& other) {
    if (this != &other) {
        SegmentLifeform::operator=(other);
        ID = other.ID;
        statut = other.statut;
        direction_rotation = other.direction_rotation;
        statut_dev = other.statut_dev;
        nbseg = other.nbseg;
    }
    return *this;
}

Coral::~Coral() {
    --nbCoral;
}

bool Coral::operator==(const Coral& other) const {
    return SegmentLifeform::operator==(other) && ID == other.ID &&
           statut == other.statut && direction_rotation == other.direction_rotation &&
           statut_dev == other.statut_dev && nbseg == other.nbseg;
}

bool Coral::operator!=(const Coral& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Coral& coral) {
    os << static_cast<const Lifeform&>(coral) << " " << coral.ID << " " << coral.statut
       << " " << coral.direction_rotation << " " << coral.statut_dev << " "
       << coral.nbseg << "\n"
       << static_cast<const SegmentLifeform&>(coral);

    return os;
}
void Coral::addSegment(double angle, double length) {
    if (segments.size() >= nbseg) {
        std::cerr
            << "Error: Cannot add more segments than the initially specified nbseg.\n";
        return;
    }
    if (!segments.empty()) {
        S2d newBase = segments.back().calculate_extremite();
        segments.emplace_back(newBase, angle, length);
    } else {
        std::cerr << "Error: No segments found to attach the new one.\n";
    }
}

void Coral::printSegments() const {
    for (const auto& segment : segments) {
        std::cout << "Base: (" << segment.getBase().x << ", " << segment.getBase().y
                  << "), Angle: " << segment.getAngle()
                  << ", Length: " << segment.getLength() << std::endl;
    }
}  // I overide the << operator in the coral class so we can delete this method

bool Coral::addUniqueID(int ID) {
    // Attempt to insert the ID into the uniqueIDs container
    auto [_, isUnique] = uniqueIDs.insert(ID);

    // Return whether the insertion was successful and the ID is unique
    return isUnique;
}

void Coral::removeUniqueID(int ID) {
    uniqueIDs.erase(ID);
}

int Coral::getID() const {
    return ID;
}

void Coral::setStatut(Statut_cor newStatut) {
    statut = newStatut;
}

Statut_cor Coral::getStatut() const {
    return statut;
}

void Coral::setDirectionRotation(Dir_rot_cor newDirection) {
    direction_rotation = newDirection;
}

Dir_rot_cor Coral::getDirectionRotation() const {
    return direction_rotation;
}

void Coral::setStatutDev(Statut_dev newStatutDev) {
    statut_dev = newStatutDev;
}

Statut_dev Coral::getStatutDev() const {
    return statut_dev;
}

void Coral::setNbSeg(unsigned int newNbSeg) {
    if (newNbSeg > 0) {
        nbseg = newNbSeg;
    } else {
        // Handle the error appropriately: set to a default value or throw an exception
        // idk voir message or smth
    }
}

unsigned int Coral::getNbSeg() const {
    return nbseg;
}

std::set<int> Coral::getUniqueIDs() {
    return uniqueIDs;
}
void Coral::setUniqueIDs(std::set<int> newUniqueIDs) {
    uniqueIDs = newUniqueIDs;
}

void Coral::clear_uniqueIDs() {
    uniqueIDs.clear();
}
