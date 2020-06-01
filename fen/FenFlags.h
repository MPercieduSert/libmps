/*Auteur: PERCIE DU SERT Maxime
 *Date: 24/04/2020
 */
#ifndef FENFLAGS_H
#define FENFLAGS_H

#include "typemps.h"

namespace fenMPS {
using namespace typeMPS;

//! Enumeration des actions
enum actionFlags : flag::flag_type {NoActions = 0x0,
                 CopierAction = 0x1,
                 CollerAction = 0x2,
                 CouperAction = 0x4,
                 EffacerAction = 0x8,
                 SupprimerAction = 0x10,
                 EditerActions = 0x1F,
                 SauverAction = 0x20,
                 SauverCommeAction = 0x40,
                 ChercherAction = 0x80};
}
#endif // FENFLAGS_H
