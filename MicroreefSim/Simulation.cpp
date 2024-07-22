/**
 * File: Simulation.cpp
 * ---------------------
 * Description: Implements the Simulation class from Simulation.h. This source file
 * includes the execution logic of starting the simulation, reading configuration
 * files, and updating the simulation's state. It handles the complexities of managing
 *              various ecological entities through their life cycles from birth to
 * natural death, enforcing unique constraints, and facilitating ecological
 * interactions. The implementation ensures accurate and efficient simulation behavior,
 * including handling file I/O for state persistence and recovery.
 *
 * Author: Bahey Shalash 
 * Version: 2.0
 * Date: 27/04/2024
 */

#include "Simulation.h"

#include <fstream>
#include <stdexcept>

bool Simulation::readFileSuccess = true;
bool Simulation::algae_birth_allowed = false;

Simulation::Simulation()
    : algaeCreationDistribution(alg_birth_rate), positionDistribution(1, max - 1) {
    e.seed(1);  // This seeds the random number generator
}

void Simulation::start(const std::string& config_file) {
    reset_simulation();
    // printEntitiesSize();  // for debugging purposes
    // std::cout << "------------------------------------" << std::endl;
    // std::cout << "Simulation start method was called." << std::endl;
    // std::cout << "readFileSuccess 1=true  0=false " << readFileSuccess << std::endl;
    // std::cout << "------------------------------------" << std::endl;
    //  Clear all entities before reading the file
    readConfigFile(config_file);
    // printEntitiesSize();  // for debugging purposes

    // std::cout << "readFileSuccess 1=true  0=false " << readFileSuccess << std::endl;
    //  saveSimulation("simulation_state.txt");
    // std::cout << "------------------------------------" << std::endl;
}
void Simulation::readConfigFile(const std::string& filename) {
    clearAllEntities();             // clear all entities before reading the file
    resetRandomEngineForNewFile();  // Reset the random engine for reproducibility
    std::ifstream file(filename);
    if (!file.is_open()) {
        reportError("Cannot open configuration file.");
        return;
    }

    // Skip comments and empty lines
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        } else {
            // Once we have a non-empty, non-comment line, put it back into the stream
            file.putback('\n');  // Newline character we've just read
            for (auto it = line.rbegin(); it != line.rend(); ++it) {
                file.putback(*it);  // Put the rest of the line back into the stream
            }
            break;  // Break the loop once we've put back a line with actual content
        }
    }

    int numAlgae;
    file >> numAlgae;
    readAlgae(file, numAlgae);
    int numCoral;
    file >> numCoral;
    readCoral(file, numCoral);
    int numScavenger;
    file >> numScavenger;
    readScavenger(file, numScavenger);

    if (!readFileSuccess) {
        clearAllEntities();
        file.close();
        return;
    }

    file.close();
    std::cout << message::success();
}

void Simulation::handleFileReadError(std::ifstream& file, int entryIndex,
                                     const std::string& entityType) const {
    std::cerr << "Failed to read " << entityType << " data at entry " << entryIndex
              << std::endl;  // maybe should remove to not mess up automatic coorection
    file.clear();            // Clear error flags
    std::string dummy;
    std::getline(file, dummy);  // Skip the problematic line
}

// Implement readAlgae, readCoral, and readScavenger methods...
//-------------------readAlgae-------------------

void Simulation::readAlgae(std::ifstream& file, int count) {
    // std::cout << "Reading " << count << " algae entries." << std::endl;
    for (int i = 0; i < count; ++i) {
        double x, y;
        unsigned int age;
        file >> x >> y >> age;
        if (file.fail()) {
            handleFileReadError(file, i, "algae");
            continue;
        }
        // std::cout << "Read algae with position (" << x << ", " << y << ") and age "
        //<< age << std::endl;

        Algae algae(S2d{x, y}, age);
        if (validateAlgae(algae)) {
            algaeVec.push_back(algae);
        } else {
            // std::cout << "Invalid algae data at entry " << i << std::endl;
        }
    }
    // std::cout << "Finished reading algae. Vector size is now " << algaeVec.size()
    //  << std::endl;
}
//-------------------validateAlgae-------------------
bool Simulation::validateAlgae(const Algae& algae) const {
    if (algae.getAge() <= 0) {
        std::cout << message::lifeform_age(algae.getAge());
        // std::exit(EXIT_FAILURE);  // redue 2
        readFileSuccess = false;
        return false;
    }
    //[1,max-1]
    if (algae.getPosition().x < 1 || algae.getPosition().x > max - 1 ||
        algae.getPosition().y < 1 || algae.getPosition().y > max - 1) {
        std::cout << message::lifeform_center_outside(algae.getPosition().x,
                                                      algae.getPosition().y);
        // std::exit(EXIT_FAILURE);
        readFileSuccess = false;
        return false;
    }
    return true;
}
// if error is detected stop the reading of the file and

