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
    /* //----debugging--
    if (segments.empty()) {
        std::cerr << "Error: Coral constructor created coral with empty segments.\n";
        std::cerr << "Coral details: " << *this << std::endl;
        exit(1);
    } else {
        std::cout << "Coral Constructor: Successfully created coral with segments.\n";
    }
    //----debugging-- */
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
    // if coral id is in the uniqueIDs set, remove it
    /* if (uniqueIDs.find(ID) != uniqueIDs.end()) {
        uniqueIDs.erase(ID);
    } */
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
/* void Coral::addSegment(double angle, double length) {
    if (!segments.empty()) {
        S2d newBase = segments.back().calculate_extremite();
        segments.emplace_back(newBase, angle, length);
    } else {
        std::cerr << "Error: No segments found to attach the new one.\n";
    }
} */

void Coral::addSegment(double angle, double length) {
    incrementNbSeg();
    if (segments.size() >= nbseg) {
        std::cerr
            << "Error: Cannot add more segments than initially specified nbseg.\n";
        std::cerr << "Coral details: " << *this << std::endl;
        return;
    }

    if (!segments.empty()) {
        S2d newBase = segments.back().calculate_extremite();
        segments.emplace_back(newBase, angle, length);
    } else {
        std::cerr << "Error: No segments found to attach the new one.\n";
    }

    /* std::cout << "Added Segment: Angle = " << angle << ", Length = " << length <<
    "\n"; std::cout << "Current Coral Segments: " << segments.size() << "\n"; */
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

std::set<int> Coral::getUniqueIDs_copy() {
    return uniqueIDs;
}

std::set<int>& Coral::getUniqueIDs() {
    return uniqueIDs;
}
void Coral::setUniqueIDs(std::set<int> newUniqueIDs) {
    uniqueIDs = newUniqueIDs;
}

void Coral::clear_uniqueIDs() {
    uniqueIDs.clear();
}

void Coral::killCoral() {
    statut = DEAD;
}

void Coral::rotate_last_segment(double angle) {
    // rotate the last segment of the coral in
    // the direction_rotation ({ TRIGO, INVTRIGO })  by given angle
    if (segments.empty()) {
        std::cerr << "Error: No segments found to rotate.\n";
        return;
    }
    // rotate the last segment by the given angle in the direction_rotation
    if (direction_rotation == TRIGO) {
        segments.back().rotate(angle);
    } else {
        segments.back().rotate(-angle);
    }
}

void Coral::switchRotationDirection() {
    // switch the direction_rotation of the coral
    if (direction_rotation == TRIGO) {
        direction_rotation = INVTRIGO;
    } else {
        direction_rotation = TRIGO;
    }
}

Segment Coral::get_last_segment() const {
    if (segments.empty()) {
        // std::cerr << "Error: No segments found to return.\n";
        // std::cout << "hello from coral.cpp ligne 188" << std::endl;
        std::cout << "coral with the problem : " << *this << "  coral ID:" << ID
                  << std::endl;

        if (statut == DEAD) {
            std::cout << "coral is dead" << std::endl;
        } else {
            std::cout << "coral is alive" << std::endl;
        }
        exit(1);
        return Segment(S2d{0, 0}, 0, 0);
    }
    return segments.back();
}

void Coral::extend_last_segment(double delta_l) {
    if (segments.empty())
        return;
    segments.back().setLength(segments.back().getLength() + delta_l);
    // std::cout << "Extended last segment to: " << segments.back().getLength() <<
    // "\n";
}

void Coral::setSegments(const std::vector<Segment>& newSegments) {
    segments = newSegments;
}

std::vector<Segment> Coral::getSegments() const {
    return segments;
}

bool Coral::isWithinBoundaries(double max) const {
    for (const auto& segment : segments) {
        S2d base = segment.getBase();
        S2d extremity = segment.calculate_extremite();
        if (base.x <= epsil_zero || base.x >= max - epsil_zero ||
            base.y <= epsil_zero || base.y >= max - epsil_zero ||
            extremity.x <= epsil_zero || extremity.x >= max - epsil_zero ||
            extremity.y <= epsil_zero || extremity.y >= max - epsil_zero) {
            /* std::cout << "Coral " << getID() << " segment out of bounds: Base("
                      << base.x << ", " << base.y << "), Extremity(" << extremity.x
                      << ", " << extremity.y << ")" << std::endl; */
            return false;
        }
    }
    return true;
}

bool Coral::last_segment_is_within_boundaries(double max) const {
    if (segments.empty()) {
        return false;
    }
    S2d base = segments.back().getBase();
    S2d extremity = segments.back().calculate_extremite();
    if (base.x <= epsil_zero || base.x >= max - epsil_zero || base.y <= epsil_zero ||
        base.y >= max - epsil_zero || extremity.x <= epsil_zero ||
        extremity.x >= max - epsil_zero || extremity.y <= epsil_zero ||
        extremity.y >= max - epsil_zero) {
        /* std::cout << "Coral " << getID() << " last segment out of bounds: Base("
                  << base.x << ", " << base.y << "), Extremity(" << extremity.x << ", "
                  << extremity.y << ")" << std::endl; */
        return false;
    }
    return true;
}

void Coral::printIDs() {
    std::cout << "Unique IDs: ";
    for (const auto& ID : uniqueIDs) {
        std::cout << ID << " ";
    }
    std::cout << std::endl;
}

void Coral::Alternate_StatutDev() {
    if (statut_dev == EXTEND) {
        statut_dev = REPRO;
    } else {
        statut_dev = EXTEND;
    }
}

void Coral::updateLastSegmentLength(double newLength) {
    if (!segments.empty()) {
        segments.back().setLength(newLength);
        // std::cout << "Updated last segment length to: " << newLength << "\n";
    } else {
        std::cerr << "Attempted to update segment length but no segments exist.\n";
    }
}

void Coral::incrementNbSeg() {
    ++nbseg;
}

void Coral::incrementNbCoral() {
    ++nbCoral;
}

void Coral::decrementNbCoral() {
    --nbCoral;
}

void Coral::remove_last_segment() {
    if (!segments.empty()) {
        segments.pop_back();
        nbseg--;
    }
}  // I added this method to remove the last segment of the coral

void Coral::set_last_segment_length(double newLength) {
    if (!segments.empty()) {
        segments.back().setLength(newLength);
    }
}

void Coral::decrease_last_segment_length(double delta) {
    if (!segments.empty() && segments.back().getLength() > delta) {
        segments.back().setLength(segments.back().getLength() - delta);
    }
}