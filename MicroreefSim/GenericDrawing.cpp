/**
 * File: DrawingArea.cpp
 * ---------------------
 * Description: Implements the DrawingArea class, providing detailed drawing functions
 *              for each type of entity and handling the rendering logic to display
 *              the simulation's current state. This implementation includes methods
 *              for geometric transformations to fit the drawing within the UI
 *              correctly, and it utilizes the Cairo graphics library for rendering.
 *
 *              Key functionalities include:
 *              - Drawing entities such as algae, coral, and scavengers
 *              - Applying transformations for correct scaling and positioning
 *              - Responding to resize events to adjust drawings appropriately
 *
 * Authors: Bahey Shalash
 * Version: 1.0
 * Date: 30/04/2024
 */

#include "GenericDrawing.h"
GenericDrawing::GenericDrawing() {}
GenericDrawing::~GenericDrawing() {}

void GenericDrawing::drawCircle(const Cairo::RefPtr<Cairo::Context>& cr, double x,
                                double y, double radius, const Color& color) {
    /* // multiply the radius by 5 to make it bigger for asthetic reasons
    //radius *= 5; */
    cr->save();  // Save the current state of the context
    cr->set_source_rgb(color.red, color.green,
                       color.blue);  // Set the color for the circle

    // Calculate line width as a percentage of the radius (e.g., 5% of the radius)
    // i didnt know what width to put so i put 5% of the radius
    double line_width = radius * 0.5;  // he's gonna be a bit FATTTTT/ THICC BOI
    cr->set_line_width(line_width);    // Set the thickness of the circle's border

    // Transform the coordinates according to the orthographic projection
    // Assume orthographic_projection has already been called for the context
    cr->arc(x, y, radius, 0, 2 * M_PI);  // Draw the circle
    cr->stroke();   // Stroke the path with the set color (outline only)
    cr->restore();  // Restore the context state
}

void GenericDrawing::drawSquare(const Cairo::RefPtr<Cairo::Context>& cr, double x,
                                double y, double side, const Color& color) {
    cr->save();  // Save the current state of the context
    cr->set_source_rgb(color.red, color.green,
                       color.blue);  // Set the color for the square

    // Calculate line width as a percentage of the side length, e.g., 5% of the side
    double line_width = side * 0.5;
    cr->set_line_width(line_width);  // Set the thickness of the square's border

    // Calculate the top-left corner based on the center coordinates (x, y)
    double half_side = side / 2;
    double x1 = x - half_side;
    double y1 = y - half_side;

    // Draw the square
    cr->rectangle(x1, y1, side, side);
    cr->stroke();   // Stroke the path with the set color (outline only)
    cr->restore();  // Restore the context state
}

void GenericDrawing::drawSegment(const Cairo::RefPtr<Cairo::Context>& cr,
                                 const Segment& segment,
                                 const Color& color) {  // blue by default

    // multiply the length by 5 to make it bigger for asthetic reasons
    Segment tempSegment = segment;
    tempSegment.setLength(segment.getLength());

    S2d base = tempSegment.getBase();
    S2d extremite = tempSegment.calculate_extremite();

    // double squareSide = d_cor;  // Side length of the square at the base, d_cor

    cr->save();  // Save the current state of the context

    // Draw the segment line
    cr->set_source_rgb(color.red, color.green,
                       color.blue);  // Example: set color to blue
    cr->set_line_width(1.0);         // Set the line width, adjust as needed
    cr->move_to(base.x, base.y);
    cr->line_to(extremite.x, extremite.y);
    cr->stroke();  // Stroke the path to draw the line

    // Draw a square at the base of the segment

    // drawSquare(cr, base.x, base.y, squareSide, color);

    // apaarently the square is not needed for all segments of a coral so i will not
    // draw it and this makes it more portable

    cr->restore();  // Restore the context state
}

void GenericDrawing::drawBoundaries(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                                    int height) {
    Color grey = Colors::Grey();  // Use the Grey color from Colors namespace
    cr->save();
    cr->set_source_rgb(grey.red, grey.blue, grey.green);
    cr->set_line_width(2);  // plus jolie

    // Start from the top-left corner
    cr->move_to(0, 0);
    cr->line_to(260, 0);    // Top edge
    cr->line_to(260, 260);  // Right edge
    cr->line_to(0, 260);    // Bottom edge
    cr->close_path();       // Back to the start (left edge)

    // 260 parce que les valeurs sont de 0 a 256 et les traits sont epais

    cr->stroke();
    cr->restore();
}
