/**
 * File: shape.h
 * -------------
 * Description: This header defines the classes and functions for geometric shapes
 *              like S2d (2D point or vector), Segment, Circle, and Square.
 *              It provides methods for geometric transformations, intersection tests,
 *              and checking inclusion within these shapes.
 *
 * Author: Bahey shalash 
 * Version: 1.0
 * Date: 27/04/2024
 */

#ifndef SHAPE_H
#define SHAPE_H

//! INTERDICTION d'utiliser using namespace std; dans les .h!!!!!!

#include <cmath>  // For math constants and functions
#include <iostream>

#include "constantes.h"  // for epsil_zero

struct S2d {  // Represents a 2D point or vector
    double x = 0;
    double y = 0;
    // Equality operator overloading
    bool operator==(const S2d& other) const {
        return std::fabs(this->x - other.x) < epsil_zero &&
               std::fabs(this->y - other.y) < epsil_zero;
    }

    // Not equal operator overloading
    bool operator!=(const S2d& other) const { return !(*this == other); }
};  // keep it in the header file sinon trop compliquer a comprendre ce qui ce passe

class Segment {
protected:
    S2d base; // Base point of the segment
    double angle; // Angle from the horizontal
    double length;  // Length of the segment

public:
    Segment(const S2d& base_, double angle_, double length_);
    Segment(const Segment& other);
    Segment& operator=(const Segment& other);
    virtual ~Segment();
    bool operator==(const Segment& other) const;
    bool operator!=(const Segment& other) const;

    // Method to normalize an angle between -π and π
    static double normalize_angle(double angle);

    // Calculate the end point of the segment based on its base, angle, and length
    S2d calculate_extremite() const;

    // Getters for the angle and length of the segment
    double getAngle() const;
    double getLength() const;
    S2d getBase() const;
    void setLength(double length_);

    static bool doIntersect(const Segment& seg1, const Segment& seg2);

    static double angular_difference(double alpha1, double alpha2);

    static bool are_segments_superimposed(const Segment& seg1, const Segment& seg2);
    bool areSegmentsInSuperposition(const Segment& seg1, const Segment& seg2);

    friend std::ostream& operator<<(std::ostream& os, const Segment& segment);

    // Method to check if two segments have a common point
};
int orientation(const S2d& p, const S2d& q, const S2d& r);
bool onSegment(const S2d& p, const S2d& q, const S2d& r);

std::ostream& operator<<(std::ostream& os, const Segment& segment);

// TODO: divode shape.h into segment.h and cercle.h and square.h

class Cercle {
protected:
    S2d centre;
    double rayon;

public:
    Cercle(const S2d& centre_, double rayon_);
    virtual ~Cercle();

    bool is_inside(const S2d& point) const;
};

class Square {
protected:
    S2d centre;
    double side;

public:
    Square(const S2d& centre_, double side_);
    virtual ~Square();

    bool is_inside(const S2d& point) const;
};

#endif  // SHAPE_H
