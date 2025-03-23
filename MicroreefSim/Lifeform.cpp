
/**
 * File: Lifeform.cpp
 * ------------------
 * Description: Implements the Lifeform class defined in Lifeform.h. This source file
 * provides detailed implementations of the constructors, destructor, and member
 * functions for handling the life form's state, such as age and position.
 *
 * Author: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#include "Lifeform.h"

unsigned int Lifeform::nbLifeforms = 0;

//-------------------Lifeform-------------------
Lifeform::Lifeform(const S2d& position, unsigned int initialAge)
    : pos(position), age(initialAge) {
    /* if (age <= 0) {
        std::cout << message::lifeform_age(age);
        std::exit(EXIT_FAILURE);  // redue 2
    } */

    ++nbLifeforms;
}

// Copy constructor implementation
Lifeform::Lifeform(const Lifeform& other) : pos(other.pos), age(other.age) {}

// Destructor implementation
Lifeform::~Lifeform() {
    --nbLifeforms;
}

// Assignment operator implementation
Lifeform& Lifeform::operator=(const Lifeform& other) {
    if (this != &other) {
        pos = other.pos;
        age = other.age;
    }
    return *this;
}

bool Lifeform::operator==(const Lifeform& other) const {
    return (pos == other.pos) && (age == other.age);
}

bool Lifeform::operator!=(const Lifeform& other) const {
    return !(*this == other);
}

unsigned int Lifeform::getAge() const {
    return age;
}

void Lifeform::incrementAge() {
    ++age;
}

S2d Lifeform::getPosition() const {
    return pos;
}

void Lifeform::setPosition(const S2d& newPosition) {
    pos = newPosition;
}

std::ostream& operator<<(std::ostream& os, const Lifeform& lifeform) {
    os << lifeform.pos.x << " " << lifeform.pos.y << " " << lifeform.age;
    return os;
}