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
 * Authors: Bahey Shalash
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
    static std::set<int> getUniqueIDs_copy();
    static void setUniqueIDs(std::set<int> newUniqueIDs);

    static void clear_uniqueIDs();
    static std::set<int>& getUniqueIDs();

    // debuging method to cout the segments vector
    void printSegments() const;
    void killCoral();  // kill the coral by setting the statut to dead
    void rotate_last_segment(double angle = delta_rot);
    void switchRotationDirection();
    Segment get_last_segment() const;

    void extend_last_segment(double delta_l);  // extend the last segment by delta_l

    void setSegments(const std::vector<Segment>& newSegments);
    std::vector<Segment> getSegments() const;
    void Alternate_StatutDev();
    void incrementNbSeg();

    bool isWithinBoundaries(double max) const;

    static void printIDs();
    void updateLastSegmentLength(double newLength);

    static void incrementNbCoral();
    static void decrementNbCoral();
    bool last_segment_is_within_boundaries(double max) const;

    void remove_last_segment();

    void set_last_segment_length(double newLength);
    void decrease_last_segment_length(double delta);

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
