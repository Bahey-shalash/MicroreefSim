/**
 * File: SimulationWindow.h
 * -------------------------
 * Description: Defines the SimulationWindow class, which extends Gtk::Window to
 *              create the main application window for the simulation. This class
 *              manages user interactions, controls simulation execution, visual
 *              updates, and file operations. It includes widgets like buttons,
 *              labels, and checkboxes that allow users to control the simulation,
 *              and it connects these UI elements to the underlying simulation logic.
 *
 *              Widgets include:
 *              - Start/Stop simulation
 *              - Step through simulation
 *              - Open and save simulation states
 *              - Adjust simulation parameters such as algae birth
 *
 * Authors: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#ifndef SIMULATION_WINDOW_H
#define SIMULATION_WINDOW_H

//-----gtkmm includes
#include <glibmm/dispatcher.h>  // For Glib::Dispatcher
#include <glibmm/main.h>        // For Glib::signal_timeout()
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/eventcontrollerkey.h>  // For Gtk::EventControllerKey
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/window.h>
//-----

#include <fstream>

#include "DrawingArea.h"
#include "Simulation.h"

class SimulationWindow : public Gtk::Window {
public:
    SimulationWindow(Simulation& simulation_);
    virtual ~SimulationWindow();
    void updateCounts();
    void Reset_INFO();

protected:
    Simulation& simulation;

    unsigned mise_a_jour_count;  // update count
    unsigned Algae_count;
    unsigned coral_count;
    unsigned scavenger_count;

    Gtk::Box mainBox, sideBox, buttonsBox, drawingBox, infoBox;
    Gtk::Box mise_a_jour_Box, algaeBox, coralBox, scavengerBox;

    Gtk::Button stepButton;
    Gtk::ToggleButton startButton;
    Gtk::Button saveButton;
    Gtk::Button openButton;
    Gtk::Button exitButton;
    Gtk::CheckButton algaeBirthCheckbox;

    Gtk::Label general_Label, infoLabel;
    Gtk::Label mise_a_jour_Label, algaeLabel, coralLabel, scavengerLabel;

    Gtk::Frame buttonFrame, infoFrame;

    DrawingArea drawingArea;

    Gtk::Label mise_a_jour_Count, algaeCountLabel, coralCountLabel,
        scavengerCountLabel;

    void setup_label_count_pair(Gtk::Box& box, Gtk::Label& label, Gtk::Label& count);

    void onStepClicked();
    void onStartClicked();
    void onSaveClicked();
    void onOpenClicked();
    void onExitClicked();
    void onAlgaeBirthChecked();

    bool onTimeout();
    bool handleKeyPress(guint keyval, guint keycode, Gdk::ModifierType state);

    void handleFileDialogResponse(int response_id, Gtk::FileChooserDialog* dialog);
    void handleSaveDialogResponse(int response_id, Gtk::FileChooserDialog* dialog);

private:
    sigc::connection m_Connection;
    int tick_interval;  // in milliseconds
};

#endif  // SIMULATION_WINDOW_H