//-------------------readCoral-------------------
void Simulation::readCoral(std::ifstream& file, int count) {
    for (int i = 0; i < count; ++i) {
        double x, y, firstAngle, firstLength;
        unsigned int age, id, nbseg;
        int statut_int, dir_rot_int, statut_dev_int;
        file >> x >> y >> age >> id >> statut_int >> dir_rot_int >> statut_dev_int >>
            nbseg;
        if (file.fail()) {
            handleFileReadError(file, i, "coral");
            continue;
        }
        file >> firstAngle >> firstLength;
        if (file.fail()) {
            handleFileReadError(file, i, "coral");
            continue;
        }
        Statut_cor statut = static_cast<Statut_cor>(statut_int);
        Dir_rot_cor dir_rot_cor = static_cast<Dir_rot_cor>(dir_rot_int);
        Statut_dev statut_dev = static_cast<Statut_dev>(statut_dev_int);
        Coral coral(S2d{x, y}, age, id, statut, dir_rot_cor, statut_dev, nbseg,
                    firstAngle, firstLength);
        for (unsigned int j = 1; j < nbseg; ++j) {
            double angle, length;
            file >> angle >> length;
            if (file.fail()) {
                std::cout << "Failed_reasegment" << j << "fororal" << i << std::endl;
                file.clear();
                std::string dummy;
                std::getline(file, dummy);
                break;  // Stop reading further segments for this coral
            }
            coral.addSegment(angle, length);
        }
        if (validateCoral(coral)) {
            coralVec.push_back(coral);
        } else {
            /* std::cout << "Invalid coral data at entry " << i
                      << std::endl;  // to be removed */

            // here i can add the clearAllEntities() but it will go on the next coral
            //  and i dont want that i want it to stop maybe using break?
        }
    }
}

//-------------------readScavenger-------------------
void Simulation::readScavenger(std::ifstream& file, int count) {
    for (int i = 0; i < count; ++i) {
        double x, y;
        unsigned int age, rayon;
        int statut_sca_int, corail_id_cible = -1;

        file >> x >> y >> age >> rayon >> statut_sca_int;

        // If scavenger status is MANGE, then read the id of the target coral
        if (statut_sca_int == MANGE) {
            file >> corail_id_cible;
        }

        if (file.fail()) {
            handleFileReadError(file, i, "scavenger");
            continue;  // Skip this scavenger and move to the next one
        }

        // Convert the integer status to the enum value
        Statut_sca statut_sca = static_cast<Statut_sca>(statut_sca_int);
        Scavenger scavenger(S2d{x, y}, age, rayon, statut_sca, corail_id_cible);
        // Validate the scavenger data here before creating an instance
        if (validateScavenger(scavenger)) {
            scavengerVec.push_back(scavenger);  // Add the scavenger to the vector
        } else {
            // std::cerr << "Invalid scavenger data at entry " << i << std::endl;
            //  Handle invalid scavenger data appropriately
        }
    }
}

//-------------------validateCoral-------------------
bool Simulation::validateCoral(const Coral& coral) const {
    if (!validateCoral_pos(coral)) {
        readFileSuccess = false;
        return false;
    }

    if (!validateCoralUniqueID(coral)) {
        readFileSuccess = false;
        return false;
    }

    if (!validateCoralSegmentAngles(coral)) {
        readFileSuccess = false;
        return false;
    }

    if (!validateCoralSegmentLengths(coral)) {
        readFileSuccess = false;
        return false;
    }

    if (!validateCoralSegmentsSuperposition(coral)) {
        readFileSuccess = false;
        return false;
    }
    if (!validateCoral_other_Segments_Superposition(coral)) {
        readFileSuccess = false;
        return false;
    }

    if (!validateCoral_self_SegmentsIntersect(coral)) {
        readFileSuccess = false;
        return false;
    }
    if (!validateCoral_other_SegmentsIntersect(coral)) {
        readFileSuccess = false;
        return false;
    }

    return true;  // Coral passes all validations
}

bool Simulation::validate_coral_age(const Coral& coral) const {
    if (coral.getAge() <= 0) {
        std::cout << message::lifeform_age(coral.getAge());

        // exit(EXIT_FAILURE);
        return false;
    }
    return true;
}

