/**
 * File: DrawingArea.cpp
 * ---------------------
 * Description: Implements the DrawingArea class, providing detailed drawing functions
 *              for each type of entity and handling the rendering logic to display
 *              the simulation's current state. This implementation includes methods
 *              for geometric transformations to fit the drawing within the UI
 * correctly, and it utilizes the Cairo graphics library for rendering.
 *
 *              Key functionalities include:
 *              - Drawing entities such as algae, coral, and scavengers
 *              - Applying transformations for correct scaling and positioning
 *              - Responding to resize events to adjust drawings appropriately
 *
 * Author: Bahey Shalash 
 * Version: 1.0
 * Date: 27/04/2024
 */

#include "DrawingArea.h"

Frame DrawingArea::Default_Frame = {0., 256., 0., 256., 1, 500, 500};
//{xMin, xMax, yMin, yMax, aspect ratio, width, height}

DrawingArea::DrawingArea() : simulation(nullptr) {
    setFrame(Default_Frame);
    set_content_width(Default_Frame.width);
    set_content_height(Default_Frame.height);
    set_draw_func(sigc::mem_fun(*this, &DrawingArea::on_draw));
}

DrawingArea::~DrawingArea() {}

void DrawingArea::updateSimulationData(const Simulation& simulation_) {
    simulation = &simulation_;
    queue_draw();  // Redraw when simulation data is updated
}

void DrawingArea::setFrame(const Frame& frame_) {
    // check if the frame is correct
    // check if theres an incoherence in the frame
    if ((frame_.xMin > frame_.xMax) or (frame_.yMin > frame_.yMax) or
        (frame_.height <= 0)) {
        std::cerr << "Frame parameters are incorrect...." << std::endl;
        return;
    } else {
        frame.aspect_ratio = (frame_.width) / (frame_.height);
        frame = frame_;
    }
}

void DrawingArea::adjustFrame(int width, int height) {
    // voir la serie
    // this function is used to adjust the frame to the window size
    frame.width = width - 20;    // Subtract margins from total width
    frame.height = height - 20;  // Subtract margins from total height

    // Preventing distorsion by adjusting the frame (cadrage)
    // to have the same proportion as the graphical area
    double new_aspect_ratio((double)width / height);
    // avoid integer division by casting to double
    //  use the reference framing as a guide for preventing distorsion
    if (new_aspect_ratio > Default_Frame.aspect_ratio) {
        // keep ymax and ymin. Adjust xmax and xmin
        frame.yMax = Default_Frame.yMax;
        frame.yMin = Default_Frame.yMin;

        double delta(Default_Frame.xMax - Default_Frame.xMin);
        double mid((Default_Frame.xMax + Default_Frame.xMin) / 2);
        frame.xMax =
            mid + 0.5 * (new_aspect_ratio / Default_Frame.aspect_ratio) * delta;
        frame.xMin =
            mid - 0.5 * (new_aspect_ratio / Default_Frame.aspect_ratio) * delta;
    } else {
        frame.xMax = Default_Frame.xMax;
        frame.xMin = Default_Frame.xMin;

        double delta(Default_Frame.yMax - Default_Frame.yMin);
        double mid((Default_Frame.yMax + Default_Frame.yMin) / 2);
        frame.yMax =
            mid + 0.5 * (Default_Frame.aspect_ratio / new_aspect_ratio) * delta;
        frame.yMin =
            mid - 0.5 * (Default_Frame.aspect_ratio / new_aspect_ratio) * delta;
    }
}

void DrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                          int height) {
    adjustFrame(width, height);
    orthographic_projection(cr, frame);
    drawBoundaries(cr, width, height);
    if (simulation != nullptr) {
        draw_all_entities(cr, simulation->get_algae_in_simulation(),
                          simulation->get_coral_in_simulation(),
                          simulation->get_scavenger_in_simulation());
    }
}

void DrawingArea::orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr,
                                          const Frame& frame) {
    cr->translate(frame.width / 2 + 10,
                  frame.height / 2 + 10);  // Adjust for the margin

    cr->scale(frame.width / (frame.xMax - frame.xMin),
              -frame.height / (frame.yMax - frame.yMin));

    cr->translate(-(frame.xMin + frame.xMax) / 2, -(frame.yMin + frame.yMax) / 2);
}








void DrawingArea::draw_an_algae(const Cairo::RefPtr<Cairo::Context>& cr,
                                const Algae& algae) {
    // draw the algae as a green circle
    drawCircle(cr, algae.getPosition().x, algae.getPosition().y, r_alg,
               Colors::Green());
}

void DrawingArea::draw_a_scavenger(const Cairo::RefPtr<Cairo::Context>& cr,
                                   const Scavenger& scavenger) {
    // draw the scavenger as a red circle
    drawCircle(cr, scavenger.getPosition().x, scavenger.getPosition().y,
               scavenger.getRadius(), Colors::Red());
}

void DrawingArea::draw_a_coral(const Cairo::RefPtr<Cairo::Context>& cr,
                               const Coral& coral) {
    // go through the segments of the coral and draw them if coral is alive color them
    // blue if coral is dead color them black
    for (const Segment& segment : coral.getSegments()) {
        if (coral.getStatut() == ALIVE) {
            drawSegment(cr, segment, Colors::Blue());
        } else {
            drawSegment(cr, segment, Colors::Black());
        }
    }

    // draw a square at the base of the coral (2 colors)
    if (coral.getStatut() == ALIVE) {
        drawSquare(cr, coral.getPosition().x, coral.getPosition().y, d_cor,
                   Colors::Blue());
    } else {
        drawSquare(cr, coral.getPosition().x, coral.getPosition().y, d_cor,
                   Colors::Black());
    }
}

/* void DrawingArea::draw_a_coral(const Cairo::RefPtr<Cairo::Context>& cr,
                               const Coral& coral) {
    bool use_blue = true; // Flag to alternate colors
    //alternating colors for the segments of the coral usfull for debugging
    // Go through the segments of the coral and draw them with alternating colors if
coral is alive for (const Segment& segment : coral.getSegments()) { if
(coral.getStatut() == ALIVE) { if (use_blue) { drawSegment(cr, segment,
Colors::Blue()); } else { drawSegment(cr, segment, Colors::Red());
            }
            use_blue = !use_blue; // Alternate color
        } else {
            drawSegment(cr, segment, Colors::Black());
        }
    }

    // Draw a square at the base of the coral (2 colors)
    if (coral.getStatut() == ALIVE) {
        drawSquare(cr, coral.getPosition().x, coral.getPosition().y, d_cor,
                   Colors::Blue());
    } else {
        drawSquare(cr, coral.getPosition().x, coral.getPosition().y, d_cor,
                   Colors::Black());
    }
} */

void DrawingArea::draw_all_entities(const Cairo::RefPtr<Cairo::Context>& cr,
                                    const std::vector<Algae>& algae,
                                    const std::vector<Coral>& corals,
                                    const std::vector<Scavenger>& scavengers) {
    for (const Algae& algae_ : algae) {
        draw_an_algae(cr, algae_);
    }

    for (const Coral& coral : corals) {
        draw_a_coral(cr, coral);
    }

    for (const Scavenger& scavenger : scavengers) {
        draw_a_scavenger(cr, scavenger);
    }
}
