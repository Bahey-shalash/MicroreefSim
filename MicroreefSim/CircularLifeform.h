
/**
 * File: CircularLifeform.h
 * ------------------------
 * Description: This header defines the CircularLifeform class, a derivative of the
 * Lifeform class, designed to represent life forms with a circular shape. It extends
 * the Lifeform class by introducing a radius attribute, providing methods for managing
 * the radius along with inherited attributes such as position and age. This class
 * models circular entities within the ecosystem simulation, adding geometric
 * characteristics specific to circular forms.
 *
 * Authors: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#ifndef CIRCULARLIFEFORM_H
#define CIRCULARLIFEFORM_H

#include "Lifeform.h"
#include "constantes.h"

class CircularLifeform : public Lifeform {
public:
    CircularLifeform(const S2d& position, double radius, unsigned int initialAge = 1);
    CircularLifeform(const CircularLifeform& other);             // Copy constructor
    CircularLifeform& operator=(const CircularLifeform& other);  // Assignment operator
    bool operator==(const CircularLifeform& other) const;
    bool operator!=(const CircularLifeform& other) const;
    virtual ~CircularLifeform();  // destructor
    double getRadius() const;
    void setRadius(double newRadius);
    friend std::ostream& operator<<(std::ostream& os,
                                    const CircularLifeform& lifeform);

protected:
    double radius;
};

std::ostream& operator<<(std::ostream& os, const CircularLifeform& lifeform);

#endif  // CIRCULARLIFEFORM_H
