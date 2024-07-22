

/**
 * File: constantes.h
 * -------------------
 * Description: This header file contains definitions of constants and enumerations
 *              used across the simulation. It defines essential parameters such as
 *              biological traits of algae, coral, and scavengers, simulation space
 *              boundaries, and various statuses that entities may hold. These
 * constants control various aspects of the simulation behavior and entity properties,
 *              facilitating standardized and centralized configuration of the
 * ecosystem.
 *
 *              Constants:
 *              - epsil_zero: Used for floating-point comparison tolerance.
 *              - r_alg: Radius of an alga.
 *              - alg_birth_rate: Probability of algae reproduction per simulation
 * step.
 *              - max_life_alg: Maximum lifespan of an alga.
 *              - max_life_cor: Maximum lifespan of a coral.
 *              - d_cor: Distance parameter for coral (not detailed here).
 *              - l_repro: Length threshold for coral reproduction.
 *              - l_seg_interne: Internal segment length for coral.
 *              - delta_rot: Increment for rotational movement.
 *              - delta_l: Length increment for growth.
 *              - max: Boundary for the simulation grid.
 *              - r_sca: Base radius of a scavenger.
 *              - r_sca_repro: Radius threshold for scavenger reproduction.
 *              - delta_r_sca: Increment for scavenger radius growth.
 *              - max_life_sca: Maximum lifespan of a scavenger.
 *
 *              Enumerations:
 *              - Statut_cor: Possible statuses for coral (DEAD, ALIVE).
 *              - Dir_rot_cor: Rotational directions for coral (TRIGO, INVTRIGO).
 *              - Statut_dev: Developmental statuses for coral (EXTEND, REPRO).
 *              - Statut_sca: States for scavengers (LIBRE, MANGE).
 *
 * Author: Bahey Shalash
 * Version: 1.0
 * Date: 27/04/2024
 */

#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

constexpr double epsil_zero(0.5);

constexpr unsigned r_alg(1);           // Rayon d'une algue
constexpr double alg_birth_rate(0.5);  // Taux de naissance des algues
constexpr unsigned max_life_alg(500);  // Durée de vie maximale d'une algue

constexpr unsigned max_life_cor(1500);
constexpr unsigned d_cor(3);
constexpr unsigned l_repro(40);
constexpr unsigned l_seg_interne(28);
constexpr double delta_rot(0.0625);
constexpr unsigned delta_l(4);

constexpr double max = 256.0;  // Maximum value for the x and y coordinates

enum Statut_cor { DEAD, ALIVE };
enum Dir_rot_cor { TRIGO, INVTRIGO };
enum Statut_dev { EXTEND, REPRO };

enum Statut_sca { LIBRE, MANGE };
constexpr unsigned r_sca(3);
constexpr unsigned r_sca_repro(10);
constexpr unsigned delta_r_sca(1);
constexpr unsigned max_life_sca(2000);

#endif  // CONSTANTES_H_INCLUDED