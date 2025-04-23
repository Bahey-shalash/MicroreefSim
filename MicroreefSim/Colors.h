/**
 * File: Colors.h
 * ---------------
 * Description: This header defines the Color struct for representing RGB colors and
 *              provides a namespace containing predefined color values. The struct and
 *              predefined colors facilitate easy usage and reference throughout the
 *              graphical components of the project.
 *
 * Authors: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#ifndef COLORS_H
#define COLORS_H

// Define a simple Color struct
struct Color {
    double red;
    double green;
    double blue;
};

// Define a namespace for predefined colors
namespace Colors {
inline Color Red() {
    return {1.0, 0.0, 0.0};
}
inline Color Green() {
    return {0.0, 1.0, 0.0};
}
inline Color Blue() {
    return {0.0, 0.0, 1.0};
}
inline Color Black() {
    return {0.0, 0.0, 0.0};
}
inline Color Grey() {
    return {0.5, 0.5, 0.5};
}
// Additional predefined colors can be added here
}  // namespace Colors

#endif  // COLORS_H
