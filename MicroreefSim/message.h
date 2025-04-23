#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

// message.h  : x fonctions pour l'affichage des messages d'erreur
//              et une fonction supplémentaire pour indiquer le succès de la lecture
//
// public version V1
//

#include <string>

namespace message
{
	// age must be strictly positive
	std::string lifeform_age(unsigned age);

	// only for the center coordinate that are explicitely visible in the file
	// allowed interval is [1, max-1]
	std::string lifeform_center_outside(double x, double y);

	// only for the inner rotation center and the extremity that have to be computed.
	// allowed interval is ]0, max[
	// use the lifeform_center_outside as message for the corail origin
	std::string lifeform_computed_outside(unsigned id, double x, double y);

	std::string segment_length_outside(unsigned id, unsigned l);

	std::string segment_angle_outside(unsigned id, double a);

	std::string scavenger_radius_outside(unsigned r);

	std::string lifeform_duplicated_id(unsigned id);

	std::string lifeform_invalid_id(unsigned id);

	// as suggested by their name the segment index values start at zero
	std::string segment_collision(unsigned id1, unsigned index1,
								  unsigned id2, unsigned index2);

	// only for the case of 2 consecutive segments
	std::string segment_superposition(unsigned id, unsigned s1, unsigned s2);

	// Everything went well => file reading and all validation checks
	std::string success();
}

#endif // MESSAGE_H_INCLUDED
