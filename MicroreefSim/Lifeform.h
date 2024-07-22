/**
 * File: Lifeform.h
 * ----------------
 * Description: This header defines the Lifeform class, which represents a generic life
 * form in the simulation. It includes attributes for position and age, and methods for
 * managing these properties. The Lifeform class serves as a base class for more
 * specific types of life forms that may have additional behaviors and characteristics.
 *
 * Author: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#ifndef LIFEFORM_H
#define LIFEFORM_H

#include <vector>

#include "shape.h"

class Lifeform {
public:
    Lifeform(const S2d& position, unsigned int initialAge = 1);
    Lifeform(const Lifeform& other);  // Copy constructor
    virtual ~Lifeform();
    friend std::ostream& operator<<(std::ostream& os, const Lifeform& lifeform);

    Lifeform& operator=(const Lifeform& other);  // Assignment operator
    bool operator==(const Lifeform& other) const;
    bool operator!=(const Lifeform& other) const;

    unsigned int getAge() const;
    void incrementAge();
    // virtual void Update()=0;//purement virtuelle
    S2d getPosition() const;
    void setPosition(const S2d& newPosition);

protected:
    S2d pos;
    unsigned int age;
    static unsigned int nbLifeforms;
};

std::ostream& operator<<(std::ostream& os, const Lifeform& lifeform);

#endif  // LIFEFORM_H
