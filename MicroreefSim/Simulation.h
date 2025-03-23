/**
 * File: Simulation.h
 * -------------------
 * Description: This header defines the Simulation class, the core of the ecosystem
 * simulation. It manages the lifecycle and interactions of various entities such as
 * algae, coral, and scavengers. This class handles configuration file parsing, entity
 *              creation and destruction, and simulation state management including
 * random events driven by internal distributions. It provides mechanisms for error
 * handling, reporting, and debug outputs, ensuring the simulation runs smoothly and
 * accurately according to predefined ecological rules.
 *
 * Author: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include <random>  //for random number generation

#include "Algae.h"
#include "Coral.h"
#include "Scavenger.h"
#include "message.h"

class Simulation {
public:
    Simulation();
    void start(const std::string& config_file);
    /* // for testing purposes
    const std::vector<Algae>& getAlgaeVector() const { return algaeVec; }
    const std::vector<Coral>& getCoralVector() const { return coralVec; }
    const std::vector<Scavenger>& getScavengerVector() const { return scavengerVec; }
  */
    void saveSimulation(const std::string& filename = "simulation_state.txt");

    unsigned getAlgaeCount() const;
    unsigned getCoralCount() const;
    unsigned getScavengerCount() const;
    bool getAlgaeBirthAllowed() const;
    bool setAlgaeBirthAllowed(bool value);
    void toggleAlgaeBirthAllowed();

    void resetRandomEngineForNewFile();  // random number generation

    void updateEntities();
    void add_Algae_To_Simulation(const Algae& algae);
    void add_Coral_To_Simulation(const Coral& coral);
    void add_Scavenger_To_Simulation(const Scavenger& scavenger);

    void reset_simulation();

    // for testing purposes
    // print out the size of

    void printEntitiesSize() const;

    std::vector<Algae> get_algae_in_simulation() const;
    std::vector<Coral> get_coral_in_simulation() const;
    std::vector<Scavenger> get_scavenger_in_simulation() const;

private:
    std::vector<Algae> algaeVec;
    std::vector<Coral> coralVec;
    std::vector<Scavenger> scavengerVec;
    static bool readFileSuccess;
    static bool algae_birth_allowed;
    // if true algae is born, stop algae birth, false bydefault

    //--------random number generation------
    std::default_random_engine e;
    std::bernoulli_distribution algaeCreationDistribution;
    std::uniform_int_distribution<unsigned> positionDistribution;
    //---------------------------------------

    void readConfigFile(const std::string& filename);
    void readAlgae(std::ifstream& file, int count);
    void readCoral(std::ifstream& file, int count);
    void readScavenger(std::ifstream& file, int count);
    void handleFileReadError(std::ifstream& file, int entryIndex,
                             const std::string& entityType) const;

    bool validateAlgae(const Algae& algae) const;

    bool validateCoral(const Coral& coral) const;
    bool validateCoral_pos(const Coral& coral) const;
    bool validateCoralUniqueID(const Coral& coral) const;
    bool validateCoralSegmentAngles(const Coral& coral) const;
    bool validateCoralSegmentLengths(const Coral& coral) const;
    bool validateCoralSegmentsSuperposition(const Coral& coral) const;
    bool validateCoral_other_Segments_Superposition(const Coral& coral) const;
    bool validateCoral_self_SegmentsIntersect(const Coral& coral) const;
    bool validateCoral_other_SegmentsIntersect(const Coral& coral) const;
    bool validateScavenger(const Scavenger& scavenger) const;
    bool validate_rayon_scavenger(const Scavenger& scavenger) const;
    bool validate_sca_corail_cible(const Scavenger& scavenger) const;

    bool validate_scavenger_pos(const Scavenger& scavenger) const;
    bool validate_scavenger_age(const Scavenger& scavenger) const;

    bool validate_coral_age(const Coral& coral) const;

    void reportError(const std::string& errorMessage) const;

    // file_saving helpers
    void saveAlgae(std::ofstream& outFile);
    void saveCorals(std::ofstream& outFile);
    void saveScavengers(std::ofstream& outFile);

    void clearAllEntities();

    void updateAlgae();
    void updateCorals();      // rendue 3
    void updateScavengers();  // rendue 3

    void death_to_algae();   // helper method for updateAlgae, better conception
    void algae_generator();  // helper method for updateAlgae, better conception

    void startAlgaeBirth();
    void stopAlgaeBirth();
};

#endif  // SIMULATION_H
