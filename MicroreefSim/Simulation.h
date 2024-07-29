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

    void remove_Algae_From_Simulation(const Algae& algae);
    void remove_Coral_From_Simulation(const Coral& coral);
    void remove_Scavenger_From_Simulation(const Scavenger& scavenger);

    void reset_simulation();

    // for testing purposes print out the size of
    void printEntitiesSize() const;
    void printCorals() const;

    std::vector<Algae> get_algae_in_simulation() const;
    std::vector<Coral> get_coral_in_simulation() const;
    std::vector<Scavenger> get_scavenger_in_simulation() const;
    void rotateCoral(Coral& coral);

    void rotateCorals();

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

    void updateAlgae();       // helper method for updateEntities
    void updateCorals();      // helper method for updateEntities
    void updateScavengers();  // helper method for updateEntities

    void death_to_algae();   // helper method for updateAlgae, better conception
    void algae_generator();  // helper method for updateAlgae, better conception

    void death_to_corals();

    void death_to_scavengers();

    void startAlgaeBirth();
    void stopAlgaeBirth();

    unsigned int generateNewUniqueID();
    void growOrReproduceCoral(Coral& coral);

    bool Do_Segment_intersect(const Coral& coral) const;
    // Check if a given coral intersects or superimposes with other segments
    bool checkCoralIntersection(const Coral& coral) const;

    // Check for algae interaction and consume any algae the coral intersects with
    void checkAndConsumeAlgae(Coral& coral);
    bool sweepingPassDetected(const Coral& coral) const;

    void print_algae_vector_with_age() const;
    void reproduceCorals();
    void reproduce_Coral_by_division(Coral& coral);
    Coral generate_coralOffspring(Coral coral);
    bool coral_algae_intersrct(Coral& coral);

    void generateScavengerOffspring(S2d position_Of_baby_scavenger);

    void remove_eaten_corals_from_simulation();

    void scavengerFeedsOnCoral(Scavenger& Scavenger);
    Coral* findNearestDeadCoral(const S2d& position);
    // we can return nullprt if no coral is found hihi!
    // TODO add assign nearest dead coral to scavenger by checking the distance between
    // all of the scavengers and the dead corals

    void moveScavenger_toDeadCoral(Scavenger& scavenger, Coral* coral);
    Coral* findCoralById(unsigned int coralId);

    void printScavengers() const;
};

#endif  // SIMULATION_H
