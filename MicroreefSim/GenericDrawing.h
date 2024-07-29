
/**
 * File: GenericDrawing.h
 * --------------------
 * Description: Defines the GenericDrawing class, a custom widget derived from
 *              Gtk::DrawingArea that provides generic drawing functionalities such
 *              as drawing circles, squares, segments, and boundaries. This class
 *              serves as a base class for more specialized drawing operations.
 *
 * Author: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */
#ifndef GENERIC_DRAWING_H
#define GENERIC_DRAWING_H

#include <cairomm/context.h>
#include <gtkmm/drawingarea.h>

#include "Colors.h"  // for predefined colors
#include "shape.h"

class GenericDrawing : public Gtk::DrawingArea {
public:
    GenericDrawing();
    virtual ~GenericDrawing();
    void drawCircle(const Cairo::RefPtr<Cairo::Context>& cr, double x, double y,
                    double radius, const Color& color);
    void drawSquare(const Cairo::RefPtr<Cairo::Context>& cr, double x, double y,
                    double side, const Color& color);  // Method to draw a square

    void drawSegment(const Cairo::RefPtr<Cairo::Context>& cr, const Segment& segment,
                     const Color& color);

    void drawBoundaries(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                        int height);
};

#endif  // GENERIC_DRAWING_H