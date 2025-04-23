/**
 * File: main.cpp
 * ---------------
 * Description: This is the main entry point for the GTKMM-based simulation
 * application, "projet". It initializes the Simulation object with a specified
 * configuration file and sets up the primary window of the application using
 * SimulationWindow.
 *
 *              The program requires a command-line argument specifying the path to a
 * configuration file, which contains initial settings for the simulation. If this
 * argument is missing, the program outputs a usage message and exits with an error
 * code.
 *
 *              Key operations include:
 *              - Parsing command-line arguments to retrieve the configuration file
 * path.
 *              - Initializing the Simulation object with the provided configuration.
 *              - Creating and running the GTKMM application with SimulationWindow as
 * the main interface.
 *
 * Usage:
 *      ./projet <textfile.txt>, le fichier txt est optionnel
 *      - projet: The compiled application binary.
 *      - textile.tct: The file containing initial settings for the simulation. This
 * file should be in the current directory or specified with a relative or absolute
 * path.
 *
 * Authors: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include "SimulationWindow.h"

int main(int argc, char** argv) {
    Simulation simulation;
    // Vérifier si un argument de ligne de commande est fourni sinon open a empty
    // window
    if (argc < 2) {
        // simulation.methode_test_demander_par_le_professeur_au_rendu_2("test_prof.txt");
        auto app = Gtk::Application::create();
        return app->make_window_and_run<SimulationWindow>(1, argv, simulation);
        // std::cerr << "Utilisation: " << argv[0] << " <fichier_de_configuration>\n";
        // return 1;  // Retourner un code d'erreur
    }

    // voir BA1 si ta du mal avec cette partie vers la fin du semestre
    const char* filename = argv[1];

    // Poursuivre avec la simulation en utilisant le nom de fichier fourni

    simulation.start(filename);
    // simulation.methode_test_demander_par_le_professeur_au_rendu_2("test_prof.txt");
    auto app = Gtk::Application::create();

    return app->make_window_and_run<SimulationWindow>(1, argv, simulation);
}
