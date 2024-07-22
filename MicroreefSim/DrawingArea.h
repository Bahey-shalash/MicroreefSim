/**
 * File: DrawingArea.h
 * --------------------
 * Description: Defines the DrawingArea class, a custom widget derived from
 *              Gtk::DrawingArea specifically tailored to render the graphical
 *              representations of the simulation entities such as algae, corals,
 *              and scavengers. This class handles all the drawing operations
 *              necessary to visualize the state of the simulation on the screen.
 *
 *              Features include:
 *              - Drawing different types of entities with distinct visual styles
 *              - Handling resize events to maintain aspect ratio
 *              - Integrating closely with the simulation data to update visuals
 *
 * Author: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#ifndef DRAWING_AREA_H
#define DRAWING_AREA_H

#include <cairomm/context.h>
#include <gtkmm/drawingarea.h>

#include "Colors.h"  // for predefined colors
#include "Simulation.h"

struct Frame {  // model framming and window parameters
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    double aspect_ratio;
    int width;
    int height;
};

class DrawingArea : public Gtk::DrawingArea {
public:
    DrawingArea();
    virtual ~DrawingArea();

    void setFrame(const Frame& frame);
    void adjustFrame(int width, int height);
    static Frame Default_Frame;  // frame of reference
    void updateSimulationData(const Simulation& simulation);
    void drawCircle(const Cairo::RefPtr<Cairo::Context>& cr, double x, double y,
                    double radius, const Color& color);

    void drawSquare(const Cairo::RefPtr<Cairo::Context>& cr, double x, double y,
                    double side, const Color& color);  // Method to draw a square

    void drawSegment(const Cairo::RefPtr<Cairo::Context>& cr, const Segment& segment,
                     const Color& color);

    void draw_an_algae(const Cairo::RefPtr<Cairo::Context>& cr, const Algae& algae);
    void draw_a_coral(const Cairo::RefPtr<Cairo::Context>& cr, const Coral& coral);
    void draw_a_scavenger(const Cairo::RefPtr<Cairo::Context>& cr,
                          const Scavenger& scavenger);

    void draw_all_entities(const Cairo::RefPtr<Cairo::Context>& cr,
                           const std::vector<Algae>& algae,
                           const std::vector<Coral>& corals,
                           const std::vector<Scavenger>& scavengers);

    void drawBoundaries(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                        int height);

protected:
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
    static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr,
                                        const Frame& frame);

private:
    Frame frame;  // holds current frame settings
    const Simulation* simulation;
};

#endif  // DRAWING_AREA_H
