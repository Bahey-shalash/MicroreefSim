/**
 * File: Scavenger.h
 * -----------------
 * Description: This header defines the Scavenger class, which is a specialized
 * derivative of the CircularLifeform class. Scavengers are circular life forms that
 * interact with coral within the simulation. This class introduces additional
 * behaviors such as moving towards, consuming coral, and reproducing. It manages
 * scavenger-specific properties like status and target coral ID, along with a static
 * count of Scavenger instances to track their population.
 *
 * Author: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#ifndef SCAVENGER_H
#define SCAVENGER_H

#include "CircularLifeform.h"

class Scavenger : public CircularLifeform {
public:
    // Inherits constructor from CircularLifeform
    Scavenger(const S2d& position, unsigned int initialAge, double radius = r_sca,
              Statut_sca status = LIBRE, int targetCoralId_ = -1);

    Scavenger(const Scavenger& other);
    Scavenger& operator=(const Scavenger& other);
    bool operator==(const Scavenger& other) const;
    bool operator!=(const Scavenger& other) const;

    virtual ~Scavenger();
    //void update();

    Statut_sca getStatus() const;
    int getTargetCoralId() const;
    static int getNbScavengers();
    
    friend std::ostream& operator<<(std::ostream& os, const Scavenger& scavenger);

private:
    Statut_sca status;
    unsigned int targetCoralId;
    static int nbsca;

    /* // Helper methods for the scavenger's behavior for le rendue 3
    void moveTowardsCoral();
    void consumeCoral();
    void reproduce();
    void die(); */
};

std::ostream& operator<<(std::ostream& os, const Scavenger& scavenger);

#endif
