/**
 * File: Coral.h
 * --------------
 * Description: This header defines the Coral class, an extension of the
 * SegmentLifeform class. Coral entities in the simulation are represented as segmented
 * life forms with unique attributes such as an ID, status, direction of rotation, and
 * developmental status. This class provides methods for segment management and
 * behavior specific to corals, such as adding segments, changing status, and handling
 * unique identification. Coral instances track their population with a static ID set
 * to ensure each instance is unique.
 *
 * Author: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#ifndef CORAL_H
#define CORAL_H

#include <set>

#include "SegmentLifeform.h"

class Coral : public SegmentLifeform {
public:
    Coral(const S2d& base_, unsigned int initialAge, int ID, Statut_cor statut,
          Dir_rot_cor direction_rotation, Statut_dev statut_dev, unsigned int nbseg,
          double firstAngle, double firstLength);

    Coral(const Coral& other);
    Coral& operator=(const Coral& other);
    bool operator==(const Coral& other) const;
    bool operator!=(const Coral& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Coral& coral);

    virtual ~Coral();

    void addSegment(double angle, double length);
    int getID() const;
    void setStatut(Statut_cor newStatut);
    Statut_cor getStatut() const;
    void setDirectionRotation(Dir_rot_cor newDirection);
    Dir_rot_cor getDirectionRotation() const;
    void setStatutDev(Statut_dev newStatutDev);
    Statut_dev getStatutDev() const;
    void setNbSeg(unsigned int newNbSeg);
    unsigned int getNbSeg() const;

    static bool checkForCollision(const S2d& base, double angle, double length);
    static bool addUniqueID(int ID);
    static void removeUniqueID(int ID);
    // uniqueIDs setter and getter
    static std::set<int> getUniqueIDs();
    static void setUniqueIDs(std::set<int> newUniqueIDs);
    
    static void clear_uniqueIDs();
    
    // debuging method to cout the segments vector
    void printSegments() const;



private:
    int ID;
    Statut_cor statut;
    Dir_rot_cor direction_rotation;
    Statut_dev statut_dev;
    unsigned int nbseg;
    static unsigned int nbCoral;
    static std::set<int> uniqueIDs;  // for checking unique IDs
};

std::ostream& operator<<(std::ostream& os, const Coral& coral);

#endif  // CORAL_H
