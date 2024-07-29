#include "CircularLifeform.h"

/**
 * File: CircularLifeform.cpp
 * --------------------------
 * Description: This source file implements the CircularLifeform class, derived from
 * the Lifeform class. It provides constructors, a destructor, and methods to manage
 * the unique attribute of radius, along with inherited features like position and age.
 * The implementation ensures that circular life forms can be effectively managed
 * within the ecosystem simulation, including operations like printing to output
 * streams, comparing life forms, and adjusting life form properties.
 *
 * Authors: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

CircularLifeform::CircularLifeform(const S2d& position, double radius,
                                   unsigned int initialAge)
    : Lifeform(position, initialAge), radius(radius) {
    /* if (pos.x < 1 || pos.x > max - 1 || pos.y < 1 || pos.y > max - 1) {
        std::cout << message::lifeform_center_outside(pos.x, pos.y);
        std::exit(EXIT_FAILURE);
    } */
}

// Copy constructor implementation
CircularLifeform::CircularLifeform(const CircularLifeform& other)
    : Lifeform(other), radius(other.radius) {}

// Destructor implementation
CircularLifeform::~CircularLifeform() {}

// Assignment operator implementation

CircularLifeform& CircularLifeform::operator=(const CircularLifeform& other) {
    if (this != &other) {
        Lifeform::operator=(other);
        radius = other.radius;
    }
    return *this;
}
bool CircularLifeform::operator==(const CircularLifeform& other) const {
    return Lifeform::operator==(other) && (radius == other.radius);
}

bool CircularLifeform::operator!=(const CircularLifeform& other) const {
    return !(*this == other);
}

double CircularLifeform::getRadius() const {
    return radius;
}

void CircularLifeform::setRadius(double newRadius) {
    radius = newRadius;
}

std::ostream& operator<<(std::ostream& os, const CircularLifeform& other) {
    os << static_cast<const Lifeform&>(other) << " " << other.radius;
    // transofirmw other en lifeform en perd de l'info mais c ce quon veut ici parceque
    // on sait affichier un liformw normale big brain
    return os;
}
