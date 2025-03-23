/**
 * File: shape.cpp
 * ----------------
 * Description: This source file implements the functionalities declared in shape.h.
 *              It includes methods for constructing geometric shapes, checking for
 *              intersections, and determining if points lie within these shapes.
 *              Detailed implementations for Segment, Circle, and Square classes
 *              are provided.
 *
 * Author: [Bahey shalash] 
 * Contribution: Bahey shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#include "shape.h"

#include <algorithm>

Segment::Segment(const S2d& base_, double angle_, double length_)
    : base(base_), angle(angle_), length(length_) {}

Segment::Segment(const Segment& other) {
    base = other.base;
    angle = other.angle;
    length = other.length;
}
Segment::~Segment() {}

Segment& Segment::operator=(const Segment& other) {
    if (this != &other) {
        base = other.base;
        angle = other.angle;
        length = other.length;
    }
    return *this;
}

bool Segment::operator==(const Segment& other) const {
    return base == other.base && angle == other.angle && length == other.length;
}

bool Segment::operator!=(const Segment& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Segment& segment) {
    os << segment.angle << " " << segment.length;
    return os;
}

// Utility function to normalize angles within the range [-π, π]
double Segment::normalize_angle(double angle) {
    while (angle > M_PI)
        angle -= 2 * M_PI;
    while (angle <= -M_PI)
        angle += 2 * M_PI;
    return angle;
}
S2d Segment::calculate_extremite() const {
    return {base.x + length * std::cos(angle), base.y + length * std::sin(angle)};
}

// Calculate angular difference
double Segment::angular_difference(double alpha1, double alpha2) {
    double diff = normalize_angle(alpha1 - alpha2);
    return normalize_angle(diff);  // Ensure the difference is within [-π, π]
}

bool Segment::are_segments_superimposed(const Segment& seg1, const Segment& seg2) {
    // Calculate the angular difference between the two segments using the getters
    double angularDiff = angular_difference(seg1.getAngle(), seg2.getAngle());

    // If the angular difference is approximately zero, then the segments might be
    // superimposed
    if (std::fabs(angularDiff) < epsil_zero) {
        S2d seg1End = seg1.calculate_extremite();
        S2d seg2End = seg2.calculate_extremite();

        // Check for alignment of endpoints with the base of the other segment
        bool isCollinear1 = orientation(seg1.getBase(), seg1End, seg2.getBase()) == 0;
        bool isCollinear2 = orientation(seg2.getBase(), seg2End, seg1.getBase()) == 0;

        // If both are collinear, it means they are superimposed, considering that they
        // share a base point
        if (isCollinear1 && isCollinear2) {
            // Check if one of the segments' endpoints lies on the other segment
            bool onSegment1 = onSegment(seg1.getBase(), seg1End, seg2End) ||
                              onSegment(seg1.getBase(), seg1End, seg2.getBase());
            bool onSegment2 = onSegment(seg2.getBase(), seg2End, seg1End) ||
                              onSegment(seg2.getBase(), seg2End, seg1.getBase());
            // The segments are superimposed if any of their non-shared endpoints lie
            // on the other segment
            return onSegment1 || onSegment2;
        }
    }

    return false;  // Segments are not superimposed
}

int orientation(const S2d& p, const S2d& q, const S2d& r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (std::fabs(val) < epsil_zero) {
        return 0;  // Collinear
    }
    return (val > 0) ? 1 : 2;  // Clockwise or counterclockwise
}

bool onSegment(const S2d& p, const S2d& q, const S2d& r) {
    // Direction vectors
    S2d pq = {q.x - p.x, q.y - p.y};
    S2d pr = {r.x - p.x, r.y - p.y};

    // Dot product for projection length
    double dotProduct = pq.x * pr.x + pq.y * pr.y;

    // Length of vector pq
    double normPQ = std::sqrt(pq.x * pq.x + pq.y * pq.y);

    // The scalar projection of pr onto pq
    double projection = dotProduct / normPQ;

    // Check if the scalar projection of pr onto pq is within the bounds of segment pq
    if (projection < -epsil_zero || projection > normPQ + epsil_zero) {
        return false;
    }

    // Calculate the perpendicular distance from point r to the line defined by segment
    // pq using the area of parallelogram approach
    double area = pq.x * pr.y - pq.y * pr.x;
    double perpendicularDistance = std::fabs(area) / normPQ;

    // The point r is on the line segment pq if the perpendicular distance is within
    // epsil_zero tolerance
    return perpendicularDistance <= epsil_zero;
}

bool Segment::doIntersect(const Segment& s1, const Segment& s2) {
    S2d p1 = s1.base, q1 = s1.calculate_extremite();
    S2d p2 = s2.base, q2 = s2.calculate_extremite();

    // Check for shared endpoints
    if (p1 == p2 || p1 == q2 || q1 == p2 || q1 == q2) {
        return false;  // Directly return false if any endpoint is shared
    }

    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4) {
        return true;
    }

    // Collinear cases - need to ensure there's actual overlap beyond endpoints
    if ((o1 == 0 && onSegment(p1, p2, q1)) || (o2 == 0 && onSegment(p1, q2, q1)) ||
        (o3 == 0 && onSegment(p2, p1, q2)) || (o4 == 0 && onSegment(p2, q1, q2))) {
        return true;
    }

    // No intersection
    return false;
}

bool Segment::areSegmentsInSuperposition(const Segment& seg1, const Segment& seg2) {
    // Check for collinearity first
    int orientation1 =
        orientation(seg1.getBase(), seg1.calculate_extremite(), seg2.getBase());
    int orientation2 = orientation(seg1.getBase(), seg1.calculate_extremite(),
                                   seg2.calculate_extremite());

    // If both orientation checks are 0, segments are collinear
    if (orientation1 == 0 && orientation2 == 0) {
        // Check if segments share any common point
        if (onSegment(seg1.getBase(), seg2.getBase(), seg1.calculate_extremite()) ||
            onSegment(seg1.getBase(), seg2.calculate_extremite(),
                      seg1.calculate_extremite()) ||
            onSegment(seg2.getBase(), seg1.getBase(), seg2.calculate_extremite()) ||
            onSegment(seg2.getBase(), seg1.calculate_extremite(),
                      seg2.calculate_extremite())) {
            return true;  // Segments are in superposition
        }
    }

    return false;  // Segments are not in superposition
}

S2d Segment::getBase() const {
    return base;
}

double Segment::getAngle() const {
    return angle;
}

double Segment::getLength() const {
    return length;
}

void Segment::setLength(double length_) {
    length = length_;
}

Cercle::Cercle(const S2d& centre_, double rayon_) : centre(centre_), rayon(rayon_) {}

bool Cercle::is_inside(const S2d& point) const {
    return std::hypot(point.x - centre.x, point.y - centre.y) < rayon + epsil_zero;
}

Cercle::~Cercle() {}

// Square

Square::Square(const S2d& centre_, double side_) : centre(centre_), side(side_) {}

Square::~Square() {}

bool Square::is_inside(const S2d& point) const {
    double half_side = side / 2;
    return (std::fabs(point.x - centre.x) <= half_side + epsil_zero &&
            std::fabs(point.y - centre.y) <= half_side + epsil_zero);
}