bool Simulation::validateCoral_pos(const Coral& coral) const {
    //-------------------validate position of all bases-------------------
    // Check if the base position of the coral is within the allowed boundaries
    //
    if (coral.getPosition().x <= 0 || coral.getPosition().x > max ||
        coral.getPosition().y <= 0 || coral.getPosition().y > max) {
        std::cout << message::lifeform_center_outside(coral.getPosition().x,
                                                      coral.getPosition().y);
        // exit(EXIT_FAILURE);
        return false;
    }

    // Check each segment for valid position
    // Check if the extremity of each segment is within the allowed boundaries
    //]0,max[
    for (const auto& segment : coral.getSegments()) {
        S2d extremity = segment.calculate_extremite();
        if (extremity.x <= 0 || extremity.x > max || extremity.y <= 0 ||
            extremity.y > max) {
            std::cout << message::lifeform_computed_outside(coral.getID(), extremity.x,
                                                            extremity.y);

            // exit(EXIT_FAILURE);
            return false;
        }
    }

    return true;
}

bool Simulation::validateCoralUniqueID(const Coral& coral) const {
    // Attempt to add the coral's ID to the set of unique IDs
    bool isUnique = Coral::addUniqueID(coral.getID());

    if (!isUnique) {
        // If the ID was already in the set, it's not unique
        std::cout << message::lifeform_duplicated_id(coral.getID());
        // exit(EXIT_FAILURE);
        return false;
    }

    // If the code reaches here, the ID was not in the set and is therefore unique
    return true;
}

bool Simulation::validateCoralSegmentAngles(const Coral& coral) const {
    for (const auto& segment : coral.getSegments()) {
        double angle = segment.getAngle();
        if (angle < -M_PI || angle > M_PI) {
            std::cerr << message::segment_angle_outside(coral.getID(), angle);
            // exit(EXIT_FAILURE);
            return false;
        }
    }
    return true;
}
bool Simulation::validateCoralSegmentLengths(const Coral& coral) const {
    for (const auto& segment : coral.getSegments()) {
        double length = segment.getLength();
        if (length < l_repro - l_seg_interne || length >= l_repro) {
            std::cerr << message::segment_length_outside(coral.getID(), length);
            // exit(EXIT_FAILURE);

            return false;
        }
    }
    return true;
}
bool Simulation::validateCoralSegmentsSuperposition(const Coral& coral) const {
    auto segments = coral.getSegments();  // segments:vecteur de segment(autoplus
                                          // jolie)
    // First, check each segment against every other segment for superposition
    for (size_t i = 0; i < segments.size(); ++i) {
        for (size_t j = i + 1; j < segments.size(); ++j) {
            // Check for superposition
            if (segments[i].areSegmentsInSuperposition(segments[i], segments[j])) {
                std::cerr << message::segment_superposition(coral.getID(), i, j);
                // exit(EXIT_FAILURE);
                return false;  // Superposition detected
            }
        }
    }
    return true;  // No superpositions detected
}

bool Simulation::validateCoral_other_Segments_Superposition(const Coral& coral) const {
    // Iterate over all corals in coralVec to compare with the current coral's segments
    for (const auto& otherCoral : coralVec) {
        for (size_t i = 0; i < coral.getSegments().size(); ++i) {
            for (size_t j = 0; j < otherCoral.getSegments().size(); ++j) {
                if (coral.getSegments()[i].areSegmentsInSuperposition(
                        coral.getSegments()[i], otherCoral.getSegments()[j])) {
                    std::cout << message::segment_superposition(coral.getID(), i, j);
                    // exit(EXIT_FAILURE);

                    return false;  // Intersection detected
                }
            }
        }
    }

    return true;  // No intersections found
}

bool Simulation::validateCoral_self_SegmentsIntersect(const Coral& coral) const {
    std::vector<Segment> segments = coral.getSegments();
    /* std::cout << "------------helloo---------------" << std::endl;
    std::cout << "segments size: " << segments.size() << std::endl; */

    /* coral.printSegments(); */
    /* std::cout << "---------------------------" << std::endl; */

    // loop through all segments and compare them to all other segments
    for (size_t i = 0; i < segments.size(); ++i) {
        for (size_t j = i + 1; j < segments.size(); ++j) {
            if (segments[i].doIntersect(segments[i], segments[j])) {
                std::cout << message::segment_collision(coral.getID(), i,
                                                        coral.getID(), j);
                // exit(EXIT_FAILURE);
                return false;  // Intersection detected
            }
        }
    }

    return true;  // no intersections are detected, return true
}

