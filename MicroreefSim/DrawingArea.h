/**
 * File: DrawingArea.h
 * --------------------
 * Description: Defines the DrawingArea class, a custom widget derived from
 *              GenericDrawing specifically tailored to render the graphical
 *              representations of the simulation entities such as algae, corals,
 *              and scavengers. This class handles all the drawing operations
 *              necessary to visualize the state of the simulation on the screen.
 *
 *              Features include:
 *              - Drawing different types of entities with distinct visual styles
 *              - Handling resize events to maintain aspect ratio
 *              - Integrating closely with the simulation data to update visuals
 *
 * Authors: Bahey Shalash
 * Version: 1.0
 * Date: 30/04/2024
 */

#ifndef DRAWING_AREA_H
#define DRAWING_AREA_H

#include "GenericDrawing.h"
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

class DrawingArea : public GenericDrawing {
public:
    DrawingArea();
    virtual ~DrawingArea();

    void setFrame(const Frame& frame);
    void adjustFrame(int width, int height);
    void updateSimulationData(const Simulation& simulation);

    void draw_an_algae(const Cairo::RefPtr<Cairo::Context>& cr, const Algae& algae);
    void draw_a_coral(const Cairo::RefPtr<Cairo::Context>& cr, const Coral& coral);
    void draw_a_scavenger(const Cairo::RefPtr<Cairo::Context>& cr,
                          const Scavenger& scavenger);

    void draw_all_entities(const Cairo::RefPtr<Cairo::Context>& cr,
                           const std::vector<Algae>& algae,
                           const std::vector<Coral>& corals,
                           const std::vector<Scavenger>& scavengers);

protected:
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
    static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr,
                                        const Frame& frame);

private:
    Frame frame;  // holds current frame settings
    const Simulation* simulation;
    static Frame Default_Frame;  // frame of reference
};

#endif  // DRAWING_AREA_H
