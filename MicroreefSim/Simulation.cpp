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
 * Version: 1.0
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
    readConfigFile(config_file);
    // std::cout << "INITIAL STATE" << std::endl;
    // print_algae_vector_with_age();
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
    Scavenger::clear_targetIDs();
}

void Simulation::updateEntities() {
    updateAlgae();
    updateCorals();
    updateScavengers();
}

void Simulation::print_algae_vector_with_age() const {
    std::cout << "Printing algae vector with age..." << std::endl;
    // max algae age:
    std::cout << "Max algae age: " << max_life_alg << std::endl;
    for (const auto& algae : algaeVec) {
        std::cout << "Algae at position (" << algae.getPosition().x << ", "
                  << algae.getPosition().y << ") with age " << algae.getAge()
                  << std::endl;
    }
}

void Simulation::updateAlgae() {
    death_to_algae();
    algae_generator();
}

void Simulation::death_to_algae() {
    // std::cout << "checking death to algae" << std::endl;
    for (size_t i = 0; i < algaeVec.size(); ++i) {
        algaeVec[i].incrementAge();
        if (algaeVec[i].getAge() >= max_life_alg) {
            algaeVec.erase(algaeVec.begin() + i);
            Algae::decrementNbAlg();
            i--;  // Decrement i to adjust for the removed element
        }
    }
    // printEntitiesSize();
    // print_algae_vector_with_age();
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

void Simulation::updateCorals() {
    std::vector<Coral> temporary_coral_vector;
    death_to_corals();
    for (auto& coral : coralVec) {
        if (coral.getStatut() == DEAD) {
            continue;  // Skip dead corals
        }
        if (coral.get_last_segment().getLength() < l_repro) {
            rotateCoral(coral);
        } else {
            if (coral.getStatutDev() == EXTEND) {
                double new_seg_length = l_repro - l_seg_interne;
                double new_angle = coral.getSegments().back().getAngle();
                coral.addSegment(new_angle, new_seg_length);
                coral.setStatutDev(REPRO);
                while (!coral.isWithinBoundaries(max) ||
                       !coral.last_segment_is_within_boundaries(max) ||
                       checkCoralIntersection(coral)) {
                    coral.rotate_last_segment(delta_rot);
                }
            } else {
                // reproduce_Coral_by_division(coral);
                const Coral old_coral = coral;
                Coral new_baby_coral = generate_coralOffspring(old_coral);
                if (new_baby_coral.getID() == -42) {
                    std::cerr << "new baby coral is invalid" << std::endl;
                } else {
                    temporary_coral_vector.push_back(new_baby_coral);
                    coral = old_coral;
                    Segment lastSegment = coral.get_last_segment();
                    coral.set_last_segment_length(lastSegment.getLength() / 2);
                }
                coral.setStatutDev(EXTEND);
            }
        }
    }
    // merge the temporary vector with the original vector
    coralVec.insert(coralVec.end(), temporary_coral_vector.begin(),
                    temporary_coral_vector.end());
}

void Simulation::updateScavengers() {
    death_to_scavengers();
    for (auto& scavenger : scavengerVec) {
        if (scavenger.getStatus() == LIBRE) {
            if (scavenger.getTargetCoralId() == -1) {
                // move to DEad coral disponible le plus proche, deplacement
                Coral* nearestDeadCoral =
                    findNearestDeadCoral(scavenger.getPosition());
                if (nearestDeadCoral == nullptr) {
                    // std::cout << "no dead corals found" << std::endl;
                } else {
                    scavenger.set_targetCoralId(nearestDeadCoral->getID());
                    moveScavenger_toDeadCoral(scavenger, nearestDeadCoral);
                    Scavenger::addTargetID(nearestDeadCoral->getID());
                }
            } else {
                // move to the target coral
                Coral* targetCoral = findCoralById(scavenger.getTargetCoralId());
                if (targetCoral == nullptr) {
                    // std::cout << "no target corals found" << std::endl;
                } else {
                    moveScavenger_toDeadCoral(scavenger, targetCoral);
                }
            }
        } else {
            // mange
            scavengerFeedsOnCoral(scavenger);
        }
    }
}

void Simulation::death_to_corals() {
    for (size_t i(0); i < coralVec.size(); ++i) {
        // check if coral's age is equal to max_life_cor if so kill it aka change kill
        // it using the killCoral() method
        coralVec[i].incrementAge();
        if (coralVec[i].getAge() == max_life_cor) {
            coralVec[i].killCoral();
        }
    }
}

void Simulation::death_to_scavengers() {
    // ckeck scavenger's age is equal to max_life_sca if so kill it aka remove it from
    // the sim aka from the vector
    for (size_t i = 0; i < scavengerVec.size(); ++i) {
        scavengerVec[i].incrementAge();
        if (scavengerVec[i].getAge() == max_life_sca) {
            scavengerVec.erase(scavengerVec.begin() + i);
            Scavenger::decrementNbScavengers();
            i--;  // Decrement i to adjust for the removed element
        }
    }
}

unsigned int Simulation::generateNewUniqueID() {
    // Retrieve the current set of unique IDs from the Coral class.
    std::set<int>& uniqueIDs = Coral::getUniqueIDs();

    // Initialize the new ID to start searching from.
    static unsigned int generated_unique_ID = 1;

    // Increment to find a unique ID not in the set.
    while (uniqueIDs.count(generated_unique_ID) != 0) {
        ++generated_unique_ID;
    }

    // After finding a unique ID, ensure it gets added to the set in the Coral class.
    uniqueIDs.insert(generated_unique_ID);

    return generated_unique_ID;
}

void Simulation::rotateCoral(Coral& coral) {
    // Check if the coral is dead (if so, do nothing)
    // std::cout << "rotating coral called" << std::endl;
    if (coral.getStatut() == DEAD) {
        return;
    }
    if (coral.getSegments().empty()) {
        std::cerr << "Error: Trying to rotate a coral with no segments, coral ID: "
                  << coral.getID() << "\n";
        exit(EXIT_FAILURE);
        return;
    }

    // std::cout << "rotating coral called" << std::endl;
    coral.rotate_last_segment(delta_rot);

    // Rotate the last segment based on the current direction

    // Check for map border collisions
    if (!coral.isWithinBoundaries(max)) {
        coral.switchRotationDirection();
        return;
    }
    // Check for intersection or superposition
    if (checkCoralIntersection(coral)) {
        coral.switchRotationDirection();
        return;
    }
    // Check for sweeping pass
    if (sweepingPassDetected(coral)) {
        coral.switchRotationDirection();
        return;
    }
    // Check for algae interaction
    checkAndConsumeAlgae(coral);
}

void Simulation::rotateCorals() {
    for (auto& coral : coralVec) {
        rotateCoral(coral);
    }
}

bool Simulation::checkCoralIntersection(const Coral& coral) const {
    auto segments = coral.getSegments();
    if (segments.empty())
        return false;  // No segments to check

    const Segment& lastSegment = segments.back();

    // Check for intersection/superposition within the same coral
    for (size_t i = 0; i < segments.size() - 1; ++i) {
        if (lastSegment.doIntersect(lastSegment, segments[i]) ||
            lastSegment.areSegmentsInSuperposition(lastSegment, segments[i])) {
            return true;
        }
    }

    // Check for intersection with other corals
    for (const auto& otherCoral : coralVec) {
        if (&coral == &otherCoral)
            continue;
        auto otherSegments = otherCoral.getSegments();
        for (const auto& segment : otherSegments) {
            if (lastSegment.doIntersect(lastSegment, segment)) {
                return true;
            }
        }
    }

    return false;
}

// work around the superposition not being detected, its slopy but i dont know how to
// fix it

bool Simulation::sweepingPassDetected(const Coral& coral) const {
    auto segments = coral.getSegments();
    if (segments.size() < 2)
        return false;  // Not enough segments to check

    const Segment& lastSegment = segments.back();
    const Segment& secondLastSegment = segments[segments.size() - 2];

    // Get the endpoints before and after rotation
    S2d lastSegmentExtremityBefore = lastSegment.calculate_extremite();
    Segment rotatedLastSegment = lastSegment;
    rotatedLastSegment.rotate(delta_rot);
    S2d lastSegmentExtremityAfter = rotatedLastSegment.calculate_extremite();

    // Determine an appropriate number of intermediate points based on segment length
    const int numIntermediatePoints =
        std::max(5, static_cast<int>(lastSegment.getLength() / 2));

    // Calculate intermediate points based on rotation direction (clockwise or
    // counterclockwise)
    std::vector<S2d> sweepingArc;
    sweepingArc.push_back(lastSegmentExtremityBefore);
    double direction = (delta_rot > 0) ? 1 : -1;
    for (int i = 1; i < numIntermediatePoints; ++i) {
        double intermediateAngle = lastSegment.getAngle() + direction * i *
                                                                std::fabs(delta_rot) /
                                                                numIntermediatePoints;
        sweepingArc.push_back(
            {lastSegment.getBase().x +
                 lastSegment.getLength() * std::cos(intermediateAngle),
             lastSegment.getBase().y +
                 lastSegment.getLength() * std::sin(intermediateAngle)});
    }
    sweepingArc.push_back(lastSegmentExtremityAfter);

    // Check for intersections between the sweeping arc and the second-last segment
    for (size_t i = 1; i < sweepingArc.size(); ++i) {
        Segment arcSegment(sweepingArc[i - 1],
                           std::atan2(sweepingArc[i].y - sweepingArc[i - 1].y,
                                      sweepingArc[i].x - sweepingArc[i - 1].x),
                           std::hypot(sweepingArc[i].x - sweepingArc[i - 1].x,
                                      sweepingArc[i].y - sweepingArc[i - 1].y));
        if (Segment::doIntersect(arcSegment, secondLastSegment)) {
            return true;  // Sweeping pass detected
        }
    }

    return false;  // No sweeping pass detected
}

void Simulation::checkAndConsumeAlgae(Coral& coral) {
    if (coral.getStatut() == DEAD) {
        return;  // Do not consume algae if the coral is dead
    }
    double hitbox_algae = epsil_zero * 1.2;
    for (size_t i = 0; i < algaeVec.size(); ++i) {
        if (coral.get_last_segment().intersectsCircle(algaeVec[i].getPosition(),
                                                      hitbox_algae)) {
            // Attempt to extend the coral's last segment
            coral.extend_last_segment(delta_l);
            // Check for boundary and intersection conditions
            if (!coral.isWithinBoundaries(max)) {
                // If any issues arise, revert the extension and don't consume the
                // algae
                coral.switchRotationDirection();
                coral.decrease_last_segment_length(delta_l);
            } else if (!coral.last_segment_is_within_boundaries(max)) {
                coral.switchRotationDirection();
                coral.decrease_last_segment_length(delta_l);

            } else if (checkCoralIntersection(coral)) {
                coral.switchRotationDirection();
                coral.decrease_last_segment_length(delta_l);

            } else {
                // Valid extension, remove the algae
                algaeVec.erase(algaeVec.begin() + i);
                break;  // Only one algae is consumed per rotation
            }
        }
    }
}

bool Simulation::coral_algae_intersrct(Coral& coral) {
    for (size_t i = 0; i < algaeVec.size(); ++i) {
        if (coral.get_last_segment().intersectsCircle(algaeVec[i].getPosition(),
                                                      r_alg)) {
            return true;
        }
    }
    return false;
}
/*
void Simulation::reproduce_Coral_by_division(Coral& coral) {
    if (coral.getSegments().empty()) {
        std::cerr << "Error: Trying to reproduce a coral with no segments, coral ID: "
                  << coral.getID() << "\n";
        exit(EXIT_FAILURE);
        return;
    }
    // if the coral is dead, do not reproduce
    if (coral.getStatut() == DEAD) {
        return;
    }
    Segment lastSegment = coral.get_last_segment();
    S2d lastSegmentExtremity = lastSegment.calculate_extremite();
    coral.set_last_segment_length(lastSegment.getLength() / 2);
    // std::cout << "old coral  right after using set_last_segment_length " << coral
    //  << std::endl;
    if (coral.getSegments().empty()) {
        std::cerr << "Error: Coral has no segments after halving during reproduction, "
                  << "coral ID: " << coral.getID() << "\n";
        exit(EXIT_FAILURE);
        return;
    }
    double offset = l_repro - l_seg_interne;
    double angle = lastSegment.getAngle();
    S2d new_coral_base = {lastSegmentExtremity.x - offset * std::cos(angle),
                          lastSegmentExtremity.y - offset * std::sin(angle)};
    unsigned int new_coral_Id = generateNewUniqueID();
    Coral::addUniqueID(new_coral_Id);
    Coral newCoral(new_coral_base, 1, new_coral_Id, ALIVE,
                   coral.getDirectionRotation(), EXTEND, 1, angle,
                   l_repro - l_seg_interne);
    Coral old_coral = coral;
    // std::cout << "old coral after new coral creation" << coral << std::endl;
    coralVec.push_back(newCoral);  //! problem here, after this line the old coral
                                   //! becomes 0 for some reason
    // replace  coral with old_coral in the vector because the coral is now 00000000
    // for some reason
    coral = old_coral;
     std::cout << "old coral after reproduction" << coral << std::endl;
    std::cout << "old coral called old_coral" << old_coral << std::endl;
    std::cout << "new coral after reproduction" << newCoral << std::endl;
    std::cout << "last element in coralVec" << coralVec.back() << std::endl;
} */ // problem here, need to investigate why the old coral becomes 0000000 but after
// rendu 3

Coral Simulation::generate_coralOffspring(Coral coral) {
    if (coral.getSegments().empty()) {
        std::cerr << "Error: Trying to reproduce a coral with no segments, coral ID: "
                  << coral.getID() << "\n";
        exit(EXIT_FAILURE);
        return Coral(S2d{0, 0}, 1, -42, DEAD, TRIGO, EXTEND, 1, 0, 0);
        ;
    }
    // if the coral is dead, do not reproduce
    if (coral.getStatut() == DEAD) {
        return Coral(S2d{0, 0}, 1, -42, DEAD, TRIGO, EXTEND, 1, 0, 0);
    }
    Segment lastSegment = coral.get_last_segment();
    S2d lastSegmentExtremity = lastSegment.calculate_extremite();

    if (coral.getSegments().empty()) {
        std::cerr << "Error: Coral has no segments after halving during reproduction, "
                  << "coral ID: " << coral.getID() << "\n";
        exit(EXIT_FAILURE);
        // retrun coral with id -42
        return Coral(S2d{0, 0}, 1, -42, DEAD, TRIGO, EXTEND, 1, 0, 0);
    }
    double offset = l_repro - l_seg_interne;
    double angle = lastSegment.getAngle();
    S2d new_coral_base = {lastSegmentExtremity.x - offset * std::cos(angle),
                          lastSegmentExtremity.y - offset * std::sin(angle)};
    unsigned int new_coral_Id = generateNewUniqueID();
    Coral::addUniqueID(new_coral_Id);
    // std::cout << "old coral after new coral id generation" << coral << std::endl;
    Coral newCoral(new_coral_base, 1, new_coral_Id, ALIVE,
                   coral.getDirectionRotation(), EXTEND, 1, angle,
                   l_repro - l_seg_interne);
    // std::cout << "new coral after reproduction" << newCoral << std::endl;
    // std::cout << "old coral after reproduction" << coral << std::endl;
    // same problem here, after this line the old coral becomes 0000000
    return newCoral;
}

void Simulation::generateScavengerOffspring(S2d position_Of_baby_scavenger) {
    for (auto& scavenger : scavengerVec) {
        if (scavenger.getRadius() >= r_sca_repro) {
            // reproduce by division
            // new position on the line ofthe eaten coral but with a distance of
            // delta_l of the parent scavenger
            Scavenger newScavenger(position_Of_baby_scavenger, 1, r_sca, LIBRE);
            scavenger.setRadius(r_sca);
            add_Scavenger_To_Simulation(newScavenger);
        }
    }
}

// le corail disparait des que tt ses segment sont consomes
void Simulation::remove_eaten_corals_from_simulation() {
    for (size_t i = 0; i < coralVec.size(); ++i) {
        if (coralVec[i].getSegments().empty()) {
            // remove the coral's id from the set of unique IDs and from the set of
            // target IDs
            Coral::removeUniqueID(coralVec[i].getID());
            Scavenger::removeTargetID(coralVec[i].getID());
            coralVec.erase(coralVec.begin() + i);
            Coral::decrementNbCoral();
            i--;  // Decrement i to adjust for the removed element
        }
    }
}

Coral* Simulation::findNearestDeadCoral(const S2d& position) {
    Coral* nearest = nullptr;
    double minDistance = std::numeric_limits<double>::max();
    // initializing the minDistance to the max value of double
    std::set<unsigned int> targetedIDs = Scavenger::getTargetIDs();

    // print out the targetedIDs and coralids
    // Scavenger::printTargetIDs();

    for (auto& coral : coralVec) {
        if (coral.getStatut() == DEAD &&
            targetedIDs.find(coral.getID()) == targetedIDs.end()) {
            double distance = calculateDistance(coral.getPosition(), position);
            if (distance < minDistance) {
                minDistance = distance;
                nearest = &coral;
            }
        }
    }
    return nearest;
}

// alimentation sur le corail mort par deplacement de delta_l
void Simulation::scavengerFeedsOnCoral(Scavenger& scavenger) {
    Coral* coral = findCoralById(scavenger.getTargetCoralId());
    if (coral == nullptr) {
        return;  // If there is no dead coral, do nothing.
    }
    if (coral->getSegments().empty() ||
        coral->getPosition() == scavenger.getPosition()) {
        Coral::removeUniqueID(coral->getID());
        Scavenger::removeTargetID(coral->getID());
        remove_Coral_From_Simulation(*coral);
        scavenger.setStatus(LIBRE);
        scavenger.set_targetCoralId(-1);
        return;  // No segments to consume.
    }
    S2d last_segment_Base = coral->get_last_segment().getBase();
    S2d current_Scavenger_Position = scavenger.getPosition();
    S2d last_segment_Extremite = coral->get_last_segment().calculate_extremite();
    // Memorize the extremite
    S2d direction = {last_segment_Base.x - current_Scavenger_Position.x,
                     last_segment_Base.y - current_Scavenger_Position.y};
    double distanceToBase =
        std::sqrt(direction.x * direction.x + direction.y * direction.y);
    // Normalize the direction vector
    if (distanceToBase > 0) {
        direction.x /= distanceToBase;
        direction.y /= distanceToBase;
    }
    S2d newPosition = {current_Scavenger_Position.x + direction.x * delta_l,
                       current_Scavenger_Position.y + direction.y * delta_l};
    if (distanceToBase <= delta_l) {
        coral->remove_last_segment();
        scavenger.setPosition(last_segment_Base);
        scavenger.increse_radius(delta_r_sca);
    } else {
        coral->decrease_last_segment_length(delta_l);
        scavenger.move(newPosition);
        scavenger.increse_radius(delta_r_sca);
    }  // if the coral is  completly consumed, change the scavenger's status to LIBRE,
       // and set the target id to -1
    if (scavenger.getRadius() >= r_sca_repro) {
        S2d position_Of_baby_scavenger = {last_segment_Extremite.x + delta_l,
                                          last_segment_Extremite.y + delta_l};
        generateScavengerOffspring(position_Of_baby_scavenger);
    }
}

void Simulation::moveScavenger_toDeadCoral(Scavenger& scavenger, Coral* coral) {
    // std::cout << "moving scavenger to dead coral" << std::endl;
    if (coral == nullptr) {
        return;  // If there is no dead coral, do nothing.
    }
    if (coral->getSegments().empty()) {
        Coral::removeUniqueID(coral->getID());
        Scavenger::removeTargetID(coral->getID());
        remove_Coral_From_Simulation(*coral);
        scavenger.setStatus(LIBRE);
        scavenger.set_targetCoralId(-1);
    }
    if (scavenger.getPosition() == coral->get_last_segment().calculate_extremite()) {
        // change the scavenger's status to MANGE
        scavenger.setStatus(MANGE);
        return;
        // if the scavenger is already at the base of the coral do nothing
    }

    S2d direction = {
        coral->get_last_segment().calculate_extremite().x - scavenger.getPosition().x,
        coral->get_last_segment().calculate_extremite().y - scavenger.getPosition().y};
    double length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length <= delta_l) {
        // set the scavenger's position to the endpoint of the coral
        scavenger.setStatus(MANGE);
        scavenger.move(coral->get_last_segment().calculate_extremite());
        return;
    }
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }
    // Move the scavenger towards the coral by delta_l
    S2d newPosition = {scavenger.getPosition().x + direction.x * delta_l,
                       scavenger.getPosition().y + direction.y * delta_l};

    scavenger.move(newPosition);
}