bool Simulation::validateCoral_other_SegmentsIntersect(const Coral& coral) const {
    // Iterate over all corals in coralVec to compare with the current coral's segments
    for (const auto& otherCoral : coralVec) {
        for (size_t i = 0; i < coral.getSegments().size(); ++i) {
            for (size_t j = 0; j < otherCoral.getSegments().size(); ++j) {
                // Assuming Segment has a method like doIntersect that checks for
                // intersection between two segments, and considering segments[i]
                // allows access to the current segment
                if (coral.getSegments()[i].doIntersect(coral.getSegments()[i],
                                                       otherCoral.getSegments()[j])) {
                    std::cout << message::segment_collision(coral.getID(), i,
                                                            otherCoral.getID(), j);
                    // exit(EXIT_FAILURE);
                    return false;  // Intersection detected
                }
            }
        }
    }

    return true;  // No intersections found
}
//-------------------validateScavenger-------------------
bool Simulation::validateScavenger(const Scavenger& scavenger) const {
    if (!validate_scavenger_pos(scavenger)) {
        readFileSuccess = false;
        return false;
    }
    if (!validate_scavenger_age(scavenger)) {
        readFileSuccess = false;
        return false;
    }
    if (!validate_rayon_scavenger(scavenger)) {
        readFileSuccess = false;
        return false;
    }
    if (!validate_sca_corail_cible(scavenger)) {
        readFileSuccess = false;
        return false;
    }
    return true;
}

bool Simulation::validate_scavenger_pos(const Scavenger& scavenger) const {
    if (scavenger.getPosition().x < 1 || scavenger.getPosition().x > max - 1 ||
        scavenger.getPosition().y < 1 || scavenger.getPosition().y > max - 1) {
        std::cout << message::lifeform_center_outside(scavenger.getPosition().x,
                                                      scavenger.getPosition().y);
        // exit(EXIT_FAILURE);
        return false;
    }
    return true;
}
bool Simulation::validate_scavenger_age(const Scavenger& scavenger) const {
    if (scavenger.getAge() <= 0) {
        std::cout << message::lifeform_age(scavenger.getAge());
        // exit(EXIT_FAILURE);
        return false;
    }
    return true;
}

