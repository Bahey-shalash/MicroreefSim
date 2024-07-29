/**
 * File: SimulationWindow.cpp
 * --------------------------
 * Description: Implements the SimulationWindow class, handling the setup of
 *              the window and its widgets, and defining the callbacks for user
 *              interactions. This file manages the linking of the GUI components
 *              with the simulation's state changes and file operations, providing
 *              a dynamic and responsive user experience.
 *
 *              Key functionalities include:
 *              - Starting and stopping the simulation
 *              - Single-step execution
 *              - Opening and saving files
 *              - Dynamically updating display of entity counts
 *              - Managing algae reproduction control via a checkbox
 *
 * Author: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#include "SimulationWindow.h"

SimulationWindow::SimulationWindow(Simulation& simulation_)
    : simulation(simulation_),
      mise_a_jour_count(0),
      Algae_count(0),
      coral_count(0),
      scavenger_count(0),
      mainBox(Gtk::Orientation::HORIZONTAL, 0),
      sideBox(Gtk::Orientation::VERTICAL, 0),
      buttonsBox(Gtk::Orientation::VERTICAL, 2),
      drawingBox(Gtk::Orientation::HORIZONTAL, 0),
      infoBox(Gtk::Orientation::VERTICAL, 0),
      mise_a_jour_Box(Gtk::Orientation::HORIZONTAL, 20),
      algaeBox(Gtk::Orientation::HORIZONTAL, 20),
      coralBox(Gtk::Orientation::HORIZONTAL, 20),
      scavengerBox(Gtk::Orientation::HORIZONTAL, 20),
      stepButton("Step"),
      startButton("Start"),
      saveButton("Save"),
      openButton("Open"),
      exitButton("Exit"),
      algaeBirthCheckbox("Naissance Algues", false),
      general_Label("General"),
      infoLabel("Info: nombre de..."),
      mise_a_jour_Label("mise à jour:"),
      algaeLabel("algues:"),
      coralLabel("coraux:"),
      scavengerLabel("charognards:"),
      drawingArea(),
      mise_a_jour_Count(std::to_string(mise_a_jour_count)),
      algaeCountLabel(std::to_string(Algae_count)),
      coralCountLabel(std::to_string(coral_count)),
      scavengerCountLabel(std::to_string(scavenger_count)),
      tick_interval(100) {
    set_title("Micro_Reef");
    set_child(mainBox);

    sideBox.set_margin_start(10);  // Add space to the left
    sideBox.set_margin_end(10);
    general_Label.set_margin_top(10);  // Add space on top of the General label

    // Info Label adjustments
    infoLabel.set_xalign(0.0);  // Align 'Info:nombre de' label to the left

    mise_a_jour_Count.set_xalign(1.0);
    algaeCountLabel.set_xalign(1.0);
    coralCountLabel.set_xalign(1.0);
    scavengerCountLabel.set_xalign(1.0);
    // Main Box setup
    mainBox.append(sideBox);
    mainBox.append(drawingBox);

    // Drawing Box setup
    drawingBox.set_size_request(500, 500);
    drawingArea.set_expand(true);
    drawingBox.append(drawingArea);

    // Buttons Box setup
    buttonFrame.set_child(buttonsBox);
    sideBox.append(buttonFrame);
    // set button box margin buttom to 0
    buttonsBox.set_margin_bottom(0);

    buttonsBox.append(general_Label);
    buttonsBox.append(exitButton);
    buttonsBox.append(openButton);
    buttonsBox.append(saveButton);
    buttonsBox.append(startButton);
    buttonsBox.append(stepButton);
    buttonsBox.append(algaeBirthCheckbox);

    // Info Box setup
    infoFrame.set_child(infoBox);
    sideBox.append(infoFrame);
    infoBox.append(infoLabel);

    // info box margin top to 0

    // Aligning values to the right side
    setup_label_count_pair(mise_a_jour_Box, mise_a_jour_Label, mise_a_jour_Count);
    setup_label_count_pair(algaeBox, algaeLabel, algaeCountLabel);
    setup_label_count_pair(coralBox, coralLabel, coralCountLabel);
    setup_label_count_pair(scavengerBox, scavengerLabel, scavengerCountLabel);

    infoBox.append(mise_a_jour_Box);
    infoBox.append(algaeBox);
    infoBox.append(coralBox);
    infoBox.append(scavengerBox);

    startButton.signal_toggled().connect(
        sigc::mem_fun(*this, &SimulationWindow::onStartClicked));

    stepButton.signal_clicked().connect(
        sigc::mem_fun(*this, &SimulationWindow::onStepClicked));

    openButton.signal_clicked().connect(
        sigc::mem_fun(*this, &SimulationWindow::onOpenClicked));

    saveButton.signal_clicked().connect(
        sigc::mem_fun(*this, &SimulationWindow::onSaveClicked));

    exitButton.signal_clicked().connect(
        sigc::mem_fun(*this, &SimulationWindow::onExitClicked));

    algaeBirthCheckbox.signal_toggled().connect(
        sigc::mem_fun(*this, &SimulationWindow::onAlgaeBirthChecked));

    updateCounts();
    drawingArea.updateSimulationData(simulation);

    auto keystroke = Gtk::EventControllerKey::create();
    keystroke->signal_key_pressed().connect(
        sigc::mem_fun(*this, &SimulationWindow::handleKeyPress), false);
    add_controller(keystroke);
}

SimulationWindow::~SimulationWindow() {
    // garbage collection with Simulation& simulation;
    delete &simulation;
    // std::cout << "SimulationWindow deleted" << std::endl;
}

void SimulationWindow::setup_label_count_pair(Gtk::Box& box, Gtk::Label& label,
                                              Gtk::Label& count) {
    label.set_xalign(0.0);  // Align the label to the left
    count.set_xalign(1.0);  // Align the count to the right
    // count.set_hexpand(true);  // Allow the count label to expand
    // when uncommenting this the text gets alligned  but the buttons expand too...
    box.append(label);
    box.append(count);
}

void SimulationWindow::updateCounts() {
    algaeCountLabel.set_text(std::to_string(simulation.getAlgaeCount()));
    coralCountLabel.set_text(std::to_string(simulation.getCoralCount()));
    scavengerCountLabel.set_text(std::to_string(simulation.getScavengerCount()));
}

void SimulationWindow::Reset_INFO() {
    mise_a_jour_count = 0;
    Algae_count = 0;
    coral_count = 0;
    scavenger_count = 0;
    mise_a_jour_Count.set_text(std::to_string(mise_a_jour_count));
    algaeCountLabel.set_text(std::to_string(Algae_count));
    coralCountLabel.set_text(std::to_string(coral_count));
    scavengerCountLabel.set_text(std::to_string(scavenger_count));
}

void SimulationWindow::onStartClicked() {
    if (startButton.get_active()) {
        m_Connection = Glib::signal_timeout().connect(
            sigc::mem_fun(*this, &SimulationWindow::onTimeout), tick_interval);
        startButton.set_label("Stop");
    } else {
        m_Connection.disconnect();
        startButton.set_label("Start");
    }
}

void SimulationWindow::onStepClicked() {
    if (!startButton.get_active()) {
        mise_a_jour_count++;
        mise_a_jour_Count.set_text(std::to_string(mise_a_jour_count));
        simulation.updateEntities();
        updateCounts();
        drawingArea.updateSimulationData(simulation);  // Update DrawingArea
    } else {
        std::cout << "the Simuation is now active" << std::endl;
    }
}

void SimulationWindow::onOpenClicked() {  // from la serie
    // simulation.reset_simulation(); //dont reset the simulation when opening a file

    auto dialog = new Gtk::FileChooserDialog("Please choose a file",
                                             Gtk::FileChooser::Action::OPEN);
    dialog->set_transient_for(*this);
    dialog->set_modal(true);
    dialog->signal_response().connect(sigc::bind(
        sigc::mem_fun(*this, &SimulationWindow::handleFileDialogResponse), dialog));

    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Open", Gtk::ResponseType::OK);

    // add filters, so that only certain file types can be selected

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_mime_type("text/plain");
    dialog->add_filter(filter_text);

    auto filter_cpp = Gtk::FileFilter::create();
    filter_cpp->set_name("C/C++ files");
    filter_cpp->add_mime_type("text/x-c");
    filter_cpp->add_mime_type("text/x-c++");
    filter_cpp->add_mime_type("text/x-c-header");
    dialog->add_filter(filter_cpp);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);
    // show dialog and wait for a user response
    dialog->show();
}

void SimulationWindow::onSaveClicked() {  // voir la serie

    auto dialog = new Gtk::FileChooserDialog("Enter a file name",
                                             Gtk::FileChooser::Action::SAVE);
    dialog->set_transient_for(*this);
    dialog->set_modal(true);
    dialog->signal_response().connect(sigc::bind(
        sigc::mem_fun(*this, &SimulationWindow::handleSaveDialogResponse), dialog));

    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Save", Gtk::ResponseType::OK);

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_mime_type("text/plain");
    dialog->add_filter(filter_text);

    auto filter_cpp = Gtk::FileFilter::create();
    filter_cpp->set_name("C/C++ files");
    filter_cpp->add_mime_type("text/x-c");
    filter_cpp->add_mime_type("text/x-c++");
    filter_cpp->add_mime_type("text/x-c-header");
    dialog->add_filter(filter_cpp);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);

    dialog->show();
}

void SimulationWindow::onExitClicked() {
    //  fermer la simulation
    exit(EXIT_SUCCESS);
}

void SimulationWindow::onAlgaeBirthChecked() {
    simulation.toggleAlgaeBirthAllowed();
}

bool SimulationWindow::onTimeout() {
    if (startButton.get_active()) {
        mise_a_jour_count++;
        mise_a_jour_Count.set_text(std::to_string(mise_a_jour_count));
        simulation.updateEntities();  // Update the simulation first
        updateCounts();               // Then update the counts
        drawingArea.updateSimulationData(
            simulation);  // Finally, update the drawing area
        return true;      // Continue the timeout
    }
    return false;  // Stop the timeout
}

bool SimulationWindow::handleKeyPress(guint keyval, guint, Gdk::ModifierType state) {
    switch (gdk_keyval_to_unicode(keyval)) {
        case 's':
            if (startButton.get_active()) {
                startButton.set_active(false);
            } else {
                startButton.set_active(true);
            }
            return true;
        case '1':
            onStepClicked();
            return true;
        case 'Q':  // pas dans le pdf du projet mais pour quitter
            hide();
            return true;
    }
    // the event has not been handled
    return false;
}

void SimulationWindow::handleFileDialogResponse(int response_id,
                                                Gtk::FileChooserDialog* dialog) {
    // voir la serie
    // Handles the response
    switch (response_id) {
        case Gtk::ResponseType::OK: {
            // Notice that this is a std::string, not a Glib::ustring.
            auto filename = dialog->get_file()->get_path();
            std::ifstream file(filename);
            std::cout << "File selected: " << filename << std::endl;

            Reset_INFO();
            simulation.start(filename);
            updateCounts();
            drawingArea.queue_draw();  // Force the drawing area to redraw
            break;
        }
        case Gtk::ResponseType::CANCEL: {
            std::cout << "Cancel clicked." << std::endl;
            break;
        }
        default: {
            std::cout << "Unexpected button clicked." << std::endl;
            break;
        }
    }
    delete dialog;
}

void SimulationWindow::handleSaveDialogResponse(int response_id,
                                                Gtk::FileChooserDialog* dialog) {
    // voir la serie

    switch (response_id) {
        case Gtk::ResponseType::OK: {
            // Notice that this is a std::string, not a Glib::ustring.
            auto filename = dialog->get_file()->get_path();
            // std::ofstream file(filename + ".txt");
            // if last 4 characters are not ".txt", append ".txt" to the filename
            if (filename.substr(filename.size() - 4) != ".txt") {
                filename += ".txt";
            }
            simulation.saveSimulation(filename);
            std::cout << "File created: " << filename << std::endl;

            break;
        }
        case Gtk::ResponseType::CANCEL: {
            std::cout << "Cancel clicked." << std::endl;
            break;
        }
        default: {
            std::cout << "Unexpected button clicked." << std::endl;
            break;
        }
    }
    delete dialog;
}