Coral* Simulation::findCoralById(unsigned int coralId) {
    for (size_t i = 0; i < coralVec.size(); ++i) {
        if (coralVec[i].getID() == coralId) {
            return &coralVec[i];
        }
    }
    return nullptr;  // Return null if no coral with the specified ID is found
}

void Simulation::remove_Algae_From_Simulation(const Algae& algae) {
    for (size_t i = 0; i < algaeVec.size(); ++i) {
        if (algaeVec[i] == algae) {
            algaeVec.erase(algaeVec.begin() + i);
            Algae::decrementNbAlg();
            break;
        }
    }
}

void Simulation::remove_Coral_From_Simulation(const Coral& coral) {
    for (size_t i = 0; i < coralVec.size(); ++i) {
        if (coralVec[i] == coral) {
            coralVec.erase(coralVec.begin() + i);
            Coral::decrementNbCoral();
            break;
        }
    }
}

void Simulation::remove_Scavenger_From_Simulation(const Scavenger& scavenger) {
    for (size_t i = 0; i < scavengerVec.size(); ++i) {
        if (scavengerVec[i] == scavenger) {
            scavengerVec.erase(scavengerVec.begin() + i);
            Scavenger::decrementNbScavengers();
            break;
        }
    }
}

void Simulation::printCorals() const {
    std::cout << "----------------Begin of corals----------------" << std::endl;
    std::cout << "there are " << coralVec.size() << " corals in the simulation"
              << std::endl;
    std::cout << "Printing corals...\n\n\n\n" << std::endl;

    for (auto& coral : coralVec) {
        std::cout << "______________________________________" << std::endl;
        std::cout << "coral id: " << coral.getID() << std::endl;
        std::cout << "number of segments from NBeg " << coral.getNbSeg() << std::endl;
        std::cout << "number of segments from getSegments() "
                  << coral.getSegments().size() << std::endl;
        std::cout << coral << std::endl;
        std::cout << "______________________________________" << std::endl;
    }

    std::cout << "----------------END OF CORALS----------------" << std::endl;
}

void Simulation::printScavengers() const {
    std::cout << "there are " << scavengerVec.size() << " scavengers in the simulation"
              << std::endl;
    std::cout << "Printing scavengers...\n\n\n\n" << std::endl;

    for (auto& scavenger : scavengerVec) {
        std::cout << "______________________________________" << std::endl;
        std::cout << scavenger << std::endl;
        if (scavenger.getStatus() == MANGE) {
            std::cout << "scavenger is feeding on coral with id: "
                      << scavenger.getTargetCoralId() << std::endl;
        } else {
            std::cout << "scavenger is not feeding on any coral,  scavenger target id:"
                      << scavenger.getTargetCoralId() << std::endl;
        }
    }

    std::cout << "----------------END OF SCAVENGERS----------------" << std::endl;
}