bool Simulation::validate_rayon_scavenger(const Scavenger& scavenger) const {
    if (scavenger.getRadius() < r_sca || scavenger.getRadius() >= r_sca_repro) {
        std::cout << message::scavenger_radius_outside(scavenger.getRadius());
        // exit(EXIT_FAILURE);
        return false;
    }
    return true;
}
bool Simulation::validate_sca_corail_cible(const Scavenger& scavenger) const {
    // Check if the scavenger is in MANGE status and then checks coralVec and see if
    // the the coral with his target id exists
    if (scavenger.getStatus() == MANGE) {
        bool found = false;
        for (const auto& coral : coralVec) {
            if (coral.getID() == scavenger.getTargetCoralId()) {
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << message::lifeform_invalid_id(scavenger.getTargetCoralId());
            // exit(EXIT_FAILURE);
            return false;
        }
    }

    return true;
}

void Simulation::reportError(const std::string& errorMessage) const {
    std::cerr << errorMessage << std::endl;
    exit(EXIT_FAILURE);
}

//____file_saving_________

void Simulation::saveSimulation(const std::string& filename) {
    std::ofstream outFile(filename);

    if (!outFile) {
        // Handle error - unable to open file for writing
        std::cerr << "Error: Unable to open file " << filename << " for writing."
                  << std::endl;
        return;
    }

    // Write the header or any initial comments
    outFile << "# Simulation State File\n" << std::endl;

    // Save each entity type
    saveAlgae(outFile);
    outFile << "\n";
    saveCorals(outFile);
    saveScavengers(outFile);

    outFile.close();
}

void Simulation::saveAlgae(std::ofstream& outFile) {
    outFile << algaeVec.size() << std::endl;
    for (const auto& algae : algaeVec) {
        outFile << "    " << algae << std::endl;
    }
}

void Simulation::saveCorals(std::ofstream& outFile) {
    outFile << coralVec.size() << std::endl;
    for (const auto& coral : coralVec) {
        outFile << "    " << coral << std::endl;
    }
}

void Simulation::saveScavengers(std::ofstream& outFile) {
    outFile << scavengerVec.size() << std::endl;
    for (const auto& scavenger : scavengerVec) {
        outFile << "    " << scavenger << std::endl;
    }
}

void Simulation::clearAllEntities() {
    algaeVec.clear();
    coralVec.clear();
    Coral::clear_uniqueIDs();
    scavengerVec.clear();
}

void Simulation::updateEntities() {
    updateAlgae();
    // updateCorals(); //rendue 3
    // updateScavengers();// rendue 3
}
/* void Simulation::updateAlgae() {
    // check if they're old enough to die!!
    for (size_t i = 0; i < algaeVec.size(); ++i) {
        // check if algae's age is equal to max_life_alg if so kill it aka remove it
        // from the vector
        algaeVec[i].incrementAge();
        if (algaeVec[i].getAge() == max_life_alg) {
            algaeVec.erase(algaeVec.begin() + i);
            Algae::decrementNbAlg();
        }
    }
    // generate new algae
    if (algae_birth_allowed) {
        if (algaeCreationDistribution(e)) {
            // Generate a random position for the new algae
            double x = positionDistribution(e);
            double y = positionDistribution(e);
            // Add new algae to the simulation
            Algae newAlgae(S2d{x, y}, 1);
            algaeVec.push_back(newAlgae);
            // or use add_Algae_To_Simulation neeed to check which is better practice
        }
    }
} */ //not very upgradable in my not so humble opinion :)

void Simulation::updateAlgae() {
    death_to_algae();
    algae_generator();
}

void Simulation::death_to_algae() {
    for (size_t i = 0; i < algaeVec.size(); ++i) {
        // check if algae's age is equal to max_life_alg if so kill it aka remove it
        // from the vector
        algaeVec[i].incrementAge();
        if (algaeVec[i].getAge() == max_life_alg) {
            algaeVec.erase(algaeVec.begin() + i);
            Algae::decrementNbAlg();
        }
    }
}

void Simulation::algae_generator() {
    // generate new algae
    if (algae_birth_allowed) {
        if (algaeCreationDistribution(e)) {
            // Generate a random position for the new algae
            double x = positionDistribution(e);
            double y = positionDistribution(e);
            // Add new algae to the simulation
            Algae newAlgae(S2d{x, y}, 1);
            algaeVec.push_back(newAlgae);
            // std::cout << "algae added to vector...." << std::endl;
            //  or use add_Algae_To_Simulation neeed to check which is better practice
        }
    }
}

unsigned Simulation::getAlgaeCount() const {
    return algaeVec.size();  // could've used nbAlg
}
unsigned Simulation::getCoralCount() const {
    return coralVec.size();  // could've used nbCor
}
unsigned Simulation::getScavengerCount() const {
    return scavengerVec.size();  // could;ve used nbSca
}

void Simulation::resetRandomEngineForNewFile() {
    e.seed(1);  // Re-seed the engine with a fixed value for reproducibility
}

void Simulation::startAlgaeBirth() {
    algae_birth_allowed = true;
}

void Simulation::stopAlgaeBirth() {
    algae_birth_allowed = false;
}

void Simulation::add_Algae_To_Simulation(const Algae& algae) {
    algaeVec.push_back(algae);
}

void Simulation::add_Coral_To_Simulation(const Coral& coral) {
    coralVec.push_back(coral);
}

void Simulation::add_Scavenger_To_Simulation(const Scavenger& scavenger) {
    scavengerVec.push_back(scavenger);
}

bool Simulation::getAlgaeBirthAllowed() const {
    return algae_birth_allowed;
}
bool Simulation::setAlgaeBirthAllowed(bool value) {
    algae_birth_allowed = value;
    return algae_birth_allowed;
    // could also make a switch mecanism aka if true stop if false start without taking
    // in any parameters actaully leys do that
}

void Simulation::toggleAlgaeBirthAllowed() {
    // 2 cases only, so if statement is better than switch
    if (algae_birth_allowed) {
        algae_birth_allowed = false;
        // std::cout << "Algae birth is now stopped." << std::endl;
    } else {
        algae_birth_allowed = true;
        // std::cout << "Algae birth is now allowed." << std::endl;
    }
}

void Simulation::reset_simulation() {
    clearAllEntities();
    resetRandomEngineForNewFile();
    readFileSuccess = true;
    // std::cout << "Simulation has been reset." << std::endl;
}

void Simulation::printEntitiesSize() const {
    std::cout << "Algae vector size: " << algaeVec.size() << std::endl;
    std::cout << "Coral vector size: " << coralVec.size() << std::endl;
    std::cout << "Scavenger vector size: " << scavengerVec.size() << std::endl;
}

std::vector<Algae> Simulation::get_algae_in_simulation() const {
    return algaeVec;
}
std::vector<Coral> Simulation::get_coral_in_simulation() const {
    return coralVec;
}
std::vector<Scavenger> Simulation::get_scavenger_in_simulation() const {
    return scavengerVec;
}
