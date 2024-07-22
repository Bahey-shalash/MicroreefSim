/**
 * File: SegmentLifeform.cpp
 * --------------------------
 * Description: Implements the SegmentLifeform class from SegmentLifeform.h. This
 * source file provides detailed implementations of constructors, destructors, and
 * methods to manage the vector of Segments. These functionalities extend the basic
 * Lifeform features, integrating operations that are specific to segmented life forms,
 *              such as equality checks that consider both positional and
 * segment-specific characteristics.
 *
 * Author: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#include "SegmentLifeform.h"

SegmentLifeform::SegmentLifeform(const S2d& base_, unsigned int initialAge,
                                 std::vector<Segment> segments_)
    : Lifeform(base_, initialAge), segments(segments_) {}

// Copy constructor implementation
SegmentLifeform::SegmentLifeform(const SegmentLifeform& other)
    : Lifeform(other), segments(other.segments) {}

// Assignment operator implementation
SegmentLifeform& SegmentLifeform::operator=(const SegmentLifeform& other) {
    if (this != &other) {
        Lifeform::operator=(other);
        segments = other.segments;
    }
    return *this;
}

// Destructor implementation
SegmentLifeform::~SegmentLifeform() {}

bool SegmentLifeform::areSegmentsInside() const {
    for (const auto& segment : segments) {
        S2d base = segment.getBase();
        if (base.x <= 0 || base.x >= max || base.y <= 0 || base.y >= max) {
            return false;
        }
    }
    return true;
}

bool SegmentLifeform::operator==(const SegmentLifeform& other) const {
    if (segments.size() != other.segments.size()) {
        return false;
    }  // si 2 SegmentLifeform n'ont pas le meme nombre de segments, ils sont
       // differents

    for (size_t i = 0; i < segments.size(); ++i) {
        if (segments[i] != other.segments[i]) {
            return false;
        }
    }
    return Lifeform::operator==(other);
}

bool SegmentLifeform::operator!=(const SegmentLifeform& other) const {
    return !(*this == other);
}

std::vector<Segment> SegmentLifeform::getSegments() const {
    return segments;
}

std::ostream& operator<<(std::ostream& os, const SegmentLifeform& other) {
    // os << static_cast<const Lifeform&>(other) << ", Segment(s): ";
    //  for each segment say its number and (segment 0 segment  1) then the segment
    for (size_t i = 0; i < other.segments.size(); ++i) {
        os << "        " << other.segments[i] << "\n";
    }

    return os;
}