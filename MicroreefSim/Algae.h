/**
 * File: Algae.h
 * --------------
 * Description: This header defines the Algae class, which extends the CircularLifeform
 * class. Algae are modeled as circular life forms with a constant radius defined by
 * `r_alg` (set to 1, as specified in constants.h). This class manages a static count
 * of Algae instances to track population levels within the simulation. It includes
 * constructors, a destructor, and methods to manage and query the count of algae
 * instances globally, emphasizing population dynamics.
 *
 * Author: Bahey Shalash
 * Version: 2.0
 * Date: 27/04/2024
 */

#ifndef ALGAE_H
#define ALGAE_H

#include "CircularLifeform.h"

class Algae : public CircularLifeform {
public:
    // Constructor taking initial position and age
    Algae(const S2d& position, unsigned int initialAge);
    Algae(const Algae& other);             // Copy constructor
    Algae& operator=(const Algae& other);  // Assignment operator
    bool operator==(const Algae& other) const;
    bool operator!=(const Algae& other) const;
    // Destructor
    virtual ~Algae();

    friend std::ostream& operator<<(std::ostream& os, const Algae& algae);

    // Static method to access and modify the static count of Algae instances
    static unsigned int getNbAlg();
    static void incrementNbAlg();
    static void decrementNbAlg();

private:
    static unsigned int nbAlg;  // Static count of Algae instances
};

std::ostream& operator<<(std::ostream& os, const Algae& algae);

#endif  // ALGAE_H
