/**
 * File: SegmentLifeform.h
 * ------------------------
 * Description: This header defines the SegmentLifeform class, a derivative of the
 * Lifeform class, designed to represent life forms composed of multiple segments. It
 * extends the Lifeform class by introducing a vector of Segments as an additional
 * attribute, allowing for the representation and manipulation of complex segmented
 * structures within the ecosystem simulation. The class provides functionalities to
 * manage these segments along with inherited Lifeform attributes.
 *
 * Author: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#ifndef SEGMENTLIFEFORM_H
#define SEGMENTLIFEFORM_H

#include "Lifeform.h"

class SegmentLifeform : public Lifeform {
public:
    SegmentLifeform(const S2d& base_, unsigned int initialAge,
                    std::vector<Segment> segments_);

    SegmentLifeform(const SegmentLifeform& other);             // Copy constructor
    SegmentLifeform& operator=(const SegmentLifeform& other);  // Assignment operator
    bool operator==(const SegmentLifeform& other) const;
    bool operator!=(const SegmentLifeform& other) const;
    virtual ~SegmentLifeform();
    bool areSegmentsInside() const;
    std::vector<Segment> getSegments() const;
    friend std::ostream& operator<<(std::ostream& os, const SegmentLifeform& lifeform);

protected:
    std::vector<Segment> segments;
};

std::ostream& operator<<(std::ostream& os, const SegmentLifeform& lifeform);

#endif