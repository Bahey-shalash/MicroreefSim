// message.cc  : x fonctions pour l'affichage des messages d'erreur
//              et une fonction supplémentaire pour indiquer le succès de la lecture
//
// public version V1.2: 
//        1) message affiché par lifeform_computed_outside est modifié
//          -> en conséquence t04.txt et out04.txt sont modifiés
//        2) t12.txt est modifié pour enlever la seconde erreur (4 est remplacé par 12)
//
// public version V1.1: avec alignements corrects et message "length" corrigé
//

#include "message.h"

using namespace std;

namespace
{
    void reorder_for_consistency(unsigned &index1, unsigned &id1,
                                 unsigned &index2, unsigned &id2)
    {
        if (index1 > index2 || (index1 == index2 && id1 > id2))
        {
            swap(index1, index2);
            swap(id1, id2);
        }
    }
}

std::string message::lifeform_age(unsigned age)
{
    return string("lifeform age (") + to_string(age) 
         + string(") is not strictly positive\n");
}

std::string message::lifeform_center_outside(double x, double y)
{
    return string("lifeform at (") + to_string(x) + string(";") 
         + to_string(y) + string(") is outside allowed boundaries of [1, max-1]\n");
}

std::string message::lifeform_computed_outside(unsigned id, double x, double y)
{
    return string("lifeform (") + to_string(id) 
         + string(") at (") + to_string(x) + string(";") 
         + to_string(y) + string(") is outside allowed boundaries of ]0, max[\n");
}

std::string message::segment_length_outside(unsigned id, unsigned l)
{
    return string("lifeform (") + to_string(id) 
         + string(") segment length (") + to_string(l)
         + string(") is outside of [l_repro-l_seg_interne, l_repro[\n");
}

std::string message::segment_angle_outside(unsigned id, double a)
{
    return string("lifeform (") + to_string(id) + string(") segment orientation (") 
         + to_string(a) + string(") is outside of allowed boundaries of [-Pi, Pi]\n");
}

std::string message::scavenger_radius_outside(unsigned r)
{
    return string("Scavenger radius (") + to_string(r) 
         + string(") is outside of allowed boundaries of [r_sca,r_sca_repro[\n");
}

std::string message::lifeform_duplicated_id(unsigned id)
{
    return string("The Lifeform id : (") + to_string(id) + string(") is not unique\n");
}

std::string message::lifeform_invalid_id(unsigned id)
{
    return string("The Lifeform id (") + to_string(id) 
         + string(") does not belong to the set of existing lifeform ids\n");
}

// as suggested by their name the segment index values start at zero
std::string message::segment_collision(unsigned id1, unsigned index1,
                                       unsigned id2, unsigned index2)
{
    reorder_for_consistency(index1, id1, index2, id2);

    return string("Lifeform (") + to_string(id1) + string(") segment (") 
         + to_string(index1) + string(") collides with Lifeform (") + to_string(id2) 
         + string(") segment (") + to_string(index2) + string(")\n");
}

// only for the case of 2 consecutive segments
std::string message::segment_superposition(unsigned id, unsigned s1, unsigned s2)
{
    return string("Lifeform id (") + to_string(id) + string(") segments (") 
         + to_string(s1) + string(", ") + to_string(s2) 
         + string(") are in superposition\n");
}

std::string message::success()
{
    return string("Correct file\n");
}
